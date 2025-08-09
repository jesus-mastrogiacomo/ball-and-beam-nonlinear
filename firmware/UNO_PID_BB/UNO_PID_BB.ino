// Project: Ball Position Control System
// Description: This sketch implements a position control system using a servo,
//              ultrasonic sensor, PID controller, and an LCD display. It supports
//              two modes of reference tracking: STEP and SINE.

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <PID_v1.h>
#include <NewPing.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); // I2C address and pin mapping
const int lcdBacklightPin = 3;                   // Backlight control pin

// Pin definitions
const int potPin     = A0;   // Potentiometer for setting reference
const int switchPin  = 13;   // Mode selection switch
const int servoPin   = 9;    // Servo control pin
#define TRIGGER_PIN  12     // Ultrasonic sensor trigger
#define ECHO_PIN     11     // Ultrasonic sensor echo
#define MAX_DISTANCE 200    // Max measurable distance in cm

// Mode flag: 0 = STEP, 1 = SINE
int mode = 0;

// For display update control
int lastMode = -1;
int lastDisplayValue = -1;

// PID variables
double Setpoint, Input, Output, ServoOutput;
float Kp = 0.9, Ki = 0.8, Kd = 0.5; // PID gains
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Actuator and sensor objects
Servo myServo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Time tracking for sine wave
unsigned long lastMillis = 0;
const float sineFrequency = 0.25; // in Hz

void setup() {
  Serial.begin(9600);

  // LCD initialization
  lcd.setBacklightPin(lcdBacklightPin, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.clear();

  // Pin modes
  pinMode(potPin, INPUT);
  pinMode(switchPin);
  myServo.attach(servoPin);

  // PID initialization
  Input = readPosition();
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-20, 20); // Servo correction range

  // Initial servo position (centered)
  ServoOutput = 81;
  myServo.write(ServoOutput);
}

void loop() {
  // Read current mode from switch
  // LOW = SINE mode (switch closed), HIGH = STEP mode (switch open)
  mode = digitalRead(switchPin) == LOW ? 1 : 0;

  // Read potentiometer and map to desired range depending on mode
  int rawPot = analogRead(potPin);
  int displayValue = (mode == 0) ? map(rawPot, 0, 750, 0, 40)   // STEP: 0–40 cm
                                 : map(rawPot, 0, 750, 0, 16);  // SINE: amplitude 0–16 cm
  displayValue = constrain(displayValue, 0, (mode == 0) ? 40 : 20);

  // Define the reference (setpoint)
  if (mode == 0) {
    // STEP: constant reference
    Setpoint = displayValue;
  } else {
    // SINE: oscillatory reference
    float t = (millis() - lastMillis) / 1000.0;
    Setpoint = 20 + displayValue * sin(2 * PI * sineFrequency * t); // centered at 20 cm
  }

  // Update LCD only when value or mode has changed
  if (mode != lastMode || displayValue != lastDisplayValue) {
    lcd.setCursor(0, 0);
    lcd.print("Mode:        ");
    lcd.setCursor(6, 0);
    lcd.print((mode == 0) ? "STEP" : "SINE");

    lcd.setCursor(0, 1);
    lcd.print((mode == 0) ? "Dist (cm):     " : "Amp (cm):      ");
    lcd.setCursor(11, 1);
    lcd.print("   ");
    lcd.setCursor(11, 1);
    lcd.print(displayValue);

    lastMode = mode;
    lastDisplayValue = displayValue;
  }

  // Read actual position from sensor
  Input = readPosition();

  // PID calculation
  myPID.Compute();

  // Send updated command to servo (center at ~79)
  ServoOutput = 79 + Output;
  myServo.write(ServoOutput);

  delay(50); // Control update rate
}

// Function to read ball position from ultrasonic sensor
float readPosition() {
  delay(40); // Wait for sensor to settle
  long cm = sonar.convert_cm(sonar.ping_median(5)); // Median of 5 pings
  cm = constrain(cm, 0, 40); // Limit to physical range
  Serial.println(cm);
  return cm;
}
