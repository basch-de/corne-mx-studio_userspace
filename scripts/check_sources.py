#!/usr/bin/env python3
"""Read-only compatibility checks against a firmware checkout; no device access."""
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path
from generate_keymap import ROOT, LAYERS, outputs

REL = Path('keyboards/keebart/corne_mx_pro/keymaps/studio')

def require(condition, message):
    if not condition:
        raise SystemExit('ERROR: ' + message)

def main():
    require(len(sys.argv) == 2, 'Usage: python3 scripts/check_sources.py /path/to/keebart/firmware')
    fw = Path(sys.argv[1]).resolve()
    actual = subprocess.check_output(['git', '-C', str(fw), 'rev-parse', 'HEAD'], text=True).strip()
    print(f'Checking Keebart firmware commit: {actual}')
    for name, expected in outputs().items():
        require((ROOT / name).read_text() == expected, f'{name} differs from its reviewed source; run generate_keymap.py and review')
    manifest = json.loads((ROOT / 'sources/provenance.json').read_text())
    for name, digest in manifest['sha256'].items():
        require(hashlib.sha256((ROOT / name).read_bytes()).hexdigest() == digest, f'Source snapshot changed: {name}')
    for name in ('rules.mk', 'vial.json'):
        require((ROOT / REL / name).read_bytes() == (fw / REL / name).read_bytes(), f'Manufacturer {name} changed')
    require((ROOT / REL / 'config.h').read_text().startswith((fw / REL / 'config.h').read_text()), 'Manufacturer config prefix changed')
    config = (ROOT / REL / 'config.h').read_text()
    require(len(re.findall(r'^#define TAPPING_TERM 250$', config, re.M)) == 1, 'Unexpected initial tapping term')
    require(not re.search(r'^#\s*(?:define|undef)\s+(?:DYNAMIC_KEYMAP_LAYER_COUNT|VIAL_INSECURE|VIA_EEPROM_BUILD_ID)', config[len((fw / REL / 'config.h').read_text()):], re.M), 'Storage ABI or unlock was overridden')
    require('#define DYNAMIC_KEYMAP_LAYER_COUNT 16' in (fw / 'keyboards/keebart/corne_mx_pro/config.h').read_text(), 'Expected 16-layer manufacturer ABI')
    data = json.loads((ROOT / 'docs/mapping.json').read_text())
    layout = json.loads((fw / 'keyboards/keebart/corne_mx_pro/info.json').read_text())['layouts']['LAYOUT_split_3x6_3_ex2']['layout']
    # Explicit electrical mapping, independent of the converter and coordinate heuristics.
    expected = ([[0,c] for c in range(7)] + [[4,c] for c in range(6,-1,-1)] +
                [[1,c] for c in range(7)] + [[5,c] for c in range(6,-1,-1)] +
                [[2,c] for c in range(6)] + [[6,c] for c in range(5,-1,-1)] +
                [[3,c] for c in range(3,6)] + [[7,c] for c in range(5,2,-1)])
    require([k['matrix'] for k in layout] == expected, '46-position physical/electrical layout changed')
    require(len(data) == 368 and len(set(map(tuple, expected))) == 46, 'Incorrect key count')
    layers = {name: [c for c in data if c['layer'] == name] for name in LAYERS}
    for name, cells in layers.items():
        require([c['position'] for c in cells] == list(range(46)), f'Position order: {name}')
        require(all(cells[p]['qmk'] == 'KC_NO' for p in (6,7,20,21)), f'Unexpected extra switch binding: {name}')
    # Every populated layer has an explicit entry, and NUMPAD retains its exit toggle.
    for layer, pos, code in [('macos',0,'LT(SYS, KC_GRV)'), ('macos',13,'LT(SYS, KC_BSLS)'),
                             ('macos',40,'LT(NAV, KC_ESC)'), ('macos',41,'LT(NUM, KC_BSPC)'),
                             ('macos',42,'LT(WIN, KC_TAB)'), ('macos',43,'LT(NUMPAD, KC_ENT)'),
                             ('macos',44,'LT(SYM, KC_SPC)'), ('macos',45,'MO(SHORT)'),
                             ('sym',33,'TG(NUMPAD)'), ('numpad',33,'TG(NUMPAD)')]:
        require(layers[layer][pos]['qmk'] == code, f'Layer reachability changed: {layer}:{pos}')
    for p, code in [(28,'QK_RBT'),(29,'QK_BOOT'),(30,'EE_CLR'),(37,'EE_CLR'),(38,'QK_BOOT'),(39,'QK_RBT')]:
        require(layers['sys'][p]['qmk'] == code, f'Mirrored SYS action missing: {p}')
    require(all(c['qmk'] not in ('EE_CLR','QK_BOOT','QK_RBT') for c in data if c['layer'] != 'sys'), 'System action escaped held SYS')
    require(all(c['qmk'] == 'KC_NO' for c in data if c['zmk'] in ('&kp C_VOICE_COMMAND','&kp C_PWR')), 'Unsupported HID usage replaced silently')
    for source in (ROOT / 'keyboards').rglob('*.c'):
        # This project must contain data only; catch callbacks, helper functions and includes.
        content = source.read_text()
        require(not re.search(r'\b(?:bool|void|uint\w*|layer_state_t|char)\s+\w+\s*\([^;{}]*\)\s*\{', content), f'Custom firmware function found: {source}')
        require(re.findall(r'^#include\s+(.+)$', content, re.M) == ['QMK_KEYBOARD_H'], f'Unexpected code inclusion: {source}')
    keymap = (ROOT / REL / 'keymap.c').read_text()
    keycodes = ((fw / 'quantum/keycodes.h').read_text() + (fw / 'quantum/quantum_keycodes.h').read_text()
                + (fw / 'quantum/keymap_extras/keymap_us.h').read_text())
    for token in set(re.findall(r'\b(?:KC_|CW_|RM_|QK_|MOD_)[A-Z0-9_]+', keymap)):
        # MOD_* bit masks live in the standard report header.
        definitions = keycodes + (fw / 'tmk_core/protocol/report.h').read_text()
        require(re.search(r'\b' + token + r'\b', definitions), f'Unknown keycode: {token}')
    require(len(re.findall(r'^    \[\d+\] = \{ENCODER_CCW_CW', keymap, re.M)) == 8, 'Encoder layer count must match the eight compiled keymap layers')
    submodules = subprocess.check_output(['git','-C',str(fw),'submodule','status','--recursive'],text=True)
    require(not any(line.startswith(('-', '+', 'U')) for line in submodules.splitlines()), 'Missing or mismatched firmware submodules; run git submodule update --init --recursive')
    print('PASS: source snapshots, stock Studio modules/identity, 8 x 46 positions, layer entries, mirrored SYS, matching encoder layers / 16-layer ABI, known keycodes, no custom functions, submodules.')
    print('Firmware compilation and hardware/GUI validation are separate and are NOT certified by this check.')

if __name__ == '__main__':
    main()
