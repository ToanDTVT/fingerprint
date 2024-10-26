#ifndef USER_DATA_H
#define USER_DATA_H

#include <stdint.h>
#include <stdio.h>

#define MAX_USERS 10
#define SIZE_OF_PASSWORD 5

typedef struct {
    //uint8_t username;
    uint8_t id;
    char user_password[SIZE_OF_PASSWORD];
    uint8_t user_fingerprint;
} DataPacket;


void func(void);


#endif //USER_DATA_H