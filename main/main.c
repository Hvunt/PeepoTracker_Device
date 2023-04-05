/**
 * @file main.c
 * @author Sergiu Popov (sg.popov@pm.me)
 * @brief 
 * @version 0.1
 * @date 30-03-2023-
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <sys/time.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_sntp.h"

#include "wifi_connection.h"
#include "mqtt_x.h"
#include "screen.h"

static const char *TAG = "APP_MAIN";

void syncronize_time(void)
{    
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    
    // set east european time
    setenv("TZ", "EET-2EEST,M3.5.0,M10.5.0/3", 1);
    tzset();
    sntp_sync_status_t state = sntp_get_sync_status();
    for (uint8_t i = 0; i < 60; i++)
    {
        state = sntp_get_sync_status();
        if (state == SNTP_SYNC_STATUS_COMPLETED){
            ESP_LOGI(TAG, "SNTP configuration is done");
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    ESP_LOGI(TAG, "SNTP doesn't completed. Last state: %d", (uint8_t) state);
}

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_LOGI(TAG, "Screen Init: %d", Screen_Init());
    while(!wifi_init_sta()){
        vTaskDelay(1);
    }
	start_mqtt_connection_handler_task();
    syncronize_time();
}
