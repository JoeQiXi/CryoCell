#pragma once

// Holds all physical parameters that describe the battery pack
struct BatteryConfig {
    double nominal_voltage_V;   // Open-circuit voltage (V)
    double r_internal_base;     // Internal resistance at reference temp (Ohms)
    double activation_energy;   // Arrhenius activation energy (J/mol)
    double gas_constant;        // Universal gas constant (J/mol·K)
    double T_ref_K;             // Reference temperature (K)
};

// Returns internal resistance at a given temperature using the Arrhenius model
double calc_resistance(double temp_C, const BatteryConfig& battery);
