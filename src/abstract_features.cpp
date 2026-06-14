#include <iostream>

#include "ta_libc.h"
#include "ta_abstract.h"

#include "abstract_features.h"

// Robust function to calculate ANY TA-Lib indicator dynamically by string name
void calculate_indicator_abstract(
    const std::string& indicator_name,
    const std::vector<MarketCandle>& data,
    int time_period
) {
    // Convert our struct array into flat parallel arrays required by TA-Lib
    int data_size = data.size();

    std::vector<double> in_open(data_size), in_high(data_size), in_low(data_size), in_close(data_size);
    for (int i = 0; i < data_size; ++i) {
        in_open[i]  = data[i].open;
        in_high[i]  = data[i].high;
        in_low[i]   = data[i].low;
        in_close[i] = data[i].close;
    }

    // Query the Abstract Layer to look up the indicator configuration by name
    const TA_FuncHandle * func_handle = nullptr;

    TA_RetCode ret = TA_GetFuncHandle(indicator_name.c_str(), &func_handle);
    if (ret != TA_SUCCESS) {
        std::cerr << "❌ Indicator '" << indicator_name << "' not found!" << std::endl;
        return;
    }

    const TA_FuncInfo* func_info = nullptr;
    ret = TA_GetFuncInfo(func_handle, &func_info);
    
    if (ret != TA_SUCCESS || !func_info) {
        std::cerr << "❌ Error: Indicator '" << indicator_name << "' not found in TA-Lib registry." << std::endl;
        return;
    }

    std::cout << "🔍 Found Indicator: " << func_info->name << " (" << func_info->hint << ")" << std::endl;

    // 3. Allocate abstract parameter tables
    TA_ParamHolder* params = nullptr;
    TA_ParamHolderAlloc(func_info->handle, &params);

    // 4. Bind the required inputs dynamically based on what the indicator requests
    // Most abstract lookups accept either 'close' or full 'ohlc' data
    TA_SetOptInputParamInteger(params, 0, time_period); // Set the integer tracking rule (e.g., periods)
    
    // For standard math metrics like SMA/EMA, we pass the close array as the primary input channel
    TA_SetInputParamRealPtr(params, 0, in_close.data());

    // 5. Prepare output arrays (allocate maximum possible boundary space)
    std::vector<double> out_real(data_size, 0.0);
    int out_beg_idx = 0;
    int out_nb_element = 0;
    TA_SetOutputParamRealPtr(params, 0, out_real.data());

    // 6. Execute the Abstract Call Function
    ret = TA_CallFunc(params, 0, data_size - 1, &out_beg_idx, &out_nb_element);

    // 7. Process and print results
    if (ret == TA_SUCCESS) {
        std::cout << "📈 Execution Success. Computed " << out_nb_element << " data entries." << std::endl;
        
        int output_tracker = 0;
        for (int i = 0; i < data_size; ++i) {
            if (i >= out_beg_idx) {
                std::cout   << "  [Bar " << i << "] Close: " << in_close[i] 
                            << " -> " << indicator_name << ": " << out_real[output_tracker] << std::endl;
                output_tracker++;
            }
        }
        
        std::cout << std::endl;

    } else {
        std::cerr << "❌ Execution failed with TA-Lib code: " << ret << std::endl;
    }

    // 8. Free up memory allocations
    TA_ParamHolderFree(params);
}
