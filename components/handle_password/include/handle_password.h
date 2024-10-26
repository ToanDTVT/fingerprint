#ifndef HANDLE_PASSWORD_H
#define HANDLE_PASSWORD_H

#include "gpio_lib.h"
#include "user_data.h"

extern DataPacket USER[MAX_USERS];

extern volatile enum statemachine currentstate;

void func2(void);
void password_task (void * pvParameters);
void create_password_task(void);


#endif //HANDLE_PASSWORD_H
