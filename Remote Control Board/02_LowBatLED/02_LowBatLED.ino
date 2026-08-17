// ==========================================================
// Module 1 & 2: LED Testing (Built-in + External)
// ==========================================================
// This program tests TWO things at once:
//
// 1. Built-in LED (on the Arduino board)
//    → Confirms the board is powered and code is running
//
// 2. External LED (on your PCB, pin 6)
//    → Confirms correct wiring, soldering, and pin control
//
// ==========================================================


// -----------------------------
// External LED pin definition
// -----------------------------
const int ledPin = 6;  // External LED connected to pin 6


void setup() {
  // Built-in LED
  pinMode(LED_BUILTIN, OUTPUT);

  // External LED
  pinMode(ledPin, OUTPUT);
}


void loop() {
  // ==========================================================
  // Turn BOTH LEDs ON
  // ==========================================================
  digitalWrite(LED_BUILTIN, HIGH);  // Built-in LED ON
  digitalWrite(ledPin, HIGH);       // External LED ON
  delay(500);

  // ==========================================================
  // Turn BOTH LEDs OFF
  // ==========================================================
  digitalWrite(LED_BUILTIN, LOW);   // Built-in LED OFF
  digitalWrite(ledPin, LOW);        // External LED OFF
  delay(500);
}



// ==========================================================
// 🔧 External LED Wiring Guide
// ==========================================================
// Pin 6 → Resistor (220–330Ω) → LED long leg (+)
// LED short leg (–) → GND
//
// IMPORTANT:
// - LEDs are polarised → they only work one way
// - Long leg = positive (to pin)
// - Short leg = ground
//
// ==========================================================


// ==========================================================
// ❌ If the external LED does NOT work:
// ==========================================================
// 1. Check LED polarity (most common issue)
// 2. Check resistor is included
// 3. Check correct pin (try 5, 6, etc.)
// 4. Inspect solder joints on PCB
// 5. Try swapping the LED (it may be damaged)
//
// If the built-in LED works but external doesn't:
// → Your code is fine, the issue is hardware
// ==========================================================


// ==========================================================
// 🎯 Challenges (optional)
// ==========================================================
// - Make the LEDs blink faster (try delay(100))
// - Create a pattern (e.g. blink-blink-pause)
// - Make the two LEDs alternate instead of syncing
// ==========================================================