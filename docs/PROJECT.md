# Technische Hinweise und Prüfstand

Persönlicher QMK External Userspace für `keebart/corne_mx_pro/standard`:
Corne MX Pro, kabelgebunden, zwei RP2040, 46 nutzbare Positionen, keine Displays.
Die Belegung wird direkt in `keymap.c` gepflegt. Historische ZMK-Kopien,
Belegungsgenerator, Mapping-JSON und feste Hersteller-Dateihashes wurden entfernt.
Buildanleitung und Übersicht stehen in der [README](../README.md).

## Dateien und Herstellerfunktionen

Die persönliche Keymap liegt unter
`keyboards/keebart/corne_mx_pro/keymaps/studio/`:

| Datei | Aufgabe |
|---|---|
| `keymap.c` | Direkt gepflegte Tasten-, Encoder- und Chordal-Handbelegung |
| `layers.h` | Gemeinsame Layernamen für Keymap und Initialisierung |
| `defaults.c` | Freigegebene einmalige Initialisierung von Settings und Tap Dances |
| `config.h` | Originale Studio-Konfiguration, ergänzt um `TAPPING_TERM 250` |
| `rules.mk` | Originale Studio-Regeln, ergänzt um `SRC += defaults.c` |
| `vial.json` | Originale Herstellergeometrie für Vial |

Der Keymapname bleibt `studio`: Mehrere Herstellerfunktionen werden nur für
diesen Namen aktiviert. Studio-Module, Pointer, Makros, Profile/Locales,
RGB-Paletten, Encoderdiagnose, Vial-UID, physischer Unlock, 16 dynamische Layer
und reservierte Flashbereiche bleiben erhalten. Die beiden Herstellerbuilds
beziehen ihre Keymaps direkt aus Keebarts Checkout und enthalten `defaults.c`
nicht. Die persönliche Belegung entsprach beim letzten Vergleich allen 368
Positionen des Vial-Referenzprojekts nach Auflösung der Aliase; die Laufzeit-
Engines der beiden Firmwarebranches sind dadurch nicht automatisch identisch.

Die Matrix verwendet `LAYOUT_split_3x6_3_ex2` mit 14 + 14 + 12 + 6 Argumenten.
Die rechte elektrische Matrix ist gespiegelt. Positionen 6, 7, 20 und 21 sind
aktuell unbelegt; die MX kann dort Schalter haben. Alle sechs Daumen sind in
der Chordal-Handtabelle mit `*` ausgenommen. Acht Layer sind vorbelegt, 16
bleiben dynamisch verfügbar. Encoder- und kompilierte Keymap-Layerzahl müssen
gleich sein. NUMPAD nutzt normale Zahlen statt Num-Lock-abhängiger Keypad-Codes.

## Initialisierung und Timing

Die freigegebene Initialisierung verwendet Standard-Vial-APIs und Keebarts
vorhandene Studio-Tap-Dance-Engine. Es gibt keine eigene Record-/Timing-
Verarbeitung und keine eigene Tap-Hold-State-Machine.

| Eintrag | Anfangswert |
|---|---|
| Setting 7: Tapping Term | 250 ms |
| Setting 22: Permissive Hold | an |
| Setting 23: Hold On Other Key Press | aus |
| Setting 26: Chordal Hold | an |
| TD 0 | Tap Cmd+Space, Hold Hyper, Frist 250 ms |
| TD 1 | Tap Caps Word Toggle, Hold SHORT, Frist 250 ms |
| TD 2 | Tap Komma, Hold Punkt, Frist 250 ms |

Die leeren Double-Tap-/Tap-Hold-Felder nutzen die Fallbacks der Studio-Engine.
Eine frühe andere Taste entscheidet beim ersten unaufgelösten Dance für Tap;
Hyper und SHORT erst nach der Hold-Frist verwenden. Globale Permissive-/Chordal-
Einstellungen ändern diese Dance-Regel nicht. Vial speichert Dance-Fristen
separat; spätere globale Timingänderungen ändern sie nicht automatisch.

