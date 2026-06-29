# Wi-Fi Performance Analysis Project (IEEE 802.11n)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

**Course:** BLM 4140 - Wireless & Mobile Networks  
**Student:** Melih Alçık (ID: 22011628)  
**University:** [Your University Name]

---

## 📋 Project Overview

This project evaluates IEEE 802.11n Wi-Fi performance through NS-3 network simulation. It runs 24 different simulation scenarios with varying configurations and generates comprehensive analysis plots comparing throughput across different parameters.

## 📁 Project Structure

```
.
├── wifi-performance-eval.cc          # Core NS-3 simulation script
├── run_experiments.sh                # Bash automation script for running all scenarios
├── plot_results.py                   # Python script for data analysis & visualization
├── BLM4140_Report_22011628.pdf       # Final results documentation with analysis
├── README.md                          # This file
├── .gitignore                        # Git ignore rules
└── results/                          # Generated simulation results (gitignored)
```

### File Descriptions

- **`wifi-performance-eval.cc`** - The core NS-3 simulation script that models IEEE 802.11n network scenarios
- **`run_experiments.sh`** - Automated bash script to execute all 24 simulation scenarios and collect results
- **`plot_results.py`** - Python utility for parsing CSV results and generating visualization plots
- **`BLM4140_Report_22011628.pdf`** - Comprehensive final report with experimental results and analysis

## 🚀 Quick Start

### Prerequisites

- **NS-3 Simulator** (tested with ns-3-dev)
- **Python 3.7+** with matplotlib and pandas
- **Bash Shell**
- **C++ Compiler** (g++/clang++)

### Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/BLM4140_Project.git
cd BLM4140_Project
```

2. Ensure NS-3 is installed and configured properly

3. Install Python dependencies:
```bash
pip install matplotlib pandas numpy
```

### Running the Experiments

#### Option 1: Automated Pipeline (Recommended)

Execute all simulations and generate plots automatically:
```bash
chmod +x run_experiments.sh
./run_experiments.sh
```

This will:
1. Run all 24 simulation scenarios
2. Log results to a CSV file
3. Generate throughput analysis plots
4. Display results summary

#### Option 2: Manual Execution

Run individual components:

**1. Execute Simulations:**
```bash
cd /path/to/ns-3-dev
./waf --run "scratch/wifi-performance-eval"
```

**2. Generate Plots:**
```bash
python plot_results.py
```

## 📊 Simulation Scenarios

The project evaluates 24 different scenarios testing various parameters:
- Different channel bandwidths
- Various distance ranges
- Multiple modulation schemes
- Different traffic patterns

Detailed scenario configurations are documented in the report.

## 📈 Results

Generated plots include:
- Throughput vs Distance
- Throughput vs Channel Bandwidth
- Throughput vs Traffic Load
- Performance Comparison across scenarios

See `BLM4140_Report_22011628.pdf` for detailed analysis and results.

## 🛠️ Technologies Used

- **NS-3 Network Simulator** - Network simulation framework
- **C++** - Simulation logic implementation
- **Python** - Data analysis and visualization
- **Bash** - Automation scripting
- **Matplotlib** - Graph plotting
- **Pandas** - Data processing

## 📝 Documentation

For detailed information about:
- Experimental methodology
- Simulation parameters
- Results analysis
- Conclusions

Please refer to the [BLM4140_Report_22011628.pdf](BLM4140_Report_22011628.pdf)

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👤 Author

**Melih Alçık**
- Student ID: 22011628
- Course: BLM 4140 - Wireless & Mobile Networks

## 📧 Contact

For questions or suggestions, please open an issue in this repository.

---

**Last Updated:** June 2026

