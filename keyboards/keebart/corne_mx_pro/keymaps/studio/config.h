#pragma once
#define VIAL_KEYBOARD_UID {0x89, 0x36, 0x2A, 0xC7, 0xFA, 0xD8, 0x89, 0x45}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#ifndef SPLIT_TRANSACTION_IDS_KB
#    define SPLIT_TRANSACTION_IDS_KB KSR_SYNC
#endif
#ifndef VIA_EEPROM_BUILD_ID
#    define VIA_EEPROM_BUILD_ID 0x4B0201u
#endif

#define VIAL_UNLOCK_COUNTER_MAX 25

// Start value for Vial's persistent global setting, not ZMK timing parity.
// qmk_settings_reset() initializes BOTH tapping and quick-tap to this value.
#define TAPPING_TERM 250
// Preserve the manufacturer's 16-layer storage ABI; eight layers are populated.
// Do not override DYNAMIC_KEYMAP_LAYER_COUNT or VIA_EEPROM_BUILD_ID here.
