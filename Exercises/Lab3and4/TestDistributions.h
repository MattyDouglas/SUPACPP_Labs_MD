// Header file for trying distribution functions on data from MysteryData20232.txt
// Matthew Douglas
// Edited 8/12/2025

#include<vector>
#include<string>
#include"gnuplot-iostream.h"
#include"FiniteFunctions.h"
#include<cmath>

using namespace std;

#pragma once

class Normal : public FiniteFunction {
    public:
        Normal(double Rmin, double Rmax, string name, double mu, double sigma);
        double callFunction(double x) override;
    private:
        double m_mu, m_sigma;
};

class CauchyLorentz : public FiniteFunction {
    public:
        CauchyLorentz(double Rmin, double Rmax, string name, double x_0, double gamma);
        double callFunction(double x) override;
    private:
        double m_x_0, m_gamma;
};

class Crystal : public FiniteFunction {
    public:
        Crystal(double Rmin, double Rmax, string name, double mu, double sigma, double alpha, double n);
        double callFunction(double x) override;
    private:
        double m_mu, m_sigma, m_alpha, m_n;
        double m_A, m_B, m_N;   // Needed for conditions
};