#include <iostream>

using namespace std;

#include <iomanip>
#include <iostream>
#include <ta_libc.h>
#include <vector>

int main(int argc, char *argv[]) 
{
    TA_RetCode retCode;

    // Initialize TA-Lib
    retCode = TA_Initialize();
    
    if (retCode == TA_SUCCESS) {
        std::cout << "TA-Lib initialized successfully!" << std::endl;
        
         // 2. Prepare mock price data (10 days of closing prices)
        std::vector<double> closePrices = {10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 14.0, 13.0, 12.0, 11.0};
    
        int startIdx = 0;
        int endIdx = closePrices.size() - 1;
        
        // 3. Define Moving Average parameters
        int timePeriod = 3; // 3-period moving average
        TA_MAType maType = TA_MAType_SMA; // Simple Moving Average

        // 4. Calculate the lookback period
        // TA-Lib functions need a certain amount of data before they can output the first valid point.
        int lookback = TA_MA_Lookback(timePeriod, maType);
        
        // Calculate how many output elements we will actually get
        int outNbElement = 0;
        int outBegIdx = 0;
        
        // Allocate space for the output
        std::vector<double> outReal(closePrices.size(), 0.0);

        // 5. Call TA_MA
        retCode = TA_MA(
            startIdx,           // startIdx
            endIdx,             // endIdx
            closePrices.data(), // inReal (input array)
            timePeriod,         // optInTimePeriod
            maType,             // optInMAType (e.g., TA_MAType_SMA, TA_MAType_EMA)
            &outBegIdx,         // outBegIdx (where the output actually starts relative to input)
            &outNbElement,      // outNbElement (how many elements were written to output)
            outReal.data()      // outReal (output array)
        );

        // 6. Process and print results
        if (retCode == TA_SUCCESS) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "TA_MA Calculation Successful!\n";
            std::cout << "Lookback periods needed: " << lookback << "\n";
            std::cout << "Output starts at input index: " << outBegIdx << "\n";
            std::cout << "Number of output elements: " << outNbElement << "\n\n";

            std::cout << "Day | Price  | MA Value\n";
            std::cout << "-----------------------\n";
            
            for (size_t i = 0; i < closePrices.size(); ++i) {
                std::cout << std::setw(3) << i << " | " << std::setw(6) << closePrices[i] << " | ";
                
                // Because of lookback, the output array maps its 0th index to the input's `outBegIdx`
                if (i >= static_cast<size_t>(outBegIdx)) {
                    int outIndex = i - outBegIdx;
                    std::cout << std::setw(8) << outReal[outIndex] << "\n";
                } else {
                    std::cout << "   NaN (Lookback period)\n";
                }
            }
        }

        // Clean up
        TA_Shutdown();
    } else {
        std::cout << "Failed to initialize TA-Lib. Error code: " << retCode << std::endl;
    }
    
    return 0;
}
