# Veille technique — notions d'électronique

Les notions apprises au fil des expériences, expliquées simplement. À compléter à chaque nouvelle expérience.

---

## 1. Le courant fait une boucle

Image de l'eau dans un tuyau : elle ne coule que s'il y a une **différence de hauteur** et un **chemin complet** du départ à l'arrivée. Si le chemin est coupé quelque part, plus rien ne coule.

| Élément | Rôle | Image |
|---|---|---|
| Broche de sortie (ex. D4) mise à `HIGH` | sort **3,3 V** | le départ, en haut |
| **GND** (*ground*, « masse ») | **0 V** | l'arrivée, en bas |

- Tout courant qui sort de l'ESP32 doit **revenir à GND**. Sans fil vers GND, la boucle est ouverte et rien ne marche.
- Toutes les broches GND de la carte sont **reliées entre elles** à l'intérieur : on prend la plus proche.

## 2. La résistance

- Elle **freine le courant**. Une LED seule laisserait passer trop de courant : elle chaufferait et pourrait griller, elle ou la broche de l'ESP32.
- La placer **avant ou après** la LED revient au même : le courant traverse les deux l'un après l'autre, et il est freiné de la même façon.
- Valeur utilisée : **220 Ω** (bandes rouge-rouge-brun). Une valeur plus forte (1 kΩ) donne une LED moins lumineuse.

## 3. La LED a un sens

- Le courant ne la traverse que du **+** (anode, **patte longue**) vers le **−** (cathode, patte courte).
- Montée à l'envers, elle reste éteinte, sans être abîmée.

## 4. Les broches de l'ESP32 (GPIO)

- Les broches « D » (D4, D14, D23…) sont **programmables** : chacune peut servir de **sortie** (envoyer du courant) ou d'**entrée** (lire un état).
- Le choix d'une broche est **en partie arbitraire**. La seule règle absolue : **le numéro dans le code doit correspondre au fil branché.**
  - Dans le code : `LED_PIN = 4` signifie « la LED est sur D4 ».
- Broches à éviter : **34 à 39**, qui ne savent que **lire** (pas de sortie possible).
- Le **côté** de la carte où se trouve une broche n'a **aucune importance électrique** : les broches sont simplement réparties sur deux rangées.

## 5. Sortie ou entrée ?

### Sortie : la LED
L'ESP32 **envoie** du courant :

```
D4 (3,3 V) → résistance → LED (+ puis −) → GND (0 V)
```

### Entrée : le bouton, avec INPUT_PULLUP
L'ESP32 **écoute** :

- `pinMode(BTN_PIN, INPUT_PULLUP)` relie la broche à 3,3 V **à l'intérieur de la puce**, à travers une résistance. Au repos, elle lit **HIGH**.
- L'appui relie la broche **directement à GND**. Elle lit alors **LOW**.
- Conséquence à retenir : **relâché = HIGH, appuyé = LOW**. C'est l'inverse de l'intuition.
- C'est pour ça que le bouton n'a besoin que de **deux fils** (broche + GND), sans résistance externe.

Pourquoi ne pas laisser la broche seule ? Une entrée reliée à rien est **flottante** : elle lit des valeurs au hasard (du bruit).

## 6. La breadboard

```
     1   2   3   4   5   6 ...
A    o   o   o   o   o   o        ┐
B    o   o   o   o   o   o        │  dans une RANGÉE (même numéro),
C    o   o   o   o   o   o        │  A-B-C-D-E sont RELIÉS
D    o   o   o   o   o   o        │
E    o   o   o   o   o   o        ┘
    ──────── creux central ────────   ← coupe chaque rangée en deux
F    o   o   o   o   o   o        ┐
G    o   o   o   o   o   o        │  F-G-H-I-J : autre groupe relié,
H    o   o   o   o   o   o        │  séparé de A-E
I    o   o   o   o   o   o        │
J    o   o   o   o   o   o        ┘
```

