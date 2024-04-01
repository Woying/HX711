#include "Key.h"

uint8_t RxBuffer1[7] = {0xAA,0x55,0x01,0x00,0x00,0x55,0xAA};
uint8_t RxBuffer2[10] = {0xAA,0x55,0x03,0x00,0x00,0x55,0xAA,0x00,0x00,0x00};
	
void KeyScan(uint8_t i,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
      switch(KeyState[i])
      {
            case KEY_UP:
            {
                  if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_RESET)   //读到低电平，按键进入抖动状态
                  {
                       KeyState[i] = KEY_DEBOUNCE;  
                  }
                  else   //读到高电平，按键按下标志置0
                  {
                       KeyFlag[i] = 0; 
                  }
            }
            break;
            case KEY_DEBOUNCE:  
            {
                  if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_RESET)  //读到低电平，按键进入按下状态
                  {
                        KeyState[i] = KEY_DOWN;  
                  }
                  else  //读到高电平，按键进入抬起状态
                  {
                        KeyState[i] = KEY_UP;
                  }
            }
            break;
            case KEY_DOWN:
            {
                  if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_SET)  //读到高电平，按键进入等待状态
                  {
                        KeyState[i] = KEY_WAIT;  
                        KeyFlag_Count = 0;
                  }
            break;
            }
            case KEY_WAIT:
            {
                  if((HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_SET)&&(++KeyFlag_Count >= 20))  //高电平的情况下，每10ms加1，加到20正好0.2s，如果大于0.2秒，进入抬起状态，下一次按下，不视为双击
                  {
                       KeyState[i] = KEY_UP; 
                       KeyFlag[i] = 1; 
                       KeyFlag_Count = 0;
                  }
                  else if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_SET)
                  {
                       KeyState[i] = KEY_WAIT; 
                  }
                  else  //小于0.2s再次读到低电平视为双击，将双击标志位置1
                  {
                        KeyState[i] = KEY_DOUBLE; 
                        KeyFlag_Double[i] = 1;
                  }
            }
            break;
            case KEY_DOUBLE:
            {
                  if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == GPIO_PIN_SET)  //读到高电平，按键进入抬起状态
                  {
                        KeyState[i] = KEY_UP;  
                        KeyFlag_Double[i] = 0;
                  }
            }
            break;
      }
}

