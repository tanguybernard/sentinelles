# Atelier Sentinelle

Projets d'initiation et d'expérimentation ESP32. **Un dossier = une expérience**, numérotée 01, 02, … Chaque dossier est auto-contenu : son `platformio.ini`, son `src/`, son README pas à pas.

```
sentinelles/
├── 01-blink-serial/        ✅ fait — LED onboard + port série (test matériel complet)
├── 02-bouton-led-externe/  ✅ fait — bouton poussoir + LED externe sur breadboard (entrées)
├── 03-toggle-led/          ✅ fait — un appui allume, le suivant éteint (mémoire + anti-rebond)
└── ...                     🔜 à venir : arrosage-plante (sonde d'humidité + relais)
```

## Idées pour plus tard

- **Lampe de bureau moulin à vent** — [vidéo](https://www.youtube.com/watch?v=NdYTs1NasPw), [guide et liste de matériel](https://www.huyvector.org/decor-lamps/windmill-desk-lamp). Sculpture en fil de cuivre et tube laiton soudés. Le montage d'origine n'a pas de microcontrôleur : batterie + module de charge USB-C + variateur de vitesse + moteur à réducteur 3 V 30 tr/min + ampoule LED Edison 39 mm. Pistes ESP32 : remplacer le variateur par du PWM (vu en 02) à travers un transistor, bouton marche/arrêt (vu en 03).

## Comment travailler sur une expérience

Se placer **dans son dossier**, puis :

```sh
cd 02-bouton-led-externe
pio run            # compiler
pio run -t upload  # flasher la carte branchée en USB
pio device monitor # voir les messages (Ctrl+C pour sortir)
```

## Créer une nouvelle expérience

On ne part jamais de zéro : on copie la dernière expérience et on vide son code.

```sh
cp -r 02-bouton-led-externe 03-mon-nom
rm 03-mon-nom/src/main.cpp 03-mon-nom/README.md   # on réécrit à neuf
cd 03-mon-nom
```

Les `.pio/` (caches de build) sont recréés automatiquement, pas la peine d'y penser.

## Notes

- Carte de référence : **ESP32 DevKit V1** (module WROOM-32, 38 broches).
- Toolchains partagés une seule fois dans `~/.platformio` — les compilations sont rapides après la première.
- Deux cartes branchées en même temps : préciser `--upload-port /dev/cu.usbserial-XXXX` (les lister avec `pio device list`).
- Notions apprises (électronique, breadboard, pièges) : voir **[VEILLE.md](VEILLE.md)**.