- **Même rangée** (même numéro, même côté du creux) → **branché ensemble**.
- **Même colonne** (même lettre : A1, A2, A3…) → **pas relié**. Chaque rangée est isolée de ses voisines.
- Ce n'est pas « côte à côte » qui compte, c'est **« même rangée »** : un fil en E1 au lieu de B1 marche pareil.
- Pour passer d'une rangée à une autre, il faut un **composant** (résistance, LED, bouton) ou un **fil**.
- **Lignes + et − sur les bords** (rouge et bleue) : c'est l'inverse, chaque ligne est reliée **sur toute sa longueur**. On s'en sert pour distribuer GND ou le 3,3 V partout.
  - Ma breadboard en a des deux côtés. Les trous y sont groupés par **5, avec un petit espace entre les groupes** : ces espaces sont **purement visuels** (pour compter les trous), la ligne reste continue.
  - Exception à connaître : sur certaines breadboards longues, la ligne est **coupée en deux au milieu** (le trait rouge ou bleu s'interrompt). Pour vérifier : multimètre en mode continuité (bip) entre les deux bouts de la ligne.
  - Usage type : **un seul fil** de GND de l'ESP32 vers la ligne **−** ; ensuite, chaque composant qui doit aller à GND est relié à cette ligne par un fil court. Pareil avec le 3,3 V sur la ligne **+**.
  - Les lignes d'un côté et de l'autre de la breadboard **ne sont pas reliées entre elles** : il faut un fil pour les relier si on veut GND des deux côtés.

## 7. Le bouton poussoir 4 pattes

- On le pose **à cheval sur le creux central**.
- Les pattes qui se font face de part et d'autre du creux sont **toujours reliées** entre elles. L'appui relie ensuite les deux rangées.
- Règle à retenir : brancher les deux fils **en diagonale** (une rangée d'un côté du creux, l'autre rangée de l'autre côté). Ça marche quel que soit le sens du bouton.
- Si les deux fils sont **sur la même rangée**, le bouton les relie en permanence : il paraît « toujours appuyé ».

## 8. Régler la luminosité : le PWM

- `digitalWrite` : tout ou rien (0 V ou 3,3 V).
- `analogWrite(broche, valeur)`, avec une valeur de **0 à 255** : la broche s'allume et s'éteint très vite. L'œil voit une LED **moins forte**. C'est le **PWM**.
- Autre solution, côté matériel : une résistance plus forte.

---

## Expérience 02 — câblage final (validé)

```
D4  → B1 ═ A1 → résistance 220 Ω → A5 ═ B5 → LED (+ B5, − B6) → B6 ═ C6 → GND
D14 → A10 ═ D10 → [bouton D10/D12/G10/G12] → G12 ═ H12 → GND
```

`═` : liaison cachée dans la breadboard (même rangée). `→` : composant ou fil.

| Fil | De (breadboard) | Vers (ESP32) |
|---|---|---|
| Signal LED | B1 | D4 (GPIO 4) |
| GND LED | C6 | GND (côté D4) |
| Signal bouton (rouge) | A10 | D14 (GPIO 14) |
| GND bouton | H12 | GND (côté D14) |

Code : `LED_PIN = 4`, `BTN_PIN = 14`, `LUMINOSITE = 40`.

## Pièges rencontrés (et comment je les ai trouvés)

| Symptôme | Cause | Comment on l'a vu |
|---|---|---|
| Rien ne se passe à l'appui | Le code utilisait D23 et D4, les fils étaient sur D4 et D14 | En comparant le code et le câblage |
| Bouton « toujours appuyé » | Les deux fils du bouton étaient sur la même rangée (11) | Le moniteur série affichait `APPUI` sans toucher au bouton |
| LED faiblement allumée en permanence | La broche de la LED était lue comme une entrée avec INPUT_PULLUP : un petit courant passait | Elle est devenue franche une fois la broche réglée en sortie |

Réflexe de débogage : **ouvrir le moniteur série** (`pio device monitor`) pour voir ce que la carte lit vraiment.

---

## À venir

- **Expérience 03 — toggle** : un appui allume, l'appui suivant éteint. Même câblage, seul le code change. Notion nouvelle : l'**anti-rebond** (un seul appui peut être compté plusieurs fois si on ne le gère pas).
