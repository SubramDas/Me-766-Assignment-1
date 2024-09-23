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
    double a = -PI / 2; // Lower limit
    double b = PI / 2;  // Upper limit
    int n = 100000; // Number of intervals

    double result = trapezoidal_rule(a, b, n);
    printf("Result of integration (OpenMP): %f\n", result);
    
    return 0;
}


