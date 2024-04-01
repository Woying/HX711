#include "DS18B20.h"




void DS18B20_IO_IN(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void DS18B20_IO_OUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}


void DS18B20_Rst(void){
	DS18B20_IO_OUT();//�������ģʽ
	
	//�������߲���ʱ750us
	DS18B20_DQ_OUT_LOW;
	delay_us(750);     
	
	//�ͷ�����Ϊ�ߵ�ƽ����ʱ�ȴ�15~60us
	DS18B20_DQ_OUT_HIGH;
	delay_us(15);
}


uint8_t DS18B20_Check(void){
	//����һ���������ʱ��
	uint8_t retry = 0;
	//������Ϊ����ģʽ
	DS18B20_IO_IN();
	while(DS18B20_DQ_IN && retry < 200){
		retry++;
		delay_us(1);
	}
	
	if(retry >= 200)
		return 1;
	else
		retry = 0;
	
	//�ж�DS18B20�Ƿ��ͷ�����
	while(!DS18B20_DQ_IN && retry < 240){
		retry++;
		delay_us(1);
	}
	
	if(retry >= 240)
		return 2;
	
	return 0;
}


void DS18B20_Write_Byte(uint8_t data){
	uint8_t j;
	uint8_t databit;
	DS18B20_IO_OUT();
	for(j=1;j<=8;j++){
		databit=data&0x01;//ȡ�������λ
		data=data>>1;     //����һλ
		if(databit){      //��ǰλд1
			DS18B20_DQ_OUT_LOW;
			delay_us(2);
			DS18B20_DQ_OUT_HIGH;
			delay_us(60);
		}else{          //��ǰλд0
			DS18B20_DQ_OUT_LOW;
			delay_us(60);
			DS18B20_DQ_OUT_HIGH;
			delay_us(2);
		}
	}
}


uint8_t DS18B20_Read_Bit(void){
	uint8_t data;
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT_LOW;
	delay_us(2);
	DS18B20_DQ_OUT_HIGH;
	DS18B20_IO_IN();
	delay_us(12);
	
	if(DS18B20_DQ_IN)
		data = 1;
	else
		data = 0;
	
	delay_us(50);
	return data;
}

uint8_t DS18B20_Read_Byte(void){
	uint8_t i,j,data;
	data = 0;
	for(i=1;i<=8;i++){
		j = DS18B20_Read_Bit();
		data = (j<<7)|(data>>1);
		/*j=0��1��j<<7=0x00��0x80����data����һλ��򣬼���1/0д�����λ���´���������λ*/

	}
	return data;
}

void DS18B20_Start(void){
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);//����ROM
	DS18B20_Write_Byte(0x44);//�¶ȱ任����
}


uint8_t DS18B20_Init(void){
	//���ų�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	DS18B20_Rst();
	return DS18B20_Check();
}

short DS18B20_Get_Temperature(){
	uint8_t temp;
	uint8_t TL,TH;
	short temperature;
	
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
  DS18B20_Write_Byte(0xcc);//����ROM
	DS18B20_Write_Byte(0xbe);//���ݴ���
	TL = DS18B20_Read_Byte();//�Ͱ�λ
	TH = DS18B20_Read_Byte();//�߰�λ
	
	//�ж��¶�ֵ�Ƿ�Ϊ����
	if(TH>0x70){
		TH = ~TH;
		TL = ~TL;
		temp = 0;
	}else
		temp = 1;
	
	temperature = TH;
	temperature <<= 8;
	temperature += TL;
	temperature = (float)temperature*0.625;
	if(temp)
		return temperature;
	else
		return -temperature;
}


