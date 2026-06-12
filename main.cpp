#include <iostream>

using namespace std;

#include <ta_libc.h> // The main TA-Lib header

int main(int argc, char *argv[]) 
{
    TA_RetCode retCode;

    // Initialize TA-Lib
    retCode = TA_Initialize();
    
    if (retCode == TA_SUCCESS) {
        std::cout << "TA-Lib initialized successfully!" << std::endl;
        
        // Clean up
        TA_Shutdown();
    } else {
        std::cout << "Failed to initialize TA-Lib. Error code: " << retCode << std::endl;
    }
    
    return 0;
}
