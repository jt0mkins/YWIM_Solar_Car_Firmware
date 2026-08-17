// Module 1: Blink Test
// Checks that the board is alive and programmable

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Set built-in LED as output
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // Turn LED ON
  delay(500);                       // Wait 0.5 seconds

  digitalWrite(LED_BUILTIN, LOW);   // Turn LED OFF
  delay(500);                       // Wait 0.5 seconds
}