// ==========================================================
// Module 5: Receiver - Servo + Motor Driver (H-Bridge)
// ==========================================================
// GOAL:
// 1. Receive wireless control packet
// 2. Control steering servo
// 3. Control motor direction + speed (L298N / H-bridge)
// 4. Maintain battery safety indicator
//
// This is the FIRST time the system can move a vehicle
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
// Servo
// -----------------------------
Servo steering;
const int servoPin = 8;


// -----------------------------
// H-Bridge Motor Driver (L298N)
// -----------------------------
const int ENA = 3;   // PWM speed control
const int IN1 = 5;   // direction pin 1
const int IN2 = 6;   // direction pin 2


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
  Serial.begin(9600);

  // Servo
  steering.attach(servoPin);

  // Motor driver pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Battery LED
  pinMode(ledPin, OUTPUT);

  // Radio setup
  if (!radio.begin()) {
    Serial.println("❌ Radio failed to initialise");
    while (1);
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  Serial.println("=== RC Receiver with Motor Control ===");
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
    // Steering servo
    // -----------------------------
    steering.write(incoming.servoPos);

    // -----------------------------
    // Motor direction control
    // -----------------------------
    if (incoming.motorDir == 1) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    else if (incoming.motorDir == -1) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
    else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);  // stop
    }

    // -----------------------------
    // Motor speed (PWM)
    // -----------------------------
    analogWrite(ENA, incoming.motorSpeed);
  }


  // ==========================================================
  // BATTERY CHECK
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

  Serial.print(" | Dir: ");
  Serial.print(incoming.motorDir);

  Serial.print(" | Speed: ");
  Serial.print(incoming.motorSpeed);

  Serial.print(" | Batt: ");
  Serial.println(lowBatt ? "LOW" : "OK");


  delay(100);
}