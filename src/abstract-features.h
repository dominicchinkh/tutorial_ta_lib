#pragma once

#include <string>
#include <vector>

#include "indicator-result.h"
#include "market-candle.h"

// Computes technical indicators dynamically via text names (e.g., "SMA", "RSI")
IndicatorResult calculateIndicatorAbstract(
    const std::string& indicatorName, 
    const std::vector<MarketCandle>& data, 
    int timePeriod
);

void printAbstractResult(
    const std::vector<MarketCandle> & history, 
    const IndicatorResult & result
);
