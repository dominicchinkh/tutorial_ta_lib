#pragma once

#include <string>
#include <vector>

// Core historical candle model shared across files
struct MarketCandle {
    double open;
    double high;
    double low;
    double close;
};

// A clean container to pass back the dynamic calculation results
struct IndicatorResult {
    int beg_idx = 0;
    int nb_element = 0;
    int nb_output = 0;
    std::vector<std::vector<double>> series; // Holds 1 or more output lines
};

// Computes technical indicators dynamically via text names (e.g., "SMA", "RSI")
IndicatorResult calculate_indicator_abstract(
    const std::string& indicator_name, 
    const std::vector<MarketCandle>& data, 
    int time_period
);
