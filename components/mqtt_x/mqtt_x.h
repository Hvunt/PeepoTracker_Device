/**
 * @file mqtt_x.h
 * @author Sergiu Popov (sg.popov@pm.me)
 * @brief 
 * @version 0.1
 * @date 30-03-2023-
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __MQTT_X_H__
#define __MQTT_X_H__

#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_system.h"
#include <string.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "screen.h"

// topics names defines. Have structure as devices/{deviceID}/{topic_name}
#define MQTT_TOPICS_HEADER_NAME     "devices/"
// subscribed to receive data to display on the OLED
#define MQTT_TOPICS_PRINT           "/print"
// the topic is used to publish data
#define MQTT_TOPICS_DATA            "/data"
// the topic is used to publish status values
#define MQTT_TOPICS_STATUS          "/status"


#define MQTT_BROKER_ADDRESS         CONFIG_MQTT_BROKER_URL
#define MQTT_BROKER_PORT            CONFIG_MQTT_BROKER_PORT
#define MQTT_PASS                   CONFIG_MQTT_BROKER_PASSWORD
#define MQTT_USERNAME               CONFIG_MQTT_BROKER_USERNAME

// #define MQTT_BROKER_ADDRESS         "70.34.252.151"
// #define MQTT_BROKER_PORT            1883
// #define MQTT_PASS                   "890peepodevice123"
// #define MQTT_USERNAME               "device"
// how many bytes per one message
#define MQTT_QUEUE_MAX_MESSAGE_LENGTH   100

#define MQTT_QUEUE_MAX_MESSAGES         10

typedef struct mqtt_x_message
{
    char buffer[MQTT_QUEUE_MAX_MESSAGE_LENGTH];
} mqtt_x_message_t;


void start_mqtt_connection_handler_task(void);
uint8_t is_mqtt_client_connected(void);
#endif // __MQTT_X_H__