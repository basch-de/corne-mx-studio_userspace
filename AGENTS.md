# Projektregeln und Arbeitswissen

Diese Datei gilt für das gesamte Repository. Sie hält die Entscheidungen aus
dem Projektauftrag und der weiteren Abstimmung fest. Neuere ausdrückliche
Nutzeranweisungen haben Vorrang. Technische Befunde beziehen sich auf den
untersuchten Quellstand und müssen bei relevanten Herstelleränderungen erneut
geprüft werden.

## Ziel und Arbeitsumfang

- Einziges Hardwareziel: `keebart/corne_mx_pro/standard`, kabelgebundene
  Corne MX Pro mit zwei RP2040 und 46 Tastenpositionen.
- Eigenständiges QMK External Userspace verwenden; keinen vollständigen
  Firmware-Fork anlegen, solange External Userspace ausreicht.
- Höchste Priorität: vorhandene Keebart-Studio-, Vial-, RGB- und
  Hardwarefunktionen erhalten. Danach die ZMK-Belegung möglichst genau portieren.
- Funktionen in dieser Reihenfolge bevorzugen: Keebart Studio, kompatible
  Vial-Funktionen, Standard-QMK. Ähnliche Namen beweisen keine Verhaltensgleichheit.
- Keine eigenen Record-/Timing-Callbacks oder Tap-Hold-State-Machines
  implementieren. Als ausdrücklich genehmigte Ausnahme ist die einmalige
  Initialisierung wie im Vial-Projekt über `via_init_kb`, `eeconfig_init_user`
  und `keyboard_post_init_user` erlaubt. Weitere eigene Erweiterungen bleiben
  genehmigungspflichtig; siehe `docs/PROJECT.md`.
- Eigene Sonderimplementierungen brauchen jeweils eine ausdrückliche
  Einzelgenehmigung. Eine Zustimmung nicht auf weitere Abweichungen ausweiten.
  Vor Nutzung eines Erweiterungspunkts dessen konfliktfreie Verkettung mit dem
  tatsächlich verwendeten Herstellerstand nachweisen.
- QMK-Kern und Hardwaredefinitionen nicht ungefragt ändern.

## Referenzen und Gerätesicherheit

- Historische Belegungsvorlage: `https://github.com/basch-de/zmk-config`,
  Commit `17dc18d4065161066a459656a87f36f11cd4ae88`,
  `config/corne_choc_pro.keymap` und zugehörige Konfiguration. Ab jetzt ist auf
  ausdrücklichen Nutzerwunsch die lokale `keymap.c` maßgeblich und direkt zu
  bearbeiten. Keine automatische Rückübertragung aus ZMK und keine zusätzliche
  aktive Belegungskopie oder Generatorabhängigkeit einführen.
- Die lokalen Projekte `../zmk-config`, `../qmk_userspace` und
  `../corne-mx-vial_userspace` sind ausschließlich lesbare Referenzen. Der Wunsch
  nach einheitlicher Architektur ist keine Genehmigung, diese Projekte zu ändern.
  Bereits vorhandene fremde Änderungen erhalten.
- Keine Firmware automatisch flashen, EEPROM-Resets durchführen, Flash-Speicher
  löschen oder Bootloader-Experimente ausführen. Aktuell sind Quelländerungen,
  Dokumentation und das Vorbereiten/Prüfen von Builds beauftragt.
- Nicht ungefragt pushen oder GitHub Actions auslösen. Das Einrichten eines
  Workflows ist von dessen Veröffentlichung und Ausführung zu unterscheiden.
- Ein späteres Flash und jede Geräteaktion benötigen eine gesonderte Entscheidung.
  Beide Controller und die unbekannte bisherige Firmware berücksichtigen.

## Vereinbarte Build-Einstellungen

Maßgebliche Datei: `.github/workflows/build.yml`.

- Firmware-Repository: `Keebart/vial-qmk-keebart`.
- Branch: `feature/keebart-studio-layer-rgb`.
- Jeden neuen Workflow-Lauf mit dem dann aktuellen Branchstand bauen. Nicht
  wieder auf einen dauerhaft festen Firmware-Commit umstellen.
