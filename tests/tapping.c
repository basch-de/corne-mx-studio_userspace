// SPDX-License-Identifier: GPL-2.0-or-later
// Sequences adapted from the read-only Vial reference tests. QMK decides taps
// and holds; storage, layer effects and the downstream report path are mocked.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mock.h"
#include "action_tapping.h"
#include "layers.h"
#include "settings.h"
#include "vial.h"

extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
static uint16_t now;
static uint32_t marker, layers;
static unsigned overflow, trace_count, input_count;
static struct { keypos_t key; uint16_t code; bool pressed; uint8_t taps; } trace[64];
static vial_tap_dance_entry_t dances[VIAL_TAP_DANCE_ENTRIES];
// Fresh upstream reset values. defaults.c must set the three hold options.
test_settings_t QS = {.tapping_term=TAPPING_TERM, .quick_tap_term=TAPPING_TERM};

bool via_eeprom_is_valid(void) { return true; }
bool is_keyboard_master(void) { return true; }
uint32_t eeconfig_read_user(void) { return marker; }
void eeconfig_update_user(uint32_t value) { marker = value; }
int qmk_settings_set(uint16_t id, const void *value, size_t size) {
    if (id == 7) { assert(size == 2); memcpy(&QS.tapping_term, value, size); }
    else {
        assert(size == 1);
        uint8_t bit = id == 22 ? 1 : id == 23 ? 2 : id == 26 ? 8 : 0;
        assert(bit);
        if (*(const uint8_t *)value) QS.tapping_v2 |= bit;
        else QS.tapping_v2 &= ~bit;
    }
    return 0;
}
int dynamic_keymap_set_tap_dance(uint8_t index, const vial_tap_dance_entry_t *entry) {
    assert(index < VIAL_TAP_DANCE_ENTRIES);
    dances[index] = *entry;
    return 0;
}
int dynamic_keymap_get_tap_dance(uint8_t index, vial_tap_dance_entry_t *entry) {
    if (index >= VIAL_TAP_DANCE_ENTRIES) return -1;
    *entry = dances[index];
    return 0;
}

uint16_t get_record_keycode(keyrecord_t *record, bool cache) {
    (void)cache;
    return record->keycode;
}
bool is_tap_record(keyrecord_t *record) {
    return IS_EVENT(record->event) &&
        (IS_QK_MOD_TAP(record->keycode) || IS_QK_LAYER_TAP(record->keycode));
}
void process_record(keyrecord_t *record) {
    if (IS_NOEVENT(record->event)) return;
    flow_tap_update_last_event(record); // Same position as upstream action.c.
    assert(trace_count < sizeof(trace) / sizeof(trace[0]));
    trace[trace_count++] = (__typeof__(trace[0])){
        record->event.key, record->keycode, record->event.pressed, record->tap.count};
    if (IS_QK_LAYER_TAP(record->keycode) && !record->tap.count) {
        uint32_t bit = 1UL << QK_LAYER_TAP_GET_LAYER(record->keycode);
        if (record->event.pressed) layers |= bit;
        else layers &= ~bit;
    }
}
void process_record_tap_hint(keyrecord_t *record) { (void)record; }
void clear_keyboard(void) { overflow++; }
// These sequences do not exercise action-code decoding/one-shot modifiers.
action_t layer_switch_get_action(keypos_t key) { (void)key; return (action_t){.code=0}; }
uint8_t get_mods(void) { return 0; }

static void event(uint8_t row, uint8_t col, uint16_t time, bool pressed) {
    now = time;
    input_count++;
    action_tapping_process((keyrecord_t){
        .event={.key={.row=row,.col=col}, .time=now, .pressed=pressed, .type=KEY_EVENT},
        .keycode=keymaps[MACOS][row][col]});
}
static void tick(uint16_t time) {
    now = time;
    action_tapping_process((keyrecord_t){.event={.time=now, .type=TICK_EVENT}});
}
static unsigned press_taps(uint8_t row, uint8_t col, unsigned index) {
    for (unsigned i = 0; i < trace_count; i++) {
        if (trace[i].key.row == row && trace[i].key.col == col &&
            trace[i].pressed && index-- == 0) return trace[i].taps;
    }
    fprintf(stderr, "Missing press at [%u,%u]\n", row, col);
    abort();
}
static void hrm(bool pressed, uint16_t time) { event(1,1,time,pressed); }

