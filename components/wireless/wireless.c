#include "wireless.h"


//TaskHandle_t wireless_task = NULL;

void task_wifi_init(){

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    cfg.nvs_enable = true;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // Xóa và khởi tạo lại NVS nếu gặp lỗi thiếu trang hoặc phiên bản NVS cũ
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t sta_cfg = {
        .sta = {
            .ssid = "Theshowcoffee T2",                    //BaPhuQuy
            .password = "Icanfly9",                        //BaTrpTNMT62
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_ERROR_CHECK(esp_wifi_connect());

}



/*=============================================================================================================*/
//{clientId:"3112",userName:"esp32",password:"Toan"}
esp_mqtt_client_config_t mqtt_cfg = {
    .broker = {
        .address = {
            .hostname = "demo.thingsboard.io",    // Set hostname only
            .port = 1883,                         // Default MQTT port
            .transport = MQTT_TRANSPORT_OVER_TCP,      // Set transport type, or remove this line
        },
        .verification = {
            .use_global_ca_store = false,
        },
    },
    .credentials = {
        .username = "esp32",
        .client_id = "9073d5b0-9b89-11ef-af67-a38a7671daf5",
        .set_null_client_id = false,
        .authentication = {
            .password = "Toan",
        },
    },
    .session = {
        .keepalive = 120,
        .disable_clean_session = false,
    },
    .network = {
        .reconnect_timeout_ms = 10000,
        .timeout_ms = 10000,
        .disable_auto_reconnect = false,
    },
    .task = {
        .priority = 5,
        .stack_size = 4096,
    }, 
    .buffer = {
        .size = 1024,
    },
    .outbox = {
        .limit = 2048,
    },
};


/*=========================================================================================================*/
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI("MQTT", "MQTT_EVENT_CONNECTED");

            //if(action.pass_open_door == true){
                // Gửi JSON data lên ThingsBoard trên topic "v1/devices/me/telemetry"
                char json_data[100];
                snprintf(json_data, sizeof(json_data), "{\"user\": %d, \"type\": %s}", data.user, data.type);
                int msg_id = esp_mqtt_client_publish(client, "v1/devices/me/telemetry", json_data, 0, 1, 0);
                ESP_LOGI("MQTT", "Sent telemetry data, msg_id=%d", msg_id);
                action.pass_open_door = false;
            //}

            //esp_mqtt_client_subscribe
            esp_mqtt_client_subscribe(event->client, "v1/devices/me/rpc/request/+", 0);
            ESP_LOGI("MQTT", "Subscribed to RPC requests");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI("MQTT", "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            // cJSON *json_data = cJSON_ParseWithLength(event->data, event->data_len);
            // if (json_data == NULL) {
            //     ESP_LOGE("MQTT", "Failed to parse JSON data");
            //     return;
            // }

            // cJSON *params = cJSON_GetObjectItem(json_data, "params");
            // if (params != NULL) {
            //     // Check the value of params and control GPIO13
            //     if (cJSON_IsBool(params)) {
            //         if (cJSON_IsTrue(params)) {
            //             // Enable GPIO13
            //             gpio_set_level(RELAY, 1);
            //             ESP_LOGI("GPIO", "GPIO13 enabled");
            //         } else {
            //             // Disable GPIO13
            //             gpio_set_level(RELAY, 0);
            //             ESP_LOGI("GPIO", "GPIO13 disabled");
            //         }
            //     }
            // } else {
            //     ESP_LOGE("MQTT", "No params field in JSON data");
            // }

            // cJSON_Delete(json_data); // Clean up JSON object



            
            // Kiểm tra method cho RELAY
            if (strstr(event->data, "\"method\":\"getValue\"") != NULL) {
                if (strstr(event->data, "\"params\":true") != NULL) {
                    gpio_set_level(RELAY, 1);  // Bật LED1
                } else if (strstr(event->data, "\"params\":false") != NULL) {
                    gpio_set_level(RELAY, 0);  // Tắt LED1
                }
            }
            // Kiểm tra method cho USER1
            if (strstr(event->data, "\"method\":\"setUSER1\"") != NULL) {
                if (strstr(event->data, "\"params\":true") != NULL) {
                    //gpio_set_level(GPIO_PIN, 1);  // Bật LED1
                    USER[1].en = 1;
                } else if (strstr(event->data, "\"params\":false") != NULL) {
                    //gpio_set_level(GPIO_PIN, 0);  // Tắt LED1
                    USER[1].en = 0;
                }
            }
            // Kiểm tra method cho USER2
            else if (strstr(event->data, "\"method\":\"setUSER2\"") != NULL) {
                if (strstr(event->data, "\"params\":true") != NULL) {
                    //gpio_set_level(GPIO_PIN, 1);  // Bật LED2
                    USER[2].en = 1;
                } else if (strstr(event->data, "\"params\":false") != NULL) {
                    //gpio_set_level(GPIO_PIN, 0);  // Tắt LED2
                    USER[2].en = 0;
                }
            }

            break;
        default:
            ESP_LOGI("MQTT", "Event ID: %d", event->event_id);
            break;
    }
}


void mqtt_app_start(){
    
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


// void send_data_via_mqtt(char *data){
//     const char* topic = "user in";
//     esp_mqtt_client_publish(client, topic, data, 0, 1, 0);
// }