#include <iomanip>
#include <iostream>

#include "ta_libc.h" // Only requires the standard library header

#include "standard_features.h"

IndicatorResult calculate_ma_standard(const std::vector<MarketCandle>& data, int timePeriod) 
{
    IndicatorResult result{};

    int dataSize = data.size();

    if (dataSize == 0) {
        return result;
    }

    // Prepare price data
    std::vector<double> closePrices(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        closePrices[i] = data[i].close;
    }

    int startIdx = 0;
    int endIdx = closePrices.size() - 1;
    
    // Define Moving Average parameters
    TA_MAType maType = TA_MAType_SMA;   // Simple Moving Average

    // 4. Calculate the lookback period
    // TA-Lib functions need a certain amount of data before they can output the first valid point.
    int lookback = TA_MA_Lookback(timePeriod, maType);
    
    // Allocate space for the output
    std::vector<double> outReal(closePrices.size(), 0.0);

    // 5. Call TA_MA
    TA_RetCode retCode = TA_MA(
        startIdx,           // startIdx
        endIdx,             // endIdx
        closePrices.data(), // inReal (input array)
        timePeriod,         // optInTimePeriod
        maType,             // optInMAType (e.g., TA_MAType_SMA, TA_MAType_EMA)
        &result.beg_idx,    // outBegIdx (where the output actually starts relative to input)
        &result.nb_element, // outNbElement (how many elements were written to output)
        outReal.data()      // outReal (output array)
    );

    // 6. Process and print results
    if (retCode == TA_SUCCESS) {

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "TA_MA Calculation Successful!\n";
        std::cout << "Lookback periods needed: " << lookback << "\n";
        std::cout << "Output starts at input index: " << result.beg_idx << "\n";
        std::cout << "Number of output elements: " << result.nb_element << "\n\n";

        std::cout << "Day | Price  | MA Value\n";
        std::cout << "-----------------------\n";
        
        for (int i = 0; i < dataSize; ++i) {
            std::cout << std::setw(3) << i << " | " << std::setw(6) << closePrices[i] << " | ";
            
            // Map the flat data index back to TA-Lib's relative output space
            if (i >= result.beg_idx) {
                int outIndex = i - result.beg_idx;
                std::cout << std::setw(8) << outReal[outIndex] << "\n";
            } else {
                std::cout << "   NaN (Lookback period)\n";
            }
        }

        // Push the flat buffer into our return structure's outer series vector
        // A single MA only has 1 output stream, so result.series[0] will hold our vector
        result.series.push_back(std::move(outReal));
    } else {
        std::cerr << "❌ Standard calculation failed." << std::endl;
        result.beg_idx = 0;
        result.nb_element = 0;
    }

    return result;
}
