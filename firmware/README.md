# Firmware — Overview

This folder contains two firmwares:

* **`UNO_PID_BB.ino`** — Arduino UNO sketch for a **PID** position loop using **HC‑SR04** and an LCD UI.
* **`ESP32_FL_BB.slx`** — Simulink model for **Feedback Linearization (FL)** running on **ESP32** with **VL53L0X** and **MPU6050**; used with **Monitor & Tune** to log data.

> **Common ground:** tie **GND of power bank, UNO, ESP32, L293D and all sensors** together. Servo signal goes **L293D OUT → 330 Ω series → servo**.

---

## Arduino UNO (PID)

* **Arduino IDE:** 2.3.6
* **Baud rate:** `9600`
* **Libraries used:**

  * `Wire.h`
  * `LCD.h` and `LiquidCrystal_I2C.h` (LCD at **0x27**)
  * `Servo.h`
  * `PID_v1.h`
  * `NewPing.h`

### Pin map — UNO

* **Potentiometer**: `A0`
* **Mode switch (STEP/SINE)**: `D13` *(shared with ESP32 button line; uses `INPUT_PULLUP`)*
* **Servo PWM**: `D9` → **L293D INx** *(see project Pin map)*
* **HC‑SR04**: `TRIG=D12`, `ECHO=D11` *(ECHO through divider when needed)*
* **LCD I²C**: `SDA=A4`, `SCL=A5`; **address 0x27**
* **LCD backlight**: `D3` (as per sketch)
* **LED (red, PID active)**: **from latch hardware** (UNO does not drive it)
* **Latch buttons**: **hardware** (UNO does not read them)

### How to build & upload

1. Open `UNO_PID_BB.ino` in Arduino IDE **2.3.6**.
2. Install listed libraries (Library Manager or zip).
3. **Board**: *Arduino UNO*. **Port**: select the UNO COM port.
4. Click **Upload**. The sketch prints the measured distance (cm) at 9600 baud.

> **Behavior summary**
>
> * `D13` LOW = **SINE mode**; HIGH = **STEP mode**.
> * STEP: setpoint from POT (0–40 cm). SINE: amplitude from POT (0–16 cm), centered at **20 cm**.
> * PID gains as in sketch: `Kp=0.9`, `Ki=0.8`, `Kd=0.5`. Servo command around **\~79–81** degrees center.

---

## ESP32 (Simulink — Feedback Linearization)

* **Model**: `ESP32_FL_BB.slx`
* **Workflow**: **Monitor & Tune** (not Build & Deploy)
* **Support package**: *Simulink Support Package for Espressif ESP32 Hardware*

### Pin map — ESP32

* **Servo PWM**: **D2** → **L293D INy** *(see project Pin map)*
* **I²C bus** (VL53L0X, MPU6050): `SDA=GPIO21`, `SCL=GPIO22` @ **400 kHz**
* **Step/Sine button input**: **D5** *(wired also to UNO `D13`)*
* **Potentiometer (ADC)**: **A6** *(board‑specific mapping; use your DevKit‑C A6 pad)*
* **LED (white, Step/Sine)**: **D18**
* **LED (yellow, FL active)** and **LED (red, PID active)**: **from latch hardware**

### I²C addresses

* **VL53L0X**: `0x29`
* **MPU6050**: `0x68`
* **LCD** is on the **UNO** (not on ESP32), address `0x27`.

### Monitor & Tune settings

1. Open **`ESP32_FL_BB.slx`**.
2. **Hardware Settings** → *Hardware board*: **ESP32 DevKit‑C** (or your exact board).
3. **Target hardware resources**:

   * **Serial port**: select the ESP32 **COM** port.
   * **I²C clock**: **400 kHz**.
4. **Solver**: set **Fixed‑step** with **step size = 0.02 s** (to match logs).
5. Set desired **Stop time** (e.g., 50 s).
6. Click **Monitor & Tune** to build and start execution; use Simulink data inspector to log signals.

> The model logs sensor signals via Monitor & Tune; you can export them and compare with CSVs using `scripts/compute_metrics.m`.

---

## Signal selection (L293D + latch)

* The project selects between **UNO** and **ESP32** PWM using the **L293D** controlled by a **bistable** (2× PN2222A). See **Assembly Notes** for details.
* Ensure only **one** PWM source is enabled at a time. The latch also drives **LEDs**: **Yellow=ESP32/FL**, **Red=Arduino/PID**.

---

## Power recap

* **Servo + L293D**: 5 V from **power bank**.
* **UNO + ESP32**: via **USB**.
* **Sensors + LCD**: from the **UNO 5 V rail** (breakouts include 5→3.3 V regulation/level shifting where needed).
* **GND**: common across all devices.

