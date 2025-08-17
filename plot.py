import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import warnings

# --- Configuration ---
CSV_FILE = "integration_comparison.csv"
OUTPUT_DIR = "plots"
DPI = 300  # High resolution for saved plots

# Suppress warnings for a cleaner output, e.g., from using log scale with zero values
warnings.filterwarnings("ignore", category=UserWarning)


def load_and_clean_data(filepath):
    """
    Loads the CSV data, cleans it for plotting, and returns a pandas DataFrame.
    """
    if not os.path.exists(filepath):
        print(f"Error: The file '{filepath}' was not found.")
        print("Please run the C program first to generate the CSV file.")
        return None

    df = pd.read_csv(filepath)

    # Convert columns to numeric, coercing errors (like 'INVALID_N') to NaN
    df["AbsoluteError"] = pd.to_numeric(df["AbsoluteError"], errors="coerce")

    # Drop rows where the calculation was invalid
    df.dropna(inplace=True)

    # Clean up method names by removing the leading number and period (e.g., "1. Left Rectangle" -> "Left Rectangle")
    df["Method"] = df["Method"].str.split(". ").str[1]

    # For Monte Carlo, error can sometimes be zero if it gets the exact answer by chance.
    # To plot on a log scale, replace zero error with a very small number.
    df["AbsoluteError"] = df["AbsoluteError"].replace(0, 1e-16)

    print("Data loaded and cleaned successfully.")
    return df


def plot_convergence(df):
    """
    PLOT 1: Error Convergence Plot
    Shows how the absolute error decreases as the number of intervals increases.
    This is the most fundamental comparison of numerical method accuracy.
    A steeper slope on this log-log plot means faster convergence.
    """
    print("Generating Plot 1: Error Convergence...")

    # Get unique function names to create a separate plot for each
    functions = df["FunctionName"].unique()

    for func_name in functions:
        plt.style.use("seaborn-v0_8-whitegrid")
        fig, ax = plt.subplots(figsize=(12, 8))

        subset_df = df[df["FunctionName"] == func_name]

        # Use seaborn for a beautiful line plot with automatic color handling and legend
        sns.lineplot(
            data=subset_df,
            x="NumIntervals",
            y="AbsoluteError",
            hue="Method",
            style="Method",  # Use different line styles as well
            markers=True,
            dashes=True,
            ax=ax,
        )

        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_title(
            f"Convergence of Integration Methods for f(x) = {func_name}",
            fontsize=16,
            weight="bold",
        )
        ax.set_xlabel("Number of Intervals (log scale)", fontsize=12)
        ax.set_ylabel("Absolute Error (log scale)", fontsize=12)
        ax.legend(title="Integration Method", fontsize=10)
        ax.grid(True, which="both", ls="--")

        plt.tight_layout()
        filename = os.path.join(
            OUTPUT_DIR, f'1_convergence_{func_name.replace("^", "")}.png'
        )
        plt.savefig(filename, dpi=DPI)
        plt.close(fig)

    print("Convergence plots saved.")


def plot_performance_vs_accuracy(df):
    """
    PLOT 2: Performance vs. Accuracy Scatter Plot
    This creative plot shows the trade-off between speed (execution time) and accuracy.
    The ideal method would be in the bottom-left corner (fast and accurate).
    """
    print("Generating Plot 2: Performance vs. Accuracy...")

    functions = df["FunctionName"].unique()

    for func_name in functions:
        plt.style.use("seaborn-v0_8-talk")
        fig, ax = plt.subplots(figsize=(14, 9))

        subset_df = df[df["FunctionName"] == func_name]

        sns.scatterplot(
            data=subset_df,
            x="ExecutionTime_ms",
            y="AbsoluteError",
            hue="Method",
            size="NumIntervals",
            sizes=(50, 500),  # Control the range of point sizes
            alpha=0.7,
            ax=ax,
            palette="viridis",
        )

        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_title(
            f"Accuracy vs. Execution Time for f(x) = {func_name}",
            fontsize=18,
            weight="bold",
        )
        ax.set_xlabel("Execution Time in milliseconds (log scale)", fontsize=14)
        ax.set_ylabel("Absolute Error (log scale)", fontsize=14)

        # Improve legend handling
        handles, labels = ax.get_legend_handles_labels()
        ax.legend(
            handles,
            labels,
            bbox_to_anchor=(1.05, 1),
            loc="upper left",
            borderaxespad=0.0,
        )

        plt.tight_layout(rect=[0, 0, 0.85, 1])  # Adjust layout to make space for legend
        filename = os.path.join(
            OUTPUT_DIR, f'2_efficiency_{func_name.replace("^", "")}.png'
        )
        plt.savefig(filename, dpi=DPI)
        plt.close(fig)

    print("Efficiency plots saved.")


