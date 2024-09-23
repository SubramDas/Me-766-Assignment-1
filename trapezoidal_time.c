#define PI 3.141592653589

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

double f(double x) {
    return cos(x);
}

// Parallel trapezoidal rule implementation
double trapezoidal_parallel(double a, double b, int N) {
    double h = (b - a) / N;
    double sum = 0.0;

    #pragma omp parallel
    {
        double local_sum = 0.0;

        #pragma omp for
        for (int i = 1; i < N; i++) {
            double x = a + i * h; // Calculate x
            local_sum += f(x); // Sum f(x) at each interval
        }

        #pragma omp atomic
        sum += local_sum; // Update the global sum atomically
    }

    sum += 0.5 * (f(a) + f(b)); // Add f(a) and f(b)
    return sum * h; // Return the final integral
}

int main() {
    double a = -PI / 2; // Lower bound
    double b = PI / 2;  // Upper bound
    int N = 1000000; // Number of divisions for integration
    int thread_counts[] = {2, 4, 6, 8}; // Different thread counts
    int num_trials = 5; // Number of trials for averaging

    printf("Threads\tAverage Time (seconds) - Parallel\n");
    printf("----------------------------------------------------------\n");

    // Timing study
    for (int t = 0; t < sizeof(thread_counts) / sizeof(thread_counts[0]); t++) {
        int num_threads = thread_counts[t];
        double total_time_serial = 0.0;
        double total_time_parallel = 0.0;

        for (int trial = 0; trial < num_trials; trial++) {
            // Set the number of OpenMP threads
            omp_set_num_threads(num_threads);

            // Parallel Timing
            double start_time = omp_get_wtime(); // Get the starting time for parallel
            trapezoidal_parallel(a, b, N); // Perform the parallel integration
            double end_time = omp_get_wtime(); // Get the ending time for parallel
            total_time_parallel += (end_time - start_time); // Calculate elapsed time for parallel
        }

        // Calculate average time for this thread count
        double average_time_parallel = total_time_parallel / num_trials;
        printf("%d\t\t\t%.6lf\n", num_threads, average_time_parallel);
    }

    return 0;
}
