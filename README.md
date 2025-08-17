
# A Comparative Analysis of Numerical Integration Methods in C

This small project presents an implementation and comparative analysis of seven numerical integration techniques, ranging from fundamental algorithms to more sophisticated methods. The primary objective is to evaluate their performance, accuracy, and convergence properties when applied to a set of common mathematical functions.

This study serves as an exercise in applying C programming principles, including memory management, data structures, and modular design, to solve a common problem in computational science. The results are systematically collected and visualized using a Python script to facilitate interpretation.

## Project Structure

The project is organized into a modular structure to promote code clarity, maintainability, and reuse.

```
.
├── integration_comparison.csv  # Raw data output from the C program
├── integration_demo            # The compiled C executable
├── integration_techniques.c    # Implementation of the integration algorithms
├── integration_techniques.h    # Header file defining the public interface
├── main.c                      # Main driver program for benchmarking
├── plot.py                     # Python script for data visualization
├── plots/                        # Output directory for generated plots
│   ├── 1_convergence_...png
│   └── ...
└── README.md                   # Project documentation
```

## Mathematical Background

Numerical integration, or quadrature, aims to approximate the value of a definite integral:

$$ I = \int_{a}^{b} f(x) \,dx $$

The methods implemented here achieve this by discretizing the interval $[a, b]$ into $N$ subintervals of equal width, $\Delta x = \frac{b-a}{N}$. The points defining these intervals are $x_i = a + i \Delta x$ for $i = 0, 1, \dots, N$.

#### 1. Rectangle Rule (Left and Right Endpoint)
This first-order method approximates the function on each subinterval as a constant, forming a rectangle.

-   **Left Endpoint Rule:** The height is determined by the function value at the start of the interval.
    $$ I \approx \Delta x \sum_{i=0}^{N-1} f(x_i) $$
-   **Right Endpoint Rule:** The height is determined by the function value at the end of the interval.
    $$ I \approx \Delta x \sum_{i=1}^{N} f(x_i) $$

#### 2. Midpoint Rule
This method improves upon the rectangle rule by using the function's value at the midpoint of each interval, which often provides a better approximation of the function's average value over that interval.
$$ I \approx \Delta x \sum_{i=0}^{N-1} f\left(x_i + \frac{\Delta x}{2}\right) $$

#### 3. Trapezoidal Rule
This rule approximates the area by summing a series of trapezoids formed by connecting the points $(x_i, f(x_i))$ and $(x_{i+1}, f(x_{i+1}))$ with a straight line. The resulting formula is an average of the left and right endpoint rules.
$$ I \approx \frac{\Delta x}{2} \left[ f(x_0) + 2f(x_1) + 2f(x_2) + \dots + 2f(x_{N-1}) + f(x_N) \right] = \frac{\Delta x}{2} \left( f(x_0) + f(x_N) + 2 \sum_{i=1}^{N-1} f(x_i) \right) $$

#### 4. Simpson's 1/3 Rule
A higher-order method that approximates the function over pairs of intervals using a quadratic polynomial (a parabola). This results in a significantly more accurate approximation for smooth functions. This rule requires that $N$ be an even number.
$$ I \approx \frac{\Delta x}{3} \left( f(x_0) + 4f(x_1) + 2f(x_2) + 4f(x_3) + \dots + 4f(x_{N-1}) + f(x_N) \right) $$

#### 5. Simpson's 3/8 Rule
Similar to the 1/3 rule, this method approximates the function using a cubic polynomial over sets of three intervals. It requires that $N$ be a multiple of 3.
$$ I \approx \frac{3\Delta x}{8} \left( f(x_0) + 3f(x_1) + 3f(x_2) + 2f(x_3) + \dots + 3f(x_{N-1}) + f(x_N) \right) $$

#### 6. Monte Carlo Integration
This probabilistic method estimates the integral based on the law of large numbers. The integral is reformulated as the product of the interval width and the expected value of the function, where the input is a uniformly distributed random variable.
$$ I = (b-a) \cdot \mathbb{E}[f(U)] \approx (b-a) \frac{1}{N} \sum_{i=1}^{N} f(u_i) $$
where each $u_i$ is a random number drawn uniformly from $[a, b]$.

## Implementation Approach

The C implementation adheres to modern software engineering practices to ensure robustness and extensibility.

*   **Modularity via Header Files:** The public API, including data structures and function prototypes, is declared in `integration_techniques.h`, separating the interface from the implementation.
*   **Data Encapsulation with Structs:** Integration parameters (`IntegrationParams`) and results (`IntegrationResult`) are encapsulated in structs to maintain clean function signatures and organized data handling.
*   **Generic Programming with Function Pointers:** The use of the `typedef double (*MathFunction)(double)` allows any compliant mathematical function to be passed as an argument, making the integration framework generic and reusable.
*   **Dynamic Memory Management:** The `malloc()` function is used to allocate storage for an array of `IntegrationResult` structs on the heap, allowing the program to handle a large and variable number of benchmark tests. The allocated memory is explicitly released with `free()` upon completion.
*   **Data Export:** All benchmark results are written to a CSV file for straightforward post-processing and analysis with external tools.

