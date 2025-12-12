// Code for testing data against 3 distributions
// Matthew Douglas
// Edited 8/12/2025

// Needs the various -lboost to get plots...

// Copy and paste this into the terminal to be able to get any plots!
//      g++ -o TestDistributions TestDistributions.cxx FiniteFunctions.cxx Sample.cxx -lboost_iostreams -lboost_system -lboost_filesystem
// All of the relevent files must be in the working folder for it to work, and Outputs/png folder must exist already
// Then run the following in the terminal 
//      ./customFunctions

#include<cmath>
#include<string>
#include"gnuplot-iostream.h"
#include"FiniteFunctions.h"
#include<vector>
#include"TestDistributions.h"
#include<iostream>
#include<fstream>
#include"Sample.h"

using namespace std;

// Normal distribution constructor
Normal::Normal(double Rmin, double Rmax, string name, double mu, double sigma) : FiniteFunction(Rmin, Rmax, name), m_mu(mu), m_sigma(sigma) {}
double Normal::callFunction(double x) {
    return exp(-0.5 * pow((x - m_mu) / m_sigma, 2)) / (m_sigma + sqrt(2*M_PI));   // Applying normal distribution
}

// Cauchy-Lorentz distribution constructor
CauchyLorentz::CauchyLorentz(double Rmin, double Rmax, string name, double x_0, double gamma) : FiniteFunction(Rmin, Rmax, name), m_x_0(x_0), m_gamma(gamma) {}
double CauchyLorentz::callFunction(double x) {
    return 1 / (M_PI * m_gamma * (1 + pow((x - m_x_0) / m_gamma, 2)));   // Applying Cauchy-Lorentz distribution
}

// Crystal-Ball distribution constructor
Crystal::Crystal(double Rmin, double Rmax, string name, double mu, double sigma, double alpha, double n) : FiniteFunction(Rmin, Rmax, name), m_mu(mu), m_sigma(sigma), m_alpha(alpha), m_n(n){
    // Setting up terms to use in distribution
    double abs_alpha = fabs(m_alpha);
    double exp_term = exp(-0.5 * abs_alpha * abs_alpha);
    m_A = pow(m_n / abs_alpha, m_n) * exp_term;
    m_B = (m_n / abs_alpha) - abs_alpha;

    double C = (m_n / abs_alpha) * (1 / (m_n - 1)) * exp_term;
    double D = sqrt(M_PI * 0.5) * (1 + erf(abs_alpha / sqrt(2)));
    m_N = 1 / (m_sigma * (C + D));
}
// Calling the function to act on x where x is the imported data
double Crystal::callFunction(double x) {
    double t = (x - m_mu) / m_sigma;
    if (t > -m_alpha){
        return m_N * exp(-t * t * 0.5);
    } else {
        return m_N * m_A * pow(m_B - t, -m_n);
    }
}

// Main driver for testing the distributions and sampling new data
int main() {
    // Reading data from txt file into vector - same as in TestFiniteFunctions.cxx
    ifstream infile("/workspaces/SUPACPP_Labs_MD/Data/MysteryData20232.txt");   // Adjust path as needed
    vector<double> data;
    double value;
    while (infile >> value){
        data.push_back(value);
    }
    if (data.empty()) {
        cerr << "No data loaded!" << endl;
        return 1;
    }

    // Plots are saved as "MysteryData20232 xxxxx.png" - must have Outputs/png folder first

    // Normal distribution
    Normal gauss(-10, 10, "Outputs/png/MysteryData20232 normal",-2, 2);  // Can change parameters here (range, mu, sigma)
    cout << "Normal Distribution - Range: [-10 10], mu: -2, sigma: 2, Integral: " << gauss.get_integral() << endl;

    // Cauchy-Lorentz distribution
    CauchyLorentz cauchy(-10, 10, "Outputs/png/MysteryData20232 cauchy",-2 ,1.5);  // Can change parameters here (range, x_0, gamma)
    cout << "Cauchy-Lorentz - Range: [-10 10], x_0: -2, gamma: 1.5, Integral: " << cauchy.get_integral() << endl;

    // Crystal-Ball distribution
    Crystal Crystal_ball(-10, 10, "Outputs/png/MysteryData20232 crystal", -2, 1, 1.5, 2);  // Can change parameters here (range, mu, sigma, alpha, n)
    cout << "Crystal-Ball - Range: [-10 10], mu: -2, sigma: 1, alpha: 1.5, n: 2, Integral: " << Crystal_ball.get_integral() << endl;

    // Sampling data from distributions
    auto sample_norm = Sample(gauss, -10, 10, 10000, 1.0);
    auto sample_cauchy = Sample(cauchy, -10, 10, 10000, 1.0);
    auto sample_crystal = Sample(Crystal_ball, -10, 10, 10000, 1.0);
    
    // Plotting sampled data along with functions
    Normal norm_samples(-10, 10, "Outputs/png/MysteryData20232 normal_samples", -2, 2);
    norm_samples.plotData(sample_norm, 40, true);
    norm_samples.plotFunction();

    CauchyLorentz cauchy_samples(-10, 10, "Outputs/png/MysteryData20232 cauchy_samples", -2, 1.5);
    cauchy_samples.plotData(sample_cauchy, 40, true);
    cauchy_samples.plotFunction();

    Crystal crystal_samples(-10, 10, "Outputs/png/MysteryData20232 crystal_samples",-2, 1, 1.5, 2);
    crystal_samples.plotData(sample_crystal, 40, true);
    crystal_samples.plotFunction();
    return 0;
}
