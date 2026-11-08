// Host-only persistence and event scenarios; no USB/flash/EEPROM device access.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mock.h"
#include "layers.h"
#include "vial.h"
#include "studio_dance.h"

static bool master = true, valid = true, connected = true;
static uint32_t user_marker, now;
static unsigned settings_writes, dance_writes, marker_writes;
static int fail_setting = -1, fail_dance = -1;
static uint16_t tapping_term = 250, quick_term = 315;
static bool permissive, hold_on_press, chordal;
static vial_tap_dance_entry_t dances[32];
static struct { uint16_t code; bool down; } output[32];
static unsigned output_count;
extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS];

bool via_eeprom_is_valid(void) { return valid; }
bool is_keyboard_master(void) { return master; }
uint32_t eeconfig_read_user(void) { return user_marker; }
void eeconfig_update_user(uint32_t value) { user_marker = value; marker_writes++; }
int qmk_settings_set(uint16_t id, const void *value, size_t size) {
    if (id == fail_setting) return -1;
    settings_writes++;
    if (id == 7) { assert(size == 2); memcpy(&tapping_term, value, 2); }
    else {
        assert(size == 1);
        bool enabled = *(const uint8_t *)value;
        if (id == 22) permissive = enabled;
        else if (id == 23) hold_on_press = enabled;
        else if (id == 26) chordal = enabled;
        else assert(!"unexpected setting write");
    }
    return 0;
}
int dynamic_keymap_set_tap_dance(uint8_t index, const vial_tap_dance_entry_t *entry) {
    assert(index < 32);
    if (index == fail_dance) return -1;
    dances[index] = *entry;
    dance_writes++;
    return 0;
}
int dynamic_keymap_get_tap_dance(uint8_t index, vial_tap_dance_entry_t *entry) {
    if (index >= 32) return -1;
    *entry = dances[index];
    return 0;
}
uint32_t timer_read32(void) { return now; }
uint16_t timer_read(void) { return (uint16_t)now; }
void wait_ms(uint16_t amount) { now += amount; }
uint8_t usb_device_state_get_configure_state(void) { return connected ? USB_DEVICE_STATE_CONFIGURED : 0; }
void vial_keycode_down(uint16_t code) {
    assert(output_count < 32);
    output[output_count].code = code; output[output_count++].down = true;
}
void vial_keycode_up(uint16_t code) {
    assert(output_count < 32);
    output[output_count].code = code; output[output_count++].down = false;
}

static void initialize(void) {
    dances[3].on_tap = KC_Z; // Slots outside 0..2 belong to the user.
    via_init_kb();
    valid = true; // Simulate VIA completing its initialization before post-init.
    keyboard_post_init_user();
}
static void check_defaults(void) {
    assert(user_marker != 0 && user_marker != 0x42530102UL);
    assert(tapping_term == 250 && permissive && chordal && !hold_on_press);
    assert(dances[0].on_tap == LGUI(KC_SPC) && dances[0].on_hold == KC_HYPR);
    assert(dances[1].on_tap == CW_TOGG && dances[1].on_hold == MO(SHORT));
    assert(dances[2].on_tap == KC_COMM && dances[2].on_hold == KC_DOT);
    for (unsigned i = 0; i < 3; i++) {
        assert(dances[i].on_double_tap == KC_NO && dances[i].on_tap_hold == KC_NO);
        assert(dances[i].custom_tapping_term == 250);
    }
    assert(dances[3].on_tap == KC_Z && quick_term == 315);
}
static void physical(uint16_t code, uint8_t row, uint8_t col, bool down, uint32_t at) {
    now = at;
    keyrecord_t record = {.event={.key={.row=row,.col=col},.time=(uint16_t)at,.pressed=down,.type=KEY_EVENT}};
    if (down) kst_observe(code, &record);
    bool passthrough = kst_key(code, &record);
    if (code >= TD(0) && code <= TD(31)) assert(!passthrough);
}
static void tick(uint32_t at) { now = at; kst_task(); }
static void check_pair(unsigned offset, uint16_t code) {
    assert(output_count == offset + 2);
    assert(output[offset].code == code && output[offset].down);
    assert(output[offset+1].code == code && !output[offset+1].down);
}

