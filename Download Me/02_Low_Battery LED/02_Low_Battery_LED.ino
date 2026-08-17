// ==========================================================
// Module 2: Receiver Board - LED Output Test
// ==========================================================
// GOAL:
// 1. Confirm built-in LED works (board alive check)
// 2. Confirm external LED circuit on PCB works
//
// This tests:
// - Microcontroller output pins
// - PCB soldering
// - External LED polarity + wiring
// ==========================================================


// -----------------------------
// External LED pin
// -----------------------------
const int lowBatPin = A3;  // External LED connected on PCB


void setup() {
  // Built-in LED
  pinMode(LED_BUILTIN, OUTPUT);

  // External LED
  pinMode(lowBatPin, OUTPUT);
}


void loop() {

  // ==========================================================
  // BOTH LEDs ON
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);  	// Built-in LED ON
  digitalWrite(lowBatPin, HIGH);       	// External LED ON
  delay(500);				// Wait for 500 ms


  // ==========================================================
  // BOTH LEDs OFF
  // ==========================================================
  digitalWrite(LED_BUILTIN, LOW);   	// Built-in LED OFF
  digitalWrite(lowBatPin, LOW);        	// External LED OFF
  delay(500);				// Wait for 500 ms
}