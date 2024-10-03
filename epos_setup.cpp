#include <iostream>
#ifdef _WIN32
#include "Definitions_windows.h"// For Windows dynamic loading
#else
#include "Definitions_linux.h"   // For Linux (RPI) dynamic loading
#endif
#include <cstdint>

using namespace std;

// Platform-specific paths
// RPI PATHS
const char* path_ftd_rpi = "/home/icarus/libftd2xx.so.1.4.8";
const char* path_lib_rpi = "/home/icarus/libEposCmd.so.6.8.1.0";

// Windows PATHS
const char* path_lib_win = "C:/Users/yann/PycharmProjects/Maxon-interface/EposCmd64.dll";


struct EPOS {
    void* keyhandle;
    uint16_t NodeID;
    unsigned int* pErrorCode;
};


// Function to setup EPOS
EPOS epos_setup(uint16_t NodeID) {
    // EPOS Variables
    int ret = 0;
    void* keyhandle = nullptr;

    uint32_t VELOCITY = 6000;     // RPM
    uint32_t ACCELERATION = 3000; // RPM/s
    uint32_t DECELERATION = 3000; // RPM/s
    unsigned int* pErrorCode = 0;

    keyhandle = VCS_OpenDevice( const_cast<char*>("EPOS4"),  const_cast<char*> ("MAXON_RS232"),  const_cast<char*> ("RS232"),  const_cast<char*> ("COM1"), reinterpret_cast<DWORD *>(pErrorCode));

    if (keyhandle != nullptr) {
        cout << "EPOS4 opened" << endl;

        VCS_ActivateProfilePositionMode(keyhandle, NodeID, reinterpret_cast<DWORD *>(&pErrorCode));

        VCS_SetPositionProfile(keyhandle, NodeID, VELOCITY, ACCELERATION, DECELERATION,
                                     reinterpret_cast<DWORD *>(&pErrorCode));
    } else {
        cout << "EPOS4 not opened" << endl;
    }
    EPOS epos = {keyhandle, NodeID, pErrorCode};
    return epos;
}
