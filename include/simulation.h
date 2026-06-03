#pragma once
#include "battery_model.h"

// Holds all parameters that control the simulation run
struct SimConfig {
    double thermal_mass_JK;     // Pack thermal inertia (J/K)
    double h_conv_WK;           // Convective heat loss coefficient (W/K)
    double temp_ambient_C;      // Ambient temperature throughout the run (C)
    double I_total_A;           // Total current drawn from pack (A)
    double heater_power_W;      // Power diverted to self-heater in trade-off mode (W)
    int    sim_duration_s;      // Total simulation time (seconds)
    double dt_s;                // Time step size (seconds)
};

// Runs both modes (no_heating and self_heating) and writes results to CSV
void run_simulation(const BatteryConfig& battery, const SimConfig& sim);
