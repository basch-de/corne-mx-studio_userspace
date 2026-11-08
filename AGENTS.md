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
- In der ersten Portierungsphase keine eigenen Firmware-Funktionen, Hooks,
  Callbacks oder Tap-Hold-State-Machines implementieren. Deklarative
  Standard-QMK-Datentabellen sind davon zu unterscheiden.
- Eigene Sonderimplementierungen brauchen jeweils eine ausdrückliche
  Einzelgenehmigung. Eine Zustimmung nicht auf weitere Abweichungen ausweiten.
  Vor Nutzung eines Erweiterungspunkts dessen konfliktfreie Verkettung mit dem
  tatsächlich verwendeten Herstellerstand nachweisen.
- QMK-Kern und Hardwaredefinitionen nicht ungefragt ändern.

## Referenzen und Gerätesicherheit

- Verbindliche Belegungsvorlage: `https://github.com/basch-de/zmk-config`,
  insbesondere `config/corne_choc_pro.keymap`, zugehörige `.conf`, Behaviors,
  Includes und physische Layouts. Keine Sofle-Belegung pauschal übernehmen.
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
- Im Vial-Referenzprojekt liegen dagegen zwei lokale Keymaps (`keebart_vial_stock`
  und `basch_de`). Dort wäre ebenfalls ein direkter Herstellerbuild ohne lokale
  Kopie möglich. Als gemeinsame Architektur wurde der hiesige Ansatz empfohlen;
  eine Umstellung des anderen Projekts ist bisher nicht beauftragt.

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
- Nullbasierte Positionen 6, 7, 20, 21 bleiben wie in der ZMK-Vorlage unbelegt.
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
- Studio bietet vorhandene Compound Actions und dynamischen Tap Dance.
  Diese sind vor eigener Programmierung zu prüfen. Eigene persistente Defaults
  werden bisher nicht initialisiert; keine unkonfigurierten TD-/Compound-Slots
  als funktionierende Firmware-Defaults ausgeben.
- Aktuelle Zwischenlösungen: Hyper als reine Modifier-Taste an Position 14 auf
  `macos`, Cmd+Space auf derselben Position in SHORT; SHORT auf rechtem äußeren
  Daumen, Caps Word separat auf SHORT:0; Komma NUMPAD:38, Punkt NUMPAD:32.
- Voice Command ist nicht QMK Assistant; Consumer Power ist nicht System Power.
  Beide Originalaktionen bleiben deaktiviert, statt ungeprüfte Ersatzcodes zu senden.
- SYS ist über linke obere Außentaste und zusätzlich rechte obere Außentaste
  erreichbar. Reset, Boot und EE_CLR liegen gespiegelt auf beiden Hälften.
  Im Split wirken reguläre Systemaktionen auf den USB-Master, nicht automatisch
  auf die physisch gedrückte Hälfte. EE_CLR ist kein Studio-Factory-Reset.
- EE_CLR hat aktuell nur den gehaltenen SYS-Zugang plus separate Taste als Schutz;
  es gibt keinen zusätzlichen Countdown. Eine Zwei-Sekunden-Haltesicherung wurde
  als genau eine erste Sonderentscheidung vorgelegt, aber nicht genehmigt.

## Dateien und Prüfungen

- `scripts/generate_keymap.py` erzeugt `keymap.c`, `docs/MAPPING.md` und
  `docs/mapping.json` aus dem versionierten ZMK-Snapshot. Änderungen dort über
  die Quelle bzw. den Generator vornehmen, nicht nur in generierten Ausgaben.
  Zusätzliche Bindings stehen explizit in `OVERRIDES`.
- `sources/` enthält den historischen ZMK-Analysesnapshot.
  `sources/provenance.json` dokumentiert den Ausgangsstand, einschließlich des
  damals geprüften Firmware-Commits `703a62b32a5bf516ef54c6d131d7377064bbafc7`.
  Diese Angabe ist kein aktiver Checkout-Pin. Snapshots und Hashes nicht
  automatisch aktualisieren oder zur Umgehung eines Checks ändern.
- `scripts/check_sources.py` prüft Kompatibilität, keine feste Firmware-Commit-ID.
  Es vergleicht u.a. Herstellerdateien, Identität, Layout und Submodule.
  Änderungen am Branch können Anpassungen oder einen erneuten Audit erfordern;
  Prüfungen nicht einfach entfernen, um einen neuen Stand durchzulassen.
- Die persönliche Quellenprüfung läuft nur im Job `personal`; unabhängige
  Herstellerbuilds dürfen dadurch nicht blockiert werden.
- Nach relevanten Keymap-/Konfigurationsänderungen:
  `python3 scripts/check_sources.py /pfad/zum/keebart-checkout` ausführen.
  Bei Workflowänderungen YAML und die enthaltenen Shellskripte prüfen.
  Jede Änderung muss `git diff --check` bestehen; neue unversionierte Dateien
  zusätzlich auf Whitespace-Fehler prüfen.
- Statische Prüfung, erfolgreicher Firmwarecompilerlauf und echte Hardware-/GUI-
  Tests immer getrennt berichten. Passende Buildflags beweisen keine vollständige
  Studio-/Vial-Laufzeitkompatibilität.
- Dokumentationsstand 08.11.2026: statische Prüfungen bestanden, noch kein
  Firmware-Build und keine Hardware-/GUI-Validierung. Keine UF2 erzeugt,
  kein Flash, Reset, Push oder Actions-Start. Diesen Status nur nach tatsächlichem
  Nachweis aktualisieren. Die lokale Buildwerkzeug-Installation wurde abgelehnt;
  eine eingerichtete lokale Toolchain nicht voraussetzen.
- Der bisherige Analysecheckout unter `/private/tmp/corne-keebart-studio` ist
  temporär und kann veraltet oder verschwunden sein; er aktualisiert sich nicht
  automatisch mit der Branchstrategie des Workflows.

Weiterführende Nachweise: [README](README.md), [Analyse](docs/ANALYSIS.md),
[Positionsmapping](docs/MAPPING.md), [Abweichungsbericht](docs/DEVIATIONS.md),
[Validierungsstand](docs/VALIDATION.md).
