// ==========================================================
// Module 8: NRF24L01 Receiver + Servo Control
// ==========================================================
// GOAL:
// 1. Receive control packet from transmitter
// 2. Drive servo from received steering value
// 3. Monitor battery and indicate low battery
// 4. Show radio link status in Serial Monitor
//
// This is the FIRST real wireless control step
// ==========================================================

#include <SPI.h>
#include <RF24.h>
#include <Servo.h>


// -----------------------------
// Radio Setup
// -----------------------------
RF24 radio(7, 9);  // CE, CSN
const byte address[6] = "00001";

struct ControlPacket {
  int servoPos;
  int motorSpeed;
  int motorDir;
};

ControlPacket incoming;


// -----------------------------
// Servo Setup
// -----------------------------
Servo steering;
const int servoPin = 8;


// -----------------------------
// Battery System
// -----------------------------
const int battPin = A0;
const int ledPin = A3;
const int lowBattThreshold = 211;


// ==========================================================
// SETUP
// ==========================================================
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  steering.attach(servoPin);

  Serial.begin(9600);

  // -----------------------------
  // Radio init
  // -----------------------------
  if (!radio.begin()) {
    Serial.println("❌ Radio failed to initialise");
    while (1);
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  Serial.println("=== Receiver Ready (Radio Active) ===");
}


// ==========================================================
// LOOP
// ==========================================================
void loop() {

  // ==========================================================
  // RADIO RECEIVE
  // ==========================================================
  bool radioAvailable = radio.available();

  if (radioAvailable) {
    radio.read(&incoming, sizeof(incoming));

    // -----------------------------
    // Servo control (from transmitter)
    // -----------------------------
    steering.write(incoming.servoPos);
  }


  // ==========================================================
  // BATTERY MONITOR
  // ==========================================================
  int battReading = analogRead(battPin);
  bool lowBatt = battReading < lowBattThreshold;

  digitalWrite(ledPin, lowBatt ? HIGH : LOW);


  // ==========================================================
  // BUILT-IN LED (heartbeat)
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);


  // ==========================================================
  // SERIAL DEBUG
  // ==========================================================
  Serial.print("Radio: ");
  Serial.print(radioAvailable ? "OK" : "NO SIGNAL");

  Serial.print(" | Servo: ");
  Serial.print(incoming.servoPos);

  Serial.print(" | Batt: ");
  Serial.println(lowBatt ? "LOW" : "OK");


  delay(100);
}