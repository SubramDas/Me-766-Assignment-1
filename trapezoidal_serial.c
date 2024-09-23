#include <stdio.h>
#include <math.h>

#define PI 3.141592653589

double f(double x) {
    return cos(x);
}

double trapezoidal_rule(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; i++) {
        double x_i = a + i * h;
        sum += f(x_i);
    }
    return sum * h;
}

int main() {
    double a = -PI / 2;  // lower bound
    double b = PI / 2;   // upper bound
    int n = 100000;       // number of subintervals

    double result = trapezoidal_rule(a, b, n);
    printf("Numerical integration result (serial): %f\n", result);

    return 0;
}
