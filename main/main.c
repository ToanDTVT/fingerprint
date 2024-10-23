#include <stdio.h>
#include "uart_lib.h"

void app_main(void)
{
    uart_init();

    // // Ví dụ gửi lệnh PS_Enroll (0x01)
    // uint8_t enroll_data[] = {0x01, 0x00, 0x00};
    // send_command(0x01, enroll_data, sizeof(enroll_data));

    // // Nhận phản hồi từ cảm biến
    // uint8_t response[64];
    // receive_response(response, sizeof(response));

    
    PS_Enroll(1);
    
}
