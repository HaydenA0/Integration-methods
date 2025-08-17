#include "integration_techniques.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CSV_FILENAME "integration_comparison.csv"

double f_quadratic(double x) { return x * x; }
const double F_QUADRATIC_EXACT = 1.0 / 3.0;

double f_sin(double x) { return sin(x); }
const double F_SIN_EXACT = 2.0;

// A Gaussian function: f(x) = e^(-x^2)
// Integral from 0 to 1 has a known value related to the error function.
// erf(1) = 0.842700792949..., integral = sqrt(PI)/2 * erf(1)
double f_gaussian(double x) { return exp(-x * x); }
const double F_GAUSSIAN_EXACT = 0.746824132812;

int main() {
  srand((unsigned int)time(NULL));

  typedef struct {
    MathFunction func;
    const char *name;
    double lower;
    double upper;
    double exact_value;
  } IntegrationProblem;

  IntegrationProblem problems[] = {
      {f_quadratic, "x^2", 0.0, 1.0, F_QUADRATIC_EXACT},
      {f_sin, "sin(x)", 0.0, M_PI, F_SIN_EXACT},
      {f_gaussian, "exp(-x^2)", 0.0, 1.0, F_GAUSSIAN_EXACT}};
  int num_problems = sizeof(problems) / sizeof(problems[0]);

  typedef struct {
    IntegrationFunc func_ptr;
    char *name;
  } Method;

  Method methods[] = {{rectangle_left_endpoint, "1. Left Rectangle"},
                      {rectangle_right_endpoint, "2. Right Rectangle"},
                      {rectangle_midpoint, "3. Midpoint Rule"},
                      {trapezoidal_rule, "4. Trapezoidal Rule"},
                      {simpsons_1_3_rule, "5. Simpson's 1/3 Rule"},
                      {simpsons_3_8_rule, "6. Simpson's 3/8 Rule"},
                      {monte_carlo_integration, "7. Monte Carlo"}};
  int num_methods = sizeof(methods) / sizeof(methods[0]);

  long long interval_counts[] = {100, 1000, 10000, 100000, 1000000};
  int num_interval_tests = sizeof(interval_counts) / sizeof(interval_counts[0]);

  int total_results = num_problems * num_methods * num_interval_tests;
  printf("Preparing to run %d test cases...\n", total_results);

  IntegrationResult *results =
      (IntegrationResult *)malloc(total_results * sizeof(IntegrationResult));
  if (results == NULL) {
    fprintf(stderr, "Fatal: Memory allocation failed for results array.\n");
    return 1;
  }
  int result_index = 0;

  for (int p_idx = 0; p_idx < num_problems; ++p_idx) {
    for (int n_idx = 0; n_idx < num_interval_tests; ++n_idx) {
      for (int m_idx = 0; m_idx < num_methods; ++m_idx) {
        long long current_n = interval_counts[n_idx];

        if (strcmp(methods[m_idx].name, "5. Simpson's 1/3 Rule") == 0 &&
            current_n % 2 != 0) {
          current_n++; // Make it even
        }
        if (strcmp(methods[m_idx].name, "6. Simpson's 3/8 Rule") == 0 &&
            current_n % 3 != 0) {
          current_n += 3 - (current_n % 3); // Make it a multiple of 3
        }

        IntegrationParams params = {.func = problems[p_idx].func,
                                    .lower_bound = problems[p_idx].lower,
                                    .upper_bound = problems[p_idx].upper,
                                    .num_intervals = current_n};

        // Time the execution
        clock_t start = clock();
        double integral_result = methods[m_idx].func_ptr(&params);
        clock_t end = clock();

        double time_spent_ms =
            ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
        double error = fabs(integral_result - problems[p_idx].exact_value);

        // Store the result
        results[result_index].function_name = (char *)problems[p_idx].name;
        results[result_index].method_name = methods[m_idx].name;
        results[result_index].num_intervals = current_n;
        results[result_index].result = integral_result;
        results[result_index].absolute_error = error;
        results[result_index].execution_time_ms = time_spent_ms;

        result_index++;
      }
    }
    printf("Completed benchmarks for function: %s\n", problems[p_idx].name);
  }

  // Export Results to CSV File
  printf("Exporting results to %s...\n", CSV_FILENAME);
  FILE *csv_file = fopen(CSV_FILENAME, "w");
  if (csv_file == NULL) {
    fprintf(stderr, "Error: Could not open file %s for writing.\n",
            CSV_FILENAME);
    free(results); // Clean up memory before exiting
    return 1;
  }

  fprintf(csv_file, "FunctionName,Method,NumIntervals,Result,AbsoluteError,"
                    "ExecutionTime_ms\n");

  for (int i = 0; i < total_results; ++i) {
    if (isnan(results[i].result)) {
      fprintf(csv_file, "%s,%s,%lld,INVALID_N,INVALID_N,%.4f\n",
              results[i].function_name, results[i].method_name,
              results[i].num_intervals, results[i].execution_time_ms);
    } else {
      fprintf(csv_file, "%s,%s,%lld,%.12f,%.12e,%.4f\n",
              results[i].function_name, results[i].method_name,
              results[i].num_intervals, results[i].result,
              results[i].absolute_error, results[i].execution_time_ms);
    }
  }

  fclose(csv_file);
  printf("Export complete.\n");

  // Free the dynamically allocated memory
  free(results);
  printf("Memory freed. Program finished successfully.\n");

  return 0;
}
