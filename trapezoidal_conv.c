#include <stdio.h>
#include <math.h>
#include <omp.h>

#define PI 3.141592653589

double f(double x) {
    return cos(x);
}

double trapezoidal_rule(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;

    #pragma omp parallel num_threads(6)
    {
        double local_sum = 0.0;

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = a + i * h;
            if (i == 0 || i == n - 1) {
                local_sum += f(x) / 2.0; // First and last terms
            } else {
                local_sum += f(x); // Middle terms
            }
        }

        #pragma omp atomic
        sum += local_sum;
        
    }

    return sum * h;
}
int main() {
    double a = -PI / 2; // Lower bound
    double b = PI / 2;  // Upper bound
    double analytical_value = 2.0; // Analytical integral value
    int divisions[] = {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000}; // Different numbers of divisions
    int num_divisions = sizeof(divisions) / sizeof(divisions[0]);

    // Perform convergence study
    printf("N\tNumerical Integral\tAbsolute Error\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < num_divisions; i++) {
        int N = divisions[i];
        double numerical_value = trapezoidal_rule(a, b, N);
        double absolute_error = fabs(numerical_value - analytical_value);
        printf("%d\t%.10lf\t%.10lf\n", N, numerical_value, absolute_error);
    }

    return 0;
}
