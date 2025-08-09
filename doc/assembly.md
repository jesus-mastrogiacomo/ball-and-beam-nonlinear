# Assembly Notes — Ball-and-Beam (MDF)

> Reference build (Spanish): [https://www.instructables.com/Arduino-Ball-and-Beam-Project-sistema-Barra-Bola-C/](https://www.instructables.com/Arduino-Ball-and-Beam-Project-sistema-Barra-Bola-C/)
>
> This document summarizes how this repository’s hardware is assembled. It follows the Instructables build closely, with small adaptations called out below.&#x20;

---

## 1) Mechanical build

**Beam**

* Material: MDF, thickness **3 mm**.
* Length: **400 mm** total.
* Pivot: **center of the beam**, using two screws (per the Instructables).

**Servo linkage**

* Actuator: **SG90 micro‑servo**.
* Link: **push‑rod/rod** from the servo horn to the beam (same concept as the Instructables).
* **Neutral (horn at 0°)**:

  * Drive the servo with a **1500 µs PWM** (or your firmware’s “neutral”).
  * Mount the horn so the **beam is near horizontal** at neutral.
  * Fine‑tune using the mechanical screw positions on the horn/rod.

**Controller & PSU placement**

* **ESP32** and **Arduino UNO** mounted on a **wooden base** (reused board).
* Power bank resides on the base; route the 5 V line only to the servo/L293D (see Power).

---

## 2) Sensor placement

* **VL53L0X (ToF)** — **right edge** of the beam, at **+0.20 m** from center.

  * Mounted on a small black **3 mm MDF** tab with **two nails**; the tab is attached to the beam with **transparent double‑sided tape**.
  * Notes: 3.3 V I²C; mind sensor orientation so the emitter faces the ball.

* **HC‑SR04 (ultrasonic)** — **left side** of the beam, at **−0.20 m** from center.

  * Has its **own MDF cutout** so the module sits flush and faces the ball.
  * Keep this sensor on the **Arduino UNO (5 V)**. Do **not** feed ECHO into the ESP32 without a divider.

* **MPU6050 (IMU)** — **press‑fit at the center** of the beam.

  * Ensure axes are consistent with your model (note which axis reads gravity when horizontal).

---

## 3) Electronics & wiring (high‑level)

**Common rules**

* Establish a **common ground (GND)** across **power bank, UNO, ESP32, L293D and all sensors**.
* The **servo signal** goes **through a 330 Ω series resistor** before entering the servo signal pin.
* **Power**: the **power bank’s 5 V** line feeds **servo + L293D** only. The **UNO and ESP32 are powered via their USB cables**.
* Keep servo leads short; if chattering, twist +5V and GND wires together and verify mechanical friction.

**Status LEDs & buttons** (mounted on a breadboard on the base)

* **Green** = system/power; **Red** = Arduino/PID active; **Yellow** = ESP32/FL active; **White** = Step/Sine indicator.
* Three buttons: **two** for the latch/flip‑flop, **one** for Step/Sine selection.

**Signal ownership / interfaces**

* **LCD 16×2 (I²C backpack)** → **Arduino UNO** (I²C on **A4 = SDA**, **A5 = SCL** by default).
* **VL53L0X** and **MPU6050** → **ESP32 I²C bus** (3.3 V logic; modules are powered from the UNO’s 5 V rail and include onboard 5→3.3 V regulation/level shifting).
* **HC‑SR04** → **Arduino UNO** (TRIG/ECHO at 5 V; do not connect ECHO to the ESP32).
* All modules share **common GND**.

**About the servo signal path**

* This build uses an **L293D** in the chain. If you only ever drive the servo from **one** controller, you may route the PWM **directly through the same 330 Ω** series resistor and leave the other source unconnected.
* If you select between UNO and ESP32, route both PWM lines to the L293D inputs and take a single output → **330 Ω** → servo (details in the repo README’s pin map).

### Signal selection: bistable latch + L293D

* **Why this design:** The Arduino kit already included an **L293D**, so it was reused as a gated buffer to **select between the UNO and the ESP32 PWM**.
* **Latch:** discrete **bistable** built with **2× PN2222A** and **two momentary buttons** (SET/RESET). The stored state drives which PWM source is active.
* **LEDs:** **Yellow = ESP32/FL active (Q)**, **Red = Arduino/PID active (/Q)**; **White** = Step/Sine; **Green** = power.
* **Wiring summary:**

  * UNO PWM → L293D **IN\_A**; ESP32 PWM → L293D **IN\_B** (see Pin map for exact pins).
  * Bistable output selects which channel is enabled; the other input is held low.
  * Selected **OUT → 330 Ω → servo signal**.
* **Safety note:** Never enable both channels at once. The **330 Ω** series resistor protects the servo during indeterminate states.
* **Alternative (not used here):** a **SPDT ON‑ON toggle** or a **74HCT157/4053 mux** would also work; we keep the L293D for kit reuse.

---

## 4) Power

* **5 V @ ≥2.1 A power bank** provides current for **servo + L293D** (servo transients are handled better this way).
* **UNO** and **ESP32** are powered via **USB** from a computer or a separate USB supply. Ensure **all grounds are tied**.
* **Sensors power**: VL53L0X, MPU6050, HC‑SR04 and the LCD **are powered from the Arduino UNO’s 5 V rail**. These breakout boards include their own **5→3.3 V regulators/leveling** where needed, so they can be safely fed from 5 V. (Always verify your specific modules before powering.)

---

## 5) First boot & calibration checklist

1. **Servo zero**

   * Command **1500 µs PWM** (or your firmware’s neutral) and adjust the horn/rod so the **ball does not roll** on a level beam.
2. **IMU bias capture**

   * With the beam **static and level**, record **\~20 s** of **MPU6050** accelerometer + gyro data; compute and store bias offsets.
3. **Potentiometer vs. LCD set‑point**

   * Rotate **POT** and verify the LCD set‑point is consistent; adjust scaling if needed.
4. **Quick sensor checks**

   * **VL53L0X** should report distance changes when the ball moves near the **right** half (expect an offset because the sensor sits at +0.20 m).
   * **HC‑SR04** should respond on the **left** half; ensure stable readings and no false echoes from edges.
5. **Controller gains**

   * If the system shows **noise or chatter**, **reduce the P gain** in your **τ → θ** PD chain (or your equivalent FL parameter).

---

## 6) Differences vs. the Instructables build

* Only the **laser‑cut MDF geometry** is reused from the Instructables link.
* All **electronics and control logic are original** to this repo (except for the classical FL law): **bistable latch + L293D** to select the servo PWM between **Arduino UNO (PID)** and **ESP32 (Feedback Linearization)**.
* **Feedback Linearization** design follows the **standard formulation from Sastry** (textbook approach), adapted to this physical build.
* **Dual sensing** scheme: VL53L0X on the right (+0.20 m), HC‑SR04 on the left (−0.20 m), and an MPU6050 press‑fit at the center for beam angle.
* **Power topology:** power bank dedicated to servo/L293D; MCUs via USB; common ground.
* **Metrics & docs:** `scripts/compute_metrics.m` for RMSE/IAE; detailed README sections with pin map and wiring rules.

---

## 7) Files in this repo relevant to assembly

* **`hardware/BOM.md`** — full parts list.
* **`README.md`**\*\* → Pin map\*\* — exact pins for ESP32/UNO and wiring rules.
* **`doc/system_photo.png`** — overall picture of the assembled hardware.

---

*Last updated: 2025-08-08.*

