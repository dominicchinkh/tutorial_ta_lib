#include <iomanip>
#include <iostream>
#include <vector>

#include <ta_abstract.h>
#include <ta_libc.h>

#include "abstract_features.h"
#include "standard_features.h"

using namespace std;

void print_result(const std::vector<MarketCandle> & history, const IndicatorResult & result)
{
    int output_tracker = 0;
    for (int i = 0; i < history.size(); ++i) {
        if (i >= result.beg_idx) {
            std::cout << "  [Bar " << i << "] Close: " << history[i].close << " -> ";
        
            // Loop through every output stream this indicator generated
            for (int out_idx = 0; out_idx < result.nb_output; ++out_idx) {
                std::cout << "Out[" << out_idx << "]: " << result.series[out_idx][output_tracker] << " ";
            }
            std::cout << std::endl;

            output_tracker++;
        }
    }
    std::cout << std::endl;

}

int main(int argc, char *argv[]) 
{
    // Initialize TA-Lib
    TA_RetCode retCode = TA_Initialize();
    
    if (retCode != TA_SUCCESS) {
        std::cout << "Failed to initialize TA-Lib. Error code: " << retCode << std::endl;
        return 1;
    }
    
    std::cout << "TA-Lib initialized successfully!" << std::endl;

    // Mock dataset
    std::vector<MarketCandle> history = {
        {100.0, 102.5, 99.0,  101.5},
        {101.5, 103.0, 100.5, 102.0},
        {102.0, 102.5, 97.5,  98.0},
        {98.0,  99.5,  96.0,  95.0},
        {95.0,  96.0,  94.0,  99.0},
        {99.0,  100.5, 95.0,  103.5},
        {103.5, 104.0, 99.0,  105.0},
        {105.0, 106.5, 103.0, 104.0},
        {104.0, 105.5, 102.0, 106.0},
        {106.0, 107.0, 104.5, 106.5}
    };

    std::cout << "=================== SYSTEM STARTUP ===================" << std::endl;

    // RUN FILE 1: Abstract Dynamic Features Module
    IndicatorResult rs1 = calculate_indicator_abstract("SMA", history, 3);
    print_result(history, rs1);

    IndicatorResult rs2 = calculate_indicator_abstract("EMA", history, 3);
    print_result(history, rs2);

    std::cout << "------------------------------------------------------" << std::endl;

    calculate_ma_standard(history, 3);

    std::cout << "======================================================" << std::endl;

    // Shutdown and release memory frames
    TA_Shutdown();

    return 0;
}
