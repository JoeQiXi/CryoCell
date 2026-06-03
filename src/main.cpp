#include "simulation.h"

int main() {

    BatteryConfig battery = {
        .nominal_voltage_V = 350.0,    // Pack open-circuit voltage (V)
        .r_internal_base   = 0.05,     // Internal resistance at 20C (Ohms)
        .activation_energy = 20000.0,  // Arrhenius Ea (J/mol)
        .gas_constant      = 8.314,    // Universal gas constant (J/mol·K)
        .T_ref_K           = 293.15    // 20C in Kelvin
    };

    SimConfig sim = {
        .thermal_mass_JK  = 150000.0,  // Pack thermal inertia (J/K)
        .h_conv_WK        = 8.0,       // Convective loss coefficient (W/K)
        .temp_ambient_C   = -20.0,     // Ambient cold-soak temperature (C)
        .I_total_A        = 300.0,     // Motor load current (A)
        .heater_power_W   = 3000.0,    // Self-heater power (W)
        .sim_duration_s   = 600,       // 10-minute run
        .dt_s             = 1.0        // 1-second time step
    };

    run_simulation(battery, sim);

    return 0;
}
