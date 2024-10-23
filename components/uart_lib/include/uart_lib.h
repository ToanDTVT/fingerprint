
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_compiler.h"
#include "esp_log.h"
#include "driver/dedic_gpio.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_rom_sys.h"

#define UART_NUM UART_NUM_1
#define BUF_SIZE 1024
#define RD_BUF_SIZE (BUF_SIZE)
#define TX_PIN GPIO_NUM_17
#define RX_PIN GPIO_NUM_16

void uart_init();
uint16_t calculate_checksum(uint8_t *packet, int length);
void send_command(uint8_t command, uint8_t *data, int data_len);
void receive_response(uint8_t *response, int len);
