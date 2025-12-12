// Header file for sampling data
// Matthew Douglas
// Edited 10/12/25

#include <vector>
#include "FiniteFunctions.h"

#pragma once

using namespace std;

// Sampler function that should work FiniteFunctions subclass
vector<double> Sample(FiniteFunction& target, double r_min, double r_max, int N_samples, double Width);