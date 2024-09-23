#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define PI 3.141592653589

// Function to integrate
double f(double x) {
    return cos(x);
}

// Monte Carlo integration function
double monte_carlo_integration(int N, double a, double b) {
    double sum = 0.0;

    // Seed the random number generator
    srand(time(NULL)); // Seed once for all threads

    #pragma omp parallel
    {
        double local_sum = 0.0;

        #pragma omp for
        for (int i = 0; i < N; i++) {
            double x = a + (b - a) * rand() / (RAND_MAX + 1.0); // Generate random point in [a, b]
            local_sum += f(x); // Evaluate the function at this point
        }

        #pragma omp critical
        {
            sum += local_sum; // Update the global sum
        }
    }

    return (b - a) * sum / N; // Calculate the integral
}

int main() {
    double a = -PI / 2; // Lower bound
    double b = PI / 2;  // Upper bound
    double analytical_value = 2.0; // Analytical integral value
    int divisions[] = {5, 10, 50, 100, 500, 1000, 500, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000}; // Different numbers of divisions
    int num_divisions = sizeof(divisions) / sizeof(divisions[0]);

    // Perform convergence study
    printf("N\tNumerical Integral\tAbsolute Error\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < num_divisions; i++) {
        int N = divisions[i];
        double numerical_value = monte_carlo_integration(N, a, b);
        double absolute_error = fabs(numerical_value - analytical_value);
        printf("%d\t%.10lf\t%.10lf\n", N, numerical_value, absolute_error);
    }

    return 0;
}
