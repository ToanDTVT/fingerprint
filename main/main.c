#include <stdio.h>
#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"
#include "handle_password.h"
#include "handle_fingerprint.h"
#include "wireless.h"


DataPacket USER[MAX_USERS];
volatile enum statemachine currentstate = STATE_IDLE;

static const char* TAG = "main";


char correct_password[SIZE_OF_PASSWORD] = {1, '1', '1', '1', '1'};
char enter_password[SIZE_OF_PASSWORD];
int number_fail = 0;


void app_main(void)
{
    gpio_init();
    uart_init();
    keypad_init();
    task_wifi_init();
    verify_password_of_AS608();

    currentstate = STATE_IDLE;

    create_password_task();
    create_fingerprint_task();

}
