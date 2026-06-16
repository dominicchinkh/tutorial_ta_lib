#pragma once

#include "indicator-result.h"
#include "market-candle.h"

// Computes specific metrics using direct compile-time bound code paths
IndicatorResult calculateMaStandard(const std::vector<MarketCandle>& data, int timePeriod);
