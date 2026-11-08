/*
 * Sentinelle — expérience n°03 : toggle
 *
 * Comportement attendu :
 *   - un appui sur le bouton  → la LED s'allume et RESTE allumée ;
 *   - l'appui suivant         → la LED s'éteint et RESTE éteinte.
 *
 * Câblage : identique à l'expérience 02 (voir cablage.svg).
 *   GPIO 4  → résistance 220 Ω → LED → GND
 *   GPIO 14 → bouton (fils en diagonale) → GND
 *
 * Deux notions nouvelles :
 *
 * 1. MÉMORISER UN ÉTAT. En exp. 02, la LED recopiait le bouton à chaque
 *    instant. Ici, le programme se souvient si la LED est allumée
 *    (variable ledAllumee) et l'inverse à chaque appui.
 *
 * 2. L'ANTI-REBOND. Un bouton mécanique ne fait pas un contact net :
 *    pendant quelques millisecondes, il « rebondit » (contact, pas
 *    contact, contact…). Sans précaution, un seul appui serait compté
 *    plusieurs fois, et la LED changerait d'état au hasard.
 *    Solution : on n'accepte un nouvel état du bouton que s'il est
 *    resté stable pendant ANTI_REBOND_MS.
 */

#include <Arduino.h>

static const int LED_PIN = 4;
static const int BTN_PIN = 14;

// Luminosité de la LED allumée : de 0 (éteinte) à 255 (pleine puissance).
static const int LUMINOSITE = 40;

// Durée pendant laquelle le bouton doit rester stable pour être cru.
static const unsigned long ANTI_REBOND_MS = 50;

bool ledAllumee = false;           // l'état mémorisé de la LED
int etatStable = HIGH;             // dernier état du bouton accepté (HIGH = relâché)
int derniereLecture = HIGH;        // dernière lecture brute, rebonds compris
unsigned long dernierChangement = 0;
unsigned long nombreAppuis = 0;
unsigned long nombreRebonds = 0;   // changements bruts ignorés (pour la curiosité)

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);   // relâché = HIGH, appuyé = LOW
  analogWrite(LED_PIN, 0);          // on démarre LED éteinte

  Serial.println();
  Serial.println("==============================");
  Serial.println(" Sentinelles — experience 03  ");
  Serial.println(" toggle : appui = on / off    ");
  Serial.println("==============================");
}

void loop() {
  int lecture = digitalRead(BTN_PIN);

  // La lecture brute vient de changer : on relance le chronomètre.
  if (lecture != derniereLecture) {
    derniereLecture = lecture;
    dernierChangement = millis();
    nombreRebonds++;
  }

  // La lecture est stable depuis assez longtemps ET différente de l'état accepté :
  // c'est un vrai changement du bouton.
  if (millis() - dernierChangement > ANTI_REBOND_MS && lecture != etatStable) {
    etatStable = lecture;

    // On ne réagit qu'au moment de l'APPUI (passage HIGH → LOW),
    // pas au relâchement : c'est la « détection de front ».
    if (etatStable == LOW) {
      ledAllumee = !ledAllumee;
      analogWrite(LED_PIN, ledAllumee ? LUMINOSITE : 0);
      nombreAppuis++;

      Serial.print("appui n°");
      Serial.print(nombreAppuis);
      Serial.print(ledAllumee ? " -> LED allumee" : " -> LED eteinte");
      Serial.print("   (changements bruts vus depuis le debut : ");
      Serial.print(nombreRebonds);
      Serial.println(")");
    }
  }
}
