#ifndef __HX711_H
#define __HX711_H

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "delay.h"
#include "math.h"
#include "Kalman.h"
#include "DS18B20.h"

#define HX711_SCK_Pin GPIO_PIN_0
#define HX711_SCK_GPIO_Port GPIOB
#define HX711_DOUT_Pin GPIO_PIN_1
#define HX711_DOUT_GPIO_Port GPIOB

#define HX711_SCK(a) (a)?(HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,GPIO_PIN_SET)):(HAL_GPIO_WritePin(GPIOB,HX711_SCK_Pin,GPIO_PIN_RESET))// PB0
#define HX711_DOUT(a) (a)?(HAL_GPIO_WritePin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin,GPIO_PIN_SET)):(HAL_GPIO_WritePin(GPIOB,HX711_DOUT_Pin,GPIO_PIN_RESET))// PB1
#define HX711_DOUT_Value HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin)// PB1

extern KFP KFP_height;
extern double weight_pizhong;
extern uint32_t temperature;

double HX711_Read(void);
double HX711_Read_avg(void);
double HX711_Read_weight_real(void);
double HX711_Read_weight_real_Kalman(void);
double HX711_Read_weight_real_temp(double weight);

#endif