void Key(void)
{	 
      if((KeyFlag[0] == 1)&&(first == true))
      {
		  first = false;
		 circle = false;
            KeyFlag[0] = 0;
		  HAL_TIM_Base_Stop_IT(&htim10);
		  weight_pizhong = HX711_Read_avg();
            HAL_TIM_Base_Start_IT(&htim10);		 
	 }
	 else if((KeyFlag[0] == 1)&&(first == false))
	 {
            KeyFlag[0] = 0;
		  HAL_TIM_Base_Stop_IT(&htim10);
		 //HAL_UART_Transmit(&huart1,RxBuffer2,10,0xffff);
		 HAL_UART_Transmit(&huart1,RxBuffer1,7,0xffff);
		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);		  
            HAL_TIM_Base_Start_IT(&htim10);	
            circle = true;		 
	 }
	 if(KeyFlag_Double[0] == 1)
      {
		  KeyFlag_Double[0] = 0;
	 	  if((State == 0)&&(circle == true))
            {     
			  State = 1;  
			  circle = false;
			  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			  weight_real = HX711_Read_weight_real();
			  weight_real = HX711_Read_weight_real_temp(weight_real);
			  //temperature = DS18B20_Get_Temperature();
			   OLED_Clear();
                  OLED_ShowChinese(0,0,0,16,1);//当
                  OLED_ShowChinese(18,0,1,16,1);//前
                  OLED_ShowChinese(36,0,2,16,1);//重
                  OLED_ShowChinese(54,0,3,16,1);//量
                  OLED_ShowChinese(72,0,4,16,1);//：
			   if(weight_real >=0 )
			   {
                  OLED_ShowChar(0,18,((uint32_t)(weight_real*10000/100000+Ascii0)),16); //1
			   }
			   else
			   {

				OLED_ShowChar(0,18,(Ascii0),16); //1
			   }
                  OLED_ShowChar(15,18,((uint32_t)(weight_real*10000/10000 )%10+Ascii0),16); //1
                  OLED_ShowChar(30,18,46,16); //.
                  OLED_ShowChar(45,18,((uint32_t)(weight_real*10000)/1000%10+Ascii0),16); //1
			   if(weight_real<1)
			   {
				   OLED_ShowChar(60,18,(Ascii0),16); //0
				   OLED_ShowChar(75,18,(Ascii0),16); //0
				   OLED_ShowChar(90,18,(Ascii0),16); //0
			   }
			   else
			   {
				   OLED_ShowChar(60,18,((uint32_t)(weight_real*10000)/100%10+Ascii0),16); //1
				   OLED_ShowChar(75,18,((uint32_t)(weight_real*10000)/10%10+Ascii0),16); //1
				   OLED_ShowChar(90,18,((uint32_t)(weight_real*10000)%10+Ascii0),16); //1
			   }
                  OLED_ShowChinese(108,18,5,16,1);//克
			   
			   OLED_ShowChinese(0,36,0,16,3);//当
                  OLED_ShowChinese(18,36,1,16,3);//前
                  OLED_ShowChinese(36,36,2,16,3);//温
                  OLED_ShowChinese(54,36,3,16,3);//度
                  OLED_ShowChinese(72,36,4,16,3);//：
                  OLED_ShowChar(79,36,((uint16_t)temperature/100+Ascii0),16); //1
                  OLED_ShowChar(86,36,((uint16_t)temperature/10%10+Ascii0),16); //1
                  OLED_ShowChar(93,36,46,16); //1
                  OLED_ShowChar(100,36,((uint16_t)temperature%10+Ascii0),16); //1
                  OLED_ShowChinese(108,36,5,16,3);//度
                  OLED_Refresh(); 
			   printf("重量为%fg,温度为%f\r\n",weight_real,temperature/10.0);	
                      RxBuffer1[3] = (uint8_t)(weight_real);
                      RxBuffer1[4] = (uint8_t)((uint32_t)(weight_real*10)%10);
                      RxBuffer1[5] = (uint8_t)((uint32_t)(weight_real*100)%10);
                      RxBuffer2[3]	= (uint8_t)temperature/10;
			       RxBuffer2[4] = (uint8_t)(temperature%10);	
		  }		
		  else if((State == 0)&&(circle == false))
            {     
			  weight_real = HX711_Read_weight_real();
			  weight_real = HX711_Read_weight_real_temp(weight_real);
			  //temperature = DS18B20_Get_Temperature();
			   OLED_Clear();
                  OLED_ShowChinese(0,0,0,16,1);//当
                  OLED_ShowChinese(18,0,1,16,1);//前
                  OLED_ShowChinese(36,0,2,16,1);//重
                  OLED_ShowChinese(54,0,3,16,1);//量
                  OLED_ShowChinese(72,0,4,16,1);//：
			   if(weight_real >=0 )
			   {
                  OLED_ShowChar(0,18,((uint32_t)(weight_real*10000/100000+Ascii0)),16); //1
			   }
			   else
			   {

				OLED_ShowChar(0,18,(Ascii0),16); //1
			   }
                  OLED_ShowChar(15,18,((uint32_t)(weight_real*10000/10000 )%10+Ascii0),16); //1
                  OLED_ShowChar(30,18,46,16); //.
                  OLED_ShowChar(45,18,((uint32_t)(weight_real*10000)/1000%10+Ascii0),16); //1
			   if(weight_real<1)
			   {
				   OLED_ShowChar(60,18,(Ascii0),16); //0
				   OLED_ShowChar(75,18,(Ascii0),16); //0
				   OLED_ShowChar(90,18,(Ascii0),16); //0
			   }
			   else
			   {
				   OLED_ShowChar(60,18,((uint32_t)(weight_real*10000)/100%10+Ascii0),16); //1
				   OLED_ShowChar(75,18,((uint32_t)(weight_real*10000)/10%10+Ascii0),16); //1
				   OLED_ShowChar(90,18,((uint32_t)(weight_real*10000)%10+Ascii0),16); //1
			   }
                  OLED_ShowChinese(108,18,5,16,1);//克
			   
			   OLED_ShowChinese(0,36,0,16,3);//当
                  OLED_ShowChinese(18,36,1,16,3);//前
                  OLED_ShowChinese(36,36,2,16,3);//温
                  OLED_ShowChinese(54,36,3,16,3);//度
                  OLED_ShowChinese(72,36,4,16,3);//：
                  OLED_ShowChar(79,36,((uint16_t)temperature/100+Ascii0),16); //1
                  OLED_ShowChar(86,36,((uint16_t)temperature/10%10+Ascii0),16); //1
                  OLED_ShowChar(93,36,46,16); //1
                  OLED_ShowChar(100,36,((uint16_t)temperature%10+Ascii0),16); //1
                  OLED_ShowChinese(108,36,5,16,3);//度
                  OLED_Refresh(); 
			   printf("重量为%fg,温度为%f\r\n",weight_real,temperature/10.0);	
                      RxBuffer1[3] = (uint8_t)(weight_real);
                      RxBuffer1[4] = (uint8_t)((uint32_t)(weight_real*10)%10);
                      RxBuffer1[5] = (uint8_t)((uint32_t)(weight_real*100)%10);
                      RxBuffer2[3]	= (uint8_t)temperature/10;
			       RxBuffer2[4] = (uint8_t)(temperature%10);	
		  }		
            else if(State == 1)
            {     
			  char i = 0;
			  State = 0; 
		       HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
                 weight_real_kalman = HX711_Read_weight_real_Kalman();	
                 weight_real_kalman = HX711_Read_weight_real_temp(weight_real_kalman);			  
			  while(i < 5)
			  {
				  weight_real_kalman = HX711_Read_weight_real_Kalman();	
                      weight_real_kalman = HX711_Read_weight_real_temp(weight_real_kalman);
				  OLED_Clear();
				  OLED_ShowChinese(0,0,0,16,4);//
                      OLED_ShowChinese(16,0,1,16,4);//
                      OLED_ShowChinese(32,0,2,16,4);//
                      OLED_ShowChinese(48,0,3,16,4);//
                      OLED_ShowChinese(64,0,4,16,4);//
				  OLED_ShowChinese(80,0,5,16,4);//
                      OLED_ShowChinese(96,0,6,16,4);//
				  OLED_ShowChinese(112,0,7,16,4);//
				  OLED_Refresh();
				  i++;
				  printf("%f,%f\r\n",weight_real_kalman,weight_real); 				  
			  }
			  i = 0;
			   OLED_Clear();
                  OLED_ShowChinese(0,0,0,16,1);//当
                  OLED_ShowChinese(18,0,1,16,1);//前
                  OLED_ShowChinese(36,0,2,16,1);//重
                  OLED_ShowChinese(54,0,3,16,1);//量
                  OLED_ShowChinese(72,0,4,16,1);//：
			   if(weight_real_kalman >=0 )
			   {
                  OLED_ShowChar(0,18,((uint32_t)(weight_real_kalman*10000/100000+Ascii0)),16); //1
			   }
			   else
			   {
			    OLED_ShowChar(0,18,(Ascii0),16); //0
			   }
                  OLED_ShowChar(15,18,((uint32_t)(weight_real_kalman*10000/10000 )%10+Ascii0),16); //1
                  OLED_ShowChar(30,18,46,16); //.
                  OLED_ShowChar(45,18,((uint32_t)(weight_real_kalman*10000)/1000%10+Ascii0),16); //1
			   if(weight_real_kalman<1)
			   {
				   OLED_ShowChar(60,18,(Ascii0),16); //0
				   OLED_ShowChar(75,18,(Ascii0),16); //0
				   OLED_ShowChar(90,18,(Ascii0),16); //0
			   }
			   else
			   {
				   OLED_ShowChar(60,18,((uint32_t)(weight_real_kalman*10000)/100%10+Ascii0),16); //1
				   OLED_ShowChar(75,18,((uint32_t)(weight_real_kalman*10000)/10%10+Ascii0),16); //1
				   OLED_ShowChar(90,18,((uint32_t)(weight_real_kalman*10000)%10+Ascii0),16); //1
			   }
                  OLED_ShowChinese(108,18,5,16,1);//克
			   
			   OLED_ShowChinese(0,36,0,16,3);//当
                  OLED_ShowChinese(18,36,1,16,3);//前
                  OLED_ShowChinese(36,36,2,16,3);//温
                  OLED_ShowChinese(54,36,3,16,3);//度
                  OLED_ShowChinese(72,36,4,16,3);//：
                  OLED_ShowChar(79,36,((uint16_t)temperature/100+Ascii0),16); //1
                  OLED_ShowChar(86,36,((uint16_t)temperature/10%10+Ascii0),16); //1
                  OLED_ShowChar(93,36,46,16); //1
                  OLED_ShowChar(100,36,((uint16_t)temperature%10+Ascii0),16); //1
                  OLED_ShowChinese(108,36,5,16,3);//度
                  OLED_Refresh(); 
			  //printf("%f,%f,%f,%f\r\n",weight_real,weight_real_kalman,KFP_height.R,KFP_height.Q);
			  printf("%f,%f,%f,%f\r\n",weight_real,weight_real_kalman,KFP_height.Kg,KFP_height.Now_P);  
                 RxBuffer1[3] = (uint8_t)(weight_real_kalman);
			  RxBuffer1[4] = (uint8_t)((uint32_t)(weight_real_kalman*10)%10);
			  RxBuffer1[5] = (uint8_t)((uint32_t)(weight_real_kalman*100)%10);
                 RxBuffer2[3]	= (uint8_t)temperature/10;
			  RxBuffer2[4] = (uint8_t)(temperature%10);
		  }			  
	  }			
}

