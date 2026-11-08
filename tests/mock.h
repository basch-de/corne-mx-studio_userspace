// Host boundary only. Compile real keycodes, our initializer and Studio's engine.
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "quantum_keycodes.h"
#include "modifiers.h"
#include "action_code.h"
#include "config.h"

#define MATRIX_ROWS 8
#define MATRIX_COLS 7
#define NUM_ENCODERS 4
#define NUM_DIRECTIONS 2
#define PROGMEM
#define ENCODER_CCW_CW(a, b) {b, a}
#define KEY_EVENT 1
#define TICK_EVENT 0
#define pgm_read_byte(p) (*(const char *)(p))
#define ac_dprintf(...) ((void)0)
#define debug_event(...) ((void)0)
#define debug_record(...) ((void)0)
#define KEYEQ(a,b) ((a).row == (b).row && (a).col == (b).col)
#define IS_NOEVENT(e) ((e).type == TICK_EVENT)
#define IS_EVENT(e) (!IS_NOEVENT(e))
#define TIMER_DIFF_16(a,b) ((uint16_t)((a)-(b)))
#define VIAL_MATRIX_MAGIC 254
#define USB_DEVICE_STATE_CONFIGURED 1

typedef struct { uint8_t row, col; } keypos_t;
typedef struct { keypos_t key; uint16_t time; bool pressed; uint8_t type; } keyevent_t;
typedef struct { bool interrupted:1; uint8_t reserved:3; uint8_t count:4; } tap_t;
typedef struct { keyevent_t event; tap_t tap; uint16_t keycode; } keyrecord_t;

uint16_t get_record_keycode(keyrecord_t *, bool);
uint16_t get_tap_keycode(uint16_t);
bool is_tap_record(keyrecord_t *);
void process_record(keyrecord_t *);
void process_record_tap_hint(keyrecord_t *);
void clear_keyboard(void);
action_t layer_switch_get_action(keypos_t);
uint8_t get_mods(void);

bool via_eeprom_is_valid(void);
bool is_keyboard_master(void);
uint32_t eeconfig_read_user(void);
void eeconfig_update_user(uint32_t);
int qmk_settings_set(uint16_t, const void *, size_t);
void via_init_kb(void);
void eeconfig_init_user(void);
void keyboard_post_init_user(void);
uint32_t timer_read32(void);
uint16_t timer_read(void);
void wait_ms(uint16_t);
uint8_t usb_device_state_get_configure_state(void);
void vial_keycode_down(uint16_t);
void vial_keycode_up(uint16_t);
#include "layout.h"
