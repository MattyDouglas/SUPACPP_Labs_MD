// Code to sample data using Sample.h as header file
// Matthew Douglas
// Edited 10/12/25

#include <random>
#include <cmath>
#include <algorithm>
#include "Sample.h"

using namespace std;

// Samples data from a chosen distribution (Normal, Cauchy-Lorentz, Crystal-Ball) using the metropolis algorithm
vector<double> Sample(FiniteFunction& target, double r_min, double r_max, int N_samples, double Width){
    vector<double> samples;
    samples.reserve(N_samples);

    // Generating random number
    mt19937 gen(random_device{}());
    normal_distribution<double> proposal(0, Width);
    uniform_real_distribution<double> uniform(0, 1);

    // Starting at the middle of the range
    double x = 0.5 * (r_min + r_max);

        for (int i = 0; i < N_samples; ++i) {
            double x_p = x + proposal(gen);
    
        // Making sure it stays within the range
        if (x_p < r_min || x_p > r_max) {
            samples.push_back(x);
            continue;
        }

        double p_current = target.callFunction(x);
        double p_new = target.callFunction(x_p);

        double A = min(1.0, static_cast<double>(p_new / p_current));

        if (uniform(gen) < A){
            x = x_p;    // Number is accepted
        }
        samples.push_back(x);   // Builds vector of sampled data
    }
    return samples;
}