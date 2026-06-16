#pragma once

#include <vector>

// A clean container to pass back the dynamic calculation results
struct IndicatorResult {
    int beginIndex = 0;
    int numElement = 0;
    int numOutput = 0;
    std::vector<std::vector<double>> series; // Holds 1 or more output lines
};
