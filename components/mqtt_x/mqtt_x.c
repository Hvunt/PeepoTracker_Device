/**
 * @file mqtt_x.c
 * @author Sergiu Popov (sg.popov@pm.me)
 * @brief 
 * @version 0.1
 * @date 30-03-2023-
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mqtt_x.h"


static const char *TAG = "MQTT_SERVICE";
static TaskHandle_t taskHandle = NULL;
static QueueHandle_t incomingMessagesQueue = NULL;

static bool clientConnected = false;

static void getID(char *id, uint8_t length){
    // if(length < sizeof(CONFIG_TEST_DEVICE_ID)){
    //     return;
    // }
    memcpy(id, CONFIG_DEVICE_ID, length);
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    char topic_name[50] = "";

    sprintf(topic_name, "%s%s", MQTT_TOPICS_HEADER_NAME, CONFIG_DEVICE_ID);
    // getID(&topic_name[len], sizeof(CONFIG_TEST_DEVICE_ID));

    switch ((esp_mqtt_event_id_t) event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT client connected with a client_id: %s", CONFIG_DEVICE_ID);
        strcat(topic_name, MQTT_TOPICS_PRINT);
        ESP_LOGI(TAG, "Topic: %s", topic_name);
        
        esp_mqtt_client_subscribe(client, topic_name, 0);
        clientConnected = 1;
        break;
    case MQTT_EVENT_DATA:
        // test

        strcat(topic_name, MQTT_TOPICS_PRINT);
        if(strlen(topic_name) == event->topic_len){
            if(event->data_len <= MQTT_QUEUE_MAX_MESSAGE_LENGTH){
                if(memcmp(topic_name, event->topic, event->topic_len) == 0){
                    if(incomingMessagesQueue != NULL){
                        mqtt_x_message_t message = {0};
                        memcpy(message.buffer, event->data, event->data_len);
                        xQueueSend(incomingMessagesQueue, &message, 100);
                    }
                } else {
                    ESP_LOGI(TAG, "%s == %s", topic_name, event->topic);
                    // break;
                }
            } else {
                ESP_LOGI(TAG, "%d <= %d", event->data_len, MQTT_QUEUE_MAX_MESSAGE_LENGTH);
                // break;
            }
        } else {
            ESP_LOGI(TAG, "%d == %d", strlen(topic_name), event->topic_len);
            // break;
        }

        xTaskNotifyGive(taskHandle);
        break;
    case MQTT_EVENT_DISCONNECTED:
        clientConnected = 0;
        break;
    default:
        break;
    }
}

static void mqtt_handler_task(void * arg){
    (void) arg;
    while (1)
    {
        if(xTaskNotifyWait(0, ULONG_MAX, NULL, portMAX_DELAY ) == pdTRUE){
            while(uxQueueMessagesWaiting(incomingMessagesQueue) > 0)
            {
                mqtt_x_message_t message = {0};
                char text[MQTT_QUEUE_MAX_MESSAGE_LENGTH] = {0};
                if(xQueueReceive(incomingMessagesQueue, &message, 100) == pdTRUE){
                    ESP_LOGI(TAG, "Got message: %s", message.buffer);
                    sprintf(text, "%s", message.buffer);
                    Screen_Print(text);
                    vTaskDelay(100);
                }
            }
        }
    }
}

uint8_t is_mqtt_client_connected(void){
    return clientConnected;
}

void start_mqtt_connection_handler_task(void){
    incomingMessagesQueue = xQueueCreate(MQTT_QUEUE_MAX_MESSAGES, sizeof(mqtt_x_message_t));
    if (incomingMessagesQueue != NULL){

        char client_id[sizeof(CONFIG_DEVICE_ID)] = CONFIG_DEVICE_ID;
        char pass[sizeof(MQTT_PASS)] = MQTT_PASS;
        char username[sizeof(MQTT_USERNAME)] = MQTT_USERNAME;

        esp_mqtt_client_config_t cfg = {0};
        cfg.broker.address.hostname = MQTT_BROKER_ADDRESS;
        cfg.broker.address.port = MQTT_BROKER_PORT;
        cfg.broker.address.transport = MQTT_TRANSPORT_OVER_TCP;
        cfg.credentials.set_null_client_id = false;
        cfg.credentials.client_id = client_id;
        cfg.credentials.authentication.password = pass;
        cfg.credentials.username = username;
        esp_mqtt_client_handle_t client = esp_mqtt_client_init(&cfg);
        esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
        esp_mqtt_client_start(client);

        xTaskCreate(mqtt_handler_task, "mqtt_handler_task", configMINIMAL_STACK_SIZE*5, NULL, 3, &taskHandle);
    } else {
        ESP_LOGE(TAG, "Doesn't have enough memory to create a messages queue");
    }
}