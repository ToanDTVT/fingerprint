#include <stdio.h>
#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"
#include "handle_password.h"

DataPacket USER[MAX_USERS];

void app_main(void)
{
    uart_init();
    PS_Enroll(1);
    func();
    func1();
}
