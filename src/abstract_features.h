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

// Computes technical indicators dynamically via text names (e.g., "SMA", "RSI")
void calculate_indicator_abstract(
    const std::string& indicator_name, 
    const std::vector<MarketCandle>& data, 
    int time_period
);
