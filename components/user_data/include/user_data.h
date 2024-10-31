#ifndef USER_DATA_H
#define USER_DATA_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_USERS 10
#define SIZE_OF_PASSWORD 5
//#define SIZE_OF_FINGERPRINT 512

typedef struct {
    //uint8_t username;
    uint8_t id;
    char user_password[SIZE_OF_PASSWORD];
    uint8_t user_fingerprint[6][139];
} DataPacket;

enum statemachine {
	STATE_IDLE,
	STATE_OPENDOOR,
	STATE_DISABLE,
	STATE_SETTING,
	STATE_IDLE_FINGERPRINT,
	STATE_ADD_FINGERPRINT,
	STATE_DELETE_FINGERPRINT
};

void func(void);


#endif //USER_DATA_H