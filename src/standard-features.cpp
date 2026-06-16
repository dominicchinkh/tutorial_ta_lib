#include <iomanip>
#include <iostream>

#include "ta_libc.h"

#include "standard-features.h"

IndicatorResult calculateMaStandard(
    const std::vector<MarketCandle>& data, 
    int timePeriod) 
{
    IndicatorResult result{};

    int dataSize = data.size();

    if (dataSize == 0) {
        return result;
    }

    //--------------------
    // Prepare price data
    std::vector<double> closePrices(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        closePrices[i] = data[i].close;
    }

    int startIndex = 0;
    int endIndex = closePrices.size() - 1;
    
    //----------------------------------
    // Define Moving Average parameters

    // Simple Moving Average
    TA_MAType taType = TA_MAType_SMA;   

    //-------------------------------
    // Calculate the lookback period

    // TA-Lib functions need a certain amount of data before they can output the first valid point.
    int lookback = TA_MA_Lookback(timePeriod, taType);
    
    //-------------------------------
    // Allocate space for the output
    std::vector<double> output(closePrices.size(), 0.0);

    //------------
    // Call TA_MA
    TA_RetCode retCode = TA_MA(
        startIndex,             // start index
        endIndex,               // end index
        closePrices.data(),     // input array
        timePeriod,             // time period
        taType,                 // TA type (e.g., TA_MAType_SMA, TA_MAType_EMA)
        &result.beginIndex,     // where the output actually starts relative to input
        &result.numElement,     // how many elements were written to output
        output.data()           // output array
    );

    //---------------------------
    // Process and print results
    if (retCode == TA_SUCCESS) {

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "TA_MA Calculation Successful!\n";
        std::cout << "Lookback periods needed: "      << lookback          << "\n";
        std::cout << "Output starts at input index: " << result.beginIndex << "\n";
        std::cout << "Number of output elements: "    << result.numElement << "\n\n";

        std::cout << "Day | Price  | MA Value\n";
        std::cout << "-----------------------\n";
        
        for (int i = 0; i < dataSize; ++i) {
            std::cout << std::setw(3) << i << " | " << std::setw(6) << closePrices[i] << " | ";
            
            // Map the flat data index back to TA-Lib's relative output space
            if (i >= result.beginIndex) {
                int outIndex = i - result.beginIndex;
                std::cout << std::setw(8) << output[outIndex] << "\n";
            } else {
                std::cout << "   NaN (Lookback period)\n";
            }
        }

        //----------------------------------------------------------------------
        // Push the flat buffer into our return structure's outer series vector

        // A single MA only has 1 output stream, so result.series[0] will hold our vector
        result.series.push_back(std::move(output));

    } else {
        std::cerr << "❌ Standard calculation failed." << std::endl;
        result.beginIndex = 0;
        result.numElement = 0;
    }

    return result;
}
