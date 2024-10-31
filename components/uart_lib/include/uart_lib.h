
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

uint8_t PS_GetImage();
uint8_t PS_GenChar(uint8_t buffer_id);
uint8_t PS_RegModel();

uint8_t PS_Store(uint8_t buffer_id, uint16_t page_id);
void PS_UpChar(uint8_t buffer_id, uint8_t *buffer_data);
void PS_DownChar(uint8_t buffer_id, uint8_t *buffer_data);

void PS_Enroll(uint16_t page_id);
uint8_t PS_Search(uint8_t buffer_id, uint16_t start_page, uint16_t page_num);
void PS_Delete(uint16_t page_id, uint16_t num);
void PS_Identify();
bool verify_password_of_AS608();