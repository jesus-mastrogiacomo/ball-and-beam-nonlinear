# Ball & Beam — Dual-Controller (PID + Feedback Linearization)

<img src="presentation.png" alt="Ball & Beam overview" width="720"/>

**Hardware:** MDF beam (40 cm), SG90 servo, VL53L0X / HC-SR04 / MPU6050, **L293D + bistable (2× PN2222A)** to select between **Arduino UNO (PID)** and **ESP32 (Feedback Linearization)**.  
**Software:** UNO sketch (.ino) + Simulink model (ESP32 with **Monitor & Tune**).

- **Hardware list →** [hardware/BOM.md](hardware/BOM.md)  
- **Assembly notes →** [doc/assembly.md](doc/assembly.md)  
- **Firmware →** [firmware/README.md](firmware/README.md)  
- **Simulation →** [simulation/README.md](simulation/README.md)  
- **Data (CSVs) →** [data/README.md](data/README.md)  
- **Figures →** [fig/README.md](fig/README.md)  
- **Videos →** [media/README.md](media/README.md)

---

## What’s different here

- Mechanical laser-cut **reused** from Instructables; **electronics + control logic are mine**.  
- **Latch + L293D** cleanly selects which MCU drives the servo.  
- **FL law** follows the standard Sastry formulation; implementation/integration here is custom.

---

## Pin map (MCU side)

### Arduino UNO (PID / UI / HC-SR04)

| Function | Pin |
|---|---|
| Servo PWM → **L293D 1A** | **D9** |
| HC-SR04 TRIG / ECHO | **D12 / D11** *(ECHO stays on UNO at 5 V)* |
| LCD I²C (PCF8574) | **SDA=A4, SCL=A5**, **addr 0x27** |
| LCD backlight | **D3** |
| STEP/SINE mode input *(driven by ESP32 D18, active HIGH)* | **D13** *(configure as `INPUT`, no pull-up)* |
| Red LED (PID active) | **driven by latch hardware** |
| GND | **common ground** |

### ESP32 (FL / VL53L0X / MPU6050)

| Function | Pin |
|---|---|
| Servo PWM → **L293D 4A** | **D2** |
| I²C (VL53L0X, MPU6050) | **SDA=GPIO21, SCL=GPIO22**, **400 kHz** |
| Step/Sine **button input** | **D5** |
| Potentiometer (ADC) | **A6 = GPIO34** *(in parallel with UNO A0)* |
| White LED (Step/Sine + mode out to UNO) | **D18** *(tied to UNO D13, active HIGH)* |
| Yellow (FL active) / Red (PID active) | **from latch hardware** |
| I²C addrs | VL53L0X `0x29`, MPU6050 `0x68` |
| GND | **common ground** |

> **Servo path (exact):** **L293D 1Y** and **4Y** are **shorted together**, then **→ 330 Ω in series → servo signal pin**.  
> **Enable logic:** the **bistable** drives **1,2EN** and **3,4EN** **complementarily** so only one PWM path is active at a time.

---

## Wiring rules (read me first)

- **Common GND** across power bank, UNO, ESP32, L293D and all sensors.
- **Servo + L293D** from the **5 V power bank**; **UNO/ESP32 via USB**.
- **Sensors + LCD** from **UNO 5 V** (breakouts include 5→3.3 V regulation/level-shifting).
- **HC-SR04** stays on the **UNO (5 V)**; don’t feed **ECHO** into the ESP32.
- **Mode line:** **ESP32 D18** drives **UNO D13** (and the white LED). **Active HIGH**, **no pull-ups** on UNO D13.
- Only **one PWM source** enabled at a time (latch controls **EN** pins on the L293D).

---

## How to run

**PID (UNO)**  

1. Open `firmware/UNO_PID_BB.ino` in **Arduino IDE 2.3.6**.  
2. Install libs: `Servo`, `PID_v1`, `NewPing`, `Wire`, `LCD`/`LiquidCrystal_I2C`.  
3. Board: **Arduino UNO** → **Upload**. The sketch prints distance (cm) at **9600 baud**.  

> If you use this exact wiring, set **pin D13 as `INPUT` (no pull-up)** and treat **HIGH = STEP** (the provided example code uses `INPUT_PULLUP`; adjust if needed).

**FL (ESP32 with Simulink)**  

1. Open `firmware/ESP32_FL_BB.slx`.  
2. **Monitor & Tune** (not Deploy): board **ESP32 DevKit-C**, correct **COM**, I²C **400 kHz**.  
3. Solver **fixed-step = 0.02 s**, set **Stop time** (e.g., 50 s).  
4. Run and (optionally) log signals from Simulink Data Inspector.

---

## Data, figures & metrics

- CSVs in `data/` (**t, pos**), `Δt = 0.02 s`, **SI units**, **right = positive**, sensor offset already applied.  
- Overlays & metrics (**RMSE / IAE**) in `fig/`, computed with [`scripts/compute_metrics.m`](scripts/compute_metrics.m).  
- **FL step videos** (`media/step_*.mp4`) include **manual disturbances** and are **not** for quantitative analysis.

---

## Status & Roadmap

**WIP — Wiring diagram**
- A clean wiring diagram (SVG) is in progress. For now, use the **Pin map** and **Wiring rules** above. The exact servo path is: **L293D 1Y & 4Y (shorted) → 330 Ω series → servo signal**; enables driven by the latch.

**Planned — Adaptive friction compensation**
- I’m adding **adaptive control** on the **P** term of the **τ → θ** controller to account for real ball/beam friction.
- Idea: estimate an effective viscous term **μ̂(t)** online (gradient or RLS) and modulate **P(t)** accordingly:
  - Add a small friction model in simulation, validate against `data/sin_*` CSVs.
  - Compare RMSE/IAE before/after with `scripts/compute_metrics.m`.
  - Expose a runtime toggle for **Adaptive P** (on/off) and log μ̂(t) for plots.

Contributions / ideas welcome. 🙂

---

## Acknowledgements

- **Laser-cut geometry** from Instructables (mechanics only):  
  <https://www.instructables.com/Arduino-Ball-and-Beam-Project-sistema-Barra-Bola-C/>
- **Feedback Linearization**: textbook law (S. Sastry). Implementation/integration here (latch + L293D + signal selection) are **original**.

**License:** [MIT](LICENSE) · **Cite:** see [`CITATION.cff`](CITATION.cff)
