/******************************************************************************
**************************Hardware interface layer*****************************
* | file      		:	DEV_Config.c
* |	version			:	V1.0
* | date			:	2020-06-17
* | function		:	Provide the hardware underlying interface	
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <stdint.h>
#include <stdlib.h>
#include "esp_system.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define USE_SPI_4W 		        0
#define USE_IIC 		        1
#define USE_IIC_SOFT	        0

#define IIC_CMD		            0X00
#define IIC_RAM		            0X40

#define I2C_MASTER_NUM                  0
#define I2C_MASTER_FREQ_HZ              400000
#define I2C_MASTER_SCL_PIN              22
#define I2C_MASTER_SDA_PIN              21
#define I2C_MASTER_TX_BUF_DISABLE       0
#define I2C_MASTER_RX_BUF_DISABLE       0
#define I2C_DEVICE_ADDRESS              0x3C

#define OLED_RST_1                      do{}while(0)
#define OLED_RST_0                      do{}while(0)


/*------------------------------------------------------------------------------------------------------*/

UBYTE 	System_Init(void);
void    System_Exit(void);

UBYTE 	SPI4W_Write_Byte(UBYTE value);
void 	I2C_Write_Byte(UBYTE value, UBYTE Cmd);

void Driver_Delay_ms(uint32_t xms);
void Driver_Delay_us(uint32_t xus);

#endif
