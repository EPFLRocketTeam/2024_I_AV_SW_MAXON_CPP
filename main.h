#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>

// Constants
#define VELOCITY 1000
#define ACCELERATION 1000
#define DECELERATION 1000

// Function prototypes
void WaitAcknowledged(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
int check_error(int ret, const std::string& function_name, int value);
int get_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
int get_position_is(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
void go_to_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position);
void move_to_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position);
void set_home_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
void go_home_motor(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
int get_current_position(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
void enable_epos(void* keyhandle, int NodeID, unsigned int* pErrorCode);
void disable_epos(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
void set_motor_to_sinus_commuted_mode(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode);
void go_to_position_thread(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position);
void move_to_position_thread(void* epos, void* keyhandle, int NodeID, unsigned int* pErrorCode, int position);

#endif // DEFINITIONS_H