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
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
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
