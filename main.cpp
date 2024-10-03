#include <iostream>
#include <thread>
#include <chrono>
#ifdef _WIN32
#include "Definitions_windows.h"// For Windows dynamic loading
#else
#include "Definitions_linux.h"   // For Linux (RPI) dynamic loading
#endif
#define VELOCITY 1000
#define ACCELERATION 1000
#define DECELERATION 1000

void WaitAcknowledged(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    int object_index = 0x6041;
    int object_sub_index = 0x0;
    int number_of_bytes_to_read = 0x02;
    unsigned int pnumber_of_bytes_read;
    unsigned int pData;
    int mask_bit_12 = 0x1000;
    int i = 0;

    while (true) {
        int ret = VCS_GetObject(keyhandle, NodeID, object_index, object_sub_index, &pData, number_of_bytes_to_read,
                                reinterpret_cast<DWORD *>(&pnumber_of_bytes_read),
                                reinterpret_cast<DWORD *>(pErrorCode));
        int bit_12 = mask_bit_12 & pData;

        if (i > 20) {
            return;
        }
        if (bit_12 == mask_bit_12) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            i++;
        } else {
            return;
        }
    }
}

int check_error(int ret, const std::string& function_name, int value) {
    if (ret == 0) {
        std::cout << "Error " << function_name << std::endl;
        return 0;
    } else {
        return value;
    }
}

int get_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    int object_index = 0x6064;
    int object_sub_index = 0x00;
    int number_of_bytes_to_read = 0x04;
    unsigned int number_of_bytes_read;
    unsigned int pData;
    int ret = VCS_GetObject(keyhandle, NodeID, object_index, object_sub_index, &pData, number_of_bytes_to_read,
                            reinterpret_cast<DWORD *>(&number_of_bytes_read), reinterpret_cast<DWORD *>(pErrorCode));
    if (ret == 1) {
        return pData;
    } else {
        std::cout << "Error reading position" << std::endl;
        return 0;
    }
}

int get_position_is(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    long pPositionIs;
    int ret = VCS_GetPositionIs(keyhandle, NodeID, &pPositionIs, reinterpret_cast<DWORD *>(pErrorCode));
    return check_error(ret, "get position is", pPositionIs);
}

void go_to_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position) {
    int ret = VCS_MoveToPosition(keyhandle, NodeID, position, 0, 0, reinterpret_cast<DWORD *>(pErrorCode));
    std::cout << check_error(ret, "go to position", 1) << std::endl;
    WaitAcknowledged(epos, keyhandle, NodeID, pErrorCode);
}

void move_to_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position) {
    int ret = VCS_MoveToPosition(keyhandle, NodeID, position, 1, 1, reinterpret_cast<DWORD *>(pErrorCode));
    std::cout << check_error(ret, "move to position", 1) << std::endl;
    WaitAcknowledged(epos, keyhandle, NodeID, pErrorCode);
}

void set_home_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    int current_position = get_position(epos, keyhandle, NodeID, pErrorCode);

    std::cout << "Current position: " << current_position << std::endl;

    int object_index = 0x30B0;
    int object_sub_index = 0x00;
    int number_of_bytes_to_read = 0x04;
    unsigned int number_of_bytes_read;

    int current_position_c = current_position;

    int ret = VCS_SetObject(keyhandle, NodeID, object_index, object_sub_index, &current_position_c, number_of_bytes_to_read,
                            reinterpret_cast<DWORD *>(&number_of_bytes_read), reinterpret_cast<DWORD *>(pErrorCode));

    if (ret == 0) {
        std::cout << "Error setting home position: " << *pErrorCode << std::endl;
    } else {
        std::cout << "Home position set successfully" << std::endl;
    }

    VCS_DefinePosition(keyhandle, NodeID, 0, reinterpret_cast<DWORD *>(pErrorCode));

    std::cout << check_error(ret, "set home position", 1) << std::endl;
}

void go_home_motor(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    move_to_position(epos, keyhandle, NodeID, pErrorCode, 0);
}

int get_current_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    long pPositionIs;
    int ret = VCS_GetPositionIs(keyhandle, NodeID, &pPositionIs, reinterpret_cast<DWORD *>(pErrorCode));
    return check_error(ret, "get current position", pPositionIs);
}

void enable_epos(void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    int ret = VCS_SetEnableState(keyhandle, NodeID, reinterpret_cast<DWORD *>(pErrorCode));
    std::cout << *pErrorCode << " " << NodeID << std::endl;
    std::cout << check_error(ret, "enable epos", 1) << std::endl;
}

void disable_epos(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    int ret = VCS_SetDisableState(keyhandle, NodeID, reinterpret_cast<DWORD *>(pErrorCode));
}

void set_motor_to_sinus_commuted_mode(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode) {
    int ret = VCS_SetMotorType(keyhandle, NodeID, 0x2, reinterpret_cast<DWORD *>(pErrorCode));
    std::cout << check_error(ret, "set motor", 1) << std::endl;
}

void go_to_position_thread(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position) {
    go_to_position(epos, keyhandle, NodeID, pErrorCode, position);
}

void move_to_position_thread(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position) {
    move_to_position(epos, keyhandle, NodeID, pErrorCode, position);
}
