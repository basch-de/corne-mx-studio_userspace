# Quellenanalyse und Kompatibilität

Prüfdatum: 08.11.2026. Alle Aussagen zu Keebart beziehen sich auf
[`703a62b32a5bf516ef54c6d131d7377064bbafc7`](https://github.com/Keebart/vial-qmk-keebart/tree/703a62b32a5bf516ef54c6d131d7377064bbafc7),
aus Branch `feature/keebart-studio-layer-rgb`. Keine Übernahme vom Vial-Branch.

Buildstrategie inzwischen geändert: Jeder neue CI-Lauf löst den aktuellen
Branchstand einmal auf und verwendet ihn für alle drei Varianten. Diese Analyse
und `sources/provenance.json` bleiben der historische Prüfstand am oben genannten
Commit. Die Quellenprüfung akzeptiert neue Revisionen, prüft jedoch weiterhin
Herstellerdateien, Identität und Layout; sie läuft nur im persönlichen Build.
Ein erfolgreicher Check ersetzt bei geändertem Herstellercode keinen erneuten
Hook-Audit und keinen Firmware-/Hardwaretest.

## Quellen und Architektur

ZMK: lokaler, sauberer Checkout `basch-de/zmk-config` am Commit
`17dc18d4065161066a459656a87f36f11cd4ae88`. Verbindliche Dateien sind
`config/corne_choc_pro.keymap`, `.conf`, `config/west.yml` (ZMK `v0.3`) und die
zugehörigen Board-Transforms/Physical Layouts. Keymap, Konfiguration und Layout
liegen als unveränderter Analysesnapshot unter `sources/`; Hashes stehen im
Manifest. Die Includes sind Standard-ZMK-Behaviors, HID-Tastencodes, Bluetooth,
RGB und externe Stromversorgung. Alle sechs lokalen Behavior-Definitionen und
alle acht Layer wurden ausgewertet. Kein fremdes Sofle-Layout übernommen.

Die vorhandenen `qmk_userspace` und `corne-mx-vial_userspace` wurden ausschließlich
gelesen. Das erste enthält bereits eine lokale Änderung an der Preonic-Keymap;
diese wurde nicht angefasst. Die eigene Tap-Hold-Implementierung des Vial-
Referenzprojekts wurde nicht übernommen.

External Userspace ist tatsächlich unterstützt: `lib/python/qmk/userspace.py`
erkennt `QMK_USERSPACE` / `user.overlay_dir` und `qmk.json`; in
`builddefs/build_keyboard.mk` werden externe Keymap-Dateien bevorzugt. Die
Firmware wird laut `builddefs/common_rules.mk` auch ins Userspace kopiert.
Es ist kein Firmware-Fork und keine Änderung an Boarddefinitionen erforderlich.

`keyboards/keebart/corne_mx_pro/info.json`, `standard/keyboard.json`, `config.h`
und `rules.mk` bestätigen RP2040, Split, Standard-Variante, RGB Matrix mit 46
LEDs und `LAYOUT_split_3x6_3_ex2`. Die Definitionen bleiben vollständig extern
im Herstellercheckout. Die ursprüngliche `keymaps/studio` ist Ausgangspunkt;
`rules.mk` und `vial.json` werden bytegleich übernommen, `config.h` lediglich um
Kommentare und `TAPPING_TERM 250` ergänzt.

## Physische Positionen

ZMK-Transform und QMK-Layout entsprechen derselben physischen Reihenfolge;
die rechte elektrische Matrix ist gespiegelt, nicht die logische Belegung.

| Logische Positionen | QMK-Matrix in dieser Reihenfolge |
|---|---|
| 0–6 | Reihe 0, Spalten 0–6 |
| 7–13 | Reihe 4, Spalten 6–0 |
| 14–20 | Reihe 1, Spalten 0–6 |
| 21–27 | Reihe 5, Spalten 6–0 |
| 28–33 | Reihe 2, Spalten 0–5 |
| 34–39 | Reihe 6, Spalten 5–0 |
| 40–42 | Reihe 3, Spalten 3–5 |
| 43–45 | Reihe 7, Spalten 5–3 |

Jeder der acht Layer besitzt 46 Argumente. Die an der Choc durch Displays
blockierten Positionen 6, 7, 20, 21 bleiben `KC_NO`; damit werden keine neuen
MX-Zusatzschalter ungefragt belegt. Die Hersteller-Encoderoptionen bleiben
aktiv. Alle vier Encoder haben auf den acht kompilierten Layern dieselben
RGB-Aktionen wie die Baseline. Die Zahl der Encoder-Layer muss wegen
`quantum/keymap_introspection.c` der Zahl der kompilierten Keymap-Layer gleichen.
Nicht kompilierte Layer 8–15 liefern beim Initialisieren `KC_TRNS`.

## Tatsächlich verwendete Funktionen

Die Einzelbelegung einschließlich jeder Ergänzung und Weglassung steht in
[MAPPING.md](MAPPING.md); maschinenlesbar in `mapping.json`.

| ZMK-Funktion | Umsetzung | Einordnung |
|---|---|---|
| `&kp` Buchstaben, Zahlen, F1–F12, Navigation | QMK `KC_*` | direkt |
| `LC`, `LA`, `LG`, `LS`, verschachtelte Shortcuts | QMK `LCTL`, `LALT`, `LGUI`, `LSFT` | direkt auf HID-Ebene |
| Sonderzeichen, `LS(SEMI)` | US-HID-Codes und Shift | direkt; Hostlayout muss passen |
| `&none`, `&trans` | `KC_NO`, `KC_TRNS` | direkt |
| `&tog NUMPAD` | `TG(NUMPAD)` an beiden Originalpositionen | direkt |
| `hml`, `hmr` | Mod-Tap und Standard-Chordal-Datentabelle | Näherung; Timing/Grenzfälle abweichend |
| `hlt`, `rhlt`, `rlt`, `lt` | Layer-Tap | Näherung; globale Vial-Einstellungen |
| `&mt MEH SQT` | `MEH_T(KC_QUOT)` | Aktionen direkt, Timing abweichend |
| Hyper/Cmd+Space | getrennt; Studio Compound Action unterstützt Kombination | UF2-Zwischenlösung |
| `lcw` | SHORT momentan + separates `CW_TOGG` | UF2-Zwischenlösung; Toggle statt Enable |
| `&mt DOT COMMA` | separate Komma-/Punkt-Tasten | UF2-Zwischenlösung |
| Volume/Mute | `KC_VOLD`, `KC_VOLU`, `KC_MUTE` | direkt; Hostwirkung noch offen |
| `C_VOICE_COMMAND` | `KC_NO` | kein identischer Standardcode nachgewiesen |
| `C_PWR` | `KC_NO` | Consumer Power ist nicht QMK System Power |
| Bluetooth-Slots, BT Clear | weggelassen | kabelgebundenes Ziel |
| `ext_power` | weggelassen | keine identische Stromversorgung |
| `studio_unlock` | weggelassen | ZMK Studio ist ein anderes Protokoll |
| RGB hue/saturation/brightness, Toggle, Effekt | RGB-Matrix-Codes `RM_*` | Näherung; Hersteller-RGB bleibt |
| `sys_reset`, `bootloader` | `QK_RBT`, `QK_BOOT` | Wirkung auf USB-Master beachten |

Der tatsächliche NUM-Daumen ist **Backspace**, WIN ist **Tab**; entgegen der
vorläufigen Beschreibung im Auftrag wird die tatsächliche Keymap übernommen.
`win` sendet Ctrl+Alt-Shortcuts eines Window-Managers; es handelt sich nicht um
eine zweite Windows-Alpha-Grundbelegung. Reguläre Ziffern bleiben reguläre
Ziffern, auch im NUMPAD: keine Abhängigkeit von Num Lock.

[ZMK v0.3 keys.h](https://github.com/zmkfirmware/zmk/blob/v0.3/app/include/dt-bindings/zmk/keys.h)
definiert Voice Command und Consumer Power auf der Consumer-Usage-Page.
Keebarts `tmk_core/protocol/report.h` sendet für `KC_ASST` stattdessen AL
Assistant `0x1CB` und für `KC_PWR` System Power Down `0x81` auf Generic Desktop.
Diese ähnlichen Namen rechtfertigen keine Gleichsetzung.

## Studio und Vial – mehr als Buildflags

`studio_rgb.mk` aktiviert nur bei `KEYMAP=studio` zusätzlich Pointer,
Tap Dance, Gerätename, Locales/Compound Actions, Makro-VM und Makrospeicherung.
Ein persönlicher Keymapname wie `basch_de` würde diese Module verlieren.
`studio_test.mk` ergänzt den vorhandenen Encoder-/Diagnose-Endpunkt.

| Bereich | Quellcodebefund / Zuständigkeit |
|---|---|
| VIA/Vial | originale `VIA_ENABLE`, `VIAL_ENABLE`, `VIALRGB_ENABLE`; `quantum/via.c`, `vial.c`, `dynamic_keymap.c` bleiben aktiv |
| Identität | Hersteller-Vial-UID `89 36 2A C7 FA D8 89 45`; originales JSON, Matrix 8×7; VID/PID unverändert |
| Physischer Unlock | Originalpositionen Matrix `(0,0)` + `(0,1)`, Originalzähler 25; `VIAL_INSECURE` wird nicht gesetzt |
| Layer | Board fordert 16; `studio_rgb_qmk.c` prüft `DYNAMIC_KEYMAP_LAYER_COUNT == KSR_LAYERS`; kein Reduzieren auf acht |
| Studio RGB | Raw-HID-Protokoll mit `KSR_COMMAND`, Modell-ID `0x0204`, 46 LEDs, Paletten und Split-RPC in Originalmodulen |
| Test/Pointer/Name/Locales/Makros | originale Quellen und Linker-Reservierungen aus `studio_rgb.mk`, ohne Userspace-Ersatz |
| Vial Settings | `qmk_settings.c`: globale Einstellungen persistent und über Protokoll abfragbar/setzbar |
| Timing | Vial besitzt Tapping Term, Quick Tap, Permissive Hold, Hold-on-other-press, Chordal Hold, Retro und Flow Tap; GUI-Unterstützung der konkret installierten App noch testen |
| Studio Dual Role | `locales/locale*.c`: 32 Compound Actions, eigene Term-/Quick-/Flow-Werte; `ksl_compound_key()` erlaubt u.a. modifizierte Codes, `MO` und `CW_TOGG` |
| Studio/Vial Tap Dance | `studio_dance.c` liest dieselben dynamischen Vial-Einträge; Herstellerengine bleibt vollständig bestehen |
| Persistenz | Vial-EEPROM, Studio-Paletten- und Makrobereiche, reservierte Linkerbereiche und ABI-Assertions unverändert |
| Darstellung | ausschließlich bekannte QMK-Codes in der Portierung; eigene Custom-Keycodes fehlen; korrekte Labels für alle MT/LT/16 Layer erfordern GUI-Test |

Firmwarefähigkeit und erfolgreiche Kommunikation mit einer konkreten
Studio-/Vial-Version sind getrennte Nachweise. Es wurde kein HID-Gerät geöffnet.
Keine Behauptung vollständiger Laufzeitkompatibilität vor Build und Gerätetest.

Die Studio-Compound-Defaults sind in `ksl_stock()` leer. Vial löscht dynamische
Tap-Dance-Einträge beim Standardreset auf leere Aktionen. Es gibt hier keinen
verwendeten deklarativen Userspace-Mechanismus für eigene persistente Defaults.
Deshalb enthalten die UF2-Defaults keine funktionslosen `TD(0)` oder
`0xC000`-Platzhalter. Eine manuelle spätere Studio-Konfiguration verwendet
vorhandene Herstellerfunktionen und braucht keine Sonderimplementierung.

## Tap-Hold und sichere Startwerte

Die QMK-Standardkeycodes haben eine 8-Bit-Tap-Action; daher passen `Cmd+Space`
und `CW_TOGG` nicht direkt in `MT` / `LT`. Das ist auch in der
[QMK-Mod-Tap-Dokumentation](https://docs.qmk.fm/mod_tap) beschrieben.
Die Regeln für Quick Tap und Unterbrechungen müssen unabhängig von ZMK bewertet
werden; siehe [QMK Tap-Hold](https://docs.qmk.fm/tap_hold).

Für frisch initialisierte Defaults setzt dieser Userspace `TAPPING_TERM=250`:
ein bewusst gewählter Kompromiss zwischen 280-ms-HRMs und 200-ms-Layer-Taps.
Wichtig: `qmk_settings_reset()` ignoriert einen eigenen `QUICK_TAP_TERM` als
Initialwert und setzt **Quick Tap ebenfalls auf TAPPING_TERM**, also 250 ms.
Der Bitmaskenwert für alle Hold-Optionen startet bei null. Compile-Defines wie
`PERMISSIVE_HOLD` ersetzen die Vial-Laufzeiteinstellungen nicht.
Bestehende persistente Einstellungen können ohnehin abweichende Werte liefern.

Späterer dokumentierter Versuchsstart über vorhandene Einstellungen, noch nicht
auf ein Gerät geschrieben: Tapping 250 ms, Quick Tap 200 ms, Permissive Hold an,
Chordal Hold an, Hold-on-other-press aus, Retro aus, Flow Tap 0.
Diese globalen Werte betreffen auch Layer-Taps und MEH; sie sind keine
ZMK-Flavor-Emulation. Die statische Standard-QMK-Handtabelle definiert beide
Hände korrekt und alle sechs Daumen als `*` (neutrale Hold-Trigger). Sie
implementiert keine Funktion und keinen Callback. Ihre Filterwirkung beginnt
erst, wenn Chordal Hold in den vorhandenen Vial-Einstellungen aktiviert wird.
Studio Compound Actions verwenden dagegen die separate Herstellerengine und
deren konfigurierbare Ausnahmen; die QMK-Tabelle wirkt dort nicht automatisch.

## Audit der Erweiterungspunkte

Es wird **kein** eigener Hook verwendet, auch keiner aus dieser Tabelle.

| Erweiterungspunkt | Besitzer im tatsächlich verwendeten Pfad | Konsequenz |
|---|---|---|
| `get_tapping_term` | starke Definition in `quantum/vial.c` | eigene Definition kollidiert / verliert Tap-Dance- und QS-Anbindung |
| `get_quick_tap_term` | starke Definition in `quantum/qmk_settings.c` | nicht ersetzen |
| `get_chordal_hold` | starke Definition in `qmk_settings.c` | nicht ersetzen; Datentabelle ist vorgesehener Standardweg |
| `get_permissive_hold`, `get_hold_on_other_key_press`, `get_retro_tapping`, `get_flow_tap_term` | `qmk_settings.c` | nicht ersetzen |
| `pre_process_record_user` | `studio_rgb_qmk.c`, Studio Test/Makros | nicht ersetzen |
| `process_record_user` | `studio_rgb_qmk.c`, Studio Tap Dance | nicht ersetzen |
| `raw_hid_receive_kb` | Studio-Protokolladapter | nicht ersetzen |
| `keyboard_post_init_kb`, `housekeeping_task_kb` | Studio-Adapter, RPC und Tasks | nicht ersetzen |
| `notify_usb_device_state_change_user` | `studio_locale.c` | nicht ersetzen |
| `matrix_scan_user`, `layer_state_set_user` | QMK schwache Standardpfade; keine eigene Studio-Definition in den eingebundenen Modulen gefunden | erste Phase verbietet auch technisch freie Hooks; keine Freigabe abgeleitet |

`common/oled_split.h` besitzt ebenfalls User-Hooks, wird aber von dieser
Studio-Keymap nicht eingebunden. Ein Kopieren anderer Keymap-Includes wäre
somit keine nachgewiesene konfliktfreie Verkettung. Eine eventuelle künftige
Erweiterung erfordert einen erneuten Audit des gesamten aktiven Aufrufpfads.

## SYS, Split und EEPROM

SYS wird links über `LT(SYS, KC_GRV)` und rechts zusätzlich über
`LT(SYS, KC_BSLS)` erreicht. Auf jeder Hälfte liegen in der unteren Zeile von
außen nach innen Neustart, Bootloader und EEPROM Clear. Es gibt keinen SYS-
Toggle und keine automatische Layerregel. Der Mindestschutz besteht aus dem
bewussten Halten des Zugangs plus einer separaten Taste. EE_CLR ist kein
langes Halten und nicht zusätzlich durch Vial-Unlock geschützt.

QMK verarbeitet reguläre Tastenevents im USB-Master (`quantum/keyboard.c` und
`split_common/split_util.c`). `quantum.c` führt Reset/Boot/EEPROM dort aus;
die physische rechte Taste adressiert nicht automatisch den rechten RP2040.
Einzeln angeschlossene Hälften können laut USB-Mastererkennung und Hand-Pin
jeweils Master werden. Rechts bleibt die rechte Matrixhälfte erhalten, daher
ist ein eigener SYS-Zugang dort nötig. Die reale Einhandfunktion ist ungetestet.

`EE_CLR` ist kein vollständiger Studio-Factory-Reset: QMK invalidiert
Einstellungen und startet neu; Studio besitzt daneben einen eigenen physisch
entsperrten Resetablauf (`studio_reset.c`) für seine Bereiche und den Peer.
Diese Herstellerfunktion bleibt unverändert und wird nicht auf EE_CLR umgelegt.

Die ursprüngliche `VIA_EEPROM_BUILD_ID=0x4B0201u` bleibt erhalten. Gültige,
passende persistente Belegungen können die kompilierten Defaults überlagern;
bei ungültiger Kennung initialisiert **die Herstellerfirmware selbst** Defaults.
Ein späteres Flash ist deshalb keine Zusicherung eines EEPROM-erhaltenden
Übergangs von einer unbekannten bisherigen Firmware. Hier wurde nur Quellcode
geändert und geprüft, kein Gerät angesprochen.

## Konfigurationswerte der ZMK-`.conf`

Der Bluetoothname, BLE, Idle/Deep Sleep und externe Stromversorgung werden
nicht auf den kabelgebundenen RP2040 übertragen. Die auskommentierte
Pointing-/Smooth-Scrolling-/BIOS-Konfiguration aktiviert dort nichts.
Display-Wachhalten sowie 5-Minuten-Idle / 30-Minuten-Sleep sind keine
QMK-identischen Einstellungen. Keebarts originale OLED-, Pointer- und
USB-Funktionen bleiben erhalten. RGB-Startfarbe, 20-%-Helligkeit, 98-%-Sättigung,
Hue 200, Helligkeitsschritt 5 und Idle-Abschaltung werden nicht blind in andere
Skalen übersetzt: Herstellerdefaults und persistente Studio-Paletten bleiben
maßgeblich. `RGB_MATRIX_SLEEP` schaltet bei Host-Suspend, nicht automatisch
nach dem ZMK-Idle-Timer; Studio bietet separat konfigurierbares Lighting-Idle.
