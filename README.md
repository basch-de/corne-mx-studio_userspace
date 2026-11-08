# Corne MX Pro – Studio External Userspace

Standardportierung der ZMK-Belegung der Corne Choc Pro BT auf
`keebart/corne_mx_pro/standard` (RP2040, 46 Positionen).

Die Konfiguration verwendet Keebarts originale Studio-Module und Vial.
Die freigegebene eigene Laufzeitlogik initialisiert ausschließlich vier
Vial-Einstellungen und drei dynamische Tap Dances. Record- und Timing-Hooks
sowie Hardware und QMK-Kern werden nicht ersetzt. Die Keymap heißt **`studio`**, weil der Hersteller
mehrere Funktionen ausschließlich für diesen Namen einkompiliert.

Stand 08.11.2026: **Der Nutzer hat den erfolgreichen Firmwarebuild nach den
letzten Änderungen bestätigt. Hardware- und GUI-Tests stehen noch aus.**
Quellenprüfung und 38 Hosttests mit Keebarts Studio- und QMK-Tapping-Engine
bestehen ebenfalls. Lauf-ID, gebaute Revisionen und Artefakte des neuen Builds
wurden noch nicht separat abgeglichen; der ältere API-geprüfte Build #1 ist in
den technischen Hinweisen dokumentiert. Es wurde durch den Agenten keine
Firmware übertragen und kein EEPROM-Reset durchgeführt.

- [Technische Hinweise, Initialisierung, Abweichungen und Prüfstand](docs/PROJECT.md)

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

Die Artefakte sind beim erfolgreichen Buildlauf auf GitHub verfügbar; lokal
werden keine UF2-Dateien versioniert. Der Workflow enthält keinen Flash-Schritt.

`standard` bezeichnet die Hardwarevariante; `default` und `studio` sind verschiedene
Keymaps. Die originale `default`-Keymap aktiviert weder Vial noch die zusätzlichen
Studio-Module. Für den vollständigen Herstellerumfang mit Studio und Vial bleibt
das Artefakt `corne-mx-pro-studio-manufacturer` vorgesehen. Beide Herstellerbuilds
verwenden einen separaten Userspace ohne persönliche Keymap-Dateien.

Die persönliche Portierung wird weiterhin auf Änderungen an Herstellerregeln,
Identität, Geometrie und Speicherformat geprüft. Eine neue Commit-ID allein
führt nicht zum Abbruch; relevante Herstelleränderungen können eine Anpassung
der Portierung erfordern. Diese persönliche Prüfung blockiert die beiden
unveränderten Herstellerbuilds nicht. Die Quellenprüfung kontrolliert die
Initialisierungshooks, deren Verkettung und Settings-IDs direkt im aktuellen
Herstellercode. Ein Abgleich fester Quelldatei-Prüfsummen findet nicht mehr statt.

## Lokale Entwicklung

VS Code: `.vscode/tasks.json` bietet Quellenprüfung und einen reinen Build-Task.
Voraussetzung für den Build ist eine separat eingerichtete QMK-Toolchain.
Der geprüfte Keebart-Checkout liegt aktuell unter
`/private/tmp/corne-keebart-studio` und gehört nicht zum Userspace-Repository.

```sh
python3 scripts/check_sources.py /private/tmp/corne-keebart-studio
python3 tests/run.py /private/tmp/corne-keebart-studio
```

Mit vorhandener QMK-Umgebung, im Keebart-Checkout:

```sh
export QMK_USERSPACE=/Users/basch/dev/corne-mx-studio_userspace
qmk compile -kb keebart/corne_mx_pro/standard -km studio
```

Die Belegung wird ab jetzt direkt in
`keyboards/keebart/corne_mx_pro/keymaps/studio/keymap.c` gepflegt. Es gibt keinen
Belegungsgenerator und keine separate aktive Bindungs-JSON. `layers.h` enthält
die Layernamen; `defaults.c` initialisiert die Vial-Einstellungen und Tap Dances.

