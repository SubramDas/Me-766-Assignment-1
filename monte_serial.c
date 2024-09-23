#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    srand(time(NULL));

    // Perform Monte Carlo integration
    for (int i = 0; i < N; i++) {
        double x = a + (b - a) * rand() / (RAND_MAX + 1.0); // Generate random point in [a, b]
        sum += f(x); // Evaluate the function at this point
    }

    double integral = (b - a) * sum / N; // Calculate the integral
    printf("Estimated integral: %lf\n", integral);

    return 0;
}
