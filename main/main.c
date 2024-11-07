#include <stdio.h>
#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"
#include "handle_password.h"
#include "handle_fingerprint.h"
#include "wireless.h"


DataPacket USER[MAX_USERS];
Data_send_server data;
Type_open_door action;
volatile enum statemachine currentstate = STATE_IDLE;

static const char* TAG = "main";


char correct_password[SIZE_OF_PASSWORD] = {1, '1', '1', '1', '1'};
char enter_password[SIZE_OF_PASSWORD];
int number_fail = 0;


void app_main(void)
{
    USER[0].en = 1;
    USER[0].user_password[0] = 1;
    USER[0].user_password[1] = '1';
    USER[0].user_password[2] = '1';
    USER[0].user_password[2] = '1';
    USER[0].user_password[4] = '1';
    gpio_init();
    uart_init();
    uart2_init();
    keypad_init();
    task_wifi_init();
    mqtt_app_start();
    verify_password_of_AS608();

    currentstate = STATE_IDLE;

    create_password_task();
    create_fingerprint_task();

}