int main(int argc, char **argv) {
    assert(argc == 2);
    const char *test = argv[1];
    // Expectations describe the actual compiled bindings used by these sequences.
    assert(keymaps[MACOS][1][1] == LCTL_T(KC_A));
    assert(keymaps[MACOS][3][5] == LT(WIN, KC_TAB));
    assert(keymaps[MACOS][7][5] == LT(NUMPAD, KC_ENT));
    assert(keymaps[MACOS][7][4] == LT(SYM, KC_SPC));
    via_init_kb(); keyboard_post_init_user();
    assert(marker && QS.tapping_term == 250 && QS.tapping_v2 == 9);

    if (!strcmp(test,"tap")) {
        hrm(true,100); hrm(false,150);
        assert(press_taps(1,1,0) == 1);
    } else if (!strcmp(test,"timeout")) {
        hrm(true,100); tick(381); hrm(false,400);
        assert(press_taps(1,1,0) == 0);
    } else if (!strcmp(test,"nested-opposite") || !strcmp(test,"nested-same")) {
        bool opposite = !strcmp(test,"nested-opposite");
        uint8_t row = opposite ? 4 : 0;
        hrm(true,100); event(row,2,150,true); event(row,2,180,false); hrm(false,220);
        assert(press_taps(1,1,0) == (opposite ? 0 : 1));
    } else if (!strcmp(test,"roll-opposite")) {
        hrm(true,100); event(4,5,130,true); hrm(false,160); event(4,5,190,false);
        assert(press_taps(1,1,0) == 1);
    } else if (!strcmp(test,"same-timeout")) {
        hrm(true,100); event(0,1,150,true); tick(381); hrm(false,420); event(0,1,450,false);
        assert(press_taps(1,1,0) == 1);
    } else if (!strcmp(test,"thumb-left") || !strcmp(test,"thumb-right")) {
        uint8_t row = !strcmp(test,"thumb-left") ? 3 : 7;
        hrm(true,100); event(row,4,130,true); event(row,4,170,false); hrm(false,200);
        assert(press_taps(1,1,0) == 0);
        assert(press_taps(row,4,0) == 1);
    } else if (!strcmp(test,"quick-hrm-in") || !strcmp(test,"quick-hrm-out")) {
        bool inside = !strcmp(test,"quick-hrm-in");
        hrm(true,100); hrm(false,180); hrm(true,inside ? 400 : 440); tick(700); hrm(false,720);
        assert(press_taps(1,1,1) == (inside ? 2 : 0));
    } else if (!strcmp(test,"quick-tab-in") || !strcmp(test,"quick-tab-out") || !strcmp(test,"quick-enter-in")) {
        bool inside = strcmp(test,"quick-tab-out") != 0;
        uint8_t row = !strcmp(test,"quick-enter-in") ? 7 : 3;
        event(row,5,100,true); event(row,5,120,false);
        event(row,5,inside ? 340 : 380,true); tick(700);
        uint32_t expected_layer = 1UL << (row == 7 ? NUMPAD : WIN);
        assert(layers == (inside ? 0 : expected_layer));
        event(row,5,720,false);
        assert(press_taps(row,5,1) == (inside ? 2 : 0));
    } else if (!strcmp(test,"layer-roll") || !strcmp(test,"layer-nested") || !strcmp(test,"thumb-hold")) {
        bool nested = strcmp(test,"layer-roll") != 0;
        bool tab = !strcmp(test,"thumb-hold");
        uint8_t row = tab ? 3 : 7, col = tab ? 5 : 4;
        event(row,col,100,true); event(0,1,130,true);
        if (nested) {
            event(0,1,160,false);
            assert(layers == (1UL << (tab ? WIN : SYM)));
            event(row,col,180,false);
        } else { event(row,col,160,false); event(0,1,180,false); }
        assert(press_taps(row,col,0) == (nested ? 0 : 1));
    } else if (!strcmp(test,"timer-wrap")) {
        hrm(true,65500); hrm(false,20); hrm(true,100); tick(500); hrm(false,550);
        assert(press_taps(1,1,0) == 1 && press_taps(1,1,1) == 2);
    } else if (!strcmp(test,"gui-timing")) {
        uint16_t term = 350;
        qmk_settings_set(7, &term, sizeof(term));
        via_init_kb(); keyboard_post_init_user(); // Reboot preserves GUI value.
        hrm(true,100); tick(400); assert(trace_count == 0);
        tick(451); hrm(false,500);
        assert(press_taps(1,1,0) == 0);
    } else { assert(!"unknown tapping scenario"); }

    // Every processed press must have a matching release and tap/hold decision.
    // Flushing also catches buffered events that the scenario failed to release.
    tick((uint16_t)(now + 1000));
    bool down[MATRIX_ROWS][MATRIX_COLS] = {0};
    uint8_t taps[MATRIX_ROWS][MATRIX_COLS] = {0};
    for (unsigned i = 0; i < trace_count; i++) {
        keypos_t key = trace[i].key;
        assert(down[key.row][key.col] != trace[i].pressed);
        if (trace[i].pressed) taps[key.row][key.col] = trace[i].taps;
        else assert(taps[key.row][key.col] == trace[i].taps);
        down[key.row][key.col] = trace[i].pressed;
    }
    for (unsigned row = 0; row < MATRIX_ROWS; row++)
        for (unsigned col = 0; col < MATRIX_COLS; col++) assert(!down[row][col]);
    assert(trace_count == input_count && input_count && overflow == 0 && layers == 0);
    printf("PASS: tapping/%s\n", test);
    return 0;
}
