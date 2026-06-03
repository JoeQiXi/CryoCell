# Project Roadmap: Cold-Environment Battery Simulator

## Phase 1: The Static Model
- [x] Define baseline battery parameters (Nominal Voltage, Capacity).
- [x] Implement the Equivalent Circuit Model (ECM).
- [x] Calculate baseline internal resistance at 20°C.
- [x] Apply the Arrhenius equation to calculate resistance spikes at -20°C.

## Phase 2: The Time Loop (Current Focus)
- [x] Implement a discrete-time simulation loop (1-second intervals).
- [ ] Integrate Coulomb counting for State of Charge (SOC) tracking.
- [x] Simulate voltage drop under a continuous load over a multi-minute flight.

## Phase 3: System Control Logic (Thermal Management)
- [x] Implement lumped capacitance thermal modeling.
- [ ] Build an active control gate (If internal temp < 0°C, trigger heater).
- [x] Calculate the power trade-off between self-heating and motor output.

## Phase 4: Data Visualization
- [x] Output time-series data of voltage sag and temperature.
- [x] Generate comparative graphs (Baseline vs. Sub-zero performance).
