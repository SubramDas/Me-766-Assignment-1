#define PI 3.141592653589

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

// Function to integrate
double f(double x) {
    return cos(x);
}

// Monte Carlo integration function
double monte_carlo_integration(int N, double a, double b) {
    double sum = 0.0;

    #pragma omp parallel
    {
        double local_sum = 0.0;
        unsigned int seed = time(NULL) ^ omp_get_thread_num(); // Unique seed for each thread

        #pragma omp for
        for (int i = 0; i < N; i++) {
            double x = a + (b - a) * rand() / (RAND_MAX + 1.0); // Generate random point in [a, b]
            local_sum += f(x); // Evaluate the function at this point
        }

        #pragma omp atomic
        sum += local_sum; // Update the global sum atomically
    }

    return (b - a) * sum / N; // Calculate the integral
}

int main() {
    double a = -PI / 2; // Lower bound
    double b = PI / 2;  // Upper bound
    int N = 1000000; // Number of random samples for integration
    int thread_counts[] = {2, 4, 6, 8}; // Different thread counts
    int num_trials = 5; // Number of trials for averaging

    printf("Threads\tAverage Time (seconds)\tIntegration Result (each trial)\n");
    printf("--------------------------------------------------------------\n");

    // Timing study
    for (int t = 0; t < sizeof(thread_counts) / sizeof(thread_counts[0]); t++) {
        int num_threads = thread_counts[t];
        double total_time = 0.0;

        for (int trial = 0; trial < num_trials; trial++) {
            // Set the number of OpenMP threads
            omp_set_num_threads(num_threads);

            // Start timing
            double start_time = omp_get_wtime(); // Get the starting time
            double result = monte_carlo_integration(N, a, b); // Perform the integration
            double end_time = omp_get_wtime(); // Get the ending time

            // Calculate elapsed time and accumulate it
            double trial_time = end_time - start_time;
            total_time += trial_time;

            // Print result and time for each trial
            printf("%d  %d: %.8lf %.6lf\n", num_threads, trial + 1, result, trial_time);
        }

        // Calculate and print average time for this thread count
        double average_time = total_time / num_trials;
        printf("Average time for %d threads: %.6lf seconds\n", num_threads, average_time);
        printf("--------------------------------------------------------------\n");
    }

    return 0;
}
