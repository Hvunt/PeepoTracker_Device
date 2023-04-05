/**
 * @file screen.c
 * @author Sergiu Popov (sg.popov@pm.me)
 * @brief 
 * @version 0.1
 * @date 31-03-2023-
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "screen.h"

static const char* TAG = "SCREEN";

static UBYTE *BlackImage = NULL;
static screen_t screen = {0};
SemaphoreHandle_t screenSemaph = NULL;
static TaskHandle_t taskHandle = NULL;

static void screen_task(void *prv);
static void draw_connection_state(void);
static void draw_battery_state(void);
static void draw_time(void);
static void draw_symbol(uint32_t *image, uint8_t width, uint8_t height, uint8_t x_start, uint8_t y_start);

static void timerExpired_cb(void);

static void screen_task(void *prv){
    (void) prv;
    while (1)
    {
        if(screenSemaph != NULL){
            if(xSemaphoreTake(screenSemaph, 100 / portTICK_PERIOD_MS) == pdTRUE){
                Paint_Clear(BLACK);

                draw_connection_state();
                draw_battery_state();
                draw_time();

                Paint_DrawLine(
                    SCREEN_STATUS_DIVIDER_LINE_X, SCREEN_STATUS_DIVIDER_LINE_Y, 
                    SCREEN_STATUS_DIVIDER_LINE_END_X, SCREEN_STATUS_DIVIDER_LINE_Y, 
                    WHITE, 1, LINE_STYLE_SOLID
                );
                Paint_DrawString_EN(SCREEN_MAIN_STRING_COORD_X, SCREEN_MAIN_STRING_COORD_Y, screen.main_string, &Font12, WHITE, WHITE);

                OLED_0in91_Display(BlackImage);
                xSemaphoreGive(screenSemaph);
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void draw_connection_state(void)
{
    // uint32_t *image = (uint32_t*) mqtt_disconn_symbol;
    char symbol = '!';
    if(is_mqtt_client_connected()){
        // image = (uint32_t*) mqtt_conn_symbol;
        symbol = '#';
    }
    Paint_DrawCircle(SCREEN_CONNECTION_STATE_COORD_X+5, SCREEN_CONNECTION_STATE_COORD_Y+5, 5, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawChar(SCREEN_CONNECTION_STATE_COORD_X+2, SCREEN_CONNECTION_STATE_COORD_Y+1, symbol, &Font8, WHITE, WHITE);

    // draw_symbol(image,  SCREEN_CONNECTION_STATE_WIDTH, SCREEN_CONNECTION_STATE_HEIGHT, 
    //                     SCREEN_CONNECTION_STATE_COORD_X, SCREEN_CONNECTION_STATE_COORD_Y);
}

static void draw_battery_state(void)
{
    uint32_t *image = (uint32_t*) battery_0;
    if (screen.battery > 81){
        image = (uint32_t*) battery_100;
    } else if((screen.battery < 80) && (screen.battery > 11)){
        image = (uint32_t*) battery_50;
    }
    draw_symbol(image, SCREEN_BATTERY_WIDTH, SCREEN_BATTERY_HEIGHT, SCREEN_BATTERY_COORD_X, SCREEN_BATTERY_COORD_Y);
}

static void draw_time(void)
{
    time_t now;
    struct tm timeinfo;
    PAINT_TIME pt = {0};

    time(&now);
    localtime_r(&now, &timeinfo);
    // strftime(strbuf, sizeof(strbuf), "%c", &timeinfo);
    pt.Year = timeinfo.tm_year;
    pt.Month = timeinfo.tm_mon;
    pt.Day = timeinfo.tm_mday;
    pt.Hour = timeinfo.tm_hour;
    pt.Min = timeinfo.tm_min;
    pt.Sec = timeinfo.tm_sec;
    Paint_DrawTime(SCREEN_TIME_COORD_X, SCREEN_TIME_COORD_Y, &pt, &Font12, WHITE, WHITE);
}

static void draw_symbol(uint32_t *image, uint8_t width, uint8_t height, uint8_t x_start, uint8_t y_start)
{
    uint8_t pix_state = 0;
    for (int8_t row = height; row > -1; row--){
        for (int8_t col = 0; col < width; col++)
        {
            pix_state = 0x01 & (image[col] >> row);
            pix_state = pix_state ? WHITE : BLACK;
            Paint_DrawPoint(
                x_start + col,
                y_start + row,
                pix_state, DOT_PIXEL_1X1, DOT_STYLE_DFT
            );
        }
    }
}

static void timerExpired_cb(void)
{
    memset(screen.main_string, 0, SCREEN_MAX_STRING_LENGTH);
}

int8_t Screen_Init(void)
{
    if(System_Init() != 0) {
        return -1;
    }
    
    OLED_0in91_Init();
    Driver_Delay_ms(500);	
    OLED_0in91_Clear();

    UWORD Imagesize = ((OLED_0in91_WIDTH%8==0)? (OLED_0in91_WIDTH/8): (OLED_0in91_WIDTH/8+1)) * OLED_0in91_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        // printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    // printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, OLED_0in91_HEIGHT, OLED_0in91_WIDTH, 90, BLACK);	
    
    // printf("Drawing\r\n");
    Paint_SelectImage(BlackImage);
    Driver_Delay_ms(500);
    Paint_Clear(BLACK);

    screenSemaph = xSemaphoreCreateBinary();
    if(screenSemaph != NULL){
        xSemaphoreGive(screenSemaph);
        if(xTaskCreate(screen_task, "screen_task", configMINIMAL_STACK_SIZE*4, NULL, 5, &taskHandle) != pdPASS){
            ESP_LOGE(TAG, "Failed to create the screen task");
            return -1;
        }
    } else {
        ESP_LOGE(TAG, "Failed to create the screen semaphore");
        return -1;
    }
    return 0;
}

void Screen_Print(char *text){
    if(xSemaphoreTake(screenSemaph, 1000/portTICK_PERIOD_MS) == pdTRUE){
        strcpy(screen.main_string, text);
        // ESP_LOGI(TAG, "screen string: %s", screen.main_string);
        xSemaphoreGive(screenSemaph);
    } else {
        ESP_LOGE(TAG, "Can't obtain the semaphore");
    }
}
