# Data README

This folder contains **measured trajectories** from the physical Ball‑and‑Beam running the **ESP32 (Feedback Linearization)** loop with **VL53L0X** position sensing.

## Files

* `sin_08.csv` — sine tracking, **amplitude 0.08 m (8 cm)**
* `sin_10.csv` — sine tracking, **amplitude 0.10 m (10 cm)**
* `sin_12.csv` — sine tracking, **amplitude 0.12 m (12 cm)**

> PID runs with the Arduino UNO were only used for a quick video demo and were **not logged**.
> FL step tests are not provided too. During step experiments under Feedback Linearization we introduced **physical perturbations** (manual disturbances) to demonstrate robustness. Those runs are **not suitable for quantitative analysis** (e.g., RMSE/IAE, time constants), so they were not logged to CSV.

## Schema

CSV files have **no header** and use **comma** as separator. Each file has **two columns**:

| Column | Name  | Units | Description                           |
| -----: | ----- | ----- | ------------------------------------- |
|      1 | `t`   | s     | Time, starting at **0 s**             |
|      2 | `pos` | m     | Ball position (see conventions below) |

Encoding: MATLAB default (numeric ASCII/UTF‑8). If your loader complains about encoding, treat as plain ASCII with comma delimiter.

## Sampling

* Nominal sample time: **Δt = 0.02 s** (50 Hz).
* The **time column is authoritative**; prefer it over assuming a fixed step.

## Conventions

* **Origin** at the **center** of the beam.
* **Right** side is **positive**.
* A fixed **+0.20 m sensor offset (VL53L0X at the right edge)** has **already been applied** in the CSVs, so `pos` is the ball position in the beam reference frame.

## Pre‑processing

* Position is **Kalman‑filtered** (only) on‑device/before logging.
* Files include **startup transients**; crop the initial window as needed during analysis.

## Quick load examples

**MATLAB**

```matlab
T = readmatrix(fullfile('data','sin_12.csv')); % no header, comma‑separated
 t   = T(:,1);
 y   = T(:,2);
 plot(t,y), grid on, xlabel('s'), ylabel('m'), title('sin\_12 measured position')
```

**Python**

```python
import numpy as np
import matplotlib.pyplot as plt
T = np.loadtxt('data/sin_12.csv', delimiter=',')
t, y = T[:,0], T[:,1]
plt.plot(t, y); plt.grid(True)
plt.xlabel('s'); plt.ylabel('m'); plt.title('sin_12 measured position')
plt.show()
```

## Provenance

* Controller: **ESP32 (Feedback Linearization)**.
* Sensor: **VL53L0X** (right side, +0.20 m from center).
* PID/UNO data were **not** captured to CSV.

## License

Unless stated otherwise, these data fall under the repository’s root **LICENSE**.