Quick Tap und Flow Tap werden vom eigenen Initialisierer nicht geschrieben.
Beim VIA-Defaultreset startet Quick Tap mit TAPPING_TERM (250 ms), Flow mit null
und die Hold-Optionen mit null. Anschließend setzt die persönliche Initialisierung
die vier Tabellenwerte. Bei gültigem vorhandenem Speicher können Quick-, Flow-
und weitere GUI-Werte abweichen. Compile-Defines sind kein Ersatz für alle
dynamischen Vial-Settings. Die Chordal-Tabelle wirkt auf QMK/Vial-Mod-/Layer-Taps;
Studios separate Compound-Engine hat eigene Einstellungen.

## Persistenz

Nur der USB-Master initialisiert. Die persönliche User-Kennung ist `0x42535401`,
getrennt vom Vial-Userspace-Marker `0x42530102` und der unveränderten Hersteller-
VIA-EEPROM-Build-ID. Initialisierung erfolgt, wenn VIA vor seinem Start ungültig
war ODER die persönliche Kennung fehlt/abweicht.

Beim ersten Übergang werden damit auch bei gültiger bestehender Konfiguration
vier Settings und TD-Slots 0–2 durch die freigegebenen Defaults ersetzt. Andere
Slots, dynamische Tastenbindungen, Studio-Paletten, Profile, Makros und Gerätename
werden von diesem Code nicht geschrieben oder gelöscht. Ein vorhandenes gültiges
dynamisches Layout kann daher die neue kompilierte Belegung weiterhin überlagern.
Ein Flash allein garantiert keine Übernahme der neuen Tastenbindungen.

Die Kennung wird erst nach allen erfolgreichen Schreibvorgängen gesetzt. Bei
einem Fehler können Teilwerte schon geschrieben sein; ohne vollständige Kennung
wird bei einem späteren Initialisierungsversuch erneut geschrieben. Normale
Neustarts mit passender Kennung erhalten spätere GUI-Werte. Kennungsänderungen
bewusst vornehmen, weil sie die Defaults erneut anwenden. Ein Herstellerreset
kann die Kennung auf null setzen; beim nächsten Start werden die persönlichen
Defaults wieder gesetzt. Der Initialisierer löst selbst keinen Reset aus.

## Erweiterungspunkte und Konfliktprüfung

Geprüfter Herstellerstand am 08.11.2026:
`703a62b32a5bf516ef54c6d131d7377064bbafc7`, Branch
`feature/keebart-studio-layer-rgb`. Der Workflow verwendet weiterhin den jeweils
aktuellen Branchstand, einmal gemeinsam für alle drei Varianten aufgelöst.

| Freigegebener Hook | Nachgewiesene Verkettung |
|---|---|
| `via_init_kb()` | Schwacher Standard in `quantum/via.c`; wird vor VIA-Gültigkeitsprüfung/Defaultinitialisierung aufgerufen; merkt sich nur den Zustand |
| `eeconfig_init_user()` | Schwacher Standard in `quantum/eeconfig.c`; setzt nur den Userwert auf null, wenn QMK den Hook selbst aufruft |
| `keyboard_post_init_user()` | Schwacher Standard in `quantum/keyboard.c`; Studio lädt in `keyboard_post_init_kb()` RGB-Daten, registriert RPC und ruft dann diesen User-Hook auf |

Keine aktive Studio-Quelle definierte diese drei Hooks selbst. `oled_split.h`
enthält zusätzliche Hooks, gehört aber nicht zu diesem Studio-Build. Die
Setting-IDs und `vial_tap_dance_entry_t` entsprechen den Herstellerdefinitionen.
`studio_dance.c` liest dynamische Einträge bei Tastenevents: kein eigener
`tap_dance_actions`-Array und kein Reload der klassischen Vial-Engine erforderlich.

| Weitere Erweiterungspunkte | Zuständigkeit / Einschränkung |
|---|---|
| `get_tapping_term` | Vial; dynamisches Timing und Tap Dance nicht durch eigene Definition ersetzen |
| `get_quick_tap_term`, `get_chordal_hold`, weitere Hold-/Flow-Tap-Callbacks | QMK Settings/Vial; vorhandene Einstellungen nutzen |
| `pre_process_record_user`, `process_record_user` | Studio-Diagnose, Makros und Tap Dance |
| `raw_hid_receive_kb` | Studio-Konfigurationsprotokoll |
| `keyboard_post_init_kb` | Studio-Initialisierung mit Aufruf des User-Hooks |
| `housekeeping_task_kb` | Studio-Tasks; kein Aufruf von `housekeeping_task_user` im geprüften Stand |
| `rgb_matrix_indicators_advanced_kb` | Studio-Paletten/Indikatoren; User-Hook nur ohne Studio-Palettenmodus |
| `notify_usb_device_state_change_user` | Studio-Profile/Betriebssystemerkennung |

