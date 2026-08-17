// ==========================================================
// Module 3: Two Potentiometers + Serial + Low Battery LED
// ==========================================================
// This program:
// 1. Blinks built-in LED (system alive)
// 2. Reads TWO potentiometers:
//      - A0 = Steering
//      - A1 = Throttle
// 3. Prints both values to Serial Monitor
// 4. Uses external LED as LOW BATTERY indicator
// ==========================================================


// -----------------------------
// Pin Definitions
// -----------------------------
const int ledPin = 6;        // External LED (LOW BAT)
const int steerPotPin = A0;  // Steering pot
const int drivePotPin = A1;  // Throttle pot
const int battPin = A7;      // Battery via divider

const int lowBattThreshold = 211;


void setup() {
  // LEDs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Serial
  Serial.begin(9600);
  Serial.println("Steering + Throttle + Battery Monitor");
}


void loop() {
  // ==========================================================
  // Read potentiometers
  // ==========================================================
  int steerRaw = analogRead(steerPotPin);  // 0 → 1023
  int driveRaw = analogRead(drivePotPin);  // 0 → 1023


  // ==========================================================
  // Read battery
  // ==========================================================
  int battReading = analogRead(battPin);
  bool lowBatt = battReading < lowBattThreshold;


  // ==========================================================
  // Serial Output
  // ==========================================================
  Serial.print("SteerRaw: ");
  Serial.print(steerRaw);

  Serial.print(" | DriveRaw: ");
  Serial.print(driveRaw);

  Serial.print(" | BatteryRaw: ");
  Serial.print(battReading);

  Serial.print(" | Battery: ");
  Serial.println(lowBatt ? "LOW" : "OK");


  // ==========================================================
  // Built-in LED (heartbeat)
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);


  // ==========================================================
  // External LED = LOW BAT indicator
  // ==========================================================
  if (lowBatt) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}