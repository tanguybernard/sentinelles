/*
 * Sentinelles — premier test "blink + série"
 *
 * But : vérifier que la carte, le câble, le driver USB-série,
 * l'upload du firmware et le port série fonctionnent tous.
 *
 * Comportement attendu :
 *   1. La LED onboard clignote 1 seconde / 1 seconde.
 *   2. Le moniteur série affiche un message à chaque clignote.
 *
 * Sur la plupart des DevKit V1 (DOIT ESP32 / WROOM-32),
 * la LED onboard est sur la broche GPIO 2.
 */

#include <Arduino.h>

// DevKit V1 : LED bleue onboard sur GPIO 2.
// GPIO 2 sert aussi au boot — pas de composant branché dessus pour l'instant,
// c'est sans risque en écriture simple.
static const int LED_PIN = 2;

void setup() {
  Serial.begin(115200);
  delay(200);  // laisser le driver USB-série s'ouvrir
  pinMode(LED_PIN, OUTPUT);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" Sentinelles — premier test   ");
  Serial.println("==============================");
  Serial.print("Reset cause : ");
  Serial.println(esp_reset_reason());
  Serial.print("Puce        : ESP32 rev ");
  Serial.println(ESP.getChipRevision());
  Serial.print("Heap libre  : ");
  Serial.print(ESP.getFreeHeap() / 1024);
  Serial.println(" Ko");
  Serial.println("LED sur GPIO 2 — clignote toutes les secondes.");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("[ON]  LED allumee");
  delay(1000);

  digitalWrite(LED_PIN, LOW);
  Serial.println("[OFF] LED eteinte");
  delay(1000);
}
