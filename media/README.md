# Media — Videos

Short demonstrations of the physical Ball‑and‑Beam. These clips are illustrative (not synchronized to logs). See `data/` for CSVs and `fig/` for overlays.

## Index

| File          | Controller                         | Sensor      | What it shows                                                                                                                 | Linked data                                                  |
| ------------- | ---------------------------------- | ----------- | ----------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| `PID.mp4`     | **Arduino UNO (PID)**              | **HC‑SR04** | Quick demo to show the **latch** and that the PID loop works with the ultrasonic sensor. Not logged.                          | —                                                            |
| `sine_08.mp4` | **ESP32 (Feedback Linearization)** | **VL53L0X** | Sine tracking with **0.08 m** amplitude.                                                                                      | `data/sin_08.csv`, `fig/simulation_vs_measured_sine08cm.png` |
| `sine_10.mp4` | **ESP32 (FL)**                     | **VL53L0X** | Sine tracking with **0.10 m** amplitude.                                                                                      | `data/sin_10.csv`, `fig/simulation_vs_measured_sine10cm.png` |
| `sine_12.mp4` | **ESP32 (FL)**                     | **VL53L0X** | Sine tracking with **0.12 m** amplitude.                                                                                      | `data/sin_12.csv`, `fig/simulation_vs_measured_sine12cm.png` |
| `step_10.mp4` | **ESP32 (FL)**                     | **VL53L0X** | **Step to +0.10 m** with **manual perturbations** to show robustness. **Demo only** — not suitable for quantitative analysis. | —                                                            |
| `step_20.mp4` | **ESP32 (FL)**                     | **VL53L0X** | **Step to +0.20 m** with manual perturbations. **Demo only**.                                                                 | —                                                            |
| `step_30.mp4` | **ESP32 (FL)**                     | **VL53L0X** | **Step to +0.30 m** with manual perturbations. **Demo only**.                                                                 | —                                                            |

## Notes

* Camera time and device time are **not synchronized**; apparent lag in the video is not a metric.
* All clips use the same hardware stack described in `doc/assembly.md` (common GND, L293D + 330 Ω series to the servo).
* The **step** videos intentionally include **physical disturbances**; no CSVs are provided for those runs.

