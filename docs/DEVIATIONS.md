# Abweichungsbericht und Einzelentscheidung

Die Standardportierung enthält keine eigenen Firmware-Funktionen.
Status **direkt** bedeutet semantische Zuordnung im Quellcode, nicht einen
Hardwaretest. Alle 368 Originalbindings stehen im [Mapping](MAPPING.md).
Aufwandsschätzungen beschreiben spätere Arbeit einschließlich Prüfung.

## Einzelbewertung

| Funktion | Ursprüngliches ZMK-Verhalten | Standardportierung | Konkrete Abweichung | Möglicher nächster Weg | Benötigte Hooks / Erweiterung | Risiko für Studio/Vial | Aufwand / Empfehlung |
|---|---|---|---|---|---|---|---|
| HRM `hml`/`hmr` | balanced, 280/200 ms, gegenüberliegende Haupttasten + sechs Daumen, Trigger auf Release | `MT`, global anfangs 250/250 ms, Standard-Handtabelle vorhanden | kein pro-Behavior-Timing; Hold-Entscheidung und Release-Filter abweichend; Chordal initial aus | zunächst Vial-Settings, danach vorhandene Studio Compound Actions mit Chordal und Daumenausnahmen prüfen | für Standardsettings keine; exakter Nachbau könnte Vial-Timinghooks ersetzen oder eine eigene Engine benötigen | Standard niedrig; exakter Eigenbau hoch wegen belegter Hooks | Settings klein, exakter Nachbau hoch; vorerst vorhandene Einstellungen testen |
| `hlt` | hold-preferred, 200 ms; NAV/Escape, NUM/Backspace, SYM/Space, SYS/Grave | `LT` mit globalen Settings | nicht hold-preferred pro Taste, initial 250 ms; empfohlenes Permissive Hold bleibt globale Näherung | Studio Compound Action pro Taste mit Hold-on-other-press-ähnlichem Modus | keine bei GUI-Konfiguration; keine eigenen Callbacks vorgesehen | niedrig auf Firmwareseite, reale Engine-/GUI-Interaktion offen | klein bis mittel; Standardfunktion bevorzugen |
| `rhlt` WIN/Tab | hold-preferred 200 ms, Quick Tap 250 ms | `LT(WIN, KC_TAB)` | Flavor global und Quick-Tap-Semantik anders; Studio begrenzt Quick auf Term | vorhandene Studio Compound Action, Quick ≤ Term | keine | niedrig; Verhalten prüfen | klein; keine identischen ZMK-Werte versprechen |
| `rlt` NUMPAD/Enter | tap-preferred 200 ms, Quick Tap 250 ms | `LT(NUMPAD, KC_ENT)` | ursprüngliche Unterscheidung zu `hlt` entfällt, Quick-Tap-Regel anders | Studio Compound Action mit tap-preferred | keine | niedrig; Engine-Test offen | klein; Standardfunktion testen |
| SHORT/Equal `&lt` | eingebautes ZMK-Layer-Tap | `LT(SHORT, KC_EQL)` | ZMK-Standarddefaults sind nicht QMK-global 250 ms | vorhandenes Timing nach Nutzung einstellen | keine | niedrig | klein; Standard beibehalten |
| MEH/Quote | eingebautes ZMK-Mod-Tap, Ctrl+Alt+Shift / Quote | `MEH_T(KC_QUOT)` | Aktionen erhalten, Default-Timing/Entscheidung unterschiedlich | Vial oder Studio Timing einstellen | keine | niedrig | klein; Standard beibehalten |
| Hyper/Cmd+Space | Hold vier linke Mods; Tap GUI+Space | Originalposition reines Hyper; GUI+Space separat SHORT:14 | kombinierte Taste aufgeteilt, Tap allein sendet keinen Shortcut | native Studio Compound Action: Tap `LGUI(KC_SPC)`, Hold `KC_HYPR` | keine bei Studio-Konfiguration; alternative eigene `process_record_user` wäre konfliktträchtig und wird nicht empfohlen | native Konfiguration niedrig; eigener User-Hook kollidiert mit Studio Dance | klein bis mittel; Studio statt Sondercode |
| Caps Word/SHORT | Tap aktiviert Caps Word, Hold SHORT; hold-preferred 200 ms | rechter äußerer Daumen `MO(SHORT)`, SHORT:0 `CW_TOGG` | Tap entfällt; separater Toggle kann aktives Caps Word ausschalten; Fortsetzungsregeln unterschiedlich | Studio Compound Action Tap `CW_TOGG`, Hold `MO(SHORT)` bietet Aktionen, aber Toggle und Unterbrechungsregeln bleiben prüfen | keine für Herstellerfunktion; exaktes Enable könnte eigene Action erfordern | native Konfiguration niedrig; eigene Record-Hooks hoch | klein bis mittel; zunächst native Kombination prüfen |
| Komma/Punkt | eingebautes `&mt DOT COMMA` | NUMPAD:38 Komma, NUMPAD:32 separater Punkt | nicht mehr eine Dual-Role-Taste; dies ist kein gültiges QMK-Mod-Tap mit Punkt als Modifier | Studio Compound Action Tap `KC_COMM`, Hold `KC_DOT`; alternativ bestehender dynamischer Tap Dance | keine bei GUI-Konfiguration | niedrig; Punkt wird beim gehaltenen Output gedrückt und kann hostseitig wiederholen; Tap Dance entscheidet bei Unterbrechung Tap | klein; Herstellerfunktion bevorzugen |
| Voice Command | Consumer Voice Command | SHORT:35 `KC_NO` | Diktierfunktion fehlt; Assistant ist anderer HID-Code | zuerst Hostshortcut als ausdrücklich gewählte Standardalternative prüfen; exakter Consumer-Report wäre eigene Aktion | für exakten Code vermutlich Custom-Keycode plus geprüfter Record-Erweiterungspunkt; `process_record_user` gehört Studio | eigene Verkettung kann Dance/Locales/Combos beeinflussen; derzeit keine Freigabe | mittel; keine automatische Ersetzung durch Assistant |
| Consumer Power | `C_PWR` auf SYS:13 | `KC_NO` | Power-Aktion fehlt; `KC_PWR` ist System Power Down mit anderer Usage-Page | Hostfunktion bewusst auswählen; exakte Consumer-Power-Action nur separat prüfen | exakter Report würde eigene Aktion und konfliktfreien Hook benötigen | mittel bis hoch bei eigener Verarbeitung | klein für gewählte Standardalternative, mittel für exakte Aktion; zunächst deaktiviert lassen |
| Caps-Word-Regeln | ZMK-Behaviordefaults | QMK-Standard, vom Hersteller `CAPS_WORD_INVERT_ON_SHIFT` | Toggle und Fortsetzungs-/Shiftregeln sind nicht als identisch bestätigt | auf Hardware Sequenzen prüfen; erst danach Bedarf für eigene Regeln | eigene `caps_word_press_user` wäre eine freizugebende Erweiterung | möglicher Eingriff in Hersteller-/Locale-Verhalten | mittel; keine Regeln blind nachbauen |
| RGB und `.conf` | ZMK-Underglow, definierte Startfarbe/Schritte, Idle-Abschaltung | originale Studio/RGB-Matrix-Defaults, `RM_*` auf SYS, persistente Paletten erhalten | andere Effektliste, Skalen und Idle-Definition; Sleep ≠ ZMK-Idle | vorhandene Studio-Paletten und Lighting-Idle einstellen | keine; eigene Layer-/Scan-Hooks nicht erforderlich | niedrig bei Herstellerfunktionen | klein; originalen RGB-Pfad erhalten |
| Bluetooth / externe Versorgung / ZMK Unlock | fünf Slots, Clear, EP Toggle, ZMK Studio Unlock | jeweilige Position `KC_NO`; physischer Vial-Unlock bleibt | bewusster Wegfall der plattformspezifischen Aktionen | kein Nachbau; Keebart Studio benutzt eigenes Protokoll | keine | kein Eingriff | keiner; weglassen |
| SYS einhändig | ZMK-Zugang nur links | zusätzlicher rechter Backslash/SYS-Layer-Tap | neue Hold-Funktion rechts; gleiche Tap-Ausgabe | Einzelhälften nach gesonderter Flashentscheidung testen | keine | niedrig; Master-/Handerkennung hardwareabhängig | klein; Quellenpfad plausibel, Praxistest offen |
| Reset/Boot/EE Clear | ZMK Reset/Boot auf beiden Seiten; kein EE Clear | QMK Reset/Boot/Clear gespiegelt auf SYS | wirkt im Split auf Master; Clear ist nicht Studio Factory Reset | vorhandenen Studio-Reset bei späterem Bedarf bewusst verwenden | keine für Standardaktionen | Herstellerfunktionen bleiben; EEPROM-Zustand kann danach neu initialisiert werden | klein; keine Resetversuche in dieser Phase |
| zusätzlicher Clear-Schutz | ZMK enthielt kein EE Clear | gehaltenes SYS + separate Clear-Taste; kein Toggle-Zugang | kein zusätzlicher Countdown, keine Bestätigungsfolge; versehentliches Drücken innerhalb SYS bleibt möglich | optional nur EE_CLR nach mindestens 2 s eigenem Halten auslösen und bei Release abbrechen | eigener Record-Abfangpunkt und periodischer Timer, z.B. geprüfte `process_record_kb`/`matrix_scan_user`-Kette; erst nach Einzelfreigabe | mittel: User-Hooks sind teilweise belegt; vollständige Weiterleitung muss nachgewiesen werden | mittel; erste vorgeschlagene Sonderentscheidung, noch nicht implementiert |
| persistente Defaults | ZMK statische Bindings | Vial lädt kompatible gespeicherte Bindings; eingebaute neue Defaults ersetzen sie nicht automatisch | späteres Flash allein garantiert keine neue sichtbare Belegung | spätere getrennte Entscheidung für Sicherung und bewusste Übernahme; keine automatischen Resets | keine automatische Initialisierung oder `eeconfig_init_user` | erzwungene Kennungsänderung könnte Daten verlieren | Übergangsplanung erforderlich; Hersteller-ID unverändert |

