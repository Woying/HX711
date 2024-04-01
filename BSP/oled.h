#ifndef __OLED_H
#define __OLED_H 

#include "stdlib.h"	
#include "stm32f4xx_hal.h"
#include <stdbool.h>


#define delay_ms(x)   HAL_Delay(x)

//-----------------OLED端口定义----------------
#define OLED_DC_Pin GPIO_PIN_8
#define OLED_DC_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_9
#define OLED_RES_GPIO_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_10
#define OLED_CS_GPIO_Port GPIOA
#define OLED_DO_Pin GPIO_PIN_4
#define OLED_DO_GPIO_Port GPIOB
#define OLED_D1_Pin GPIO_PIN_5
#define OLED_D1_GPIO_Port GPIOB


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB,OLED_DO_Pin,GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB,OLED_DO_Pin,GPIO_PIN_SET)
 
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB,OLED_D1_Pin,GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB,OLED_D1_Pin,GPIO_PIN_SET)
 
#define OLED_RES_Clr() HAL_GPIO_WritePin(GPIOA,OLED_RES_Pin,GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(GPIOA,OLED_RES_Pin,GPIO_PIN_SET)
 
#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOA,OLED_DC_Pin,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOA,OLED_DC_Pin,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_RESET,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_RESET,GPIO_PIN_SET)


 
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define uint8_t unsigned char
#define uint32_t unsigned int
extern bool first;
	
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t n1);
void OLED_ScrollDisplay(uint8_t num,uint8_t space);
void OLED_WR_BP(uint8_t x,uint8_t y);
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[]);
void OLED_Init(void);

#endif
