#ifndef __KALMAN
#define __KALMAN

#include "stdint.h"
#include "stm32f4xx_hal.h"

//结构体类型定义
typedef struct 
{
    float LastP;//上次估算协方差 初始化值为0.02
    float Now_P;//当前估算协方差 初始化值为0
    float out;//卡尔曼滤波器输出 初始化值为0
    float Kg;//卡尔曼增益 初始化值为0
    float Q;//过程噪声协方差 初始化值为
    float R;//观测噪声协方差 初始化值为
}KFP;//Kalman Filter parameter


float kalmanFilter(KFP *kfp,float input);

#endif