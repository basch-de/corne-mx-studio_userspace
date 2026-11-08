// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "layers.h"
#include "qmk_settings.h"
#include "via.h"
#include "vial.h"
#include "dynamic_keymap.h"

// Separate from the Vial userspace marker and the manufacturer's EEPROM build ID.
// Changing this marker deliberately reapplies the settings and slots below.
#define USER_SETTINGS_VERSION 0x42535401UL
static bool vial_settings_invalid;

// Standard dynamic Vial entries consumed by Keebart's existing Studio engine.
static const vial_tap_dance_entry_t default_tap_dances[] = {
    {LGUI(KC_SPC), HYPR(KC_NO), KC_NO, KC_NO, TAPPING_TERM},
    {CW_TOGG, MO(SHORT), KC_NO, KC_NO, TAPPING_TERM},
    {KC_COMM, KC_DOT, KC_NO, KC_NO, TAPPING_TERM},
};

void via_init_kb(void) {
    // VIA invokes this before checking/restoring its persistent keymap defaults.
    vial_settings_invalid = !via_eeprom_is_valid();
}

void eeconfig_init_user(void) {
    // Called by QMK initialization/reset; does not initiate a reset itself.
    eeconfig_update_user(0);
}

void keyboard_post_init_user(void) {
    // Keebart's keyboard_post_init_kb() explicitly chains to this hook.
    if (!is_keyboard_master()) return;
    if (!vial_settings_invalid && eeconfig_read_user() == USER_SETTINGS_VERSION) return;

    const uint16_t term = TAPPING_TERM;
    const uint8_t enabled = 1;
    const uint8_t disabled = 0;
    // IDs audited against the branch's qmk_settings.c; no timing-hook overrides.
    if (qmk_settings_set(7, &term, sizeof(term)) != 0 ||
        qmk_settings_set(22, &enabled, sizeof(enabled)) != 0 ||
        qmk_settings_set(23, &disabled, sizeof(disabled)) != 0 ||
        qmk_settings_set(26, &enabled, sizeof(enabled)) != 0) return;

    for (uint8_t i = 0; i < sizeof(default_tap_dances) / sizeof(default_tap_dances[0]); ++i) {
        vial_tap_dance_entry_t entry = default_tap_dances[i];
        if (dynamic_keymap_set_tap_dance(i, &entry) != 0) return;
    }
    // Mark complete only after every write succeeds; normal restarts preserve GUI edits.
    eeconfig_update_user(USER_SETTINGS_VERSION);
    vial_settings_invalid = false;
}
