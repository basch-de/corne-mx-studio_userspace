# Validierung

Stand 08.11.2026: **statische Quellenprüfung bestanden; Compiler- und
Hardwarevalidierung offen**. Keine UF2 wurde erzeugt. Die Installation der
isolierten lokalen Buildwerkzeuge wurde abgelehnt; der ARM-Compiler wurde
nicht installiert. Die für die Analyse heruntergeladenen Herstellerquellen und
rekursiven Submodule liegen in `/private/tmp/corne-keebart-studio`.

| Prüffeld | Quellcode / Offline | Firmware kompiliert | Hardware / GUI |
|---|---|---|---|
| External Userspace / Ziel | bestätigt | offen | nicht erforderlich |
| 8 Layer × 46 Positionen | alle 368 zugeordnet und geprüft | offen | offen |
| elektrische Spiegelung rechts | Layout gegen explizite Matrixliste geprüft | offen | offen |
| Layerzugänge / NUMPAD-Ausgang | Bindings und Priorität geprüft | offen | reale Sequenzen offen |
| Modifier, Shortcuts, Ziffern | Standardcodes geprüft | offen | Hostlayout/App offen |
| HRM / Tap-Hold | Hersteller-QS, Globals und Handtabelle geprüft | offen | Timingfolgen offen |
| Studio / Vial | Module, starke Hooks, UID, JSON und ABI geprüft | offen | beide Clients offen |
| RGB / Encoder | Herstellerpfade und 8 passende Encoder-Layer geprüft | offen | Effekte, Persistenz, Split offen |
| SYS beidseitig | beide Zugänge und gespiegelte Codes geprüft | offen | Einzelhälften offen |
| Flash / EEPROM | keine Geräteoperation ausgeführt | entfällt | kein Test autorisiert |

Ausgeführt:

```sh
python3 scripts/check_sources.py /private/tmp/corne-keebart-studio
git diff --check
```

Zusätzlich bestanden: JSON-Syntax aller Projektdateien, YAML-Syntax des
Workflows (Ruby/Psych) und Whitespace-Prüfung auch aller neuen, noch nicht
versionierten Dateien. Ergebnis der Zuordnung: 321 direkte Bindings, 28
Näherungen, 6 dokumentierte Ergänzungen, 3 aufgeteilte Zwischenlösungen,
2 nicht übertragene HID-Aktionen und 8 plattformspezifische Weglassungen.
Diese Kategorien sind pro Position, nicht pro Funktionsart gezählt.

Der Checker prüft außerdem Generator-Drift, Snapshot-Hashes, Herstellerdateien,
bekannte Codes, keine eigenen C-Funktionen, unverändertes Storage-ABI und korrekt
ausgecheckte Submodule. Er ersetzt keinen Firmwarecompiler. Die Workflow-Datei
ist für drei voneinander getrennte Builds vorbereitet. Ein erfolgreicher
Actions-Lauf wurde nicht behauptet oder vorweggenommen; es wurde kein Push
und kein Workflow-Dispatch ausgeführt.

## Nächster Buildnachweis

Nach Übernahme des Arbeitsstands müssen alle drei Jobs (`manufacturer`, `personal`,
`standard-default`)
erfolgreich sein. UF2s und Provenance müssen herunterladbar sein und zur
für den jeweiligen Lauf gemeinsam aufgelösten Branchrevision gehören.
Diese steht in jedem Provenance-Artefakt unter `firmware-commit.txt`.
Die persönliche Quellenprüfung prüft Kompatibilität statt einer festen Commit-ID
und läuft ausschließlich im persönlichen Job. Die Herstellerbuilds können
unabhängig von einer fehlgeschlagenen persönlichen Kompatibilitätsprüfung laufen.
Compilerfehler der Baseline sind von Fehlern
des Userspace zu unterscheiden. Fehlerlogs bleiben auch bei fehlgeschlagenem
Build als Provenance-Artefakt erhalten. Keine Artefakte aus einem alten Lauf
als Nachweis für einen neuen Stand verwenden.

## Späterer Hardwaretest nach gesonderter Entscheidung

Diese Liste ist ein Prüfplan, keine Freigabe für Flash, Reset oder Clear.

1. Hersteller-/Gerätezustand und bestehende Studio-/Vial-Daten sichern und
   den Übergang von der bisherigen Firmware bewusst entscheiden.
2. Matrix und beide Hälften mit einem Keytester prüfen; alle Layer aktivieren
   und verlassen; normale Ziffern bei beiden Num-Lock-Zuständen testen.
3. HRM: isolierter Tap, langer Hold, gleichseitige Rolls, gegenüberliegende
   verschachtelte Taps, alle sechs Daumen, Wiederholung und schnelle Doppelanschläge.
   Layer-Taps getrennt beurteilen, nicht von HRM-Ergebnissen ableiten.
4. Studio und Vial nacheinander prüfen: Identifikation, physisches Entsperren,
   Lesen aller 16 dynamischen Layer, eine reversible Belegungsänderung, erneutes
   Lesen in beiden Clients. Gleichzeitige Zugriffe sind nicht getestet.
5. Studio-Paletten, Effekte, Encoderoptionen, Makros, Pointer, Tap Dance,
   Locales und gespeicherte Einstellungen gezielt prüfen. Inaktive Features
   nicht allein wegen passender Buildflags als getestet markieren.
6. Nach später ausdrücklich erlaubtem Neustart Persistenz und Split-Synchronität
   kontrollieren. Jede Hälfte einzeln per USB auf ihre korrekte Hand und ihren
   SYS-Zugang prüfen. Keine SYS-Systemaktion zur bloßen Erreichbarkeitsprüfung
   drücken. Boot/Clear bleiben eigene destruktive Entscheidungen.

Insbesondere EE_CLR ist auf SYS direkt bei Tastendruck wirksam und weder ein
vollständiger Studio-Factory-Reset noch automatisch ein Reset beider Controller.
Die mögliche zusätzliche Haltesicherung ist die einzige derzeit vorgelegte
Sonderentscheidung; siehe [Abweichungsbericht](DEVIATIONS.md).
