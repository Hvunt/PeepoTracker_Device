/******************************************************************************
**************************Hardware interface layer*****************************
* | file      		:	DEV_Config.c
* |	version			:	V1.0
* | date			:	2020-06-17
* | function		:	Provide the hardware underlying interface	
******************************************************************************/
#include "DEV_Config.h"
#include <stdio.h>		//printf()
#include <string.h>
#include <stdlib.h>

/********************************************************************************
function:	System Init
note:
	Initialize the communication method
********************************************************************************/
uint8_t System_Init(void)
{
#if USE_SPI_4W
	printf("USE_SPI_4W\r\n");
#elif USE_IIC
	printf("USE_IIC\r\n");
	i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_PIN,
        .scl_io_num = I2C_MASTER_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
#elif USE_IIC_SOFT
	printf("USEI_IIC_SOFT\r\n");
	OLED_CS_0;
	OLED_DC_0;
#endif
  return 0;
}

void System_Exit(void)
{

}
/********************************************************************************
function:	Hardware interface
note:
	SPI4W_Write_Byte(value) : 
		HAL library hardware SPI
		Register hardware SPI
		Gpio analog SPI
	I2C_Write_Byte(value, cmd):
		HAL library hardware I2C
********************************************************************************/
uint8_t SPI4W_Write_Byte(uint8_t value)
{
#if 0
    HAL_SPI_Transmit(&hspi1, &value, 1, 500);
#elif 0
    char i;
    for(i = 0; i < 8; i++) {
        SPI_SCK_0;
        if(value & 0X80)
            SPI_MOSI_1;
        else
            SPI_MOSI_0;
        Driver_Delay_us(10);
        SPI_SCK_1;
        Driver_Delay_us(10);
        value = (value << 1);
    }
#elif 0
    __HAL_SPI_ENABLE(&hspi1);
    SPI1->CR2 |= (1) << 12;

    while((SPI1->SR & (1 << 1)) == 0)
        ;

    *((__IO uint8_t *)(&SPI1->DR)) = value;

    while(SPI1->SR & (1 << 7)) ; //Wait for not busy

    while((SPI1->SR & (1 << 0)) == 0) ; // Wait for the receiving area to be empty

    return *((__IO uint8_t *)(&SPI1->DR));
#else 
    return 0;
#endif
}

void I2C_Write_Byte(uint8_t value, uint8_t Cmd)
{
    uint8_t W_Buf[2] = {Cmd, value};
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, I2C_DEVICE_ADDRESS, W_Buf, 2, 1000));
}

/********************************************************************************
function:	Delay function
note:
	Driver_Delay_ms(xms) : Delay x ms
	Driver_Delay_us(xus) : Delay x us
********************************************************************************/
void Driver_Delay_ms(uint32_t xms)
{
    vTaskDelay(xms / portTICK_PERIOD_MS);
}

void Driver_Delay_us(uint32_t xus)
{
    for(int j=xus; j > 0; j--);
}