int main(int argc, char **argv) {
    assert(argc == 2);
    const char *test = argv[1];
    if (!strcmp(test,"fresh") || !strcmp(test,"old-vial-marker")) {
        if (!strcmp(test,"old-vial-marker")) user_marker = 0x42530102UL;
        initialize(); check_defaults();
        assert(settings_writes == 4 && dance_writes == 3 && marker_writes == 1);
    } else if (!strcmp(test,"preserve")) {
        initialize();
        tapping_term = 370; permissive = false; chordal = false;
        dances[0].on_tap = KC_X; dances[1].custom_tapping_term = 410;
        via_init_kb(); keyboard_post_init_user();
        assert(settings_writes == 4 && dance_writes == 3 && marker_writes == 1);
        assert(tapping_term == 370 && !permissive && !chordal);
        assert(dances[0].on_tap == KC_X && dances[1].custom_tapping_term == 410);
    } else if (!strcmp(test,"invalid-via")) {
        initialize(); valid = false;
        initialize(); check_defaults();
        assert(settings_writes == 8 && dance_writes == 6);
    } else if (!strcmp(test,"slave")) {
        master = false; initialize();
        assert(!user_marker && !settings_writes && !dance_writes && !marker_writes);
        master = true; keyboard_post_init_user(); check_defaults();
    } else if (!strcmp(test,"setting-failure")) {
        fail_setting = 23; initialize();
        assert(!user_marker && !dance_writes && !marker_writes);
        fail_setting = -1; keyboard_post_init_user(); check_defaults();
    } else if (!strcmp(test,"dance-failure")) {
        fail_dance = 1; initialize();
        assert(!user_marker && dance_writes == 1 && !marker_writes);
        fail_dance = -1; keyboard_post_init_user(); check_defaults();
    } else if (!strcmp(test,"qmk-init-marker")) {
        initialize(); eeconfig_init_user();
        assert(!user_marker && settings_writes == 4 && dance_writes == 3);
        via_init_kb(); keyboard_post_init_user(); check_defaults();
        assert(settings_writes == 8 && dance_writes == 6);
    } else if (!strcmp(test,"keymap")) {
        assert(keymaps[MACOS][0][0] == LT(SYS, KC_GRV));
        assert(keymaps[MACOS][4][0] == KC_BSLS);
        assert(keymaps[MACOS][1][0] == TD(0));
        assert(keymaps[MACOS][7][3] == TD(1));
        assert(keymaps[NUMPAD][6][1] == TD(2));
        assert(keymaps[SHORT][0][0] == KC_NO && keymaps[SHORT][1][0] == KC_NO);
        assert(keymaps[NUMPAD][2][4] == KC_NO);
        assert(keymaps[SHORT][6][4] == KC_ASST && keymaps[SYS][4][0] == KC_PWR);
        assert(keymaps[MACOS][3][3] == LT(NAV, KC_ESC));
        assert(keymaps[MACOS][3][4] == LT(NUM, KC_BSPC));
        assert(keymaps[MACOS][3][5] == LT(WIN, KC_TAB));
        assert(keymaps[MACOS][7][5] == LT(NUMPAD, KC_ENT));
        assert(keymaps[MACOS][7][4] == LT(SYM, KC_SPC));
        assert(keymaps[MACOS][2][0] == LT(SHORT, KC_EQL));
        assert(keymaps[SYM][2][5] == TG(NUMPAD) && keymaps[NUMPAD][2][5] == TG(NUMPAD));
        assert(keymaps[SYS][2][0] == QK_RBT && keymaps[SYS][6][0] == QK_RBT);
        assert(keymaps[SYS][2][1] == QK_BOOT && keymaps[SYS][6][1] == QK_BOOT);
        assert(keymaps[SYS][2][2] == EE_CLR && keymaps[SYS][6][2] == EE_CLR);
        for (unsigned layer = 0; layer < 8; layer++) {
            assert(keymaps[layer][0][6] == KC_NO && keymaps[layer][4][6] == KC_NO);
            assert(keymaps[layer][1][6] == KC_NO && keymaps[layer][5][6] == KC_NO);
            if (layer != SYS) for (unsigned row = 0; row < 8; row++) {
                for (unsigned col = 0; col < 7; col++) {
                    uint16_t code = keymaps[layer][row][col];
                    assert(code != QK_RBT && code != QK_BOOT && code != EE_CLR);
                }
            }
        }
        assert(chordal_hold_layout[3][3] == '*' && chordal_hold_layout[7][3] == '*');
    } else {
        initialize();
        if (!strcmp(test,"hyper-tap")) {
            physical(TD(0),1,0,true,10); physical(TD(0),1,0,false,50); tick(260);
            check_pair(0,LGUI(KC_SPC));
        } else if (!strcmp(test,"hyper-hold")) {
            physical(TD(0),1,0,true,10); tick(259); assert(!output_count);
            tick(260); assert(output_count == 1 && output[0].down && output[0].code == KC_HYPR);
            physical(TD(0),1,0,false,300); check_pair(0,KC_HYPR);
        } else if (!strcmp(test,"short-tap") || !strcmp(test,"short-hold")) {
            physical(TD(1),7,3,true,10);
            if (!strcmp(test,"short-tap")) {
                physical(TD(1),7,3,false,50); tick(260); check_pair(0,CW_TOGG);
            } else { tick(260); physical(TD(1),7,3,false,300); check_pair(0,MO(SHORT)); }
        } else if (!strcmp(test,"comma-tap") || !strcmp(test,"dot-hold")) {
            physical(TD(2),6,1,true,10);
            if (!strcmp(test,"comma-tap")) {
                physical(TD(2),6,1,false,50); tick(260); check_pair(0,KC_COMM);
            } else { tick(260); physical(TD(2),6,1,false,300); check_pair(0,KC_DOT); }
        } else if (!strcmp(test,"interrupt")) {
            physical(TD(0),1,0,true,10); physical(KC_A,1,1,true,50);
            assert(output_count == 1 && output[0].code == LGUI(KC_SPC));
            physical(TD(0),1,0,false,60); check_pair(0,LGUI(KC_SPC));
            tick(400); assert(output_count == 2);
        } else if (!strcmp(test,"gui-edit")) {
            dances[0].on_tap = KC_Z; dances[0].on_hold = KC_LCTL; dances[0].custom_tapping_term = 400;
            physical(TD(0),1,0,true,10); tick(260); assert(!output_count);
            tick(410); physical(TD(0),1,0,false,500); check_pair(0,KC_LCTL);
            physical(TD(0),1,0,true,600); physical(TD(0),1,0,false,650); tick(1000);
            check_pair(2,KC_Z);
        } else if (!strcmp(test,"double")) {
            physical(TD(2),6,1,true,10); physical(TD(2),6,1,false,30);
            physical(TD(2),6,1,true,60); physical(TD(2),6,1,false,80); tick(310);
            assert(output_count == 4);
            for (unsigned i=0;i<4;i++) assert(output[i].code == KC_COMM && output[i].down == (i%2==0));
        } else if (!strcmp(test,"disconnect")) {
            physical(TD(0),1,0,true,10); tick(260); connected = false; tick(300);
            check_pair(0,KC_HYPR);
        } else if (!strcmp(test,"synthetic")) {
            now = 10;
            keyrecord_t record = {.event={.key={.row=VIAL_MATRIX_MAGIC,.col=VIAL_MATRIX_MAGIC},.pressed=true,.time=10}};
            kst_observe(TD(0), &record); assert(kst_key(TD(0), &record));
            tick(300); assert(!output_count);
        } else assert(!"unknown scenario");
    }
    printf("PASS: %s\n", test);
    return 0;
}
