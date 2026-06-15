#include <iostream>

#include "ta_libc.h"
#include "ta_abstract.h"

#include "abstract_features.h"

// Robust function to calculate ANY TA-Lib indicator dynamically by string name
IndicatorResult calculate_indicator_abstract(
    const std::string& indicator_name,
    const std::vector<MarketCandle>& data,
    int time_period
) {
    IndicatorResult result{};

    // Convert our struct array into flat parallel arrays required by TA-Lib
    int data_size = data.size();

    if (data_size == 0) {
        return result;
    }

    std::vector<double> in_open(data_size), in_high(data_size), in_low(data_size), in_close(data_size);
    for (int i = 0; i < data_size; ++i) {
        in_open[i]  = data[i].open;
        in_high[i]  = data[i].high;
        in_low[i]   = data[i].low;
        in_close[i] = data[i].close;
    }

    // Query the Abstract Layer to look up the indicator configuration by name
    const TA_FuncHandle * func_handle = nullptr;

    //--------------------
    // Get Function Handle
    if (TA_GetFuncHandle(indicator_name.c_str(), &func_handle) != TA_SUCCESS) {
        std::cerr << "❌ Indicator '" << indicator_name << "' not found!" << std::endl;
        return result;
    }

    const TA_FuncInfo* func_info = nullptr;
    
    if (TA_GetFuncInfo(func_handle, &func_info) != TA_SUCCESS || !func_info) {
        std::cerr << "❌ Error: Indicator '" << indicator_name << "' not found in TA-Lib registry." << std::endl;
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
    TA_SetOptInputParamInteger(params, 0, time_period); 
    
    // For standard math metrics like SMA/EMA, we pass the close array as the primary input channel
    TA_SetInputParamRealPtr(params, 0, in_close.data());

    //--------------------------------------------------------------------
    // Dynamically allocate space for ALL outputs this indicator provides
    result.nb_output = func_info->nbOutput;
    result.series.resize(result.nb_output, std::vector<double>(data_size, 0.0));

    for (int i = 0; i < result.nb_output; ++i) {
        TA_SetOutputParamRealPtr(params, i, result.series[i].data());
    }

    //------------------------------------
    // Execute the Abstract Call Function
    TA_RetCode ret = TA_CallFunc(params, 0, data_size - 1, &result.beg_idx, &result.nb_element);

    //----------------------------
    // Free up memory allocations
    TA_ParamHolderFree(params);

    return result;
}
