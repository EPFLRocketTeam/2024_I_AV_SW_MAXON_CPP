// epos_setup.h
#ifndef EPOS_SETUP_H
#define EPOS_SETUP_H

#include <cstdint>

struct EPOS {
    void* keyhandle;
    uint16_t NodeID;
    unsigned int* pErrorCode;
};

EPOS epos_setup(uint16_t NodeID);

#endif // EPOS_SETUP_H