## Vorhandene Studio-Funktionen vor Sondercode verwenden

Die folgenden Einträge sind **Konfigurationsrezepte für später**, keine bereits
in ein Gerät geschriebene Konfiguration und kein behauptetes Importformat.
Firmwarefähigkeiten wurden anhand von `locales/locale.c`, `locale_runtime.c`
und `studio_locale.c` geprüft; die konkrete Studio-Oberfläche ist noch nicht
getestet. Ein Factory Reset ist für diese Prüfung nicht vorgesehen.

| Aktion | Tap | Hold | Vorschlag für ersten Versuch |
|---|---|---|---|
| Hyper | `LGUI(KC_SPC)` | `KC_HYPR` | 200–250 ms, Unterbrechung bewusst testen |
| Caps/SHORT | `CW_TOGG` | `MO(5)` | 200 ms, hold-preferred-ähnliche Einstellung; Toggle-Abweichung bleibt |
| Komma/Punkt | `KC_COMM` | `KC_DOT` | 200 ms; Wiederholung beim Halten testen |
| HRM | jeweiliger Buchstabe | jeweiliger Modifier | 280 ms / Quick 200 ms, Permissive + Chordal, Daumen ausnehmen |
| Layer-Taps | jeweiliger Basistastencode | `MO(layer)` | 200 ms; `hlt` und `rlt` getrennt konfigurieren |

