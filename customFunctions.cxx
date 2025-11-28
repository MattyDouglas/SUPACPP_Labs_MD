// Functions to be used in AnalyseData.cxx - Matthew Douglas
// Edited 26/11/25

// Compiled with AnalyseData.cxx and header file customFunctions.h

#include "customFunctions.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cctype>
#include <numeric>
#include <algorithm>
#include <limits>
using namespace std;

// Reading data from text file to x and y - vectors in this case
bool ReadData(const string& inputfile, vector<float>& x, vector<float>& y) {
    ifstream in(inputfile);
    if (!in.is_open()) return false;

    x.clear();
    y.clear();

    string line;
    // skips headers on the first line
    if (!getline(in, line)) return true;

    while (getline(in, line)) {

        istringstream ss(line);
        string x_str, y_str;

        // Looks for values before the comma and assigns to x_str - spits error message if nothing found
        if (!getline(ss, x_str, ',')) {
            cerr << "Skipped line (no first value): [" << line << "]" << endl;
            continue;   // Skips the value if nothing is found in x
        }
        if (!getline(ss, y_str, ',')) {
            size_t pos = line.find(',');   // Finds comma as delimiter
            if (pos == string::npos) {
                cerr << "Skipped line (no second value): [" << line << "]" << endl;
                continue;   // Skips the value if nothing found in y
            }
            // Takes anything after the comma and places it in y_str
            y_str = line.substr(pos + 1);
        }

        // Converts strings to floats and adds to vectors x and y
        float xf = stof(x_str);
        float yf = stof(y_str);
        x.push_back(xf);
        y.push_back(yf);
    }
    return true;
}

// Prints data from vecots into the terminal
void PrintData(const vector<vector<float>>& cols, const string& sep, size_t N) {
    size_t n = cols[0].size();
    if (N == 0) N = n;              // Prints everything if zero entered
    if (N > n) {
        cout << "Too many lines entered, printing first 5 lines instead" << endl;
        N = 5;
    }

    // Prints data to the terminal - in 2 columns for raw data
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < cols.size(); ++j) {
            cout << cols [j] [i];
            if (j + 1 < cols.size()) cout << sep;
        }
        cout << endl;
    }
}

// Calculates magnitude of the vectors using coordinates given
vector<float> Mag_calc(const vector<float>& x, const vector<float>& y) {
    size_t n = x.size();
    vector<float> Magnitudes;
    Magnitudes.reserve(n);  // allocates storage space in advance

    // Calculates magnitude of each vector - assuming x and y components are in the input file
    for (size_t i = 0; i < n; ++i){
        float x_vec = x[i];
        float y_vec = y[i];
        Magnitudes.push_back(sqrt(x_vec*x_vec + y_vec*y_vec));  // Builds vector of magnitudes
    }
    return Magnitudes;
}

// Linear fit using least squares method - returns gradient and intercept
Fit_output Linear_fit(const vector<float>& x, const vector<float>& y, const vector<float>& sigma, const string& savepath) {
    Fit_output result {0.0, 0.0, 0.0};
    size_t n = min({x.size(), y.size(), sigma.size()});

    //Calculating mean values in each vector
    float mean_x = accumulate(x.begin(), x.begin() + n, 0.0) / static_cast<float>(n);
    float mean_y = accumulate(y.begin(), y.begin() + n, 0.0) / static_cast<float>(n);

    //Finds sums of each vector and components of least-squares calculation
    float sum_x = 0.0;
    float sum_y = 0.0;
    
    for (size_t i = 0; i < n; ++i){
        float dx = (x[i]) - mean_x;
        float dy = (y[i]) - mean_y;

        // Components of least squares calculation
        sum_x += dx * dx;
        sum_y += dy * dx;
    }

    // Calculates gradient and y-intercept
    result.grad = sum_y / sum_x;
    result.intercept = mean_y - result.grad * mean_x;

    // Calculating chi^2 value
    float chi2_total = 0.0;
    
    for (size_t i = 0; i < n; ++i){
        float y_predict = result.grad * x[i] + result.intercept;
        float residual = y[i] - y_predict;
        chi2_total += (residual / sigma[i]) * (residual / sigma[i]);
    }
    result.chi2 = chi2_total / n;
    // Saving to file - change name of file in AnalyseData.cxx
    ofstream out(savepath);
    if (out) {
        out << "y = " << result.grad << "x + " << result.intercept << endl;
        out << "chi^2 / N = " << result.chi2 << endl;
    }
    else {
        cout << "Failed to open file to save" << endl;
    }

    return result;
}

// Recursive function from exponents
float Power_calc(float base, int exp) {
    if (exp == 0) return 1.0f;   // Base case = x^0 = 1

    // 
    if (exp % 2 == 0) {
        float half = Power_calc(base, exp / 2);   // Takes half of exponent if even, then squares result
        return half * half;
    } else {
        return base * Power_calc(base, exp - 1);  // Reduces exponent by 1 if odd, gives x * x^y where y is now even
    }
}

// Calculates x^y for each row in the data file
vector<float> x_pow_y(const vector<float>& x, const vector<float>& y, size_t idx) {
    if (idx >= x.size() || idx >= y.size()) return {};

    // Rounds exponent to nearest whole number
    int Rounded_y = static_cast<int>(std::round(y[idx]));

    vector<float> result = x_pow_y(x, y, idx + 1);
    result.push_back(Power_calc(x[idx], Rounded_y)); // Builds vector of power calculations (comes out backwards??)
    return result;
}

// Saving results of functions to separate files
void Save_results(const vector<float>& magnitudes, const vector<float>& power, float grad, float intercept) {

    // User chooses the input for which outputs to save
    cout << "Which results would you like to save?" << endl;
    cout << "1. Magnitudes" << endl;
    cout << "2. Fitting result (gradient and intercept)" << endl;
    cout << "3. x^y" << endl;
    cout << "Enter your choices (as many as you want)" << endl;

    // Takes in user choice
    string user_choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Needed here for a second user input!
    getline(cin, user_choice);    // Reads the entire input
    istringstream iss(user_choice);
    int Choice;
    string filename;

    // Assigns filename and data depending on user choices
    while (iss >> Choice) {
        if (Choice == 1) {
            filename = "Magnitudes.txt";
            ofstream outFile(filename);
            for (float val : magnitudes) outFile << val << endl;
            cout << "Magnitudes have been saved to: " << filename << endl;
        } 
        else if (Choice == 2) {
            filename = "Fitting.txt";
            ofstream outFile(filename);
            outFile << "Gradient = " << grad << endl;
            outFile << "Intercept = " << intercept << endl;
            cout << "Fitting coefficients have been saved to: " << filename << endl;
        } 
        else if (Choice == 3) {
            filename = "Powers.txt";
            ofstream outFile(filename);
            for (float val : power) outFile << val << endl;
            cout << "Powers have been saved to: " << filename << endl;
        } else {
            cout << "Invalid input" << endl;
        }
    }
}