- Der Job `firmware-revision` löst den Branch genau einmal auf. Alle drei
  Buildvarianten verwenden anschließend dieselbe Commit-ID aus dessen Output.
  Damit laufen sie nicht versehentlich gegen unterschiedliche Branchstände.
- Neue Keebart-Commits lösen allein keinen Workflow in diesem Repository aus.
  Trigger sind `push`, `pull_request` und `workflow_dispatch`; kein Zeitplan.
- Runner wie bei Keebart: `ubuntu-latest`.
- Buildbefehl wie bei Keebart: `qmk mass-compile -j "$(nproc)"`, hier mit dem
  expliziten Ziel `"keebart/corne_mx_pro/standard:${BUILD_KEYMAP}"` pro Matrix-Job.
- Actions auf Hauptversion referenzieren: `actions/checkout@v4` und
  `actions/upload-artifact@v4`. Die vorherigen festen Action-Commit-Hashes wurden
  auf Nutzerwunsch durch dieses an Keebart angelehnte Schema ersetzt.
- Der Buildcontainer bleibt wie im geprüften Keebart-Workflow per Digest fest:
  `ghcr.io/qmk/qmk_cli@sha256:2dc05fc9f32efebd6b05c2b8676ee548358bc7e151e9dbf4dac6b6eed4513b07`.
  Compiler und enthaltene Python-Pakete werden damit durch das Image bestimmt.
- Git-Submodule rekursiv laden, jeweils an den Commits des aufgelösten
  Firmwarestands; nicht unabhängig auf die neuesten Submodule-Branches wechseln.
- UF2-Dateien als getrennte Actions-Artefakte bereitstellen. Firmware- und
  Userspace-Commit, Submodule, Compiler, Python-Pakete und UF2-SHA256 protokollieren.
  Bei Fehlern auch `.build/failed.log.*` ausgeben und mit den Logs hochladen.
- Kein Flash-Schritt im Workflow. Bestehende reine Build-Tasks in VS Code erhalten.
- Bei weiteren CI-Änderungen Keebarts tatsächlichen Workflow vergleichen.
  Dessen zuletzt geprüfte `ci.yml` baut `default` und `vial`, aber nicht `studio`;
  sie ist daher kein Buildnachweis für unsere Studio-Varianten.

## Drei Builds, zwei Targets, eine lokale Keymap

| Variante | Keymap | Quelle | UF2-Artefakt |
|---|---|---|---|
| `manufacturer` | `studio` | direkt aus Keebarts Checkout | `corne-mx-pro-studio-manufacturer` |
| `personal` | `studio` | lokaler Userspace | `corne-mx-pro-studio-personal` |
| `standard-default` | `default` | direkt aus Keebarts Checkout | `corne-mx-pro-standard-default` |

- External Userspace überschreibt die passende Hersteller-Keymap. Ohne lokale
  Keymap-Dateien verwendet QMK die originale Keymap aus dem Firmwarecheckout.
- Herstellerbuilds verwenden `stock-userspace/` mit lediglich einer `qmk.json`;
  die persönliche Variante verwendet `userspace/` mit unseren Keymap-Dateien.
- Deshalb stehen in `qmk.json` nur die zwei verschiedenen Ziele `studio` und
  `default`. Die dritte Variante entsteht durch den unterschiedlichen Userspace
  in der Workflow-Matrix. Ein Build nur anhand von `qmk.json` bildet nicht alle
  drei CI-Varianten ab.
- `standard` ist die Hardwarevariante, nicht der Keymapname. Die originale
  `default`-Keymap aktiviert nach geprüftem Stand weder Vial noch die zusätzlichen
  Studio-Module. Die vollständige Studio-Herstellerreferenz heißt `manufacturer`.
- Die Studio-Keymap nicht einfach umbenennen: Keebarts `studio_rgb.mk` aktiviert
  mehrere Module nur bei `KEYMAP=studio`, darunter Pointer, Tap Dance, Gerätename,
  Locales/Compound Actions und Makros. Originale Includes und Flags erhalten.
