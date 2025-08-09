# Simulation — Quick Start

* **MATLAB/Simulink**: tested on **R2023a**.
* No extra toolboxes beyond **Simulink** for the plant/controller model.
* Open **`Simulation_FL_BB.slx`** and press **Run**. Default stop time is **50 s**.

## Model defaults

* **Solver**: variable‑step (auto selection).
* **Initial conditions**: ball at **x = 0 m** (beam center).
* **Reference**: **step** at **t = 10 s** to **0.10 m**.
* **Friction / sensor noise**: **not included** in this model.
* **Units**: **SI** (m, s, rad). Right side of the beam is **positive**.

## Editable parameters (inside the model)

You can change these directly in block masks/parameters:

* **Tracking reference** (sine/step/etc.).
* **Center bias** (static offset if needed).
* **Beam inertia `J`**.
* **Ball mass `m`**.
* (All of the above are grouped in the **`calculate tau`** subsystem.)

> Note: This project **does not** use an external `parameters.m`. All values live **inside** the Simulink model.

## Scopes / what you’ll see

The model uses **three Scopes** (no signals are sent to the MATLAB workspace by default):

1. **Control input** — angular acceleration (**u**).
2. **Computed torque** — the internal **τ** from the `calculate tau` block.
3. **Position vs. reference** — ball position and the tracking reference.

## (Optional) Exporting signals to the workspace

If you want to compare the simulation to measured CSVs, enable logging to the workspace:

1. Add **To Workspace** blocks or enable **Scope → Log data to workspace**.
2. Recommended names & format:

   * **`r_sim`** = reference; **`y_sim`** = simulated position.
   * **Format**: *Structure with time*.
3. Then you can call the provided script in `scripts/compute_metrics.m`.

Example (MATLAB):

```matlab
% Assuming you logged Structure-with-time variables r_sim and y_sim
r = r_sim;               % reference
out_r = y_sim;           % simulated position

% Now compare to data
data = readmatrix('data/sin_12.csv');
t_real = data(:,1); y_real = data(:,2);
S = compute_metrics(t_real, y_real, out_r.time, out_r.signals.values, ...
                    struct('idx_start',1,'lag_sec',0,'label','simulation vs data'));
```

## Reproducing the default run

1. Open `Simulation_FL_BB.slx`.
2. Ensure stop time is **50** and the reference is a **step to 0.10 m at t=10 s**.
3. Run and inspect the three Scopes listed above.

## Notes

* This Simulink model represents a **nominal** Ball‑and‑Beam without viscous friction or sensor noise; for closer matching to hardware you may add a small viscous term and measurement noise.
* All parameters are inside the model; there is no `Model InitFcn` hook.

