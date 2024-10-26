#include <stdio.h>
#include <stdint.h>
#include "uart_lib.h"

void uart_init() {
    const uart_config_t uart_config = {
        .baud_rate = 57600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    // Cấu hình UART
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
}


// Hàm tính tổng kiểm tra
uint16_t calculate_checksum(uint8_t *packet, int length) {
    uint16_t checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum += packet[i];
    }
    return checksum;
}

// Hàm gửi gói lệnh
void send_command(uint8_t command, uint8_t *data, int data_len) {
    uint8_t packet[64];
    int index = 0;

    // Header
    packet[index++] = 0xEF;
    packet[index++] = 0x01;

    // Address (mặc định 4 byte 0xFFFFFFFF)
    for (int i = 0; i < 4; i++) {
        packet[index++] = 0xFF;
    }

    // Command
    packet[index++] = command;

    // Length
    packet[index++] = (data_len >> 8) & 0xFF;
    packet[index++] = data_len & 0xFF;

    // Data
    for (int i = 0; i < data_len; i++) {
        packet[index++] = data[i];
    }

    // Checksum
    uint16_t checksum = calculate_checksum(packet, index);
    packet[index++] = (checksum >> 8) & 0xFF;
    packet[index++] = checksum & 0xFF;

    // Gửi gói lệnh qua UART
    uart_write_bytes(UART_NUM, (const char *)packet, index);
}

void receive_response(uint8_t *response, int len) {
    int length = uart_read_bytes(UART_NUM, response, len, 1000 / portTICK_PERIOD_MS);
    if (length > 0) {
        printf("Received %d bytes: ", length);
        for (int i = 0; i < length; i++) {
            printf("%02X ", response[i]);
        }
        printf("\n");
    }
}


// Lấy ảnh vân tay từ cảm biến
uint8_t PS_GetImage() {
    send_command(0x01, NULL, 0); // Gửi lệnh lấy ảnh vân tay
    uint8_t response[64];
    receive_response(response, sizeof(response));
    return response[9]; // Mã phản hồi
}


// Tạo đặc trưng vân tay từ hình ảnh (buffer_id = 1 hoặc 2)
uint8_t PS_GenChar(uint8_t buffer_id) {
    uint8_t data[1] = {buffer_id};
    send_command(0x02, data, 1); // Gửi lệnh tạo đặc trưng vân tay
    uint8_t response[64];
    receive_response(response, sizeof(response));
    return response[9]; // Mã phản hồi
}


// Kết hợp hai đặc trưng vân tay thành mẫu
uint8_t PS_RegModel() {
    send_command(0x05, NULL, 0); // Gửi lệnh kết hợp đặc trưng vân tay
    uint8_t response[64];
    receive_response(response, sizeof(response));
    return response[9]; // Mã phản hồi
}



// Lưu mẫu vân tay vào bộ nhớ (page_id: vị trí lưu trữ)
uint8_t PS_Store(uint8_t buffer_id, uint16_t page_id) {
    uint8_t data[3];
    data[0] = buffer_id;
    data[1] = (page_id >> 8) & 0xFF;
    data[2] = page_id & 0xFF;
    send_command(0x06, data, 3); // Gửi lệnh lưu mẫu vân tay
    uint8_t response[64];
    receive_response(response, sizeof(response));
    return response[9]; // Mã phản hồi
}


// Hàm PS_Enroll: Đăng ký vân tay
void PS_Enroll(uint16_t page_id) {
    uint8_t status;
    printf("Place your finger on the sensor.\n");
    
    // Lần 1: Lấy ảnh và tạo đặc trưng
    do {
        status = PS_GetImage();
    } while (status != 0x00); // Chờ lấy ảnh thành công

    status = PS_GenChar(1); // Tạo đặc trưng ở buffer 1
    if (status != 0x00) {
        printf("Failed to generate character from image. Error: %d\n", status);
        return;
    }

    printf("Remove your finger and place it again.\n");
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Chờ người dùng bỏ tay ra

    // Lần 2: Lấy ảnh và tạo đặc trưng lần 2
    do {
        status = PS_GetImage();
    } while (status != 0x00); // Chờ lấy ảnh thành công

    status = PS_GenChar(2); // Tạo đặc trưng ở buffer 2
    if (status != 0x00) {
        printf("Failed to generate character from image. Error: %d\n", status);
        return;
    }

    // Kết hợp hai đặc trưng thành mẫu vân tay
    status = PS_RegModel();
    if (status != 0x00) {
        printf("Failed to combine fingerprint templates. Error: %d\n", status);
        return;
    }

    // Lưu mẫu vân tay vào bộ nhớ
    status = PS_Store(1, page_id); // Lưu mẫu ở buffer 1 vào vị trí page_id
    if (status == 0x00) {
        printf("Fingerprint enrolled successfully at position %d.\n", page_id);
    } else {
        printf("Failed to store fingerprint. Error: %d\n", status);
    }
}


void PS_Search(uint16_t start_page, uint16_t page_num) {
    uint8_t data[4];
    data[0] = (start_page >> 8) & 0xFF;
    data[1] = start_page & 0xFF;
    data[2] = (page_num >> 8) & 0xFF;
    data[3] = page_num & 0xFF;
    
    send_command(0x04, data, 4);
    
    uint8_t response[64];
    receive_response(response, sizeof(response));

    if (response[9] == 0x00) {
        printf("Fingerprint found at position: %d\n", (response[10] << 8) | response[11]);
    } else {
        printf("Fingerprint not found.\n");
    }
}


void PS_Store_1(uint8_t buffer_id, uint16_t page_id) {
    uint8_t data[3];
    data[0] = buffer_id;  // Buffer ID (1 hoặc 2)
    data[1] = (page_id >> 8) & 0xFF;  // Page ID
    data[2] = page_id & 0xFF;
    
    send_command(0x06, data, 3);
    
    uint8_t response[64];
    receive_response(response, sizeof(response));

    if (response[9] == 0x00) {
        printf("Fingerprint stored successfully.\n");
    } else {
        printf("Failed to store fingerprint.\n");
    }
}


void PS_Delete(uint16_t page_id, uint16_t num) {
    uint8_t data[4];
    data[0] = (page_id >> 8) & 0xFF;
    data[1] = page_id & 0xFF;
    data[2] = (num >> 8) & 0xFF;
    data[3] = num & 0xFF;
    
    send_command(0x0C, data, 4);
    
    uint8_t response[64];
    receive_response(response, sizeof(response));

    if (response[9] == 0x00) {
        printf("Fingerprint deleted successfully.\n");
    } else {
        printf("Failed to delete fingerprint.\n");
    }
}


void PS_Identify() {
    send_command(0x03, NULL, 0);
    
    uint8_t response[64];
    receive_response(response, sizeof(response));

    if (response[9] == 0x00) {
        printf("Fingerprint identified at position: %d\n", (response[10] << 8) | response[11]);
    } else {
        printf("Failed to identify fingerprint.\n");
    }
}

