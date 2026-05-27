#include <iostream>
#include <cmath>
using namespace std;

int main() {
    // Phase 1: Baseline Battery Parameters
    // Simulating a high-capacity lithium-ion pack in extreme cold

    double nominal_voltage_V = 350.0;     // Pack open-circuit voltage
    double capacity_Ah      = 230.0;      // Total capacity in Amp-hours
    double temp_ambient_C   = -20.0;      // Ambient temperature (cold soak)
    double temp_reference_C =  20.0;      // Standard test temperature
    double r_internal_base  =  0.05;      // Internal resistance at reference temp (Ohms)

    // Motor load current — representative of a hybrid-electric propulsion stage
    double motor_current_A  = 300.0;      // Amps drawn by motor during cruise

    std::cout << "=== CryoCell: Phase 1 Static Analysis ===\n";
    std::cout << "Pack: " << nominal_voltage_V << " V | "
              << capacity_Ah << " Ah | Ambient: "
              << temp_ambient_C << " C\n\n";

    // --- Arrhenius cold-resistance model ---
    // R(T) = R_ref * exp((Ea/Rg) * (1/T_cold - 1/T_ref))
    const double activation_energy_J = 20000.0;   // Ea (J/mol)
    const double gas_constant_R      = 8.314;      // J/(mol·K)

    double T_cold_K = temp_ambient_C   + 273.15;
    double T_ref_K  = temp_reference_C + 273.15;

    double arrhenius_factor = std::exp(
        (activation_energy_J / gas_constant_R) * (1.0 / T_cold_K - 1.0 / T_ref_K)
    );
    double r_internal_cold = r_internal_base * arrhenius_factor;

    std::cout << "[Arrhenius Model]\n";
    std::cout << "  R_base (20 C)  : " << r_internal_base << " Ohms\n";
    std::cout << "  R_cold (-20 C) : " << r_internal_cold << " Ohms"
              << "  (factor: " << arrhenius_factor << "x)\n\n";

    // --- Voltage drop under motor load ---
    // V_terminal = V_OCV - I * R_internal
    double voltage_drop_V  = motor_current_A * r_internal_cold;
    double terminal_voltage = nominal_voltage_V - voltage_drop_V;
    double power_delivered_kW = (terminal_voltage * motor_current_A) / 1000.0;

    std::cout << "[Voltage Drop at " << motor_current_A << " A load]\n";
    std::cout << "  Voltage drop    : " << voltage_drop_V  << " V\n";
    std::cout << "  Terminal voltage: " << terminal_voltage << " V\n";
    std::cout << "  Motor power out : " << power_delivered_kW << " kW\n\n";

    // Baseline comparison: what would the drop be at room temperature?
    double voltage_drop_warm   = motor_current_A * r_internal_base;
    double terminal_warm       = nominal_voltage_V - voltage_drop_warm;
    double power_warm_kW       = (terminal_warm * motor_current_A) / 1000.0;

    std::cout << "[Warm baseline (20 C) for comparison]\n";
    std::cout << "  Voltage drop    : " << voltage_drop_warm << " V\n";
    std::cout << "  Terminal voltage: " << terminal_warm     << " V\n";
    std::cout << "  Motor power out : " << power_warm_kW     << " kW\n\n";

    double power_loss_kW = power_warm_kW - power_delivered_kW;
    std::cout << ">>> Cold-soak power penalty: " << power_loss_kW << " kW\n";

    return 0;
}
