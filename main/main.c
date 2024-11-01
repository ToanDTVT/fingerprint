#include <stdio.h>
#include "uart_lib.h"
#include "gpio_lib.h"
#include "user_data.h"
#include "handle_password.h"
#include "handle_fingerprint.h"
#include "wireless.h"


#define SSID "Theshowcoffee T1"
#define PASS "Icanfly9"


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

    func4();

    currentstate = STATE_IDLE;
    
    

    while(1){

        switch(currentstate){

            case STATE_IDLE:
                memset(enter_password, 0, sizeof(enter_password));
                handle_input_password_idle(enter_password);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            break;

            case STATE_SETTING:
                printf("=========================================== \n");
                printf("         SETTING: \n");
                printf("1. PASSWORD      2.FINGERPRINT \n");
                printf("=========================================== \n");
                while(1){
                    char press_keypad_1 = keypad_get_char();
                    if(press_keypad_1 == '1'){
                        handle_setting_password(enter_password);
                        break;
                    }
                    if(press_keypad_1 == '2'){
                        printf("=========================================== \n");
                        printf("          SETTING PASSWORD: \n");
                        printf("1. ADD PASSWORD      2. DELETE PASSWORD \n");
                        printf("=========================================== \n");

                        char press_keypad_2 = 0;
                        while(1){
                            press_keypad_2 = keypad_get_char();
                            if(press_keypad_2 == '1'){
                                printf("ADD PASSWORD: \n");
                                currentstate = STATE_ADD_FINGERPRINT;
                                break;
                            }
                            if(press_keypad_2 == '2'){ 
                                printf("DELETE PASSWORD: \n"); 
                                currentstate = STATE_DELETE_FINGERPRINT; 
                                break;
                            }
                            vTaskDelay(500/portTICK_PERIOD_MS);
                        }

                        vTaskDelay(100/portTICK_PERIOD_MS);
                    }
                    vTaskDelay(100/portTICK_PERIOD_MS);
                }
            break;

            case STATE_DISABLE:
            break;

            case STATE_IDLE_FINGERPRINT:
            break;

            case STATE_ADD_FINGERPRINT:
                printf("1.NEW USER        2.OLD USER");
                
                char press_keypad_3 = 0;
                do{
                    press_keypad_3 = keypad_get_char();
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                }while(press_keypad_3 == 0);
                
                if(press_keypad_3 == '1'){
                    ESP_LOGW(TAG, "ADD NEW USER FINGER: ");
                    for(int i = 1; i <= MAX_USERS; i++){
                        if((USER[i].id == 0)){
                            if(USER[i].fingerprint_enable == 0){
                                PS_Enroll(i);
                                USER[i].id = i;
                                USER[i].fingerprint_enable = 1;
                                ESP_LOGI(TAG, "ADD FINGERPRINT SUCCESS !!");
                                currentstate = STATE_IDLE;
                                break;
                            }
                        }
                    }
                }
                
                if(press_keypad_3 == '2'){
                    ESP_LOGW(TAG, "ADD OLD USER FINGER: ");
                    uint8_t p = choose_user();
                    if(USER[p].id == p){
                        PS_Enroll(p);
                        USER[p].fingerprint_enable = 1;
                        ESP_LOGI(TAG, "ADD FINGERPRINT SUCCESS !!");
                        currentstate = STATE_IDLE;
                        break;
                    }else{
                        ESP_LOGI(TAG, "USER NO EXIST !! ");
                        currentstate = STATE_IDLE;
                        break;
                    }
                }
            break;

            case STATE_DELETE_FINGERPRINT:
                uint8_t p = choose_user();
                if(USER[p].id == p){
                    PS_Delete(p, 1);
                    currentstate = STATE_IDLE;
                }else{
                    ESP_LOGI(TAG, "USER NO EXIST !! ");
                    currentstate = STATE_IDLE;
                }
            break;

            case STATE_OPENDOOR:
                open_door();
                vTaskDelay(1000/portTICK_PERIOD_MS);
                currentstate = STATE_IDLE;
            break;
        }

        

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


    // create_password_task();
    // create_fingerprint_task();

}