Studio verwendet Linker-Wrapper für `mousekey_on`, `mousekey_off`, `mousekey_task`,
`mousekey_clear`, `mousekey_send` und `mousekey_get_report`. Direkte Erweiterungen
in QMK/Vial binden Compound-Aktionen, Keymap-/Encoderauflösung, Makros, Combos,
Key Overrides, RGB und USB-Deskriptoren ein. Nicht definierte User-Hooks sind
keine automatische Freigabe: Auch `matrix_scan_user` und `layer_state_set_user`
brauchen vor eigener Nutzung eine gesonderte Entscheidung und erneute Prüfung.

`scripts/check_sources.py` prüft Herstellerkonfiguration, Geometrie, schwache
Initialisierungshooks, den User-Hook-Aufruf und Settings-IDs. Für mögliche doppelte
Hooks werden die aktuellen Studio-Makefiles, deren Quellen und lokale Includes
gelesen. Bedingte Abschnitte werden konservativ mit erfasst; das ist keine
C-Präprozessoranalyse. Feste Quelldatei-Hashes werden nicht verglichen. Funktionale
Herstelleränderungen erfordern weiterhin fachliche Prüfung und einen Build.

## Verbleibende Unterschiede zu ZMK

| Funktion | Unterschied und sinnvoller nächster Schritt |
|---|---|
| HRM `hml`/`hmr` | ZMK balanced, 280/200 ms und Release-Trigger; hier globale QMK-Settings 250 ms, Permissive/Chordal. Keine exakte Flavor-/Release-Emulation; zunächst Praxis testen |
| `hlt` | ZMK hold-preferred 200 ms; hier normales `LT` mit globalem Timing. Vorhandene Studio Compound Actions wären eine spätere Alternative |
| `rhlt` WIN/Tab | ZMK hold-preferred 200 ms / Quick 250 ms; globale QMK-Regeln unterscheiden sich. Studio Compound Quick Tap wird auf Term begrenzt |
| `rlt` NUMPAD/Enter | ZMK tap-preferred 200 ms / Quick 250 ms; diese getrennte Flavor-Auswahl entfällt bei den normalen Layer-Taps |
| SHORT/Equal und MEH/Quote | Aktionen als Standard-LT/MT vorhanden, andere Timing-/Unterbrechungsdefaults |
| Hyper, Caps/SHORT, Komma/Punkt | Vorhandener Studio Tap Dance statt ZMK-Hold-Tap; frühe Unterbrechung Tap, Fristen 250 ms, Wiederholung hostabhängig |
| Caps Word | `CW_TOGG` statt ZMK-Enable; Toggle, Shift-Invertierung, Fortsetzungs- und Idle-Regeln können abweichen |
| Voice Command / Consumer Power | Bewusst freigegebene Vial-Alternativen `KC_ASST` / `KC_PWR`, andere HID-Aktionen; Hostwirkung offen |
| RGB und Energie | Hersteller-RGB-Matrix/Paletten statt ZMK-Underglow-Defaults; Effekte, Skalen und Idle-/Suspendverhalten unterscheiden sich |
| BLE, externe Versorgung, ZMK Unlock | Für das kabelgebundene Ziel weggelassen; Keebart Studio verwendet ein eigenes Protokoll |

Normale Zeichen und Shortcuts verwenden US-HID-Codes; das Hostlayout muss passen.
QMK-MT/LT können die komplexen Tap-Aktionen Cmd+Space oder Caps Word nicht einfach
in ihrem 8-Bit-Tap-Feld darstellen. Vorhandene Studio-/Vial-Funktionen bleiben
vor eigener Programmierung zu bevorzugen. Exakte eigene ZMK-Nachbildungen würden
weitere Hooks/Verarbeitung benötigen, haben Konfliktrisiken und sind nicht genehmigt.