def plot_final_accuracy_ranking(df):
    """
    PLOT 3: Final Accuracy Ranking Bar Plot
    Compares the absolute error of each method at the highest number of intervals.
    This provides a clear "winner" for accuracy for each function.
    """
    print("Generating Plot 3: Final Accuracy Ranking...")

    # Filter for the results with the maximum number of intervals
    max_n = df["NumIntervals"].max()
    final_df = df[df["NumIntervals"] == max_n]

    plt.style.use("seaborn-v0_8-pastel")
    fig, ax = plt.subplots(figsize=(15, 8))

    sns.barplot(
        data=final_df,
        x="FunctionName",
        y="AbsoluteError",
        hue="Method",
        ax=ax,
        palette="magma",
    )

    ax.set_yscale("log")
    ax.set_title(f"Final Accuracy Ranking at N = {max_n:,}", fontsize=16, weight="bold")
    ax.set_xlabel("Mathematical Function", fontsize=12)
    ax.set_ylabel("Absolute Error (log scale)", fontsize=12)
    ax.legend(title="Method", bbox_to_anchor=(1.02, 1), loc="upper left")
    ax.tick_params(axis="x", rotation=10)  # Slightly rotate x-axis labels if needed

    plt.tight_layout(rect=[0, 0, 0.85, 1])
    filename = os.path.join(OUTPUT_DIR, "3_final_accuracy_ranking.png")
    plt.savefig(filename, dpi=DPI)
    plt.close(fig)
    print("Final accuracy plot saved.")


def plot_execution_time_heatmap(df):
    """
    PLOT 4: Execution Time Heatmap
    A creative way to visualize the execution time across all methods and interval counts.
    Darker cells indicate longer computation time.
    """
    print("Generating Plot 4: Execution Time Heatmap...")

    # We average the time across the different functions for a general overview
    pivot_df = df.pivot_table(
        index="Method",
        columns="NumIntervals",
        values="ExecutionTime_ms",
        aggfunc="mean",  # Average time across all functions
    )

    plt.style.use("classic")
    fig, ax = plt.subplots(figsize=(12, 8))

    sns.heatmap(
        pivot_df,
        annot=True,  # Annotate cells with the time values
        fmt=".2f",  # Format annotations to 2 decimal places
        cmap="rocket_r",  # Use a reversed colormap (darker is higher)
        linewidths=0.5,
        ax=ax,
    )

    ax.set_title("Average Execution Time (ms) Heatmap", fontsize=16, weight="bold")
    ax.set_xlabel("Number of Intervals", fontsize=12)
    ax.set_ylabel("Integration Method", fontsize=12)

    plt.tight_layout()
    filename = os.path.join(OUTPUT_DIR, "4_execution_time_heatmap.png")
    plt.savefig(filename, dpi=DPI)
    plt.close(fig)
    print("Execution time heatmap saved.")


def main():
    """
    Main function to orchestrate the plotting process.
    """
    # Create the output directory if it doesn't exist
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    # Load and process the data
    data_df = load_and_clean_data(CSV_FILE)

    if data_df is not None:
        # Generate all plots
        plot_convergence(data_df)
        plot_performance_vs_accuracy(data_df)
        plot_final_accuracy_ranking(data_df)
        plot_execution_time_heatmap(data_df)

        print(
            f"\nAll plots have been successfully saved to the '{OUTPUT_DIR}/' directory."
        )


if __name__ == "__main__":
    main()
