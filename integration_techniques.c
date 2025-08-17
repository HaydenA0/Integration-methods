#include "integration_techniques.h"
#include <math.h>
#include <stdlib.h>

// Common validation check for integration parameters
static int is_invalid(const IntegrationParams *params) {
  return (params == NULL || params->func == NULL ||
          params->num_intervals <= 0 ||
          params->upper_bound <= params->lower_bound);
}

// --- 1. Rectangle Method (Left Endpoint) ---
double rectangle_left_endpoint(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double dx = (b - a) / n;
  double sum = 0.0;

  for (long long i = 0; i < n; ++i) {
    sum += f(a + i * dx);
  }

  return sum * dx;
}

// --- 2. Rectangle Method (Right Endpoint) ---
double rectangle_right_endpoint(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double dx = (b - a) / n;
  double sum = 0.0;

  for (long long i = 1; i <= n; ++i) {
    sum += f(a + i * dx);
  }

  return sum * dx;
}

// --- 3. Rectangle Method (Midpoint Rule) ---
double rectangle_midpoint(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double dx = (b - a) / n;
  double sum = 0.0;

  for (long long i = 0; i < n; ++i) {
    sum += f(a + (i + 0.5) * dx);
  }

  return sum * dx;
}

// --- 4. Trapezoidal Rule ---
double trapezoidal_rule(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double dx = (b - a) / n;
  double sum = 0.5 * (f(a) + f(b)); // Add endpoints (weighted by 0.5)

  for (long long i = 1; i < n; ++i) {
    sum += f(a + i * dx);
  }

  return sum * dx;
}

// --- 5. Simpson's 1/3 Rule ---
double simpsons_1_3_rule(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;
  if (params->num_intervals % 2 != 0) {
    // Simpson's 1/3 rule requires an even number of intervals
    return NAN;
  }

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double dx = (b - a) / n;
  double sum = f(a) + f(b); // Start with endpoints

  // Add terms with coefficient 4 (odd indices)
  for (long long i = 1; i < n; i += 2) {
    sum += 4.0 * f(a + i * dx);
  }

  // Add terms with coefficient 2 (even indices)
  for (long long i = 2; i < n - 1; i += 2) {
    sum += 2.0 * f(a + i * dx);
  }

  return sum * dx / 3.0;
}

// --- 6. Simpson's 3/8 Rule ---
double simpsons_3_8_rule(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;
  if (params->num_intervals % 3 != 0) {
    // Simpson's 3/8 rule requires n to be a multiple of 3
    return NAN;
  }

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double dx = (b - a) / n;
  double sum = f(a) + f(b); // Start with endpoints

  for (long long i = 1; i < n; ++i) {
    if (i % 3 == 0) {
      sum += 2.0 * f(a + i * dx);
    } else {
      sum += 3.0 * f(a + i * dx);
    }
  }

  return sum * dx * 3.0 / 8.0;
}

// --- 7. Monte Carlo Integration ---
double monte_carlo_integration(const IntegrationParams *params) {
  if (is_invalid(params))
    return NAN;

  double a = params->lower_bound;
  double b = params->upper_bound;
  long long n = params->num_intervals;
  MathFunction f = params->func;

  double sum = 0.0;
  double range = b - a;

  for (long long i = 0; i < n; ++i) {
    // Generate a random number in [0, 1] and scale it to [a, b]
    double random_x = a + ((double)rand() / RAND_MAX) * range;
    sum += f(random_x);
  }

  // Average value of f(x) is sum/n. Integral is average value * width.
  return (sum / n) * range;
}
