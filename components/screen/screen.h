/**
 * @file screen.h
 * @author Sergiu Popov (sg.popov@pm.me)
 * @brief 
 * @version 0.1
 * @date 31-03-2023-
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <string.h>
#include <stdint.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "GUI_Paint.h"
#include "ImageData.h"
#include "OLED_0in91.h"
#include "esp_log.h"
#include "mqtt_x.h"

#define SCREEN_CONNECTION_STATE_WIDTH       12
#define SCREEN_CONNECTION_STATE_HEIGHT      16
#define SCREEN_CONNECTION_STATE_COORD_X     OLED_0in91_WIDTH - SCREEN_CONNECTION_STATE_WIDTH - 1
#define SCREEN_CONNECTION_STATE_COORD_Y     1


#define SCREEN_BATTERY_COORD_X              95
#define SCREEN_BATTERY_COORD_Y              3
#define SCREEN_BATTERY_WIDTH                14
#define SCREEN_BATTERY_HEIGHT               8

#define SCREEN_TIME_COORD_X                 00
#define SCREEN_TIME_COORD_Y                 00

#define SCREEN_STATUS_DIVIDER_LINE_X        1
#define SCREEN_STATUS_DIVIDER_LINE_Y        14
#define SCREEN_STATUS_DIVIDER_LINE_END_X    OLED_0in91_WIDTH

#define SCREEN_MAIN_STRING_COORD_X          0
#define SCREEN_MAIN_STRING_COORD_Y          20
#define SCREEN_MAX_STRING_LENGTH            100

typedef struct 
{
    char main_string[SCREEN_MAX_STRING_LENGTH];
    uint8_t battery;
    uint8_t connection_state;
} screen_t;

static const uint32_t battery_0[] = {
    0b01111110,
    0b01000010,
    0b01000010,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111
};

// C-array for 50% battery state
static const uint32_t battery_50[] = {
    0b01111110,
    0b01000010,
    0b01000010,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b11111111
};

// C-array for 100% battery state
static const uint32_t battery_100[] = {
    0b01111110,
    0b01000010,
    0b01000010,
    0b10100101,
    0b10100101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b10111101,
    0b11111111
};

static const uint32_t connection[] = {
    0b11111111,
    0b01111111,
    0b00111111,
    0b00011111,
    0b00001111,
};

static const uint32_t mqtt_conn_symbol[] = {
    0b0000111111110000,
    0b0001100110011000,
    0b0011000110001100,
    0b0110000110000110,
    0b1100000110000011,
    0b1111111111111111,
    0b1111111111111111,
    0b1100000110000011,
    0b0110000110000110,
    0b0011000110001100,
    0b0001100110011000,
    0b0000111111110000
    // 0b00111100,
    // 0b01000010,
    // 0b10011001,
    // 0b10000001,
    // 0b10011001,
    // 0b01000010,
    // 0b00111100,
    // 0b00000000
};

static const uint32_t mqtt_disconn_symbol[] = {
    0b0000111111100000,
    0b0001100000011000,
    0b0011000000001100,
    0b0110000000000110,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b1100000000000011,
    0b0110000000000110,
    0b0011000000001100,
    0b0001100000011000,
    0b0000111111100000
    // 0b00000000,
    // 0b00111100,
    // 0b01000010,
    // 0b10100101,
    // 0b10000001,
    // 0b10100101,
    // 0b01000010,
    // 0b00111100
};

int8_t  Screen_Init(void);
void    Screen_Print(char *text);

#endif // __SCREEN_H__