- Das inzwischen außerhalb dieses Projekts geänderte Vial-Referenzprojekt
  verwendet ebenfalls einen direkten Herstellerbuild (`vial`) und einen
  persönlichen Userspace (`basch_de`). Änderungen dort bleiben hier weiterhin
  nicht beauftragt. Keine veraltete lokale Herstellerkopie voraussetzen.

## Keymap, Timing und Kompatibilität

- Acht belegte Layer in dieser Reihenfolge: `macos`, `num`, `sym`, `nav`, `win`,
  `short`, `numpad`, `sys` (0–7). Die tatsächliche Quelle hat NUM/Backspace und
  WIN/Tab; vorläufige Beschreibungen nicht anstelle der Keymap übernehmen.
- Hersteller-ABI mit 16 dynamischen Layern beibehalten. Nicht auf acht reduzieren:
  Studio prüft das Speicherformat. Acht persönliche Layer bedeuten nicht acht
  Speicherplätze insgesamt. Encoder- und kompilierte Keymap-Layerzahl müssen
  gemäß QMK-Assertion übereinstimmen.
- `LAYOUT_split_3x6_3_ex2`: Zeilen mit 14, 14, 12 und 6 Bindings. Rechte
  elektrische Matrix ist gespiegelt; Positionen nicht ungeprüft umnummerieren.
- Die MX hat 46 nutzbare Positionen und keine Displays. Nullbasierte Positionen
  6, 7, 20, 21 bleiben bisher wie in der ZMK-Vorlage unbelegt; das ist eine
  Belegungsentscheidung, keine Hardware-Einschränkung. Elektrisch sind das
  `[0,6]`, `[4,6]`, `[1,6]`, `[5,6]`.
  Die sechs Daumen liegen auf 40–45. Numpad nutzt normale Ziffern statt
  Num-Lock-abhängiger Keypad-Codes. Beide NUMPAD-Toggle-Positionen erhalten.
- Keine Hersteller-UID, physischen Unlock, EEPROM-Kennung, Flashaufteilung oder
  RGB-/Encoderfunktionen ungefragt ändern. Keine eigene Versionsänderung erzwingen,
  um vorhandene dynamische Einstellungen zurückzusetzen.
- Vial besitzt bereits `get_tapping_term`, `get_quick_tap_term`,
  `get_chordal_hold` und weitere Timing-Callbacks. Studio verwendet insbesondere
  `pre_process_record_user` und `process_record_user` sowie weitere Hooks.
  Keine vermeintlich freien User-Hooks aus anderen Keymaps kopieren.
- `TAPPING_TERM=250` ist nur ein Startwert. Beim geprüften Herstellerreset setzt
  Vial auch Quick Tap auf diesen Wert und startet die Hold-Optionen bei null.
  Compile-Defines sind kein Ersatz für persistente Vial-Einstellungen.
- Die vorhandene Chordal-Handtabelle markiert alle sechs Daumen als `*`.
  Sie wirkt erst bei aktiviertem QMK/Vial-Chordal-Hold; Studios eigene
  Compound-Engine hat separate Einstellungen. Keine ZMK-Timinggleichheit behaupten.
- Die fehlenden Tasten und die Initialisierung aus dem Vial-Projekt wurden
  ausdrücklich freigegeben: MACOS:14 `TD(0)` Cmd+Space/Hyper, MACOS:45 `TD(1)`
  Caps Word/SHORT, NUMPAD:38 `TD(2)` Komma/Punkt; Fristen zunächst je 250 ms.
  Die separaten Zwischenlösungstasten auf SHORT:0, SHORT:14 und NUMPAD:32
  sind wieder unbelegt. Die vorhandene Studio-Engine bleibt zuständig; frühe
  Unterbrechung entscheidet Tap. Keine eigene Tap-Dance-Engine einbauen.
