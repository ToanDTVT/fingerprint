#ifndef GPIO_LIB_H
#define GPIO_LIB_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_compiler.h"
#include "esp_log.h"
#include "driver/dedic_gpio.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_rom_sys.h"
#include "user_data.h"



/*GPIO for keypad*/
//ROW
#define ROW_1 GPIO_NUM_33             //pin2 keypad
#define ROW_2 GPIO_NUM_12             //pin7 keypad
#define ROW_3 GPIO_NUM_14             //pin6 keypad
#define ROW_4 GPIO_NUM_26             //pin4 keypad

//COL
#define COL_1 GPIO_NUM_25             //pin5 keypad
#define COL_2 GPIO_NUM_32             //pin1 keypad
#define COL_3 GPIO_NUM_27             //pin3 keypad


/*GPIO for relay*/
#define RELAY GPIO_NUM_13             


void func1(void);

void gpio_init();
void keypad_init();
char keypad_get_char();
void open_door(void);


#endif // GPIO_LIB_H
