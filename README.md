# IEEE 802.11ac Wi-Fi Performance Analysis via NS-3 Simulation

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![NS-3](https://img.shields.io/badge/Simulator-NS--3-blue.svg)](https://www.nsnam.org/)
[![C++](https://img.shields.io/badge/Language-C%2B%2B-orange.svg)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/Python-3.7%2B-green.svg)](https://www.python.org/)

**Course:** BLM 4140 - Wireless & Mobile Networks  
**Instructor:** Dr. Mehmet Şükrü KURAN  
**Student:** Melih Alçık (ID: 22011628)  
**University:** Yıldız Technical University (YTU)  
**Date:** June 2026

---

## Overview

This project evaluates the aggregated throughput performance of an **IEEE 802.11ac** (Wi-Fi 5) network under varying conditions using the [NS-3 network simulator](https://www.nsnam.org/). The core motivation comes from the well-known **Bianchi model**, which shows that Wi-Fi throughput degrades as the number of competing stations increases — but the Bianchi model only covers the legacy DCF mechanism. This project investigates whether the same degradation occurs when **modern MAC-layer mechanisms** (EDCA, TXOP, Block ACK, Frame Aggregation) are enabled.

### Research Question

> Does total Wi-Fi throughput still decrease with increasing station count when modern 802.11ac MAC mechanisms (EDCA, TXOP, Block ACK, Frame Aggregation) are active — and how does enabling RTS/CTS change this behavior?

---

## Simulation Parameters

| Parameter | Values Tested |
|-----------|--------------|
| `numSTA` (number of stations) | 4, 8, 12, 20 |
| `macMechanism` | EDCA only / EDCA + RTS/CTS |
| `totalLoadPercent` (offered load) | 50%, 80%, 90% |

**Total scenarios:** 4 × 2 × 3 = **24 scenarios**

### Fixed Configuration

- **Standard:** IEEE 802.11ac (Wi-Fi 5), 2.4 GHz band
- **Channel Bandwidth:** 20 MHz
- **MIMO:** 1×1 (single antenna, single spatial stream)
- **Guard Interval:** Short Guard Interval (SGI) → raw PHY rate: 72.2 Mbps (MCS7)
- **Rate Adaptation:** Minstrel HT algorithm
- **Topology:** Single BSS — one AP, all STAs directly connected
- **Traffic:** Uplink only, TCP protocol (OnOff application)
- **Duration:** 10 seconds per scenario
- **MAC Features Enabled:** EDCA (QoS), TXOP, Block ACK, Frame Aggregation (A-MPDU + A-MSDU)

---

## Project Structure

```
.
├── wifi-performance-eval.cc   # NS-3 C++ simulation script
├── run_experiments.sh         # Bash script to run all 24 scenarios automatically
├── plot_results.py            # Python script to parse results and generate plots
├── BLM4140_Report_22011628.pdf  # Full report with results, graphs, and analysis
├── README.md                  # This file
├── .gitignore
└── results/                   # Generated CSV and plots (gitignored)
    ├── results.csv
    ├── plot_load_50.png
    ├── plot_load_80.png
    └── plot_load_90.png
```

---

## How It Works

### 1. NS-3 Simulation (`wifi-performance-eval.cc`)

The C++ simulation script:
- Creates **1 AP + N STA nodes** in a BSS topology
- Places all STAs 1 meter from the AP (maximum signal strength → highest MCS)
- Configures **802.11ac** with Minstrel HT rate manager
- Sets **short guard interval**, **A-MPDU** (max 65535 bytes) and **A-MSDU** (max 7935 bytes) for frame aggregation
- Enables or disables **RTS/CTS** based on the `macMechanism` parameter (threshold = 0 to enable, 999999 to disable)
- Runs **TCP OnOff** clients on each STA sending uplink traffic to a PacketSink on the AP
- Outputs a single line: the measured total throughput in **Mbit/sec**

### 2. Automation Script (`run_experiments.sh`)

Loops over all combinations of `load × mac × sta`, runs each scenario, captures the throughput output, and appends a row to `results.csv` with format: `Load,MAC,STA,Throughput`.

### 3. Plotting (`plot_results.py`)

Reads `results.csv` and generates **3 separate graphs** — one per `totalLoadPercent` value (50%, 80%, 90%). Each graph shows:
- **X-axis:** Number of STAs (4, 8, 12, 20)
- **Y-axis:** Total Throughput (Mbit/sec)
- **Two curves:** EDCA only vs. EDCA + RTS/CTS

---

## Quick Start

### Prerequisites

- [NS-3](https://www.nsnam.org/) simulator (tested with ns-3-dev)
- **Python 3.7+** with `matplotlib` and `pandas`
- **Bash** shell (Linux/macOS)
- **C++17** compiler (g++ or clang++)

### Setup

```bash
# 1. Clone this repo
git clone https://github.com/darthshadoww/BLM4140-Wireless-Mobile-Networks-Wi-Fi-Performance-Analysis-Project-.git
cd BLM4140-Wireless-Mobile-Networks-Wi-Fi-Performance-Analysis-Project-

# 2. Copy the simulation script to your NS-3 scratch folder
cp wifi-performance-eval.cc /path/to/ns-3-dev/scratch/

# 3. Install Python dependencies
pip install matplotlib pandas numpy
```

### Run All 24 Scenarios

```bash
# Edit WORKSPACE_DIR and NS3_DIR paths in run_experiments.sh first
chmod +x run_experiments.sh
./run_experiments.sh
```

This will:
1. Run all 24 simulation scenarios
2. Save results to `results.csv`
3. Generate `plot_load_50.png`, `plot_load_80.png`, `plot_load_90.png`

### Run a Single Scenario Manually

```bash
cd /path/to/ns-3-dev
./ns3 run "scratch/wifi-performance-eval --numSTA=8 --macMechanism=EDCA --totalLoadPercent=80"
```

---

## Results Summary

Three throughput vs. station-count graphs are generated (one per load level). Each graph compares EDCA-only versus EDCA+RTS/CTS performance across 4, 8, 12, and 20 stations.

### Key Findings

**50% Load:** EDCA drops from 24.7 Mbps (4 STAs) to 13.6 Mbps (20 STAs). RTS/CTS outperforms throughout — Frame Aggregation makes the RTS overhead worthwhile by letting STAs reserve the channel and send large data bursts.

**80% Load:** EDCA degrades sharply after 8 STAs, falling to 13.1 Mbps at 20 STAs due to heavy collisions and backoff. RTS/CTS eliminates most collisions and sustains 26.2 Mbps at 20 STAs.

**90% Load:** EDCA effectively saturates at 13.1 Mbps with 20 STAs — stations spend more time in backoff than transmitting. RTS/CTS holds steady at 26.1 Mbps, confirming that strict channel reservation is essential in dense, heavily loaded Wi-Fi networks.

**Conclusion:** Even with modern MAC mechanisms (EDCA, TXOP, Block ACK, Frame Aggregation) enabled, throughput still degrades with increasing station count under EDCA-only — consistent with the Bianchi model's predictions. RTS/CTS significantly mitigates this degradation at high loads.

Full analysis is documented in [BLM4140_Report_22011628.pdf](BLM4140_Report_22011628.pdf).

---

## Technologies

| Tool | Purpose |
|------|---------|
| NS-3 | Network simulation framework |
| C++ | Simulation script implementation |
| Python / Matplotlib | Result visualization |
| Pandas | CSV data processing |
| Bash | Experiment automation |

---

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

---

## Author

**Melih Alçık** — Student ID: 22011628  
Course: BLM 4140 Wireless & Mobile Networks, Yıldız Technical University
