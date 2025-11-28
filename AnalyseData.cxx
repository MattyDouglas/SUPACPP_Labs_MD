// SUPACPP Assignment 1 - Matthew Douglas
// Edited 26/11/25

// To run this code you need to copy the following into the terminal:
// g++ -o AnalyseData AnalyseData.cxx customFunctions.cxx
// Followed by:
// ./AnalyseData

// It should then run you through the code, with user inputs for the number 
// of lines to print, and which outputs to save

#include "customFunctions.h"
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    vector<float> x, y;
    // Name of file to open - change as needed
    const string file = "/workspaces/SUPACPP_Labs_MD/Exercises/Lab1and2/input2D_float.txt";

    // Reads data into x and y variables - spits error message if file not opened
    if (!ReadData(file, x, y)) {
        cout << "Failed to open " << file << endl;
        return 1;
    }

    // Prints data in x and y vectors to the terminal
    size_t N;
    cout << "How many lines to print?" << endl;
    cin >> N;
    cout << "x values" << " " << "y values" << endl;
    PrintData({x, y}, " ", N);


    // Calculates magnitude of vectors and prints to terminal
    vector<float> Mags = Mag_calc(x, y);
    cout << "Magnitudes:" << endl;
    PrintData({Mags}, " ", N);

    // Applies linear fit to the data and prints resulting function to the terminal
    vector<float> sigma_x, sigma_y;
    // Reads error file into two vectors - Change file path as needed
    if (!ReadData("/workspaces/SUPACPP_Labs_MD/Exercises/Lab1and2/error2D_float.txt", sigma_x, sigma_y)) {
        cout << "Failed to open file" << endl;
        return 1;
    }

    Fit_output fit = Linear_fit(x, y, sigma_y, "Linear_equation.txt");

    // Prints output function and error analysis to terminal
    cout << "Linear Fit: y = " << fit.grad << "x + " << fit.intercept << endl;
    cout << "Chi^2 value: " << fit.chi2 << endl;

    // Calculating x^y for each element
    vector<float> exponent_calc = x_pow_y(x, y);

    // Printing results
    cout << "x^y per element: " << endl;
    for (float val : exponent_calc){
        cout << val << endl;
    }

    // Saving results to separate files
    Save_results(Mags, exponent_calc, fit.grad, fit.intercept);
}
