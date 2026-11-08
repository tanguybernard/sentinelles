# Expérience n°01 — Blink + série (test matériel complet)

> **But** : vérifier que tout fonctionne — carte, câble USB, driver, compilation, upload, communication série.
> **Résultat attendu** : la LED bleue onboard clignote toutes les secondes, et les messages `[ON]/[OFF]` apparaissent dans le terminal.

## Le programme

Le firmware utilisé est dans ce dossier : [`main.cpp`](main.cpp).
C'est une copie conforme de celui qui a été flashé le premier jour.

Sans surprise, il fait tourner en boucle :

```cpp
digitalWrite(LED_PIN, HIGH);  // allumer GPIO 2 (la LED bleue)
delay(1000);                  // attendre 1 s
digitalWrite(LED_PIN, LOW);   // éteindre
delay(1000);                  // attendre 1 s
```

## Les commandes, pas à pas

Elles se tapent ** Depuis le dossier du projet (`sentinelles/`), pas depuis ce dossier-ci.
Sur votre machine, `pio` se trouve dans `~/.local/bin` — si la commande n'est pas
trouvée, ajouter `export PATH="$HOME/.local/bin:$PATH"` devant, ou tapez les
commandes depuis un terminal zsh neuf.

### 1. Compiler (traduit le code pour l'ESP32, 1ʳᵉ fois : longue, ensuite rapide)

```sh
pio run
```

### 2. Envoyer sur la carte (« flasher » — la carte doit être branchée en USB)

```sh
pio run -t upload
```

La carte redémarre toute seule à la fin, et le programme tourne.

### 3. Voir les messages (commande interactive — dans un vrai terminal)

```sh
pio device monitor
```

Pour sortir du moniteur : `Ctrl+C`.
Note : à chaque ouverture du moniteur, la carte redémarre — c'est normal.

### 4. Tout enchaîner d'un coup (compiler + envoyer + moniteur)

```sh
pio run -t upload && pio device monitor
```

## Dépannage rapide

| Symptôme | Cause probable |
|---|---|
| `pio` introuvable | `~/.local/bin` pas dans le PATH → réouvrir un terminal ou exporter le PATH |
| Pas de port `/dev/cu.usbserial-*` | câble USB "charge seule" sans fils data → changer de câble |
| Upload échoue, timetable ou permission | fermer tout moniteur série déjà ouvert (il bloque le port) |
| Charabia dans le moniteur | vitesses différentes de part et d'autre → vérifier 115200 dans le code **et** `monitor_speed` dans `platformio.ini` |
| Octets illisibles seulement au tout début | log du bootloader ROM à 74880 bauds → normal, ignorer |

## Petites variantes pour s'entraîner

1. **Rythme** : passer les deux `delay(1000)` à `delay(200)`.
2. **SOS** : 3 courts (100 ms), 3 longs (500 ms), 3 courts, puis pause de 2 s.
3. **Version sans `delay()`** (à connaître pour la suite du projet) :

```cpp
void loop() {
  digitalWrite(LED_PIN, (millis() / 1000) % 2);  // HIGH les secondes paires
}
```

`millis()` = le temps écoulé depuis le démarrage, en ms. Contrairement à
`delay()`, il ne bloque rien — indispensable dès qu'on lira des capteurs
« en parallèle » de la logique d'arrosage.
