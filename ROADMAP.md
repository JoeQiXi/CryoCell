# Project Roadmap: Cold-Environment Battery Simulator

## Phase 1: The Static Model (Current Focus)
- [ ] Define baseline battery parameters (Nominal Voltage, Capacity).
- [ ] Implement the Equivalent Circuit Model (ECM).
- [ ] Calculate baseline internal resistance at 20°C.
- [ ] Apply the Arrhenius equation to calculate resistance spikes at -20°C.

## Phase 2: The Time Loop
- [ ] Implement a discrete-time simulation loop (1-second intervals).
- [ ] Integrate Coulomb counting for State of Charge (SOC) tracking.
- [ ] Simulate voltage drop under a continuous load over a 30-minute flight.

## Phase 3: System Control Logic (Thermal Management)
- [ ] Implement lumped capacitance thermal modeling.
- [ ] Build an active control gate (If internal temp < 0°C, trigger heater).
- [ ] Calculate the power trade-off between self-heating and motor output.

## Phase 4: Data Visualization
- [ ] Output time-series data of voltage sag and temperature.
- [ ] Generate comparative graphs (Baseline vs. Sub-zero performance).