- `defaults.c` initialisiert auf dem Master bei fehlender/abweichender User-Kennung
  oder ungültigem VIA-Speicher vier Settings (IDs 7/22/23/26) und TD-Slots 0–2.
  Normale Neustarts erhalten GUI-Werte. Beim ersten Übergang können vorhandene
  Werte in diesen Settings/Slots ersetzt werden. User-Kennung `0x42535401` nicht
  mit dem Vial-Userspace-Marker oder der Hersteller-Build-ID verwechseln.
  Quick Tap wird nicht separat geschrieben. Keine Keymap-/Palette-/Profilresets.
- SHORT:35 `KC_ASST` und SYS:13 `KC_PWR` wurden als Vial-gleiche Standardcodes
  freigegeben. Voice Command ist trotzdem nicht Assistant; Consumer Power
  ist nicht System Power. Hostwirkung bleibt zu prüfen.
- SYS ist über die linke obere Außentaste erreichbar. Die rechte obere
  Außentaste (MACOS:13) sendet auf ausdrücklichen Nutzerwunsch wie im Vial-Projekt
  und ZMK-Original reines `KC_BSLS`; keinen rechten SYS-Layer-Tap wieder einführen.
  Ein rechter SYS-Zugang ist für das Flashen nicht zwingend: Sind beide Hälften
  verbunden und rechts per USB am Host, kann links SYS gehalten werden; QK_BOOT
  wirkt dann auf den rechten USB-Master. Die rechte Hälfte allein hat aktuell
  keinen SYS-Zugang über ihre Belegung. Reset, Boot und EE_CLR liegen im SYS-Layer
  weiterhin gespiegelt auf beiden Hälften.
  Im Split wirken reguläre Systemaktionen auf den USB-Master, nicht automatisch
  auf die physisch gedrückte Hälfte. EE_CLR ist kein Studio-Factory-Reset.
- EE_CLR hat aktuell nur den gehaltenen SYS-Zugang plus separate Taste als Schutz;
  es gibt keinen zusätzlichen Countdown. Eine Zwei-Sekunden-Haltesicherung wurde
  als genau eine erste Sonderentscheidung vorgelegt, aber nicht genehmigt.

## Dateien und Prüfungen

- `keyboards/keebart/corne_mx_pro/keymaps/studio/keymap.c` enthält die direkt
  gepflegte Belegung und Encoder-Map; `layers.h` die Layernamen, `defaults.c` die
  freigegebene Erstinitialisierung. Der Generator und die aktive Mapping-JSON
  wurden auf Nutzerwunsch entfernt. Nicht wieder einführen.
- Alle Layer nach der physischen Struktur ausrichten: 14 + 14 + 12 + 6
  Argumente in vierzehn logischen Spalten. Vollständige Keycode-Ausdrücke bleiben
  Zellen; keine Bindings nur zum Formatieren ändern.
- Historische ZMK-Kopien, Mapping und Analyse-/Vergleichsberichte wurden auf
  Nutzerwunsch gelöscht. Keine lokale ZMK-Abhängigkeit oder parallele aktuelle
  Belegung einführen. Neue Tastenänderungen nur in `keymap.c` pflegen und
  betroffene Prüfungen der kompilierten C-Daten anpassen.
- `scripts/check_sources.py` prüft Kompatibilität, keine feste Firmware-Commit-ID.
  Es vergleicht u.a. Herstellerdateien, Identität, Layout und Submodule.
  Änderungen am Branch können Anpassungen oder einen erneuten Audit erfordern;
  Prüfungen nicht einfach entfernen, um einen neuen Stand durchzulassen.
- Die persönliche Quellenprüfung läuft nur im Job `personal`; unabhängige
  Herstellerbuilds dürfen dadurch nicht blockiert werden.
- Erkenntnis aus dem geprüften Vial-Userspace (`c20c4e3`): keine eigenen
  Record-/Timing-Callbacks; nur drei
  Initialisierungshooks und eine Handtabelle. `via_init_kb`, `eeconfig_init_user`
  und `keyboard_post_init_user` haben im geprüften Studio-Pfad keine direkten
  Symbolkollisionen; Studio ruft den letzten ausdrücklich auf. Die Übernahme
  wurde inzwischen freigegeben und umgesetzt; Persistenz und Slotbelegung
  sind in `docs/PROJECT.md` dokumentiert. Klassische Vial-
  Hosttests beweisen nicht die Studio-Tap-Dance-Engine. Den aktuellen Vial-Stand
  nicht mit einer älteren eigenen Tap-Hold-State-Machine gleichsetzen.
