#include <iostream>
#ifdef _WIN32
#include <windows.h> // For Windows dynamic loading
#else
#include <dlfcn.h>   // For Linux (RPI) dynamic loading
#endif
#include <cstdint>

using namespace std;

// Platform-specific paths
// RPI PATHS
const char* path_ftd_rpi = "/home/icarus/libftd2xx.so.1.4.8";
const char* path_lib_rpi = "/home/icarus/libEposCmd.so.6.8.1.0";

// Windows PATHS
const char* path_lib_win = "C:/Users/yann/PycharmProjects/Maxon-interface/EposCmd64.dll";

// Function to setup EPOS
void* epos_setup(bool RPI, uint16_t NodeID, const char* usb, const char* path_win) {
    // EPOS Variables
    int ret = 0;
    void* keyhandle = nullptr;

    uint32_t VELOCITY = 6000;     // RPM
    uint32_t ACCELERATION = 3000; // RPM/s
    uint32_t DECELERATION = 3000; // RPM/s

    uint32_t pErrorCode = 0;
    uint32_t pDeviceErrorCode = 0;

    void* epos = nullptr;

    // Load libraries
    if (RPI) {
        // Linux (RPI) shared library loading
        void* epos = dlopen(path_lib_rpi, RTLD_LAZY);
        if (!epos) {
            cerr << "Failed to load EPOS library on RPI!" << endl;
            return nullptr;
        }

        typedef void* (*VCS_OpenDevice_t)(const char*, const char*, const char*, const char*, uint32_t*);
        VCS_OpenDevice_t VCS_OpenDevice = (VCS_OpenDevice_t)dlsym(epos, "VCS_OpenDevice");
        keyhandle = VCS_OpenDevice("EPOS4", "MAXON SERIAL V2", "USB", usb, &pErrorCode);
    } else {
        // Windows DLL loading
        void* epos = LoadLibrary(path_win);
        if (!epos) {
            cerr << "Failed to load EPOS library on Windows!" << endl;
            return nullptr;
        }

        typedef void* (*VCS_OpenDevice_t)(const char*, const char*, const char*, const char*, uint32_t*);
        VCS_OpenDevice_t VCS_OpenDevice = (VCS_OpenDevice_t)GetProcAddress((HMODULE)epos, "VCS_OpenDevice");
        keyhandle = VCS_OpenDevice("EPOS4", "MAXON SERIAL V2", "USB", usb, &pErrorCode);
    }

    if (keyhandle != nullptr) {
        cout << "EPOS4 opened" << endl;

        // Set operation mode to profile position mode
        typedef int (*VCS_ActivateProfilePositionMode_t)(void*, uint16_t, uint32_t*);
        VCS_ActivateProfilePositionMode_t VCS_ActivateProfilePositionMode = nullptr;
        if (RPI) {
            VCS_ActivateProfilePositionMode = (VCS_ActivateProfilePositionMode_t)dlsym(epos, "VCS_ActivateProfilePositionMode");
        } else {
            VCS_ActivateProfilePositionMode = (VCS_ActivateProfilePositionMode_t)GetProcAddress((HMODULE)epos, "VCS_ActivateProfilePositionMode");
        }
        ret = VCS_ActivateProfilePositionMode(keyhandle, NodeID, &pErrorCode);

        // Set position profile
        typedef int (*VCS_SetPositionProfile_t)(void*, uint16_t, uint32_t, uint32_t, uint32_t, uint32_t*);
        VCS_SetPositionProfile_t VCS_SetPositionProfile = nullptr;
        if (RPI) {
            VCS_SetPositionProfile = (VCS_SetPositionProfile_t)dlsym(epos, "VCS_SetPositionProfile");
        } else {
            VCS_SetPositionProfile = (VCS_SetPositionProfile_t)GetProcAddress((HMODULE)epos, "VCS_SetPositionProfile");
        }
        ret = VCS_SetPositionProfile(keyhandle, NodeID, VELOCITY, ACCELERATION, DECELERATION, &pErrorCode);
    } else {
        cout << "EPOS4 not opened" << endl;
    }

    return keyhandle;
}
