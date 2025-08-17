#ifndef INTEGRATION_TECHNIQUES_H
#define INTEGRATION_TECHNIQUES_H

#include <stdio.h>

// A function pointer type that represents the mathematical function to be
// integrated. It takes a double (x) and returns a double (f(x)).
typedef double (*MathFunction)(double);

// A struct to hold all the parameters for an integration task.
// This keeps the function signatures clean and organized.
typedef struct {
  MathFunction func;       // Pointer to the function to integrate
  double lower_bound;      // Lower limit of integration (a)
  double upper_bound;      // Upper limit of integration (b)
  long long num_intervals; // Number of subintervals (n)
} IntegrationParams;

// A struct to store the outcome of an integration method.
// This will be used to collect data for the final CSV report.
typedef struct {
  char *method_name;       // Name of the integration technique
  char *function_name;     // Name of the mathematical function being integrated
  long long num_intervals; // The number of intervals used
  double result;           // The computed value of the integral
  double absolute_error;   // The absolute error compared to the exact value
  double execution_time_ms; // Time taken for the computation in milliseconds
} IntegrationResult;

// --- Function Prototypes for the 7 Integration Techniques ---
// Techniques are ordered from simple to more sophisticated.

/**
 * @brief 1. Rectangle Method (Left Endpoint)
 * Approximates the integral by summing the areas of rectangles whose height is
 * determined by the function value at the left endpoint of each interval.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral.
 */
double rectangle_left_endpoint(const IntegrationParams *params);

/**
 * @brief 2. Rectangle Method (Right Endpoint)
 * Approximates the integral by summing the areas of rectangles whose height is
 * determined by the function value at the right endpoint of each interval.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral.
 */
double rectangle_right_endpoint(const IntegrationParams *params);

/**
 * @brief 3. Rectangle Method (Midpoint Rule)
 * A more accurate rectangle method where the height of each rectangle is the
 * function value at the midpoint of the interval.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral.
 */
double rectangle_midpoint(const IntegrationParams *params);

/**
 * @brief 4. Trapezoidal Rule
 * Approximates the integral by summing the areas of trapezoids formed under the
 * curve, effectively averaging the left and right endpoint methods.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral.
 */
double trapezoidal_rule(const IntegrationParams *params);

/**
 * @brief 5. Simpson's 1/3 Rule
 * A powerful method that approximates the function over pairs of intervals
 * using a quadratic polynomial (a parabola). Requires an even number of
 * intervals.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral. Returns NAN for an odd number
 * of intervals.
 */
double simpsons_1_3_rule(const IntegrationParams *params);

/**
 * @brief 6. Simpson's 3/8 Rule
 * Similar to the 1/3 rule, but uses a cubic polynomial over sets of three
 * intervals. Requires the number of intervals to be a multiple of 3.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral. Returns NAN if n is not a
 * multiple of 3.
 */
double simpsons_3_8_rule(const IntegrationParams *params);

/**
 * @brief 7. Monte Carlo Integration
 * A probabilistic method that approximates the integral by taking the average
 * value of the function at random points within the interval and multiplying by
 * the interval width.
 * @param params A pointer to the IntegrationParams struct.
 * @return The approximate value of the integral.
 */
double monte_carlo_integration(const IntegrationParams *params);

#endif // INTEGRATION_TECHNIQUES_H
