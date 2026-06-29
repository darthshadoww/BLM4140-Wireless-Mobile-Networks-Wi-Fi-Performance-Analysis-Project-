# Wi-Fi Performance Analysis Project (IEEE 802.11n)

**Course:** BLM 4140 - Wireless & Mobile Networks  
**Student:** Melih Alçık  

---

## Project Structure
* `wifi-performance-eval.cc` - The core NS-3 simulation script.
* `run_experiments.sh` - Bash automation script to run all 24 simulation scenarios automatically.
* `plot_results.py` - Python script to parse the CSV results and generate the required throughput plots.
* `BLM4140_Report_22011628.pdf` - The final results documentation containing the analysis and plots.

---

## How to Run Everything Automatically

To replicate the entire simulation dataset and regenerate all three required plot images using the automation pipeline, follow these steps in the terminal:

### 1. Run the Simulations
Execute the bash script from your root `ns-3-dev` directory to automatically run all 24 scenarios and log the data to a CSV file:
```bash
chmod +x run_experiments.sh
./run_experiments.sh
