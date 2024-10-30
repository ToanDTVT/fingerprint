#ifndef HANDLE_FINGERPRINT_H
#define HANDLE_FINGERPRINT_H

#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"

extern DataPacket USER[MAX_USERS];

extern volatile enum statemachine currentstate;

void func3(void);

#endif // HANDLE_FINGERPRINT_H