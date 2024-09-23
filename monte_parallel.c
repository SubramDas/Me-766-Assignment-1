#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define PI 3.141592653589

double f(double x) {
    return cos(x); // Define the function
}

int main() {
    int N = 10000000; // Number of random samples
    double a = -PI / 2; // Lower bound
    double b = PI / 2; // Upper bound
    double sum = 0.0;

    // Seed the random number generator
    srand(time(NULL)); // Seed the random number generator once

    #pragma omp parallel num_threads(6)
    {
        double local_sum = 0.0;

        // Generate random points in parallel
        #pragma omp for 
        for (int i = 0; i < N; i++) {
            // Generate a random point in [a, b] using rand()
            double x = a + (b - a) * rand() / (RAND_MAX + 1.0); 
            local_sum += f(x); // Evaluate the function at this point
        }

        // Update the global sum in a critical section
        #pragma omp critical
        {
            sum += local_sum; // Update the global sum
        }
    }

    double integral = (b - a) * sum / N; // Calculate the integral
    printf("Estimated integral: %lf\n", integral);

    return 0;
}