- Die Prüfsummen-Datei für den Initialisierungsaudit wurde auf Nutzerwunsch
  entfernt. Keine festen Hersteller-Dateihashes wieder einführen. Der Quellencheck
  prüft weiterhin schwache Initialisierungshooks, Studios User-Hook-Aufruf,
  mögliche Doppeldefinitionen in den über die Studio-Regeln gefundenen Quellen
  samt lokalen Includes sowie die Settings-IDs. Diese Textprüfungen ersetzen
  keinen vollständigen Audit bei funktionalen Herstelleränderungen.
- Nach relevanten Keymap-/Konfigurationsänderungen:
  `python3 scripts/check_sources.py /pfad/zum/keebart-checkout` und
  `python3 tests/run.py /pfad/zum/keebart-checkout` ausführen. Die 38 Hosttests
  verwenden die echten Studio-Tap-Dance-Module sowie QMKs `action_tapping.c`
  und die Hersteller-Timing-Callbacks. 18 aus der Vial-Referenz übernommene
  Sequenzen prüfen HRM, Chordal Hold, Daumen-Ausnahmen, Layer-Tap, Quick Tap,
  Timer-Überlauf und GUI-Timing anhand der kompilierten Studio-Keymap.
  Speicher/USB/HID und nachgelagerte Layerwirkungen bleiben simuliert; das ist
  kein vollständiger QMK-Pipeline-Test. Keine klassische Vial-Tap-Dance-Engine
  als Ersatz für die Studio-Tests einbauen.
  Bei Workflowänderungen YAML und die enthaltenen Shellskripte prüfen.
  Jede Änderung muss `git diff --check` bestehen; neue unversionierte Dateien
  zusätzlich auf Whitespace-Fehler prüfen.
- Statische Prüfung, erfolgreicher Firmwarecompilerlauf und echte Hardware-/GUI-
  Tests immer getrennt berichten. Passende Buildflags beweisen keine vollständige
  Studio-/Vial-Laufzeitkompatibilität.
- Dokumentationsstand 08.11.2026: statische Prüfungen und erster GitHub-Actions-
  Build bestanden. Lauf `42480355588` (#1, Versuch 1) für Userspace-Commit
  `bbf0e5e465c5394b94cc69a25f46160aa842537b`: Resolver sowie `manufacturer`,
  `personal` und `standard-default` erfolgreich; persönliche Kompatibilitätsprüfung
  und alle Compile-/Upload-Schritte erfolgreich. Drei UF2- und drei Provenance-
  Artefakte laut API vorhanden, Archivinhalt nicht separat geprüft.
  Hardware-/GUI-Validierung bleibt offen; kein Flash oder EEPROM-Reset erfolgt.
  Nach diesem Build wurden MACOS:13 auf reines Backslash zurückgestellt und
  die Vial-gleichen Tasten samt Initialisierung ergänzt. Quellenprüfung und
  38 Hosttests bestehen; dieser neue Stand benötigt einen neuen Firmware-Build.
  Details und Lauf-Link in `docs/PROJECT.md`. Zukünftige Firmwareänderungen
  nicht mit diesem älteren Lauf als buildgeprüft ausgeben. Die lokale
  Buildwerkzeug-Installation wurde abgelehnt; eine eingerichtete lokale
  Toolchain nicht voraussetzen.
- Der bisherige Analysecheckout unter `/private/tmp/corne-keebart-studio` ist
  temporär und kann veraltet oder verschwunden sein; er aktualisiert sich nicht
  automatisch mit der Branchstrategie des Workflows.

Weiterführende Nachweise: [README](README.md) und
[technische Hinweise und Prüfstand](docs/PROJECT.md).
