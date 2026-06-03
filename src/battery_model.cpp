#include "battery_model.h"
#include <cmath>

double calc_resistance(double temp_C, const BatteryConfig& battery) {
    double T_K = temp_C + 273.15;
    return battery.r_internal_base * std::exp(
        (battery.activation_energy / battery.gas_constant) *
        (1.0 / T_K - 1.0 / battery.T_ref_K)
    );
}
