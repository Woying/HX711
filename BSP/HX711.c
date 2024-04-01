#include "HX711.h"


double HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i;
	HX711_DOUT(1); 
	HAL_GPIO_WritePin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin,GPIO_PIN_SET);
	delay_us(1);
	HX711_SCK(0); 
	count=0; 
	while(HX711_DOUT_Value); 
	for(i=0;i<24;i++)
	{ 
		HX711_SCK(1); 
		count=count<<1; 
		delay_us(1);
		HX711_SCK(0); 
		if(HX711_DOUT_Value)
			count++; 
		delay_us(1);
	} 
	HX711_SCK(1); 
	count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);
	HX711_SCK(0);  
	return(count);
}

double HX711_Read_avg(void)
{
	double sum = 0;    // Ϊ�˼�С��һ��ȡ��10��ֵ����ƽ��ֵ��
	double avg = 0;
//	HAL_Delay(500);
	for (int i = 0; i < 10; i++) // ѭ����Խ�ྫ��Խ�ߣ���Ȼ�ķѵ�ʱ��ҲԽ��
	sum += HX711_Read();  // �ۼ�
	avg = sum/10.0; // ��ƽ��ֵ���о���
	return avg;
}

double HX711_Read_weight_real()
{
	double real = 0;
     real = 0.0009495*(weight_pizhong-HX711_Read_avg());
	return real;
}

double HX711_Read_weight_real_Kalman()
{    
	double weight_K = HX711_Read(); 
     weight_K = 0.0009495*(weight_pizhong-weight_K);
	return kalmanFilter(&KFP_height,weight_K);
}

double HX711_Read_weight_real_temp(double weight)
{
	temperature = DS18B20_Get_Temperature();
	return (weight * (1 - (20 - temperature/10.0) / 10 * 0.002));
}