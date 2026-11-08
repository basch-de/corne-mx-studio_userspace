#!/usr/bin/env python3
"""Read-only compatibility checks against a firmware checkout; no device access."""
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

REL = Path('keyboards/keebart/corne_mx_pro/keymaps/studio')

def require(condition, message):
    if not condition:
        raise SystemExit('ERROR: ' + message)

def main():
    require(len(sys.argv) == 2, 'Usage: python3 scripts/check_sources.py /path/to/keebart/firmware')
    fw = Path(sys.argv[1]).resolve()
    actual = subprocess.check_output(['git', '-C', str(fw), 'rev-parse', 'HEAD'], text=True).strip()
    print(f'Checking Keebart firmware commit: {actual}')
    require((ROOT / REL / 'vial.json').read_bytes() == (fw / REL / 'vial.json').read_bytes(), 'Manufacturer vial.json changed')
    rules_suffix = '\n# User-approved initialization; all Studio processing stays in manufacturer modules.\nSRC += defaults.c\n'
    require((ROOT / REL / 'rules.mk').read_text() == (fw / REL / 'rules.mk').read_text() + rules_suffix, 'Expected stock Studio rules plus defaults.c only')
    require((ROOT / REL / 'config.h').read_text().startswith((fw / REL / 'config.h').read_text()), 'Manufacturer config prefix changed')
    config = (ROOT / REL / 'config.h').read_text()
    require(len(re.findall(r'^#define TAPPING_TERM 250$', config, re.M)) == 1, 'Unexpected initial tapping term')
    require(not re.search(r'^#\s*(?:define|undef)\s+(?:DYNAMIC_KEYMAP_LAYER_COUNT|VIAL_INSECURE|VIA_EEPROM_BUILD_ID)', config[len((fw / REL / 'config.h').read_text()):], re.M), 'Storage ABI or unlock was overridden')
    require('#define DYNAMIC_KEYMAP_LAYER_COUNT 16' in (fw / 'keyboards/keebart/corne_mx_pro/config.h').read_text(), 'Expected 16-layer manufacturer ABI')
    layout = json.loads((fw / 'keyboards/keebart/corne_mx_pro/info.json').read_text())['layouts']['LAYOUT_split_3x6_3_ex2']['layout']
    # Explicit electrical mapping, independent of the converter and coordinate heuristics.
    expected = ([[0,c] for c in range(7)] + [[4,c] for c in range(6,-1,-1)] +
                [[1,c] for c in range(7)] + [[5,c] for c in range(6,-1,-1)] +
                [[2,c] for c in range(6)] + [[6,c] for c in range(5,-1,-1)] +
                [[3,c] for c in range(3,6)] + [[7,c] for c in range(5,2,-1)])
    require([k['matrix'] for k in layout] == expected, '46-position physical/electrical layout changed')
    require(len(layout) == 46 and len(set(map(tuple, expected))) == 46, 'Incorrect physical key count')
    # Keymap shape, layer entries and protected actions are checked against the
    # actual compiled C arrays by tests/run.py; no second binding source is used.
    # The approved exception is initial configuration, not a replacement event engine.
    hooks = {'via_init_kb', 'eeconfig_init_user', 'keyboard_post_init_user'}
    for name, source in [('via_init_kb','quantum/via.c'),('eeconfig_init_user','quantum/eeconfig.c'),('keyboard_post_init_user','quantum/keyboard.c')]:
        require(re.search(r'__attribute__\(\(weak\)\)\s+void\s+' + name + r'\(', (fw / source).read_text()), f'{name} is no longer a weak default')
    require(re.search(r'\bkeyboard_post_init_user\s*\(\s*\)\s*;', (fw / 'keyboards/keebart/common/studio_rgb_qmk.c').read_text()), 'Studio no longer chains user initialization')
    # Discover the current Studio source/include graph instead of pinning files
    # by hash. This is a conservative text check, not a C preprocessor or proof
    # of complete runtime compatibility.
    common = (fw / 'keyboards/keebart/common').resolve()
    pending = []
    for name in ('studio_rgb.mk', 'studio_test.mk'):
        pending += [fw / source for source in re.findall(r'keyboards/keebart/common/[\w/]+\.c', (common / name).read_text())]
    require(pending, 'No Studio sources found in manufacturer rules')
    checked = set()
    while pending:
        source = pending.pop().resolve()
        if source in checked:
            continue
        checked.add(source)
        content = source.read_text()
        definitions = set(re.findall(r'\bvoid\s+(\w+)\s*\([^;{}]*\)\s*\{', content))
        require(not (definitions & hooks), f'Studio source now owns an initialization hook: {source.relative_to(fw)}')
        for include in re.findall(r'^\s*#\s*include\s+"([^"]+)"', content, re.M):
            for candidate in (source.parent / include, common / include):
                candidate = candidate.resolve()
                if candidate.is_file() and common in candidate.parents and candidate.suffix in ('.c','.h','.inc'):
                    pending.append(candidate)
                    break
    settings = (fw / 'quantum/qmk_settings.c').read_text()
    for setting, field in [(7,'tapping_term'), (22,'QS_tapping_permissive_hold_bit'), (23,'QS_tapping_hold_on_other_key_press_bit'), (26,'QS_tapping_chordal_hold_bit')]:
        pattern = (r'DECLARE_STATIC_SETTING\(\s*7\s*,\s*tapping_term\s*\)' if setting == 7 else
                   rf'DECLARE_STATIC_BITSETTING\(\s*{setting}\s*,\s*tapping_v2\s*,\s*{field}\s*\)')
        require(re.search(pattern, settings), f'Initialization setting ID {setting} changed meaning')
    require({s.name for s in (ROOT / REL).glob('*.c')} == {'keymap.c','defaults.c'}, 'Unreviewed keymap source added')
    for source in (ROOT / 'keyboards').rglob('*.c'):
        content = source.read_text()
        definitions = re.findall(r'\b(?:bool|void|uint\w*|layer_state_t|char)\s+(\w+)\s*\([^;{}]*\)\s*\{', content)
        expected_functions = hooks if source == ROOT / REL / 'defaults.c' else set()
        require(set(definitions) == expected_functions and len(definitions) == len(expected_functions), f'Unapproved firmware function found: {source}')
        includes = ['QMK_KEYBOARD_H', '"layers.h"']
        if source.name == 'defaults.c':
            includes += ['"qmk_settings.h"', '"via.h"', '"vial.h"', '"dynamic_keymap.h"']
        require(re.findall(r'^#include\s+(.+)$', content, re.M) == includes, f'Unexpected code inclusion: {source}')
    keymap = (ROOT / REL / 'keymap.c').read_text()
    keycodes = ((fw / 'quantum/keycodes.h').read_text() + (fw / 'quantum/quantum_keycodes.h').read_text()
                + (fw / 'quantum/keymap_extras/keymap_us.h').read_text())
    for token in set(re.findall(r'\b(?:KC_|CW_|RM_|QK_|MOD_)[A-Z0-9_]+', keymap)):
        # MOD_* bit masks live in the standard report header.
        definitions = keycodes + (fw / 'tmk_core/protocol/report.h').read_text()
        require(re.search(r'\b' + token + r'\b', definitions), f'Unknown keycode: {token}')
    submodules = subprocess.check_output(['git','-C',str(fw),'submodule','status','--recursive'],text=True)
    require(not any(line.startswith(('-', '+', 'U')) for line in submodules.splitlines()), 'Missing or mismatched firmware submodules; run git submodule update --init --recursive')
    print('PASS: stock Studio modules/identity, 46-position layout, 16-layer ABI, initialization hooks/chaining, setting IDs, submodules.')
    print('Firmware compilation and hardware/GUI validation are separate and are NOT certified by this check.')

if __name__ == '__main__':
    main()
