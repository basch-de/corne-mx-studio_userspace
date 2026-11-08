// Host storage boundary; the timing callbacks themselves come from upstream.
#pragma once
#include "mock.h"
typedef struct {
    uint16_t tapping_term, quick_tap_term, flow_tap_term;
    uint8_t tapping_v2;
} test_settings_t;
extern test_settings_t QS;
#define QS_tapping_permissive_hold (QS.tapping_v2 & 1)
#define QS_tapping_hold_on_other_key_press (QS.tapping_v2 & 2)
#define QS_tapping_retro_tapping (QS.tapping_v2 & 4)
#define QS_tapping_chordal_hold (QS.tapping_v2 & 8)
uint16_t qs_get_tapping_term(uint16_t, keyrecord_t *);
