#!/bin/bash
# run_experiments.sh
# Automation script for the 802.11ac Wi-Fi simulation performance experiments.

set -e

# Define directories
WORKSPACE_DIR="/home/melihalcik/Desktop/Kablosuz-ve-MobilAglar"
NS3_DIR="$WORKSPACE_DIR/ns-3-dev"
CSV_FILE="$WORKSPACE_DIR/results.csv"

# Clear or initialize CSV file
echo "Initializing results.csv..."
echo "Load,MAC,STA,Throughput" > "$CSV_FILE"

# Define configurations
LOADS=(50 80 90)
MACS=("EDCA" "RTS/CTS")
STAS=(4 8 12 20)

echo "=========================================================================="
echo "Starting Wi-Fi Performance Simulation Experiments..."
echo "=========================================================================="

for load in "${LOADS[@]}"; do
    for mac in "${MACS[@]}"; do
        for sta in "${STAS[@]}"; do
            echo "Running Scenario: Load = ${load}%, MAC = ${mac}, STAs = ${sta}..."
            
            # Execute the simulation inside the ns-3 directory and capture only the throughput value
            throughput=$(cd "$NS3_DIR" && ./ns3 run "scratch/wifi-performance-eval --numSTA=${sta} --macMechanism=${mac} --totalLoadPercent=${load}")
            
            # Print the captured output for visibility
            echo " -> Measured Throughput: ${throughput} Mbit/sec"
            
            # Write to CSV
            echo "${load},${mac},${sta},${throughput}" >> "$CSV_FILE"
        done
    done
done

echo "=========================================================================="
echo "Experiments finished successfully! Data saved to results.csv."
echo "=========================================================================="
echo "Generating plots using matplotlib..."
python3 "$WORKSPACE_DIR/plot_results.py"
echo "=========================================================================="
echo "Saved to graphs (plot_load_50.png, plot_load_80.png, plot_load_90.png)."
echo "=========================================================================="
