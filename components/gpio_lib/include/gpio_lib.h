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

extern DataPacket USER[MAX_USERS];

void func1(void);


#endif // GPIO_LIB_H
