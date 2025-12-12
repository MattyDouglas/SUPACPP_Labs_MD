// Testing the FiniteFunctions class
// Matthew Douglas
// Edited 7/12/25

// Using MysteryData20232.txt as input file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "FiniteFunctions.h"
#include "Sample.h"

using namespace std;

int main (){

    // Constructing output file
    FiniteFunction f(-10, 10, "Outputs/png/MysteryData20232plot");

    // Reading data from txt file into vector
    ifstream infile("/workspaces/SUPACPP_Labs_MD/Data/MysteryData20232.txt");   // Adjust path as needed
    vector<double> data;
    double value;
    while (infile >> value){
        data.push_back(value);
    }

    // Plotting histogrammed data
    f.plotData(data, 40, true);
    f.plotFunction();   // Overlays default curve
    cout << "Plot for MysteryData20232.txt" << endl;
    return 0;
}