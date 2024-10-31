#include <stdio.h>
#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"
#include "handle_password.h"
#include "handle_fingerprint.h"
//#include <Arduino.h>

DataPacket USER[MAX_USERS];
volatile enum statemachine currentstate = STATE_IDLE;
static const char* TAG = "main";

void app_main(void)
{
    gpio_init();
    uart_init();
    keypad_init();
    
    if(verify_password_of_AS608() == true){
        PS_Enroll(1);
        vTaskDelay(1000/portTICK_PERIOD_MS);

        //while(1){
            uint8_t status;
            ESP_LOGW(TAG, "Place your finger on the sensor.");
            
            // Lấy ảnh và tạo đặc trưng
            do {
                status = PS_GetImage();
                ESP_LOGI(TAG, "PS_GetImage response: %02x", status);
            } while (status != 0x00); // Chờ lấy ảnh thành công

            status = PS_GenChar(1); // Tạo đặc trưng ở buffer 1
            if (status != 0x00) {
                ESP_LOGE(TAG, "PS_GENCHAR_1: Failed to generate character from image. Error: %d\n", status);
            }

            //PS_Search(1, 0, 160);
            
            PS_UpChar(1, USER[1].user_fingerprint);

            PS_DownChar(2, USER[1].user_fingerprint);

            PS_UpChar(2, USER[1].user_fingerprint);

        //}

    }

    //PS_Enroll(1);

    //create_password_task();

    // func();
    // func1();
    // func2();
    // func3();
}