SYS wird nur links über Grave/SYS erreicht; rechts bleibt reines Backslash.
Mit beiden verbundenen Hälften steuert der linke Zugang auch den rechten
USB-Master. Rechts allein fehlt SYS über die aktuelle Belegung; physische
Bootloader-Taster bleiben verfügbar. Reset, Boot und EEPROM Clear liegen im
SYS-Layer gespiegelt. Die Aktionen wirken auf den USB-Master, nicht automatisch
auf den Controller unter der gedrückten Taste. `EE_CLR` ist kein vollständiger
Studio-Factory-Reset und kein garantierter Reset beider Controller.

Offene, bisher nicht genehmigte Sonderoption: ausschließlich `EE_CLR` erst nach
zwei Sekunden eigenem Halten auslösen, vorheriges Loslassen bricht ab. Dafür
wären eigener Record-/Timer-Code und erneuter Konfliktaudit nötig, Aufwand mittel.
Aktuell schützt nur der gehaltene SYS-Zugang plus separate Taste; Clear löst
sofort beim Drücken aus. Bootloader und Neustart gehören nicht zu dieser Option.
Eine spätere Zustimmung gilt nur für die einzeln benannte Änderung.

## Validierungsstand

Stand 08.11.2026: **Quellenprüfung und 38 Hosttests bestanden; der erfolgreiche
Firmwarebuild nach den letzten Änderungen wurde vom Nutzer bestätigt.
Hardware-/GUI-Tests bleiben offen**.

Die Buildbestätigung erfolgte nach der Übernahme der zusätzlichen Tapping-Tests.
Lauf-ID, tatsächlich gebaute Userspace-/Firmware-Revisionen, einzelne Jobs und
Artefakte dieses neuen Builds wurden noch nicht separat abgeglichen. Die
folgenden API-geprüften Angaben beziehen sich ausschließlich auf den älteren
Build #1 und sind kein Detailnachweis für den neu gemeldeten Lauf.

