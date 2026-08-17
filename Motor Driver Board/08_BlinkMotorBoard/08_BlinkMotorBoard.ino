// ==========================================================
// Module 1: Receiver Board - Built-in LED Blink
// ==========================================================
// GOAL:
// - Confirm the motor driver board is powered
// - Confirm code uploads successfully
// - Confirm microcontroller is alive
//
// If this does NOT work, nothing else should be attempted.
// ==========================================================


void setup() {
  // Built-in LED on most Arduino boards
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // LED ON
  delay(500);

  digitalWrite(LED_BUILTIN, LOW);   // LED OFF
  delay(500);
}