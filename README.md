# Corne MX Pro – Studio External Userspace

Standardportierung der ZMK-Belegung der Corne Choc Pro BT auf
`keebart/corne_mx_pro/standard` (RP2040, 46 Positionen).

Die Konfiguration verwendet Keebarts originale Studio-Module und Vial.
Eigene Firmware-Funktionen, Hooks und Änderungen an Hardware oder QMK-Kern sind
nicht enthalten. Die Keymap heißt zwingend **`studio`**, weil der Hersteller
mehrere Funktionen ausschließlich für diesen Namen einkompiliert.

Stand 08.11.2026: **Quellen und Positionszuordnung geprüft; Firmware noch nicht
kompiliert und nicht auf Hardware oder mit den GUIs getestet.** Die lokale
Installation der Buildwerkzeuge wurde abgelehnt. Es wurden keine Firmware,
EEPROM-Einstellungen oder Referenzprojekte verändert. Es wurde nichts gepusht
und kein GitHub-Actions-Lauf gestartet.

- [Analyse und Kompatibilitätsprüfung](docs/ANALYSIS.md)
- [Vollständige Zuordnung aller 368 Bindings](docs/MAPPING.md)
- [Abweichungsbericht und erste Einzelentscheidung](docs/DEVIATIONS.md)
- [Validierungsstand und spätere Hardwareprüfung](docs/VALIDATION.md)
- [Quellrevisionen und Prüfsummen](sources/provenance.json)

## Build in GitHub Actions

Der Workflow `.github/workflows/build.yml` baut die unveränderte Studio-Hersteller-
Baseline, die persönliche Studio-Keymap und zusätzlich die vollständige originale
`default`-Keymap des Standard-Boards in drei getrennten Jobs.
Jeder neue Workflow-Lauf ermittelt zuerst den aktuellen Commit von
`feature/keebart-studio-layer-rgb`. Alle drei Varianten verwenden anschließend
genau diesen gemeinsamen Commit und dasselbe festgelegte QMK-Container-Image.
Die tatsächlich gebaute Revision wird im jeweiligen Provenance-Artefakt gespeichert.
Ein neuer Keebart-Commit allein startet keinen Lauf in unserem Repository;
er wird beim nächsten manuellen Start, Push oder Pull Request übernommen.
Nach Übernahme der Dateien in GitHub ist ein manueller Start
unter **Actions → Build Corne MX Pro UF2 → Run workflow** möglich.
Pushes und Pull Requests führen ebenfalls Builds aus.

Wie Keebarts CI verwendet der Workflow `ubuntu-latest` und
`qmk mass-compile -j "$(nproc)"`. Pro Matrix-Job wird genau das ausgewählte
Corne-Buildziel übergeben. Bei einem Fehler werden auch die separaten
`failed.log.*`-Dateien von QMK ausgegeben und als Buildnachweis gespeichert.

Erwartete Artefakte nach erfolgreichem Lauf:

- `corne-mx-pro-studio-manufacturer`: originale Studio-UF2.
- `corne-mx-pro-studio-personal`: persönliche Studio-UF2.
- `corne-mx-pro-standard-default`: unveränderte Hersteller-`default`-UF2.
- `provenance-manufacturer` / `provenance-personal` / `provenance-standard-default`: Revisionen, Submodule,
  Compiler, Python-Pakete, Buildlog und UF2-Prüfsumme.

Die Artefakte werden erst durch einen erfolgreichen GitHub-Lauf erzeugt. Dieser
Arbeitsstand enthält noch keine UF2. Der Workflow enthält keinen Flash-Schritt.

`standard` bezeichnet die Hardwarevariante; `default` und `studio` sind verschiedene
Keymaps. Die originale `default`-Keymap aktiviert weder Vial noch die zusätzlichen
Studio-Module. Für den vollständigen Herstellerumfang mit Studio und Vial bleibt
das Artefakt `corne-mx-pro-studio-manufacturer` vorgesehen. Beide Herstellerbuilds
verwenden einen separaten Userspace ohne persönliche Keymap-Dateien.

Die persönliche Portierung wird weiterhin auf Änderungen an Herstellerregeln,
Identität, Geometrie und Speicherformat geprüft. Eine neue Commit-ID allein
führt nicht zum Abbruch; relevante Herstelleränderungen können eine Anpassung
der Portierung erfordern. Diese persönliche Prüfung blockiert die beiden
unveränderten Herstellerbuilds nicht. Der Analysestand und das Quellenmanifest
unter `sources/` dokumentieren weiterhin die ursprünglich untersuchte Revision.

## Lokale Entwicklung

VS Code: `.vscode/tasks.json` bietet Quellenprüfung und einen reinen Build-Task.
Voraussetzung für den Build ist eine separat eingerichtete QMK-Toolchain.
Der geprüfte Keebart-Checkout liegt aktuell unter
`/private/tmp/corne-keebart-studio` und gehört nicht zum Userspace-Repository.

```sh
python3 scripts/check_sources.py /private/tmp/corne-keebart-studio
```

Mit vorhandener QMK-Umgebung, im Keebart-Checkout:

```sh
export QMK_USERSPACE=/Users/basch/dev/corne-mx-studio_userspace
qmk compile -kb keebart/corne_mx_pro/standard -km studio
```

Der Quellchecker benötigt nur die Python-Standardbibliothek. Die
Generierungsdateien werden aus dem versionierten ZMK-Snapshot erstellt:

```sh
python3 scripts/generate_keymap.py
```

Anschließend Änderungen an Keymap und Mapping bewusst prüfen. Die zusätzlichen
Tasten stehen explizit in `OVERRIDES`; neue ZMK-Behaviors führen zum Abbruch des
Generators statt zu einer geratenen Umsetzung.

## Belegung und Zwischenlösungen

Acht belegte Layer: `macos`, `num`, `sym`, `nav`, `win`, `short`, `numpad`, `sys`.
Die 16 dynamischen Layer des Herstellers bleiben erhalten; 8–15 sind nicht mit
einer persönlichen Belegung versehen. Positionen sind nullbasiert.

- Linke Daumen: NAV/Escape, NUM/Backspace, WIN/Tab.
- Rechte Daumen: NUMPAD/Enter, SYM/Space, momentaner SHORT-Layer.
- SYS: äußere linke obere Taste halten; zusätzlich äußere rechte obere Taste
  halten (Tap bleibt Backslash).
- Hyper liegt außen links in der Home Row als reine Modifier-Taste.
  Cmd+Space liegt auf derselben Position 14 im SHORT-Layer; Caps Word auf SHORT:0.
  Ein kurzer Druck auf die Hyper-Taste allein sendet keinen Shortcut.
- NUMPAD: Komma bleibt Position 38; separater Punkt auf Position 32.
- SYS: unten außen Neustart, daneben Bootloader, daneben EEPROM Clear –
  auf beiden Hälften. SYS muss bewusst gehalten werden. EEPROM Clear hat keine
  zusätzliche Haltefrist und wird bei Tastendruck ausgelöst.

Die vorherigen kombinierten Sondertasten sind damit zunächst aufgeteilt.
Studio bietet spätere Konfigurationsmöglichkeiten; siehe Abweichungsbericht.

Vor einem späteren Flash ist eine getrennte Entscheidung erforderlich.
Eine passende bestehende dynamische Keymap kann die einkompilierte Belegung
überlagern. Es wird absichtlich keine Versionsänderung erzwungen, die vorhandene
Einstellungen zurücksetzt. Auch die Herstellerinitialisierung bei inkompatiblem
EEPROM ist vor einem späteren Flash gesondert zu beurteilen.
