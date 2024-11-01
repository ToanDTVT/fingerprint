#include <stdio.h>
#include "handle_fingerprint.h"

static const char* TAG = "handle_fingerprint";

// TaskHandle_t fingerprint_task_handle = NULL;

void fingerprint_task(){

    if(verify_password_of_AS608() == true){

        while(1){

            // uint8_t status;
            // ESP_LOGW(TAG, "Place your finger on the sensor.");
            // do {
            //     status = PS_GetImage();
            // } while (status != 0x00); // Chờ lấy ảnh thành công
            // status = PS_GenChar(1); // Tạo đặc trưng ở buffer 1
            // if (status != 0x00) {
            //     ESP_LOGE(TAG, "PS_GENCHAR_1: Failed to generate character from image. Error: %d\n", status);
            // }

            switch(currentstate){
                case STATE_IDLE:
                    uint8_t status;
                    ESP_LOGW(TAG, "Place your finger on the sensor.");
                    do {
                        status = PS_GetImage();
                    } while (status != 0x00); // Chờ lấy ảnh thành công
                    status = PS_GenChar(1); // Tạo đặc trưng ở buffer 1
                    if (status != 0x00) {
                        ESP_LOGE(TAG, "PS_GENCHAR_1: Failed to generate character from image. Error: %d\n", status);
                    }

                    uint8_t x = PS_Search(1, 0, 160); 
                    if(x != 0){
                        currentstate = STATE_OPENDOOR;
                    }
                break;

                case STATE_IDLE_FINGERPRINT:
                break;

                case STATE_ADD_FINGERPRINT:
                    // printf("1.NEW USER        2.OLD USER");
                    
                    // char press_keypad_3 = 0;
                    // do{
                    //     press_keypad_3 = keypad_get_char();
                    //     vTaskDelay(500 / portTICK_PERIOD_MS);
                    // }while(press_keypad_3 == 0);
                    
                    // if(press_keypad_3 == '1'){
                    //     ESP_LOGW(TAG, "ADD NEW USER FINGER: ");
                    //     for(int i = 1; i <= MAX_USERS; i++){
                    //         if((USER[i].id == 0)){
                    //             if(USER[i].fingerprint_enable == 0){
                    //                 PS_Enroll(i);
                    //                 USER[i].id = i;
                    //                 USER[i].fingerprint_enable = 1;
                    //                 ESP_LOGI(TAG, "ADD FINGERPRINT SUCCESS !!");
                    //                 currentstate = STATE_IDLE;
                    //                 break;
                    //             }
                    //         }
                    //     }
                    // }
                    
                    // if(press_keypad_3 == '2'){
                    //     ESP_LOGW(TAG, "ADD OLD USER FINGER: ");
                    //     uint8_t p = choose_user();
                    //     if(USER[p].id == p){
                    //         PS_Enroll(p);
                    //         USER[p].fingerprint_enable = 1;
                    //         ESP_LOGI(TAG, "ADD FINGERPRINT SUCCESS !!");
                    //         currentstate = STATE_IDLE;
                    //         break;
                    //     }else{
                    //         ESP_LOGI(TAG, "USER NO EXIST !! ");
                    //         currentstate = STATE_IDLE;
                    //         break;
                    //     }
                    // }
                break;

                case STATE_DELETE_FINGERPRINT:
                    // uint8_t p = choose_user();
                    // if(USER[p].id == p){
                    //     PS_Delete(p, 1);
                    //     currentstate = STATE_IDLE;
                    // }else{
                    //     ESP_LOGI(TAG, "USER NO EXIST !! ");
                    //     currentstate = STATE_IDLE;
                    // }
                break;

                case STATE_OPENDOOR:
                    open_door();
                    vTaskDelay(1000/portTICK_PERIOD_MS);
                    currentstate = STATE_IDLE;
                break;

                case STATE_DISABLE:
                break;

                case STATE_SETTING:
                break;
            }

        }
        
    }else{

        printf("Did not find fingerprint sensor \n");

    }

    vTaskDelay(1000/portTICK_PERIOD_MS);

}

// void create_fingerprint_task(void){
//     xTaskCreate(fingerprint_task, "fingerprint task", 2048, NULL, 5, &fingerprint_task_handle);
// }

void func3(void){}