import pandas as pd
import matplotlib.pyplot as plt
import os

def main():
    csv_file = "results.csv"
    if not os.path.exists(csv_file):
        print(f"Error: {csv_file} not found. Please run the simulations first.")
        return

    # Read data
    df = pd.read_csv(csv_file)
    
    # Load values to plot
    loads = [50, 80, 90]
    
    for load in loads:
        # Filter data for the specific offered load
        df_load = df[df["Load"] == load]
        
        # Split into EDCA and RTS/CTS
        df_edca = df_load[df_load["MAC"] == "EDCA"].sort_values("STA")
        df_rts_cts = df_load[df_load["MAC"] == "RTS/CTS"].sort_values("STA")
        
        # Set up a premium layout
        fig, ax = plt.subplots(figsize=(9, 6.5), dpi=200)
        
        # Plot EDCA (Sleek deep blue line, circular markers)
        ax.plot(
            df_edca["STA"], 
            df_edca["Throughput"], 
            color="#0C4B8E", 
            marker="o", 
            markersize=9, 
            linewidth=2.5, 
            label="EDCA (RTS/CTS Disabled)",
            linestyle="-"
        )
        
        # Plot RTS/CTS (Sleek crimson red dashed line, square markers)
        ax.plot(
            df_rts_cts["STA"], 
            df_rts_cts["Throughput"], 
            color="#D9383A", 
            marker="s", 
            markersize=9, 
            linewidth=2.5, 
            label="RTS/CTS Enabled (Threshold = 0)",
            linestyle="--"
        )
        
        # Titles and labels (Sleek modern typography fallback)
        ax.set_title(
            f"IEEE 802.11ac Wi-Fi Performance - {load}% Offered Load", 
            fontsize=14, 
            fontweight="bold", 
            pad=15, 
            color="#222222"
        )
        ax.set_xlabel("Number of STAs (numSTA)", fontsize=11, labelpad=10, color="#333333")
        ax.set_ylabel("Total Throughput (Mbit/sec)", fontsize=11, labelpad=10, color="#333333")
        
        # Set exact x-axis ticks
        ax.set_xticks([4, 8, 12, 20])
        ax.set_xticklabels(["4", "8", "12", "20"], fontweight="semibold")
        
        # Faint gray background grid for clean readability
        ax.grid(True, which="both", linestyle=":", linewidth=0.75, color="#cccccc", alpha=0.8)
        
        # Polish axes spining
        for spine in ["top", "right"]:
            ax.spines[spine].set_visible(False)
        ax.spines["left"].set_color("#888888")
        ax.spines["bottom"].set_color("#888888")
        
        # Legend with premium round frame and soft shadow
        ax.legend(
            loc="best", 
            fontsize=10.5, 
            frameon=True, 
            facecolor="#ffffff", 
            edgecolor="#e0e0e0", 
            framealpha=0.9
        )
        
        # Tight layout to optimize space
        plt.tight_layout()
        
        # Save as a high-quality PNG
        filename = f"plot_load_{load}.png"
        plt.savefig(filename, facecolor="#ffffff", edgecolor="none")
        plt.close()
        
        print(f"Generated and saved: {filename}")

if __name__ == "__main__":
    main()
