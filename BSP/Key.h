#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "HX711.h"
#include <stdbool.h>
#include "stdio.h"
#include "oled.h"
#include "DS18B20.h"


#define Key_Num 1
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Ascii0 48 

 
typedef enum
{
      KEY_UP = 0,
      KEY_DEBOUNCE,
      KEY_DOWN,
      KEY_DOUBLE,
      KEY_WAIT
}KEY_STATE;     //按键的五种状态，通过枚举类型实现

extern KFP KFP_height;

extern TIM_HandleTypeDef htim10;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern bool first;
extern KEY_STATE KeyState[Key_Num];
extern uint8_t KeyFlag[Key_Num];
extern uint8_t KeyFlag_Double[Key_Num];  //按键双击标志，1按下，0没按 
extern uint16_t KeyFlag_Count;

extern double weight_real;
extern double weight_pizhong;
extern double weight_real_kalman;
extern uint32_t temperature;
extern uint8_t State;
extern bool circle;



void KeyScan(uint8_t i,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Key(void);
#endif