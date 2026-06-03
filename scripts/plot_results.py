"""
CryoCell — Plot simulation results
Reads results/simulation.csv (produced by the C++ simulator) and renders a
4-panel figure showing how cold-soak affects the battery over time, comparing
the 'no heating' and 'self heating' strategies.

Usage:
    python3 scripts/plot_results.py
"""

import csv
import os
import matplotlib.pyplot as plt

# ----------------------------------------------------------------------
# Locate files relative to the project root (one level up from scripts/)
# ----------------------------------------------------------------------
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CSV_PATH = os.path.join(PROJECT_ROOT, "results", "simulation.csv")
OUT_PATH = os.path.join(PROJECT_ROOT, "results", "simulation_plot.png")


def load_csv(path):
    """Read the CSV into a dict keyed by mode, each holding column lists."""
    data = {}
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            mode = row["mode"]
            d = data.setdefault(mode, {
                "time_s": [], "temp_C": [], "r_internal_ohm": [],
                "terminal_voltage_V": [], "motor_power_kW": [],
                "cumulative_energy_kWh": [],
            })
            d["time_s"].append(float(row["time_s"]))
            d["temp_C"].append(float(row["temp_C"]))
            d["r_internal_ohm"].append(float(row["r_internal_ohm"]))
            d["terminal_voltage_V"].append(float(row["terminal_voltage_V"]))
            d["motor_power_kW"].append(float(row["motor_power_kW"]))
            d["cumulative_energy_kWh"].append(float(row["cumulative_energy_kWh"]))
    return data


# Friendly labels and colors per mode
STYLE = {
    "no_heating":   {"label": "No heating",   "color": "#1f77b4"},
    "self_heating": {"label": "Self heating", "color": "#d62728"},
}


def plot(data):
    fig, axes = plt.subplots(2, 2, figsize=(12, 8))
    fig.suptitle(
        "CryoCell — Cold-Soak Battery Performance (-20 C ambient)",
        fontsize=15, fontweight="bold",
    )

    panels = [
        ("temp_C",                "Pack Temperature",   "Temperature (C)"),
        ("r_internal_ohm",        "Internal Resistance", "Resistance (Ohms)"),
        ("motor_power_kW",        "Motor Power Output",  "Power (kW)"),
        ("cumulative_energy_kWh", "Cumulative Energy to Motor", "Energy (kWh)"),
    ]

    for ax, (key, title, ylabel) in zip(axes.flat, panels):
        for mode, d in data.items():
            s = STYLE.get(mode, {"label": mode, "color": "gray"})
            ax.plot(d["time_s"], d[key], label=s["label"],
                    color=s["color"], linewidth=2)
        ax.set_title(title, fontweight="bold")
        ax.set_xlabel("Time (s)")
        ax.set_ylabel(ylabel)
        ax.grid(True, alpha=0.3)
        ax.legend()

    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig(OUT_PATH, dpi=150)
    print(f"Plot saved -> {OUT_PATH}")


if __name__ == "__main__":
    if not os.path.exists(CSV_PATH):
        raise SystemExit(
            f"Could not find {CSV_PATH}\n"
            "Run the simulator first:  make run"
        )
    plot(load_csv(CSV_PATH))
