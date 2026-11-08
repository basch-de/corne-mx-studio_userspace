#!/usr/bin/env python3
"""Host checks of defaults, native Studio Tap Dance and QMK tapping decisions.

Hardware, EEPROM, settings API and HID emission are mocked. No device access.
No claim of RP2040 compilation or complete QMK processing-pipeline coverage.
"""
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FW = Path(sys.argv[1]).resolve()
KEYMAP = ROOT / 'keyboards/keebart/corne_mx_pro/keymaps/studio'
COMMON = FW / 'keyboards/keebart/common'
SCENARIOS = [
    'fresh', 'old-vial-marker', 'preserve', 'invalid-via', 'slave',
    'setting-failure', 'dance-failure', 'qmk-init-marker', 'keymap',
    'hyper-tap', 'hyper-hold', 'short-tap', 'short-hold', 'comma-tap',
    'dot-hold', 'interrupt', 'gui-edit', 'double', 'disconnect', 'synthetic',
]
TAPPING_SCENARIOS = [
    'tap', 'timeout', 'nested-opposite', 'nested-same', 'roll-opposite',
    'same-timeout', 'thumb-left', 'thumb-right', 'thumb-hold',
    'quick-hrm-in', 'quick-hrm-out', 'quick-tab-in', 'quick-tab-out',
    'quick-enter-in', 'layer-roll', 'layer-nested', 'timer-wrap', 'gui-timing',
]
with tempfile.TemporaryDirectory(prefix='corne-studio-host-') as directory:
    tmp = Path(directory)
    for name in ('quantum.h','qmk_settings.h','via.h','dynamic_keymap.h','usb_device_state.h'):
        (tmp / name).write_text('#include "mock.h"\n')
    (tmp / 'qmk_settings.h').write_text('#include "settings.h"\n')
    # Use the actual upstream storage declaration, not a separately maintained ABI.
    vial = (FW / 'quantum/vial.h').read_text()
    struct = re.search(r'typedef struct \{\s*uint16_t on_tap;.*?\} vial_tap_dance_entry_t;', vial, re.S)
    assert struct, 'Vial Tap Dance ABI declaration changed'
    (tmp / 'vial.h').write_text('#pragma once\n#include "mock.h"\n#define VIAL_TAP_DANCE_ENTRIES 32\n' + struct[0] + '''
_Static_assert(sizeof(vial_tap_dance_entry_t)==10, "Unexpected Vial ABI");
int dynamic_keymap_get_tap_dance(uint8_t, vial_tap_dance_entry_t *);
int dynamic_keymap_set_tap_dance(uint8_t, const vial_tap_dance_entry_t *);
''')
    # Compile the directly maintained keymap against the manufacturer's matrix order.
    layout = json.loads((FW / 'keyboards/keebart/corne_mx_pro/info.json').read_text())['layouts']['LAYOUT_split_3x6_3_ex2']['layout']
    matrix = [['KC_NO'] * 7 for _ in range(8)]
    for i, item in enumerate(layout):
        row, col = item['matrix']
        matrix[row][col] = f'k{i}'
    macro = '#define LAYOUT_split_3x6_3_ex2(' + ','.join(f'k{i}' for i in range(46)) + ') { '
    macro += ', '.join('{' + ','.join(row) + '}' for row in matrix) + ' }\n'
    (tmp / 'layout.h').write_text(macro)
    options = [os.environ.get('CC','cc'), '-std=gnu11', '-Wall', '-Wextra', '-Werror',
               '-DQMK_KEYBOARD_H="mock.h"', '-I'+str(tmp), '-I'+str(ROOT/'tests'),
               '-I'+str(KEYMAP), '-I'+str(COMMON), '-I'+str(FW/'quantum'),
               '-I'+str(FW/'quantum/keymap_extras'), '-I'+str(FW/'quantum/sequencer')]
    shape = tmp / 'keymap-shape.c'
    shape.write_text('#include "' + str(KEYMAP/'keymap.c') + '"\n' + '''
_Static_assert(sizeof(keymaps)/sizeof(keymaps[0]) == 8, "Expected eight populated layers");
_Static_assert(sizeof(encoder_map)/sizeof(encoder_map[0]) == 8, "Encoder/keymap layer mismatch");
''')
    subprocess.run(options + ['-DENCODER_MAP_ENABLE', '-fsyntax-only', str(shape)], check=True)
    subprocess.run(options + [str(KEYMAP/'keymap.c'), str(KEYMAP/'defaults.c'),
                             str(COMMON/'tap_dance.c'), str(COMMON/'studio_dance.c'),
                             str(ROOT/'tests/scenarios.c'), '-o', str(tmp/'scenarios')], check=True)
    for scenario in SCENARIOS:
        subprocess.run([str(tmp/'scenarios'), scenario], check=True)

    # Quoted hardware includes resolve to host stubs. Engine sources stay intact.
    for name in ('action.h', 'action_layer.h', 'action_util.h', 'timer.h'):
        (tmp / name).write_text('#include "mock.h"\n')
    for name in ('action_tapping.c', 'action_tapping.h'):
        shutil.copy(FW / 'quantum' / name, tmp / name)
    # Link the checked-out timing callbacks, not reimplemented decisions.
    settings = (FW / 'quantum/qmk_settings.c').read_text()
    hooks = settings[settings.index('uint16_t qs_get_tapping_term'):settings.index('uint16_t get_combo_term')]
    hooks += settings[settings.index('bool get_chordal_hold'):]
    vial = (FW / 'quantum/vial.c').read_text()
    start = vial.index('uint16_t get_tapping_term(')
    hooks += vial[start:vial.index('uint16_t tap_dance_count(', start)]
    action = (FW / 'quantum/action.c').read_text()
    start = action.index('uint16_t get_tap_keycode(')
    hooks += action[start:action.index('\n}\n', start) + 3]
    (tmp / 'timing-hooks.c').write_text('#include "mock.h"\n#include "action_tapping.h"\n#include "settings.h"\n' + hooks)
    # Match Vial Settings' timing flags; Flow Tap is stored as zero on fresh reset.
    tapping_options = options + ['-Wno-unused-parameter', '-DQMK_SETTINGS',
        '-DTAPPING_TERM_PER_KEY', '-DQUICK_TAP_TERM_PER_KEY',
        '-DPERMISSIVE_HOLD_PER_KEY', '-DHOLD_ON_OTHER_KEY_PRESS_PER_KEY',
        '-DRETRO_TAPPING_PER_KEY', '-DCHORDAL_HOLD', '-DFLOW_TAP_TERM=321']
    subprocess.run(tapping_options + [str(KEYMAP/'keymap.c'), str(KEYMAP/'defaults.c'),
        str(tmp/'action_tapping.c'), str(tmp/'timing-hooks.c'),
        str(ROOT/'tests/tapping.c'), '-o', str(tmp/'tapping')], check=True)
    for scenario in TAPPING_SCENARIOS:
        subprocess.run([str(tmp/'tapping'), scenario], check=True)
print(f'{len(SCENARIOS) + len(TAPPING_SCENARIOS)} host scenarios passed '
      f'({len(SCENARIOS)} Studio/defaults, {len(TAPPING_SCENARIOS)} QMK tapping).')
