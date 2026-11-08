/*
 * Sentinelle — expérience n°02 : bouton poussoir + LED externe
 *
 * Premier contact avec le breadboard et les ENTRÉES.
 *
 * Comportement attendu :
 *   - appui sur le bouton  → LED externe allumée ;
 *   - relâchement          → LED éteinte.
 *
 * Câblage :
 *   GPIO 4 → résistance 220 Ω → anode (patte longue) de la LED
 *   cathode de la LED → ligne GND (bleue) du breadboard
 *
 *   GPIO 14 → patte A du bouton (fils du bouton en DIAGONALE)
 *   patte B du bouton → ligne GND
 *   (pas d'autre résistance : on utilise le mode INPUT_PULLUP
 *    de l'ESP32, qui a une résistance de tirage intégrée)
 *
 * Le point clé à comprendre : la broche du bouton est "flottante"
 * quand on n'appuie pas — INPUT_PULLUP la tire à 3,3 V par défaut,
 * l'appui la connecte à GND. C'est pourquoi un appui se lit LOW
 * et un non-appui se lit HIGH.
 */

#include <Arduino.h>

static const int LED_PIN = 4;
static const int BTN_PIN = 14;
// Attention : internes ! Les GPIO 34-39 ne savent QUE lire.
// LED sur GPIO 4, bouton sur GPIO 14 : deux broches sûres sur une DevKit V1.

// Luminosité de la LED quand on appuie : de 0 (éteinte) à 255 (pleine puissance).
// analogWrite fait clignoter la LED très vite (PWM) : l'œil voit une LED moins forte.
static const int LUMINOSITE = 40;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);   // tirage interne à 3,3 V
  Serial.println();
  Serial.println("==============================");
  Serial.println(" Sentinelles — experience 02  ");
  Serial.println(" bouton + LED externe         ");
  Serial.println("==============================");
}

void loop() {
  // Le bouton relié à GND : appui = LOW, relâché = HIGH.
  int etat = digitalRead(BTN_PIN);
  analogWrite(LED_PIN, etat == LOW ? LUMINOSITE : 0);

  // --- Instrumentation de diagnostic ---
  // On ne parle que quand l'état CHANGE, plus un résumé toutes les 2 s.
  static int precedent = HIGH;
  static unsigned long dernierResume = 0;

  if (etat != precedent) {
    Serial.println(etat == LOW ? "appui  -> LED allumee"
                               : "relache -> LED eteinte");
    precedent = etat;
  }

  if (millis() - dernierResume > 2000) {
    Serial.print("etat bouton = ");
    Serial.print(etat == LOW ? "APPUI  " : "REPOS  ");
    Serial.print(" | LED = ");
    Serial.print(etat == LOW ? 1 : 0);
    Serial.print(" | temps = ");
    Serial.print(millis() / 1000);
    Serial.println(" s");
    dernierResume = millis();
  }

  delay(20);  // petit anti-rebond
}