## Results and Analysis

The following visualizations were generated from the benchmark data to compare the performance and accuracy of the implemented methods.

### Plot 1: Convergence Analysis

These log-log plots illustrate the reduction in absolute error as the number of intervals ($N$) increases. The slope of the line indicates the order of convergence.

![Convergence for exp(-x^2)](./plots/1_convergence_exp(-x2).png)
![Convergence for sin(x)](./plots/1_convergence_sin(x).png)
![Convergence for x^2](./plots/1_convergence_x2).png

**Interpretation:**
*   **Superior Convergence of Simpson's Rules:** Simpson's methods (a combination of 1/3 and 3/8 rules in the plot) exhibit the fastest convergence rates. The steep decline in error signifies a high order of accuracy (O($\Delta x^4$)), allowing them to reach machine precision with a relatively small $N$.
*   **Second-Order Convergence:** The Midpoint and Trapezoidal rules show parallel convergence lines, characteristic of second-order methods (O($\Delta x^2$)). They are significantly more accurate than the first-order rectangle rules.
*   **Probabilistic Convergence of Monte Carlo:** The Monte Carlo method displays the slowest convergence rate, approximately O($1/\sqrt{N}$). Its error reduction is independent of function smoothness but is clearly less effective for these one-dimensional, well-behaved functions.

### Plot 2: Accuracy vs. Computational Cost

These plots visualize the trade-off between execution time and absolute error. An ideal algorithm would occupy the bottom-left region (low cost, high accuracy). The point size corresponds to the number of intervals.

![Efficiency for exp(-x^2)](./plots/2_efficiency_exp(-x2).png)
![Efficiency for sin(x)](./plots/2_efficiency_sin(x).png)
![Efficiency for x^2](./plots/2_efficiency_x2).png

**Interpretation:**
*   **Computational Efficiency:** Simpson's Rule demonstrates exceptional computational efficiency. The plots show that a marginal increase in execution time yields a substantial reduction in error, making it highly effective for achieving high-precision results.
*   **Diminishing Returns:** For lower-order methods, achieving higher accuracy requires a disproportionately large increase in computational effort (execution time), indicating lower efficiency.

### Plot 3: Final Accuracy Ranking

This bar chart compares the minimum absolute error achieved by each method at the maximum number of intervals tested.

![Final Accuracy Ranking](./plots/3_final_accuracy_ranking.png)

**Interpretation:**
This plot provides a stark visualization of the accuracy ceiling for each method. Only Simpson's rule is visible because its final error is several orders of magnitude smaller than that of the other methods, which fall below the plot's precision threshold. This highlights its clear superiority for high-accuracy applications.

### Plot 4: Execution Time Analysis

This heatmap presents the average execution time (in milliseconds) for each method across the tested range of intervals.

![Execution Time Heatmap](./plots/4_execution_time_heatmap.png)

**Interpretation:**
*   **Linear Scalability:** All deterministic algorithms exhibit execution times that scale linearly with the number of intervals ($N$), as expected from their single-loop implementation.
*   **Computational Overhead of Random Number Generation:** The Monte Carlo method consistently shows slightly higher execution times compared to other methods for the same $N$. This overhead can be attributed to the computational cost of the pseudo-random number generator function call within its main loop.

## How to Run This Project

1.  **Prerequisites:** A C compiler (e.g., `gcc`) is required. For visualization, Python 3 and the `pandas`, `matplotlib`, and `seaborn` libraries are necessary.

2.  **Compile and Execute the C Program:**
    Use the following command to compile the source files. The `-lm` flag is required to link the math library.
    ```bash
    gcc -Wall -Wextra -o integration_demo main.c integration_techniques.c -lm
    ```
    Run the compiled program to generate the data file:
    ```bash
    ./integration_demo
    ```
    This will create `integration_comparison.csv` in the project directory.

3.  **Generate Visualizations:**
    Install the required Python packages if you haven't already:
    ```bash
    pip install pandas matplotlib seaborn
    ```
    Execute the plotting script:
    ```bash
    python plot.py
    ```
    The resulting plots will be saved in the `plots/` directory.

## Conclusion

The results unequivocally demonstrate that for one-dimensional, well-behaved functions, Simpson's Rule offers the best balance of accuracy and computational efficiency. Its higher-order approximation of the integrand allows it to converge rapidly to a precise solution. While simpler methods like the Trapezoidal and Midpoint rules are valid and instructive, their practical use is limited when high accuracy is required. The Monte Carlo method, though inefficient here, remains an indispensable tool for high-dimensional or complex integration problems where deterministic quadrature is infeasible.
