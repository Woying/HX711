#ifndef __DS18B20_H
#define __DS18B20_H 

#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include "delay.h"


#define uint8_t unsigned char
#define uint32_t unsigned int


#define DQ_Pin GPIO_PIN_6
#define DQ_GPIO_Port GPIOA
#define  DS18B20_DQ_OUT_HIGH       HAL_GPIO_WritePin(DQ_GPIO_Port, DQ_Pin, GPIO_PIN_SET)
#define  DS18B20_DQ_OUT_LOW        HAL_GPIO_WritePin(DQ_GPIO_Port, DQ_Pin, GPIO_PIN_RESET)
#define  DS18B20_DQ_IN             HAL_GPIO_ReadPin(DQ_GPIO_Port, DQ_Pin)


void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);
void DS18B20_Rst(void);
uint8_t DS18B20_Check(void);
void DS18B20_Write_Byte(uint8_t data);
uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Start(void);
uint8_t DS18B20_Init(void);
short DS18B20_Get_Temperature(void);



#endif