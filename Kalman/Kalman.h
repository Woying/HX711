#ifndef __KALMAN
#define __KALMAN

#include "stdint.h"
#include "stm32f4xx_hal.h"

//�ṹ�����Ͷ���
typedef struct 
{
    float LastP;//�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;//��ǰ����Э���� ��ʼ��ֵΪ0
    float out;//�������˲������ ��ʼ��ֵΪ0
    float Kg;//���������� ��ʼ��ֵΪ0
    float Q;//��������Э���� ��ʼ��ֵΪ
    float R;//�۲�����Э���� ��ʼ��ֵΪ
}KFP;//Kalman Filter parameter


float kalmanFilter(KFP *kfp,float input);

#endif