[Build #1, Versuch 1](https://github.com/basch-de/corne-mx-studio_userspace/actions/runs/42480355588)
war laut GitHub Actions erfolgreich, für Userspace-Commit
`bbf0e5e465c5394b94cc69a25f46160aa842537b`, gestartet am 08.11.2026 um
13:27:38 Uhr Europe/Berlin. Resolver, alle drei Compilerjobs und alle Uploads
waren erfolgreich, ebenso die damalige persönliche Quellenprüfung.

| Job | UF2-Artefakt | Buildnachweis |
|---|---|---|
| `manufacturer` | `corne-mx-pro-studio-manufacturer` | `provenance-manufacturer` |
| `personal` | `corne-mx-pro-studio-personal` | `provenance-personal` |
| `standard-default` | `corne-mx-pro-standard-default` | `provenance-standard-default` |

Alle sechs Artefakte waren bei der API-Prüfung vorhanden und nicht abgelaufen.
Die Archive wurden nicht separat heruntergeladen oder inhaltlich geprüft.
Firmware-Commit und UF2-Prüfsummen stehen in den Buildnachweisen. Nach diesem
Build wurden Backslash, die Vial-gleichen Sondertasten und die Initialisierung
geändert. Build #1 bestätigt daher nicht diesen aktuellen Arbeitsstand.
Generator-, Archiv- und Hashdateien wurden später ohne Belegungsänderung entfernt.

```sh
python3 scripts/check_sources.py /pfad/zum/keebart-checkout
python3 tests/run.py /pfad/zum/keebart-checkout
git diff --check
```

Quellenprüfung, Python-/JSON-/YAML-/Shellsyntax und Whitespace wurden geprüft.
Die bisherigen 20 Host-Szenarien verwenden echte QMK-Keycodes und die unveränderten
Herstellermodule `tap_dance.c` / `studio_dance.c`. Geprüft sind Erststart, alte
Vial-Kennung, GUI-Persistenz, ungültiger VIA-Speicher, Slave ohne Schreiben,
Schreibfehler, Markerinitialisierung, kompilierte Keymap, die drei Tap-/Hold-
Aktionen, Unterbrechung, GUI-Timing, Doppeltipp, USB-Abbruch und synthetische Events.
Die C-Datenprüfungen erfassen Layer-/Encoderanzahl, 46 Positionen, Layerzugänge,
Zusatzpositionen und gespiegelte, auf SYS beschränkte Systemcodes.

Weitere 18 Sequenzen aus der lesbaren Vial-Referenz verwenden die unveränderte
`action_tapping.c` samt Header aus demselben Studio-Firmwarecheckout. Die
Timing-Callbacks aus `qmk_settings.c` und `vial.c` sowie `get_tap_keycode` aus
`action.c` werden als unveränderte Quellabschnitte eingebunden. Falls sich diese
Abschnitte upstream ändern, kann eine Anpassung des Testaufbaus nötig werden.
Die Sequenzen greifen auf die kompilierte Studio-Keymap zu und prüfen:

- HRM-Tap/Timeout, verschachtelte Anschläge gleicher und anderer Hand, Rolls
  und die Chordal-Ausnahme für Daumen beider Seiten;
- Quick Tap innerhalb/außerhalb des Zeitfensters für HRM und WIN/Tab sowie
  innerhalb des Zeitfensters für NUMPAD/Enter;
- Layer-Tap als Tap bei Rolls und als Hold bei verschachtelten Anschlägen,
  einschließlich Aktivierung und Freigabe des simulierten Layers;
- 16-Bit-Timer-Überlauf und globales GUI-Timing, das die Initialisierung erhält.

Jede Sequenz prüft passende Press-/Release-Paare, konsistente Tap-Zähler,
freigegebene Layer und keinen Überlauf des QMK-Wartepuffers. Diese Tests laufen
separat von den nativen Studio-Tap-Dance-Tests; die gemeinsame Verarbeitung von
Tap Dance und Tap-Hold im vollständigen Firmware-Aufrufpfad ist nicht abgedeckt.
Flow Tap ist wie nach einem frischen Herstellerreset mit gespeichertem Wert
null eingestellt; eigene Flow-Tap-Szenarien sind nicht enthalten.

EEPROM, Settings-API, Zeit, USB-Zustand und HID-Ausgabe sind simuliert. Die Tests
für QMK-Tapping simulieren außerdem die nachgelagerte Layerwirkung und prüfen
die Tap-/Hold-Entscheidung statt echter Modifier- oder USB-Reports. Sie
ersetzen weder den kompletten QMK-Aufrufpfad noch ARM-/UF2- oder Gerätetests.
Eine lokale ARM-Toolchain wurde nicht installiert; Host-Compiler und Firmware-
Compiler sind unterschiedliche Nachweise. Nichts wurde geflasht oder auf der
Tastatur zurückgesetzt. Auch ein erfolgreicher Firmwarebuild beweist noch keine
vollständige Studio-/Vial-Kommunikation oder ZMK-identisches Timing.

Neue Firmwareänderungen benötigen einen neuen Lauf mit allen drei erfolgreichen
Jobs, verfügbaren UF2-/Provenance-Artefakten und gemeinsamem Firmware-Commit.
Die persönliche Quellen-/Hostprüfung blockiert die unabhängigen Herstellerjobs
nicht. Alte Artefakte nicht als Nachweis eines neuen Stands ausgeben.

## Offene Hardwaretests

Dies ist ein Prüfplan, keine Flash-/Resetfreigabe:

1. Bestehenden Hersteller-/Gerätezustand und Studio-/Vial-Daten sichern und den
   Übergang von der bisherigen Firmware separat entscheiden.
2. Matrix, beide Hälften, Layerzugänge und normale Ziffern bei beiden Num-Lock-
   Zuständen prüfen. Rechte USB-Masterrolle und linke SYS-Bedienung testen.
3. HRM-Taps/Holds, gleichseitige Rolls, gegenüberliegende verschachtelte Taps,
   sechs Daumentrigger, Quick Tap und Wiederholung prüfen; Layer-Taps separat.
4. Studio und Vial nacheinander prüfen: Identifikation, physischer Unlock, Lesen
   der 16 Layer, reversible Änderung und erneutes Lesen in beiden Clients.
   Gleichzeitige Zugriffe sind nicht getestet.
5. RGB-Paletten, Effekte, Encoderoptionen, Makros, Pointer, Tap Dance, Profile und
   Locale-Aktionen gezielt prüfen. Vorhandene Flags sind kein Bediennachweis.
6. Nach separat erlaubtem Neustart Persistenz und Split-Synchronität sowie jede
   Hälfte allein prüfen. Keine Systemaktion zur bloßen Zugangsprüfung drücken;
   Bootloader, Clear und Flash bleiben getrennte Entscheidungen.
