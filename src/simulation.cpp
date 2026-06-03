#include "simulation.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

// -----------------------------------------------------------------------
// Internal helper: runs one mode and writes its rows to the CSV
// Returns final cumulative energy delivered to the motor (kWh)
// -----------------------------------------------------------------------
static double run_mode(
    const BatteryConfig& battery,
    const SimConfig&     sim,
    double               heater_W,
    const string&        label,
    ofstream&            csv,
    bool                 print_table)
{
    double temp_C     = sim.temp_ambient_C;
    double energy_kWh = 0.0;

    if (print_table) {
        cout << "\n--- " << label << " ---\n";
        cout << setw(8)  << "Time(s)"
             << setw(10) << "Temp(C)"
             << setw(11) << "R(Ohm)"
             << setw(12) << "V_term(V)"
             << setw(14) << "P_motor(kW)"
             << setw(20) << "Cumul.Energy(kWh)\n";
    }

    for (int t = 0; t <= sim.sim_duration_s; t++) {
        double r      = calc_resistance(temp_C, battery);
        double v_term = battery.nominal_voltage_V - (sim.I_total_A * r);
        double P_motor = max(0.0, (v_term * sim.I_total_A - heater_W) / 1000.0);

        energy_kWh += P_motor * (sim.dt_s / 3600.0);

        csv << t      << ","
            << label  << ","
            << temp_C << ","
            << r      << ","
            << v_term << ","
            << P_motor << ","
            << energy_kWh << "\n";

        if (print_table && t % 60 == 0) {
            cout << setw(8)  << t
                 << setw(10) << temp_C
                 << setw(11) << r
                 << setw(12) << v_term
                 << setw(14) << P_motor
                 << setw(20) << energy_kWh << "\n";
        }

        // Euler forward thermal update
        double P_joule = sim.I_total_A * sim.I_total_A * r;
        double P_loss  = sim.h_conv_WK * (temp_C - sim.temp_ambient_C);
        temp_C        += (P_joule + heater_W - P_loss) * sim.dt_s / sim.thermal_mass_JK;
    }

    return energy_kWh;
}

// -----------------------------------------------------------------------
// Finds the second at which self-heating cumulative energy overtakes
// no-heating cumulative energy (-1 if it never happens)
// -----------------------------------------------------------------------
static int find_crossover(const BatteryConfig& battery, const SimConfig& sim) {
    double temp_A = sim.temp_ambient_C, temp_B = sim.temp_ambient_C;
    double cum_A  = 0.0,               cum_B  = 0.0;

    for (int t = 1; t <= sim.sim_duration_s; t++) {
        double r_A = calc_resistance(temp_A, battery);
        double r_B = calc_resistance(temp_B, battery);

        double pm_A = max(0.0, (battery.nominal_voltage_V - sim.I_total_A * r_A)
                               * sim.I_total_A / 1000.0);
        double pm_B = max(0.0, (battery.nominal_voltage_V - sim.I_total_A * r_B)
                               * sim.I_total_A / 1000.0 - sim.heater_power_W / 1000.0);

        cum_A += pm_A * (sim.dt_s / 3600.0);
        cum_B += pm_B * (sim.dt_s / 3600.0);

        if (cum_B > cum_A) return t;

        double loss_A = sim.h_conv_WK * (temp_A - sim.temp_ambient_C);
        double loss_B = sim.h_conv_WK * (temp_B - sim.temp_ambient_C);
        temp_A += (sim.I_total_A * sim.I_total_A * r_A - loss_A)
                  * sim.dt_s / sim.thermal_mass_JK;
        temp_B += (sim.I_total_A * sim.I_total_A * r_B + sim.heater_power_W - loss_B)
                  * sim.dt_s / sim.thermal_mass_JK;
    }
    return -1;
}

// -----------------------------------------------------------------------
// Public entry point
// -----------------------------------------------------------------------
void run_simulation(const BatteryConfig& battery, const SimConfig& sim) {
    cout << fixed << setprecision(3);
    cout << "=== CryoCell — Dynamic Thermal Simulation ===\n\n";

    // Static analysis summary
    double r_cold = calc_resistance(sim.temp_ambient_C, battery);
    cout << "Cold-start R_internal : " << r_cold << " Ohms"
         << "  (baseline 20C: " << battery.r_internal_base << " Ohms)\n";
    cout << "Cold-start motor power: "
         << ((battery.nominal_voltage_V - sim.I_total_A * r_cold) * sim.I_total_A) / 1000.0
         << " kW  (warm baseline: "
         << ((battery.nominal_voltage_V - sim.I_total_A * battery.r_internal_base)
             * sim.I_total_A) / 1000.0
         << " kW)\n";

    // Open CSV
    ofstream csv("results/simulation.csv");
    csv << "time_s,mode,temp_C,r_internal_ohm,terminal_voltage_V,"
        << "motor_power_kW,cumulative_energy_kWh\n";

    // Run both modes
    double e_no_heat   = run_mode(battery, sim, 0.0,               "no_heating",  csv, true);
    double e_self_heat = run_mode(battery, sim, sim.heater_power_W, "self_heating", csv, true);

    csv.close();

    // Trade-off report
    int crossover = find_crossover(battery, sim);
    cout << "\n=== Trade-off Analysis ===\n";
    cout << "Total energy — no heating  : " << e_no_heat   << " kWh\n";
    cout << "Total energy — self-heating: " << e_self_heat << " kWh\n";
    if (crossover > 0)
        cout << "Self-heating pays off at t = " << crossover << " s\n";
    else
        cout << "Self-heating did not pay off within the simulation window.\n";

    cout << "\nResults saved to results/simulation.csv\n";
}
