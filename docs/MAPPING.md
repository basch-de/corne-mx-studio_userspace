# Positionszuordnung

Positionen sind nullbasiert und folgen der ZMK-Binding-Reihenfolge.
Zeilen: 0–13, 14–27, 28–39, Daumen 40–45. `KC_NO` blockiert; `KC_TRNS` fällt durch.
Die Spalte Status unterscheidet direkte Umsetzung, Näherung und dokumentierte Änderungen.

| Layer | Position | ZMK | QMK | Status / Hinweis |
|---|---:|---|---|---|
| macos | 0 | `&hlt SYS GRAVE` | `LT(SYS, KC_GRV)` | approximate  |
| macos | 1 | `&kp Q` | `KC_Q` | direct  |
| macos | 2 | `&kp W` | `KC_W` | direct  |
| macos | 3 | `&kp E` | `KC_E` | direct  |
| macos | 4 | `&kp R` | `KC_R` | direct  |
| macos | 5 | `&kp T` | `KC_T` | direct  |
| macos | 6 | `&none` | `KC_NO` | direct  |
| macos | 7 | `&none` | `KC_NO` | direct  |
| macos | 8 | `&kp Y` | `KC_Y` | direct  |
| macos | 9 | `&kp U` | `KC_U` | direct  |
| macos | 10 | `&kp I` | `KC_I` | direct  |
| macos | 11 | `&kp O` | `KC_O` | direct  |
| macos | 12 | `&kp P` | `KC_P` | direct  |
| macos | 13 | `&kp BSLH` | `LT(SYS, KC_BSLS)` | documented-addition Right-hand SYS entry; tap remains backslash |
| macos | 14 | `&mt LA(LC(LG(LSHFT))) LG(SPACE)` | `KC_HYPR` | split-interim  |
| macos | 15 | `&hml LCTRL A` | `MT(MOD_LCTL, KC_A)` | approximate  |
| macos | 16 | `&hml LGUI S` | `MT(MOD_LGUI, KC_S)` | approximate  |
| macos | 17 | `&hml LALT D` | `MT(MOD_LALT, KC_D)` | approximate  |
| macos | 18 | `&hml LSHFT F` | `MT(MOD_LSFT, KC_F)` | approximate  |
| macos | 19 | `&kp G` | `KC_G` | direct  |
| macos | 20 | `&none` | `KC_NO` | direct  |
| macos | 21 | `&none` | `KC_NO` | direct  |
| macos | 22 | `&kp H` | `KC_H` | direct  |
| macos | 23 | `&hmr RSHFT J` | `MT(MOD_RSFT, KC_J)` | approximate  |
| macos | 24 | `&hmr RALT K` | `MT(MOD_RALT, KC_K)` | approximate  |
| macos | 25 | `&hmr RGUI L` | `MT(MOD_RGUI, KC_L)` | approximate  |
| macos | 26 | `&hmr RCTRL SEMI` | `MT(MOD_RCTL, KC_SCLN)` | approximate  |
| macos | 27 | `&mt LA(LC(LSHFT)) SQT` | `MEH_T(KC_QUOT)` | approximate  |
| macos | 28 | `&lt SHORT EQUAL` | `LT(SHORT, KC_EQL)` | approximate  |
| macos | 29 | `&kp Z` | `KC_Z` | direct  |
| macos | 30 | `&kp X` | `KC_X` | direct  |
| macos | 31 | `&kp C` | `KC_C` | direct  |
| macos | 32 | `&kp V` | `KC_V` | direct  |
| macos | 33 | `&kp B` | `KC_B` | direct  |
| macos | 34 | `&kp N` | `KC_N` | direct  |
| macos | 35 | `&kp M` | `KC_M` | direct  |
| macos | 36 | `&kp COMMA` | `KC_COMM` | direct  |
| macos | 37 | `&kp DOT` | `KC_DOT` | direct  |
| macos | 38 | `&kp FSLH` | `KC_SLSH` | direct  |
| macos | 39 | `&kp MINUS` | `KC_MINS` | direct  |
| macos | 40 | `&hlt NAV ESC` | `LT(NAV, KC_ESC)` | approximate  |
| macos | 41 | `&hlt NUM BSPC` | `LT(NUM, KC_BSPC)` | approximate  |
| macos | 42 | `&rhlt WIN TAB` | `LT(WIN, KC_TAB)` | approximate  |
| macos | 43 | `&rlt NUMPAD RET` | `LT(NUMPAD, KC_ENT)` | approximate  |
| macos | 44 | `&hlt SYM SPACE` | `LT(SYM, KC_SPC)` | approximate  |
| macos | 45 | `&lcw SHORT 0` | `MO(SHORT)` | split-interim  |
| num | 0 | `&kp COMMA` | `KC_COMM` | direct  |
| num | 1 | `&kp N1` | `KC_1` | direct  |
| num | 2 | `&kp N2` | `KC_2` | direct  |
| num | 3 | `&kp N3` | `KC_3` | direct  |
| num | 4 | `&kp N4` | `KC_4` | direct  |
| num | 5 | `&kp N5` | `KC_5` | direct  |
| num | 6 | `&none` | `KC_NO` | direct  |
| num | 7 | `&none` | `KC_NO` | direct  |
| num | 8 | `&kp N6` | `KC_6` | direct  |
| num | 9 | `&kp N7` | `KC_7` | direct  |
| num | 10 | `&kp N8` | `KC_8` | direct  |
| num | 11 | `&kp N9` | `KC_9` | direct  |
| num | 12 | `&kp N0` | `KC_0` | direct  |
| num | 13 | `&kp BSPC` | `KC_BSPC` | direct  |
| num | 14 | `&kp FSLH` | `KC_SLSH` | direct  |
| num | 15 | `&kp LCTRL` | `KC_LCTL` | direct  |
| num | 16 | `&kp LGUI` | `KC_LGUI` | direct  |
| num | 17 | `&kp LALT` | `KC_LALT` | direct  |
| num | 18 | `&kp LSHFT` | `KC_LSFT` | direct  |
| num | 19 | `&kp MINUS` | `KC_MINS` | direct  |
| num | 20 | `&none` | `KC_NO` | direct  |
| num | 21 | `&none` | `KC_NO` | direct  |
| num | 22 | `&kp COLON` | `KC_COLN` | direct  |
| num | 23 | `&kp LEFT` | `KC_LEFT` | direct  |
| num | 24 | `&kp DOWN` | `KC_DOWN` | direct  |
| num | 25 | `&kp UP` | `KC_UP` | direct  |
| num | 26 | `&kp RIGHT` | `KC_RIGHT` | direct  |
| num | 27 | `&kp DOT` | `KC_DOT` | direct  |
| num | 28 | `&kp F12` | `KC_F12` | direct  |
| num | 29 | `&kp F1` | `KC_F1` | direct  |
| num | 30 | `&kp F2` | `KC_F2` | direct  |
| num | 31 | `&kp F3` | `KC_F3` | direct  |
| num | 32 | `&kp F4` | `KC_F4` | direct  |
| num | 33 | `&kp F5` | `KC_F5` | direct  |
| num | 34 | `&kp F6` | `KC_F6` | direct  |
| num | 35 | `&kp F7` | `KC_F7` | direct  |
| num | 36 | `&kp F8` | `KC_F8` | direct  |
| num | 37 | `&kp F9` | `KC_F9` | direct  |
| num | 38 | `&kp F10` | `KC_F10` | direct  |
| num | 39 | `&kp F11` | `KC_F11` | direct  |
| num | 40 | `&trans` | `KC_TRNS` | direct  |
| num | 41 | `&trans` | `KC_TRNS` | direct  |
| num | 42 | `&trans` | `KC_TRNS` | direct  |
| num | 43 | `&trans` | `KC_TRNS` | direct  |
| num | 44 | `&trans` | `KC_TRNS` | direct  |
| num | 45 | `&trans` | `KC_TRNS` | direct  |
| sym | 0 | `&kp GRAVE` | `KC_GRV` | direct  |
| sym | 1 | `&kp EXCL` | `KC_EXLM` | direct  |
| sym | 2 | `&kp AT` | `KC_AT` | direct  |
| sym | 3 | `&kp HASH` | `KC_HASH` | direct  |
| sym | 4 | `&kp DLLR` | `KC_DLR` | direct  |
| sym | 5 | `&kp PRCNT` | `KC_PERC` | direct  |
| sym | 6 | `&none` | `KC_NO` | direct  |
| sym | 7 | `&none` | `KC_NO` | direct  |
| sym | 8 | `&kp CARET` | `KC_CIRC` | direct  |
| sym | 9 | `&kp AMPS` | `KC_AMPR` | direct  |
| sym | 10 | `&kp ASTRK` | `KC_ASTR` | direct  |
| sym | 11 | `&kp LPAR` | `KC_LPRN` | direct  |
| sym | 12 | `&kp RPAR` | `KC_RPRN` | direct  |
| sym | 13 | `&kp BSPC` | `KC_BSPC` | direct  |
| sym | 14 | `&kp TILDE` | `KC_TILD` | direct  |
| sym | 15 | `&kp LCTRL` | `KC_LCTL` | direct  |
| sym | 16 | `&kp LGUI` | `KC_LGUI` | direct  |
| sym | 17 | `&kp LALT` | `KC_LALT` | direct  |
| sym | 18 | `&kp LSHFT` | `KC_LSFT` | direct  |
| sym | 19 | `&none` | `KC_NO` | direct  |
| sym | 20 | `&none` | `KC_NO` | direct  |
| sym | 21 | `&none` | `KC_NO` | direct  |
| sym | 22 | `&kp MINUS` | `KC_MINS` | direct  |
| sym | 23 | `&kp FSLH` | `KC_SLSH` | direct  |
| sym | 24 | `&kp LBKT` | `KC_LBRC` | direct  |
| sym | 25 | `&kp RBKT` | `KC_RBRC` | direct  |
| sym | 26 | `&kp BSLH` | `KC_BSLS` | direct  |
| sym | 27 | `&kp EQUAL` | `KC_EQL` | direct  |
| sym | 28 | `&none` | `KC_NO` | direct  |
| sym | 29 | `&none` | `KC_NO` | direct  |
| sym | 30 | `&none` | `KC_NO` | direct  |
| sym | 31 | `&none` | `KC_NO` | direct  |
| sym | 32 | `&none` | `KC_NO` | direct  |
| sym | 33 | `&tog NUMPAD` | `TG(NUMPAD)` | direct  |
| sym | 34 | `&kp UNDER` | `KC_UNDS` | direct  |
| sym | 35 | `&kp QMARK` | `KC_QUES` | direct  |
| sym | 36 | `&kp LBRC` | `KC_LCBR` | direct  |
| sym | 37 | `&kp RBRC` | `KC_RCBR` | direct  |
| sym | 38 | `&kp PIPE` | `KC_PIPE` | direct  |
| sym | 39 | `&kp PLUS` | `KC_PLUS` | direct  |
| sym | 40 | `&trans` | `KC_TRNS` | direct  |
| sym | 41 | `&trans` | `KC_TRNS` | direct  |
| sym | 42 | `&trans` | `KC_TRNS` | direct  |
| sym | 43 | `&trans` | `KC_TRNS` | direct  |
| sym | 44 | `&trans` | `KC_TRNS` | direct  |
| sym | 45 | `&trans` | `KC_TRNS` | direct  |
| nav | 0 | `&none` | `KC_NO` | direct  |
| nav | 1 | `&kp LG(W)` | `LGUI(KC_W)` | direct  |
| nav | 2 | `&kp LA(LG(LEFT))` | `LALT(LGUI(KC_LEFT))` | direct  |
| nav | 3 | `&kp LG(T)` | `LGUI(KC_T)` | direct  |
| nav | 4 | `&kp LA(LG(RIGHT))` | `LALT(LGUI(KC_RIGHT))` | direct  |
| nav | 5 | `&none` | `KC_NO` | direct  |
| nav | 6 | `&none` | `KC_NO` | direct  |
| nav | 7 | `&none` | `KC_NO` | direct  |
| nav | 8 | `&kp PG_UP` | `KC_PGUP` | direct  |
| nav | 9 | `&kp LG(LEFT)` | `LGUI(KC_LEFT)` | direct  |
| nav | 10 | `&kp LG(RIGHT)` | `LGUI(KC_RIGHT)` | direct  |
| nav | 11 | `&none` | `KC_NO` | direct  |
| nav | 12 | `&none` | `KC_NO` | direct  |
| nav | 13 | `&kp DEL` | `KC_DEL` | direct  |
| nav | 14 | `&none` | `KC_NO` | direct  |
| nav | 15 | `&kp LCTRL` | `KC_LCTL` | direct  |
| nav | 16 | `&kp LGUI` | `KC_LGUI` | direct  |
| nav | 17 | `&kp LALT` | `KC_LALT` | direct  |
| nav | 18 | `&kp LSHFT` | `KC_LSFT` | direct  |
| nav | 19 | `&none` | `KC_NO` | direct  |
| nav | 20 | `&none` | `KC_NO` | direct  |
| nav | 21 | `&none` | `KC_NO` | direct  |
| nav | 22 | `&kp PG_DN` | `KC_PGDN` | direct  |
| nav | 23 | `&kp LEFT` | `KC_LEFT` | direct  |
| nav | 24 | `&kp DOWN` | `KC_DOWN` | direct  |
| nav | 25 | `&kp UP` | `KC_UP` | direct  |
| nav | 26 | `&kp RIGHT` | `KC_RIGHT` | direct  |
| nav | 27 | `&kp BSPC` | `KC_BSPC` | direct  |
| nav | 28 | `&kp LS(LG(Z))` | `LSFT(LGUI(KC_Z))` | direct  |
| nav | 29 | `&kp LG(Z)` | `LGUI(KC_Z)` | direct  |
| nav | 30 | `&kp LG(X)` | `LGUI(KC_X)` | direct  |
| nav | 31 | `&kp LG(C)` | `LGUI(KC_C)` | direct  |
| nav | 32 | `&kp LG(V)` | `LGUI(KC_V)` | direct  |
| nav | 33 | `&kp SPACE` | `KC_SPC` | direct  |
| nav | 34 | `&kp LG(A)` | `LGUI(KC_A)` | direct  |
| nav | 35 | `&kp LS(LA(LEFT))` | `LSFT(LALT(KC_LEFT))` | direct  |
| nav | 36 | `&kp LA(LEFT)` | `LALT(KC_LEFT)` | direct  |
| nav | 37 | `&kp LA(RIGHT)` | `LALT(KC_RIGHT)` | direct  |
| nav | 38 | `&kp LS(LA(RIGHT))` | `LSFT(LALT(KC_RIGHT))` | direct  |
| nav | 39 | `&none` | `KC_NO` | direct  |
| nav | 40 | `&trans` | `KC_TRNS` | direct  |
| nav | 41 | `&trans` | `KC_TRNS` | direct  |
| nav | 42 | `&trans` | `KC_TRNS` | direct  |
| nav | 43 | `&trans` | `KC_TRNS` | direct  |
| nav | 44 | `&trans` | `KC_TRNS` | direct  |
| nav | 45 | `&trans` | `KC_TRNS` | direct  |
| win | 0 | `&none` | `KC_NO` | direct  |
| win | 1 | `&kp LC(LA(D))` | `LCTL(LALT(KC_D))` | direct  |
| win | 2 | `&kp LC(LA(F))` | `LCTL(LALT(KC_F))` | direct  |
| win | 3 | `&kp LC(LA(G))` | `LCTL(LALT(KC_G))` | direct  |
| win | 4 | `&kp LC(LA(U))` | `LCTL(LALT(KC_U))` | direct  |
| win | 5 | `&kp LC(LA(I))` | `LCTL(LALT(KC_I))` | direct  |
| win | 6 | `&none` | `KC_NO` | direct  |
| win | 7 | `&none` | `KC_NO` | direct  |
| win | 8 | `&kp LC(LA(U))` | `LCTL(LALT(KC_U))` | direct  |
| win | 9 | `&kp LC(LA(I))` | `LCTL(LALT(KC_I))` | direct  |
| win | 10 | `&kp LC(LA(D))` | `LCTL(LALT(KC_D))` | direct  |
| win | 11 | `&kp LC(LA(F))` | `LCTL(LALT(KC_F))` | direct  |
| win | 12 | `&kp LC(LA(G))` | `LCTL(LALT(KC_G))` | direct  |
| win | 13 | `&none` | `KC_NO` | direct  |
| win | 14 | `&none` | `KC_NO` | direct  |
| win | 15 | `&kp LC(LA(LEFT))` | `LCTL(LALT(KC_LEFT))` | direct  |
| win | 16 | `&kp LC(LA(DOWN))` | `LCTL(LALT(KC_DOWN))` | direct  |
| win | 17 | `&kp LC(LA(UP))` | `LCTL(LALT(KC_UP))` | direct  |
| win | 18 | `&kp LC(LA(RIGHT))` | `LCTL(LALT(KC_RIGHT))` | direct  |
| win | 19 | `&kp LC(LA(RET))` | `LCTL(LALT(KC_ENT))` | direct  |
| win | 20 | `&none` | `KC_NO` | direct  |
| win | 21 | `&none` | `KC_NO` | direct  |
| win | 22 | `&kp LC(LA(RET))` | `LCTL(LALT(KC_ENT))` | direct  |
| win | 23 | `&kp LC(LA(LEFT))` | `LCTL(LALT(KC_LEFT))` | direct  |
| win | 24 | `&kp LC(LA(DOWN))` | `LCTL(LALT(KC_DOWN))` | direct  |
| win | 25 | `&kp LC(LA(UP))` | `LCTL(LALT(KC_UP))` | direct  |
| win | 26 | `&kp LC(LA(RIGHT))` | `LCTL(LALT(KC_RIGHT))` | direct  |
| win | 27 | `&none` | `KC_NO` | direct  |
| win | 28 | `&none` | `KC_NO` | direct  |
| win | 29 | `&kp LC(LA(E))` | `LCTL(LALT(KC_E))` | direct  |
| win | 30 | `&kp LC(LA(R))` | `LCTL(LALT(KC_R))` | direct  |
| win | 31 | `&kp LC(LA(T))` | `LCTL(LALT(KC_T))` | direct  |
| win | 32 | `&kp LC(LA(J))` | `LCTL(LALT(KC_J))` | direct  |
| win | 33 | `&kp LC(LA(K))` | `LCTL(LALT(KC_K))` | direct  |
| win | 34 | `&kp LC(LA(J))` | `LCTL(LALT(KC_J))` | direct  |
| win | 35 | `&kp LC(LA(K))` | `LCTL(LALT(KC_K))` | direct  |
| win | 36 | `&kp LC(LA(E))` | `LCTL(LALT(KC_E))` | direct  |
| win | 37 | `&kp LC(LA(R))` | `LCTL(LALT(KC_R))` | direct  |
| win | 38 | `&kp LC(LA(T))` | `LCTL(LALT(KC_T))` | direct  |
| win | 39 | `&none` | `KC_NO` | direct  |
| win | 40 | `&trans` | `KC_TRNS` | direct  |
| win | 41 | `&trans` | `KC_TRNS` | direct  |
| win | 42 | `&trans` | `KC_TRNS` | direct  |
| win | 43 | `&trans` | `KC_TRNS` | direct  |
| win | 44 | `&trans` | `KC_TRNS` | direct  |
| win | 45 | `&trans` | `KC_TRNS` | direct  |
| short | 0 | `&none` | `CW_TOGG` | documented-addition Separate Caps Word toggle |
| short | 1 | `&none` | `KC_NO` | direct  |
| short | 2 | `&none` | `KC_NO` | direct  |
| short | 3 | `&none` | `KC_NO` | direct  |
| short | 4 | `&none` | `KC_NO` | direct  |
| short | 5 | `&none` | `KC_NO` | direct  |
| short | 6 | `&none` | `KC_NO` | direct  |
| short | 7 | `&none` | `KC_NO` | direct  |
| short | 8 | `&none` | `KC_NO` | direct  |
| short | 9 | `&none` | `KC_NO` | direct  |
| short | 10 | `&none` | `KC_NO` | direct  |
| short | 11 | `&none` | `KC_NO` | direct  |
| short | 12 | `&none` | `KC_NO` | direct  |
| short | 13 | `&kp LC(LG(Q))` | `LCTL(LGUI(KC_Q))` | direct  |
| short | 14 | `&none` | `LGUI(KC_SPC)` | documented-addition Separate Cmd+Space shortcut; base position holds Hyper |
| short | 15 | `&none` | `KC_NO` | direct  |
| short | 16 | `&none` | `KC_NO` | direct  |
| short | 17 | `&kp LS(LG(N4))` | `LSFT(LGUI(KC_4))` | direct  |
| short | 18 | `&kp LC(LS(LG(N4)))` | `LCTL(LSFT(LGUI(KC_4)))` | direct  |
| short | 19 | `&none` | `KC_NO` | direct  |
| short | 20 | `&none` | `KC_NO` | direct  |
| short | 21 | `&none` | `KC_NO` | direct  |
| short | 22 | `&none` | `KC_NO` | direct  |
| short | 23 | `&kp C_VOL_DN` | `KC_VOLD` | direct  |
| short | 24 | `&kp C_MUTE` | `KC_MUTE` | direct  |
| short | 25 | `&kp C_VOL_UP` | `KC_VOLU` | direct  |
| short | 26 | `&none` | `KC_NO` | direct  |
| short | 27 | `&none` | `KC_NO` | direct  |
| short | 28 | `&none` | `KC_NO` | direct  |
| short | 29 | `&kp LS(LG(O))` | `LSFT(LGUI(KC_O))` | direct  |
| short | 30 | `&kp LS(LG(M))` | `LSFT(LGUI(KC_M))` | direct  |
| short | 31 | `&kp LS(LG(K))` | `LSFT(LGUI(KC_K))` | direct  |
| short | 32 | `&none` | `KC_NO` | direct  |
| short | 33 | `&kp LS(LG(H))` | `LSFT(LGUI(KC_H))` | direct  |
| short | 34 | `&none` | `KC_NO` | direct  |
| short | 35 | `&kp C_VOICE_COMMAND` | `KC_NO` | unsupported-hid-usage  |
| short | 36 | `&kp LC(LG(SPACE))` | `LCTL(LGUI(KC_SPC))` | direct  |
| short | 37 | `&none` | `KC_NO` | direct  |
| short | 38 | `&none` | `KC_NO` | direct  |
| short | 39 | `&none` | `KC_NO` | direct  |
| short | 40 | `&trans` | `KC_TRNS` | direct  |
| short | 41 | `&trans` | `KC_TRNS` | direct  |
| short | 42 | `&trans` | `KC_TRNS` | direct  |
| short | 43 | `&trans` | `KC_TRNS` | direct  |
| short | 44 | `&trans` | `KC_TRNS` | direct  |
| short | 45 | `&trans` | `KC_TRNS` | direct  |
| numpad | 0 | `&none` | `KC_NO` | direct  |
| numpad | 1 | `&none` | `KC_NO` | direct  |
| numpad | 2 | `&none` | `KC_NO` | direct  |
| numpad | 3 | `&none` | `KC_NO` | direct  |
| numpad | 4 | `&none` | `KC_NO` | direct  |
| numpad | 5 | `&none` | `KC_NO` | direct  |
| numpad | 6 | `&none` | `KC_NO` | direct  |
| numpad | 7 | `&none` | `KC_NO` | direct  |
| numpad | 8 | `&kp FSLH` | `KC_SLSH` | direct  |
| numpad | 9 | `&kp N7` | `KC_7` | direct  |
| numpad | 10 | `&kp N8` | `KC_8` | direct  |
| numpad | 11 | `&kp N9` | `KC_9` | direct  |
| numpad | 12 | `&kp MINUS` | `KC_MINS` | direct  |
| numpad | 13 | `&kp EQUAL` | `KC_EQL` | direct  |
| numpad | 14 | `&none` | `KC_NO` | direct  |
| numpad | 15 | `&kp LCTRL` | `KC_LCTL` | direct  |
| numpad | 16 | `&kp LGUI` | `KC_LGUI` | direct  |
| numpad | 17 | `&kp LALT` | `KC_LALT` | direct  |
| numpad | 18 | `&kp LSHFT` | `KC_LSFT` | direct  |
| numpad | 19 | `&none` | `KC_NO` | direct  |
| numpad | 20 | `&none` | `KC_NO` | direct  |
| numpad | 21 | `&none` | `KC_NO` | direct  |
| numpad | 22 | `&kp ASTRK` | `KC_ASTR` | direct  |
| numpad | 23 | `&kp N4` | `KC_4` | direct  |
| numpad | 24 | `&kp N5` | `KC_5` | direct  |
| numpad | 25 | `&kp N6` | `KC_6` | direct  |
| numpad | 26 | `&kp PLUS` | `KC_PLUS` | direct  |
| numpad | 27 | `&kp RET` | `KC_ENT` | direct  |
| numpad | 28 | `&none` | `KC_NO` | direct  |
| numpad | 29 | `&none` | `KC_NO` | direct  |
| numpad | 30 | `&none` | `KC_NO` | direct  |
| numpad | 31 | `&none` | `KC_NO` | direct  |
| numpad | 32 | `&none` | `KC_DOT` | documented-addition Separate dot beside the existing NUMPAD toggle |
| numpad | 33 | `&tog NUMPAD` | `TG(NUMPAD)` | direct  |
| numpad | 34 | `&kp N0` | `KC_0` | direct  |
| numpad | 35 | `&kp N1` | `KC_1` | direct  |
| numpad | 36 | `&kp N2` | `KC_2` | direct  |
| numpad | 37 | `&kp N3` | `KC_3` | direct  |
| numpad | 38 | `&mt DOT COMMA` | `KC_COMM` | split-interim  |
| numpad | 39 | `&kp LS(SEMI)` | `LSFT(KC_SCLN)` | direct  |
| numpad | 40 | `&trans` | `KC_TRNS` | direct  |
| numpad | 41 | `&trans` | `KC_TRNS` | direct  |
| numpad | 42 | `&trans` | `KC_TRNS` | direct  |
| numpad | 43 | `&trans` | `KC_TRNS` | direct  |
| numpad | 44 | `&trans` | `KC_TRNS` | direct  |
| numpad | 45 | `&trans` | `KC_TRNS` | direct  |
| sys | 0 | `&none` | `KC_NO` | direct  |
| sys | 1 | `&bt BT_SEL 0` | `KC_NO` | omitted-platform-specific  |
| sys | 2 | `&bt BT_SEL 1` | `KC_NO` | omitted-platform-specific  |
| sys | 3 | `&bt BT_SEL 2` | `KC_NO` | omitted-platform-specific  |
| sys | 4 | `&bt BT_SEL 3` | `KC_NO` | omitted-platform-specific  |
| sys | 5 | `&bt BT_SEL 4` | `KC_NO` | omitted-platform-specific  |
| sys | 6 | `&none` | `KC_NO` | direct  |
| sys | 7 | `&none` | `KC_NO` | direct  |
| sys | 8 | `&none` | `KC_NO` | direct  |
| sys | 9 | `&none` | `KC_NO` | direct  |
| sys | 10 | `&none` | `KC_NO` | direct  |
| sys | 11 | `&none` | `KC_NO` | direct  |
| sys | 12 | `&none` | `KC_NO` | direct  |
| sys | 13 | `&kp C_PWR` | `KC_NO` | unsupported-hid-usage  |
| sys | 14 | `&bt BT_CLR` | `KC_NO` | omitted-platform-specific  |
| sys | 15 | `&ext_power EP_TOG` | `KC_NO` | omitted-platform-specific  |
| sys | 16 | `&none` | `KC_NO` | direct  |
| sys | 17 | `&none` | `KC_NO` | direct  |
| sys | 18 | `&none` | `KC_NO` | direct  |
| sys | 19 | `&none` | `KC_NO` | direct  |
| sys | 20 | `&none` | `KC_NO` | direct  |
| sys | 21 | `&none` | `KC_NO` | direct  |
| sys | 22 | `&rgb_ug RGB_HUD` | `RM_HUED` | approximate  |
| sys | 23 | `&rgb_ug RGB_HUI` | `RM_HUEU` | approximate  |
| sys | 24 | `&rgb_ug RGB_SAD` | `RM_SATD` | approximate  |
| sys | 25 | `&rgb_ug RGB_SAI` | `RM_SATU` | approximate  |
| sys | 26 | `&rgb_ug RGB_BRD` | `RM_VALD` | approximate  |
| sys | 27 | `&rgb_ug RGB_BRI` | `RM_VALU` | approximate  |
| sys | 28 | `&sys_reset` | `QK_RBT` | approximate  |
| sys | 29 | `&bootloader` | `QK_BOOT` | approximate  |
| sys | 30 | `&none` | `EE_CLR` | documented-addition Left EEPROM clear behind held SYS |
| sys | 31 | `&none` | `KC_NO` | direct  |
| sys | 32 | `&none` | `KC_NO` | direct  |
| sys | 33 | `&studio_unlock` | `KC_NO` | omitted-platform-specific  |
| sys | 34 | `&rgb_ug RGB_TOG` | `RM_TOGG` | approximate  |
| sys | 35 | `&rgb_ug RGB_EFF` | `RM_NEXT` | approximate  |
| sys | 36 | `&none` | `KC_NO` | direct  |
| sys | 37 | `&none` | `EE_CLR` | documented-addition Right EEPROM clear behind held SYS |
| sys | 38 | `&bootloader` | `QK_BOOT` | approximate  |
| sys | 39 | `&sys_reset` | `QK_RBT` | approximate  |
| sys | 40 | `&none` | `KC_NO` | direct  |
| sys | 41 | `&none` | `KC_NO` | direct  |
| sys | 42 | `&none` | `KC_NO` | direct  |
| sys | 43 | `&none` | `KC_NO` | direct  |
| sys | 44 | `&none` | `KC_NO` | direct  |
| sys | 45 | `&none` | `KC_NO` | direct  |
