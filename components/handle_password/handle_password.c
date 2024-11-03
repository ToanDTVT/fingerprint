#include <stdio.h>
#include "handle_password.h"


 #define TAG "password"

// char correct_password[SIZE_OF_PASSWORD] = {1, '1', '1', '1', '1'};
// char enter_password[SIZE_OF_PASSWORD];
// int number_fail = 0;

// TaskHandle_t password_task_handle = NULL;


int check_pass_open_door(char *enter_password){
    int i;
    for(i = 0; i <= MAX_USERS; i++){
        if((strcmp(enter_password, correct_password) == 0) || (strcmp(enter_password, USER[i].user_password) == 0)){
            printf("=========================================== \n");
            printf("SUCCESSFUL!! OPEN THE DOOR! \n");
            printf("id: %d \n", USER[i].id);
            printf("=========================================== \n");
            return 1;    //Mat khau dung
        }
    }
    printf("=========================================== \n");
    printf("FAILURE!! TRY AGAIN! \n");
    printf("=========================================== \n");
    return 0;   //Mat khau sai
}



void handle_input_password_idle(char *enter_password){

    int x = 1;
    char press_keypad;
    enter_password[0] = 1;
    printf("Password: \n");

    while(x < SIZE_OF_PASSWORD){
        press_keypad = keypad_get_char();
        if((press_keypad != 0) && (press_keypad != '*') && (press_keypad != '#')){
            printf("%c \n", press_keypad);
            enter_password[x] = press_keypad;
            press_keypad = 0;
            x++;
        }
        if(press_keypad == '*'){
            currentstate = STATE_SETTING;
            vTaskDelay(300/portTICK_PERIOD_MS);
            break;
        }
        vTaskDelay(300/portTICK_PERIOD_MS);
    }
    printf("\n");

    if(check_pass_open_door(enter_password) == 1){
        currentstate = STATE_OPENDOOR;
        number_fail = 0;
    }else number_fail++;

}


int check_password(char *enter_password, char press_keypad_2){

    if(press_keypad_2 == '1'){
        int i;
        for(i = 1; i <= MAX_USERS; i++){
            if(USER[i].id == 0){
                for(int z = 0; z < SIZE_OF_PASSWORD; z++){USER[i].user_password[z] = enter_password[z];}
                USER[i].id = i;                           //người sử dụng thứ i
                printf("=========================================== \n");
                printf("HAVE SLOT!! ADD SUCCESS \n");
                printf("=========================================== \n");
                return 1;                                 //còn chỗ trống
            }
        }
        printf("=========================================== \n");
        printf("FULL SLOT!! TRY AGAIN! \n");
        printf("=========================================== \n");
        return 2;                                         //hết chỗ trống
    }
    
    if(press_keypad_2 == '2'){
        int i;
        for(i = 1; i <= MAX_USERS; i++){
            if((USER[i].id != 0) && (strcmp(enter_password, USER[i].user_password) == 0)){
                memset(USER[i].user_password, 0, sizeof(USER[i].user_password));
                USER[i].id = 0;                                                                  //người sử dụng thứ i
                printf("=========================================== \n");
                printf("DELETE SUCCESS \n");
                printf("=========================================== \n");
                return 3;                                                                        //xóa thành công
            }
        }
        printf("=========================================== \n");
        printf("PASSWORD NO EXIST \n");
        printf("=========================================== \n");
        return 4;                                                                                //Không thành công 
    }

    return 0;
}


void handle_setting_password(char *enter_password){
    
    // printf("=========================================== \n");
    // printf("          SETTING PASSWORD: \n");
    // printf("1. ADD PASSWORD      2. DELETE PASSWORD \n");
    // printf("=========================================== \n");
    ESP_LOGI(TAG, "SETTING PASSWORD: 1.ADD PASSWORD         2.DELETE PASSWORD");
    char press_keypad_2;
    while(1){
        press_keypad_2 = keypad_get_char();
        if(press_keypad_2 == '1'){ printf("ADD PASSWORD: \n"); break;}
        if(press_keypad_2 == '2'){ printf("DELETE PASSWORD: \n"); break;}
        vTaskDelay(500/portTICK_PERIOD_MS);
    }

    vTaskDelay(500/portTICK_PERIOD_MS);

    int i = 1;
    char press_keypad_3; 
    memset(enter_password, 0, sizeof(enter_password));
    enter_password[0] = 1;
    while(i < SIZE_OF_PASSWORD){
        press_keypad_3 = keypad_get_char();
        if(press_keypad_3 != 0){
            printf("%c \n", press_keypad_3);
            enter_password[i] = press_keypad_3;
            press_keypad_3 = 0;
            i++;
        }
        vTaskDelay(300/portTICK_PERIOD_MS);
    }
    printf("\n");
    if((check_password(enter_password, press_keypad_2) == 1) || (check_password(enter_password, press_keypad_2) == 3)){
        currentstate = STATE_IDLE;
    }

}


void password_task (void * pvParameters){

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
                        printf("          SETTING FINGERPRINT: \n");
                        printf("1. ADD FINGERPRINT      2. DELETE FINGERPRINT \n");
                        printf("=========================================== \n");

                        char press_keypad_2 = 0;
                        while(1){
                            press_keypad_2 = keypad_get_char();
                            if(press_keypad_2 == '1'){
                                printf("ADD FINGERPRINT: \n");
                                currentstate = STATE_ADD_FINGERPRINT;
                                break;
                            }
                            if(press_keypad_2 == '2'){ 
                                printf("DELETE FINGERPRINT: \n"); 
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

}

// void create_password_task(void){
//     xTaskCreate(password_task, "password task", 2048, NULL, 3, &password_task_handle);
// }


void func2(void)
{

}
