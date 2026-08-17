// ==========================================================
// Module 3: Receiver Board - Servo + Low Battery + LED Test
// ==========================================================
// GOAL:
// 1. Built-in LED confirms board is running
// 2. External LED indicates LOW battery condition
// 3. Servo sweeps to confirm PWM + mechanical system works
//
// This tests:
// - Analog input (battery monitor)
// - Digital output (LEDs)
// - PWM output (servo)
// ==========================================================

#include <Servo.h>


// -----------------------------
// Pins
// -----------------------------
const int ledPin = A3;     // External LED (LOW battery indicator)
const int battPin = A0;    // Battery input (via divider)
const int servoPin = 8;    // Servo signal pin

const int lowBattThreshold = 211;


// -----------------------------
// Servo object
// -----------------------------
Servo steering;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  steering.attach(servoPin);
}


void loop() {

  // ==========================================================
  // Built-in LED heartbeat
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);


  // ==========================================================
  // Servo test (sweep motion)
  // ==========================================================
  for (int pos = 0; pos <= 180; pos += 5) {
    steering.write(pos);
    delay(10);
  }

  for (int pos = 180; pos >= 0; pos -= 5) {
    steering.write(pos);
    delay(10);
  }


  // ==========================================================
  // Battery reading
  // ==========================================================
  int battReading = analogRead(battPin);
  bool lowBatt = battReading < lowBattThreshold;


  // ==========================================================
  // External LED = LOW BAT indicator
  // ==========================================================
  digitalWrite(ledPin, lowBatt ? HIGH : LOW);
}