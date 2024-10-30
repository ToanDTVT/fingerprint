#include <stdio.h>
#include "handle_fingerprint.h"





// void handle_input_fingerprint_idle(){

//     while(1){
//         uint8_t status;
//         printf("Place your finger on the sensor.\n");
        
//         // Lấy ảnh và tạo đặc trưng
//         do {
//             status = PS_GetImage();
//         } while (status != 0x00); // Chờ lấy ảnh thành công

//         status = PS_GenChar(1); // Tạo đặc trưng ở buffer 1
//         if (status != 0x00) {
//             printf("Failed to generate character from image. Error: %d\n", status);
//         }

//         PS_Search(1, 0, 160);

//     }

// }



// void fingerprint_task(){

//     while(1){

//         if(verify_password_of_AS608() == true){

//             switch(currentstate){
            
//                 case STATE_IDLE_FINGERPRINT:
//                     handle_input_fingerprint_idle();
//                 break;

//                 case STATE_ADD_FINGERPRINT:
                    
//                 break;

//                 case STATE_DELETE_FINGERPRINT:
//                 break;

//                 case STATE_OPENDOOR:
//                     open_door();
//                     vTaskDelay(1000/portTICK_PERIOD_MS);
//                     currentstate = STATE_IDLE;
//                 break;

//             }

//         }else{

//             printf("Did not find fingerprint sensor \n");

//         }

//         vTaskDelay(1000/portTICK_PERIOD_MS);

//     }

// }

void func3(void){}