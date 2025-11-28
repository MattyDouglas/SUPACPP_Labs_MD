// .h file for functions used in AnalyseData.cxx - Matthew Douglas
// Edited 26/11/25

#pragma once
#include <string>
#include <vector>

using namespace std;

// Function to read data from txt file and sort into vectors x and y
bool ReadData(const string& inputfile, vector<float>& x, vector<float>& y);

// Function to print data from x and y vectors to the terminal
void PrintData(const vector<vector<float>>& cols, const string& sep, size_t N);

// Calculates magnitude of vectors - assuming points in x and y are coordinates
vector<float> Mag_calc(const vector<float>& x, const vector<float>& y);

// Takes data and fits straight line using least-squares method

// Structure for the fitting output
struct Fit_output{
    float grad;
    float intercept;
    float chi2;
};
Fit_output Linear_fit(const vector<float>& x, const vector<float>& y, const vector<float>& sigma, const string& savepath);

// Recursion part for exponents
float Power_calc(float base, int exp);

// Taking power (x^y) for each row in the data file
vector<float> x_pow_y(const vector<float>& x, const vector<float>& y, size_t idx = 0);

// Saving output from functions based on user input choice
void Save_results(const vector<float>& magnitudes, const vector<float>& power, float grad, float intercept);