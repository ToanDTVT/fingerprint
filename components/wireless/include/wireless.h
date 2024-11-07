#ifndef WIRELESS_H
#define WIRELESS_H


#include "gpio_lib.h"
#include "user_data.h"
#include "mqtt_client.h"
#include "cJSON.h"

extern DataPacket USER[MAX_USERS];
extern Data_send_server data;
extern Type_open_door action;

void mqtt_app_start();
void task_wifi_init();
//void create_task_wireless(void);
void func4(void);

#endif // WIRELESS_H