#pragma once

#include "abstract_features.h" // For the MarketCandle definition

// Computes specific metrics using direct compile-time bound code paths
void calculate_ma_standard(const std::vector<MarketCandle>& data, int timePeriod);
