| Qty | Designator(s) | Part / Specification | Notes / Alternatives |
|-----|---------------|----------------------|----------------------|
| 1   | PSU1          | 5 V @ ≥ 2.1 A power bank (USB-A output) | Powers servo and L293D (and/or MCUs) |
| 1   | MCU1          | ESP32 DevKit-C (30-pin, WROOM-32E) | Runs Feedback-Linearization firmware |
| 1   | MCU2          | Arduino UNO R3 | Runs PID firmware |
| 1   | DRIVER1       | L293D H-bridge IC | Switches servo signal between MCUs |
| 2   | Q1–Q2         | PN2222A NPN transistor | Bistable flip-flop |
| 1   | SERVO1        | SG90 micro-servo, 180° | Actuator |
| 1   | DIST1         | VL53L0X Time-of-Flight sensor (I²C, 2 m) | FL position feedback |
| 1   | IMU1          | MPU6050 6-axis IMU (I²C)| Beam inclination |
| 1   | US1           | HC-SR04 ultrasonic sensor | PID position feedback (UNO side) |
| 1   | LCD1          | 16 × 2 LCD + I²C backpack (PCF8574) | UNO side |
| 1   | POT1          | 10 kΩ panel potentiometer | Manual set-point |
| 3   | BTN1–BTN3     | 6 × 6 mm tactile switches | 2× flip-flop, 1× Step/Sine mode |
| 1   | LED_G         | 5 mm green LED | “System ON” |
| 1   | LED_W         | 5 mm white LED | Step/Sine indicator |
| 1   | LED_R         | 5 mm red LED | Arduino / PID active |
| 1   | LED_Y         | 5 mm yellow LED | ESP32 / FL active |
| 2   | R_LED_GW      | 5.1 kΩ resistor (¼ W) | Current limit for green & white LEDs (indicatorim) |
| 2   | R_LED_RY      | 1 kΩ resistor (¼ W) | Current limit for red & yellow LEDs |
| 2   | R_BASE        | 10 kΩ resistor (¼ W) | Base bias for PN2222A (bistable) |
| 1   | R_SERVO       | 330 Ω resistor (¼ W) | In series with L293D output → servo |
| 1   | C_BULK1       | 470 µF electrolytic, ≥ 6.3 V | Supply smoothing |
| 1   | C_BULK2       | 1000 µF electrolytic, ≥ 6.3 V | Supply smoothing |
| —   | JUMPERS       | ~40 × Dupont M-M, ~20 × Dupont M-F | Wiring harness |
| 2   | USB_CABLES    | USB-A → board connectors | UNO: usually USB-B; ESP32: Micro-USB or USB-C (depends on board) |
| 1   | BALL1         | 40 mm ping-pong ball | Controlled object |
| 1   | BEAM          | MDF beam + base/servo mount | Replicates the Instructables design (see `doc/assembly.md`) |
| —   | MISC HW       | M3 screws, nuts, standoffs, double-sided tape | **Common GND** between PSU, UNO, ESP32, L293D and all sensors |

