#include <stdio.h>
#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"
#include "handle_password.h"
#include "handle_fingerprint.h"
//#include <Arduino.h>

DataPacket USER[MAX_USERS];
volatile enum statemachine currentstate = STATE_IDLE;


void app_main(void)
{
    gpio_init();
    uart_init();
    keypad_init();
    
    if(verify_password_of_AS608() == true){
        PS_Enroll(1);
    }

    //PS_Enroll(1);

    //create_password_task();

    // func();
    // func1();
    // func2();
    // func3();
}