Studio-Compound-Timing ist nicht QMKs globales Mod-Tap-Timing. Insbesondere
Quick Tap > Term wird im Herstellercode begrenzt. ZMKs 250-ms-Quick-Tap bei
200-ms-Term kann deshalb nicht als identisches Zahlenpaar übernommen werden.
Der vorhandene Studio Tap Dance ist eine weitere Standardoption, löst bei
Unterbrechung aber einen Tap auf; das ist kein gleichwertiges hold-preferred.
Dynamische Einträge werden gemeinsam mit Vial gespeichert und müssen vor
Benutzung samt Tastenbindung eingerichtet sein. Keine Slots werden ungefragt
belegt oder bestehende Einträge überschrieben.

## Genau eine erste offene Sonderentscheidung

Vorschlag: **zusätzliche Haltesicherung ausschließlich für EE_CLR**.
Gewünschtes Verhalten: nach Aktivierung von SYS die Clear-Taste mindestens zwei
Sekunden halten; Loslassen vorher bricht ab. Bootloader und Neustart würden
hierbei nicht mitgeändert.

Das ist bisher nicht implementiert. Eine Zustimmung gilt nur für diesen Schutz
und den dafür erforderlichen erneuten Hook-/Verkettungsaudit. Wenn eine
konfliktfreie Weiterleitung an alle Studio-/Vial-Pfade nicht nachweisbar ist,
wird der Schutz nicht implementiert. Alle anderen Sonderentscheidungen bleiben
offen. Bis zu einer Entscheidung bleibt der dokumentierte Standard-
Layerzugang der einzige Clear-Schutz; es findet weiterhin kein Flash statt.
