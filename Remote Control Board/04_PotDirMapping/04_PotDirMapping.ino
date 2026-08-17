// ==========================================================
// Module 4: Pot Calibration + Adaptive Mapping
// ==========================================================
// GOAL:
// - Move pots through full range
// - Let system discover min/max values
// - Use those values for mapping instead of guessing
// ==========================================================


// -----------------------------
// Pin Definitions
// -----------------------------
const int steerPotPin = A0;
const int drivePotPin = A1;
const int battPin = A7;
const int ledPin = 6;

const int lowBattThreshold = 211;


// ==========================================================
// CALIBRATION VALUES
// ==========================================================
// ⭐ IMPORTANT:
// Move BOTH potentiometers through their full range
// (full left/right, full forward/back)
//
// These values will update automatically while running.
// Watch Serial Monitor to see them change.
//
int steerMin = 1023;
int steerMax = 0;

int driveMin = 1023;
int driveMax = 0;


// Center + deadzone tuning
int deadzone = 30;


// Output structure
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

  Serial.println("=== Calibration Mode ===");
  Serial.println("Move BOTH pots through full range!");
}


// ==========================================================
// LOOP
// ==========================================================
void loop() {

  // -----------------------------
  // Read raw inputs
  // -----------------------------
  int steerRaw = analogRead(steerPotPin);
  int driveRaw = analogRead(drivePotPin);
  int battReading = analogRead(battPin);

  bool lowBatt = battReading < lowBattThreshold;


  // ==========================================================
  // ⭐ CALIBRATION SECTION
  // ==========================================================
  // This automatically learns the min and max values
  // as the pots are moved around

  if (steerRaw < steerMin) steerMin = steerRaw;
  if (steerRaw > steerMax) steerMax = steerRaw;

  if (driveRaw < driveMin) driveMin = driveRaw;
  if (driveRaw > driveMax) driveMax = driveRaw;


  // ==========================================================
  // OUTPUT MAPPING (uses learned values)
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
  // SERIAL MONITOR OUTPUT
  // ==========================================================
  Serial.print("Steer: ");
  Serial.print(steerRaw);
  Serial.print(" [min:");
  Serial.print(steerMin);
  Serial.print(" max:");
  Serial.print(steerMax);
  Serial.print("]");

  Serial.print(" | Drive: ");
  Serial.print(driveRaw);
  Serial.print(" [min:");
  Serial.print(driveMin);
  Serial.print(" max:");
  Serial.print(driveMax);
  Serial.print("]");

  Serial.print(" | Servo: ");
  Serial.print(outgoing.servoPos);

  Serial.print(" | Dir: ");
  Serial.print(outgoing.motorDir);

  Serial.print(" | Speed: ");
  Serial.print(outgoing.motorSpeed);

  Serial.print(" | Batt: ");
  Serial.println(lowBatt ? "LOW" : "OK");


  // ==========================================================
  // LED OUTPUTS
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);

  digitalWrite(ledPin, lowBatt ? HIGH : LOW);
}