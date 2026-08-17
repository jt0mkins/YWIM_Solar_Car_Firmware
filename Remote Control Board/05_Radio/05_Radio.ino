// ==========================================================
// Module 5: NRF24L01 Radio Transmitter (RC Sender)
// ==========================================================
// GOAL:
// - Read calibrated steering + throttle pots
// - Convert into control values
// - Send data wirelessly using NRF24L01
//
// This is the transmitter side of your RC system
// ==========================================================

#include <SPI.h>
#include <RF24.h>


// -----------------------------
// NRF24L01 Setup
// -----------------------------
RF24 radio(7, 9);  // CE, CSN
const byte address[6] = "00001";


// -----------------------------
// Pin Definitions
// -----------------------------
const int steerPotPin = A0;
const int drivePotPin = A1;
const int battPin = A7;
const int ledPin = 6;

const int lowBattThreshold = 211;


// ==========================================================
// CALIBRATION VALUES (learned at runtime)
// ==========================================================
int steerMin = 1023;
int steerMax = 0;

int driveMin = 1023;
int driveMax = 0;

int deadzone = 30;


// ==========================================================
// RADIO PACKET STRUCTURE
// ==========================================================
struct ControlPacket {
  int servoPos;
  int motorSpeed;
  int motorDir;
};

ControlPacket outgoing;


// ==========================================================
// SETUP
// ==========================================================
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

  // -----------------------------
  // Radio setup
  // -----------------------------
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  Serial.println("=== NRF24L01 RC Transmitter ===");
  Serial.println("Move sticks to calibrate + control output");
}


// ==========================================================
// LOOP
// ==========================================================
void loop() {

  // -----------------------------
  // Read inputs
  // -----------------------------
  int steerRaw = analogRead(steerPotPin);
  int driveRaw = analogRead(drivePotPin);
  int battReading = analogRead(battPin);

  bool lowBatt = battReading < lowBattThreshold;


  // ==========================================================
  // CALIBRATION (auto-learn min/max)
  // ==========================================================
  if (steerRaw < steerMin) steerMin = steerRaw;
  if (steerRaw > steerMax) steerMax = steerRaw;

  if (driveRaw < driveMin) driveMin = driveRaw;
  if (driveRaw > driveMax) driveMax = driveRaw;


  // ==========================================================
  // MAPPING (calibrated values → control outputs)
  // ==========================================================
  outgoing.servoPos = map(steerRaw, steerMin, steerMax, 0, 180);

  int driveCenter = (driveMin + driveMax) / 2;

  if (abs(driveRaw - driveCenter) < deadzone) {
    outgoing.motorDir = 0;
    outgoing.motorSpeed = 0;
  }
  else if (driveRaw < driveCenter) {
    outgoing.motorDir = -1;
    outgoing.motorSpeed = map(driveRaw, driveMin, driveCenter - deadzone, 255, 0);
  }
  else {
    outgoing.motorDir = 1;
    outgoing.motorSpeed = map(driveRaw, driveCenter + deadzone, driveMax, 0, 255);
  }


  // ==========================================================
  // TRANSMIT DATA
  // ==========================================================
  bool success = radio.write(&outgoing, sizeof(outgoing));


  // ==========================================================
  // SERIAL DEBUG
  // ==========================================================
  Serial.print("Steer: ");
  Serial.print(steerRaw);

  Serial.print(" | Servo: ");
  Serial.print(outgoing.servoPos);

  Serial.print(" | Drive: ");
  Serial.print(driveRaw);

  Serial.print(" | Dir: ");
  Serial.print(outgoing.motorDir);

  Serial.print(" | Speed: ");
  Serial.print(outgoing.motorSpeed);

  Serial.print(" | TX: ");
  Serial.print(success ? "OK" : "FAIL");

  Serial.print(" | Batt: ");
  Serial.println(lowBatt ? "LOW" : "OK");


  // ==========================================================
  // STATUS LED
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);

  digitalWrite(ledPin, lowBatt ? HIGH : LOW);
}