Nach einer Belegungsänderung Quellenprüfung und Hosttests ausführen. Die Tests
kompilieren die echte Keymap mit Herstellerlayout und QMK-Keycodes und prüfen
Layerzugänge, SYS-Aktionen und die aktuellen Tap-Dance-Bindings. Betroffene
Erwartungen bei beauftragten Belegungsänderungen bewusst mit anpassen.
Zusätzliche Sequenzen prüfen mit der echten QMK-Tapping-Engine Home Row Mods,
Chordal Hold, Daumen-Ausnahmen, Layer-Tap, Quick Tap, Timer-Überlauf und
persistiertes GUI-Timing. Speicher und die nachgelagerte Reportverarbeitung sind
simuliert; der vollständige Firmware-Aufrufpfad bleibt außerhalb dieser Tests.
Der Quellchecker braucht nur die Python-Standardbibliothek; die Hosttests zusätzlich
`cc`. Die Matrix bleibt in vier physischen Zeilen mit 14 + 14 + 12 + 6 Argumenten.

Historische ZMK-Kopien und Portierungstabellen wurden entfernt. Die aktuelle
Belegung steht ausschließlich in `keymap.c`; die verbleibende Dokumentation
beschreibt Abweichungen, Initialisierung und Validierung.

## Belegung und Voreinstellungen

Acht belegte Layer: `macos`, `num`, `sym`, `nav`, `win`, `short`, `numpad`, `sys`.
Die 16 dynamischen Layer des Herstellers bleiben erhalten; 8–15 sind nicht mit
einer persönlichen Belegung versehen. Positionen sind nullbasiert.

- Linke Daumen: NAV/Escape, NUM/Backspace, WIN/Tab.
- Rechte Daumen: NUMPAD/Enter, SYM/Space, Tap Caps Word / Hold SHORT.
- SYS: äußere linke obere Taste halten. Die rechte obere Außentaste sendet wie
  im Vial-Projekt und ZMK-Original ausschließlich Backslash.
- Außen links in der Home Row: Tap Cmd+Space / Hold Hyper, `TD(0)`.
- Rechter äußerer Daumen: Tap Caps Word / Hold SHORT, `TD(1)`.
- NUMPAD Position 38: Tap Komma / Hold Punkt, `TD(2)`.
- SHORT Position 35: Assistant (`KC_ASST`); SYS Position 13: System Power
  (`KC_PWR`). Beide sind bewusst andere HID-Aktionen als die früheren ZMK-Codes.
- SYS: unten außen Neustart, daneben Bootloader, daneben EEPROM Clear –
  auf beiden Hälften. SYS muss bewusst gehalten werden. EEPROM Clear hat keine
  zusätzliche Haltefrist und wird bei Tastendruck ausgelöst.

Die drei Tap Dances starten mit je 250 ms. Bei früher Unterbrechung entscheidet
Studios Herstellerengine für Tap. Hyper und SHORT erst nach der Hold-Frist nutzen.
Die globalen Mod-/Layer-Tap-Startwerte sind 250 ms, Permissive Hold und Chordal
Hold an, Hold On Other Key Press aus. Das ist keine exakte ZMK-Flavor-Emulation.

Die einmalige Initialisierung läuft nur auf dem USB-Master und setzt diese vier
Einstellungen sowie TD-Slots 0–2, wenn die persönliche Studio-Kennung fehlt oder
abweicht oder VIA seinen Speicher neu initialisiert hat. Dabei können auch
bereits gespeicherte Werte in diesen vier Einstellungen und drei Slots ersetzt
werden. Weitere Slots bleiben erhalten. Die Kennung wird erst nach erfolgreichen
Schreibvorgängen gesetzt; normale Neustarts behalten spätere GUI-Änderungen.
Quick Tap wird nicht separat gesetzt; bei frischem VIA-Speicher startet es mit
250 ms. Vorhandene andere Quick-Tap-Werte bleiben erhalten.

Vor einem späteren Flash ist eine getrennte Entscheidung erforderlich.
Eine passende bestehende dynamische Keymap kann die einkompilierte Belegung
überlagern. Es wird absichtlich keine Versionsänderung erzwungen, die vorhandene
Einstellungen zurücksetzt. Auch die Herstellerinitialisierung bei inkompatiblem
EEPROM ist vor einem späteren Flash gesondert zu beurteilen.
