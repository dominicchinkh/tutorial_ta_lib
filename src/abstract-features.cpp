#include <iostream>

#include "ta_libc.h"
#include "ta_abstract.h"

#include "abstract-features.h"

// Robust function to calculate ANY TA-Lib indicator dynamically by string name
IndicatorResult calculateIndicatorAbstract(
    const std::string& indicatorName,
    const std::vector<MarketCandle>& data,
    int timePeriod
) {
    IndicatorResult result{};

    // Convert our struct array into flat parallel arrays required by TA-Lib
    int data_size = data.size();

    if (data_size == 0) {
        return result;
    }

    std::vector<double> inputOpen(data_size), inputHigh(data_size), inputLow(data_size), inputClose(data_size);
    for (int i = 0; i < data_size; ++i) {
        inputOpen[i]  = data[i].open;
        inputHigh[i]  = data[i].high;
        inputLow[i]   = data[i].low;
        inputClose[i] = data[i].close;
    }

    // Query the Abstract Layer to look up the indicator configuration by name
    const TA_FuncHandle * func_handle = nullptr;

    //--------------------
    // Get Function Handle

    // Available functions are stored in tables: ta-lib/src/ta_abstract/tables
    
    if (TA_GetFuncHandle(indicatorName.c_str(), &func_handle) != TA_SUCCESS) {
        std::cerr << "❌ Indicator '" << indicatorName << "' not found!" << std::endl;
        return result;
    }

    const TA_FuncInfo* func_info = nullptr;
    
    if (TA_GetFuncInfo(func_handle, &func_info) != TA_SUCCESS || !func_info) {
        std::cerr << "❌ Error: Indicator '" << indicatorName << "' not found in TA-Lib registry." << std::endl;
        return result;
    }

    std::cout << "🔍 Found Indicator: " << func_info->name << " (" << func_info->hint << ")" << std::endl;

    //-----------------------------------
    // Allocate abstract parameter tables
    TA_ParamHolder* params = nullptr;
    TA_ParamHolderAlloc(func_info->handle, &params);

    //---------------------------------------------------------------------------
    // Bind the required inputs dynamically based on what the indicator requests
    // Most abstract lookups accept either 'close' or full 'ohlc' data

    // Set the integer tracking rule (e.g., periods)
    TA_SetOptInputParamInteger(params, 0, timePeriod); 
    
    // For standard math metrics like SMA/EMA, we pass the close array as the primary input channel
    TA_SetInputParamRealPtr(params, 0, inputClose.data());

    //--------------------------------------------------------------------
    // Dynamically allocate space for ALL outputs this indicator provides
    result.numOutput = func_info->nbOutput;
    result.series.resize(result.numOutput, std::vector<double>(data_size, 0.0));

    for (int i = 0; i < result.numOutput; ++i) {
        TA_SetOutputParamRealPtr(params, i, result.series[i].data());
    }

    //------------------------------------
    // Execute the Abstract Call Function
    TA_RetCode ret = TA_CallFunc(params, 0, data_size - 1, &result.beginIndex, &result.numElement);

    if (ret != TA_SUCCESS) {
        result.beginIndex = 0;
        result.numElement = 0;
    }

    //----------------------------
    // Free up memory allocations
    TA_ParamHolderFree(params);

    return result;
}

void printAbstractResult(
    const std::vector<MarketCandle> & history, 
    const IndicatorResult & result
)
{
    int output_tracker = 0;
    for (int i = 0; i < history.size(); ++i) {
        std::cout << "  [Bar " << i << "] Close: " << history[i].close << " -> ";

        if (i >= result.beginIndex) {
            // Loop through every output stream this indicator generated
            for (int out_idx = 0; out_idx < result.numOutput; ++out_idx) {
                std::cout << "Out[" << out_idx << "]: " << result.series[out_idx][output_tracker] << " ";
            }
            std::cout << std::endl;

            output_tracker++;
        } 
        else {
            std::cout << "(Lookback period)\n";
        }
    }
    std::cout << std::endl;

}
