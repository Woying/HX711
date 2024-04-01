/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2023-02-25
 * @brief       使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
 *              提供delay_init初始化函数， delay_us和delay_ms等延时函数
 * @license     Copyright (c) 2022-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F767开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20220719
 * 第一次发布
 * V1.1 20230225
 * 修改SYS_SUPPORT_OS部分代码, 默认仅支持UCOSII 2.93.01版本, 其他OS请参考实现
 * 修改delay_init不再使用8分频,全部统一使用MCU时钟
 * 修改delay_us使用时钟摘取法延时, 兼容OS
 * 修改delay_ms直接使用delay_us延时实现.
 *
 ****************************************************************************************************
 */


#include "delay.h"


static uint32_t g_fac_us = 0;  /* us延时倍乘数 */

void delay_init(uint16_t sysclk)
{
#if SYS_SUPPORT_OS                                      /* 如果需要支持OS */
    uint32_t reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);/* SYSTICK使用外部时钟源,频率为HCLK */
    g_fac_us = sysclk;                                  /* 不论是否使用OS,g_fac_us都需要使用 */
#if SYS_SUPPORT_OS                                      /* 如果需要支持OS. */
    reload = sysclk;                                    /* 每秒钟的计数次数 单位为M */
    reload *= 1000000 / delay_ostickspersec;            /* 根据delay_ostickspersec设定溢出时间,reload为24位
                                                         * 寄存器,最大值:16777216,在168M下,约合0.09986s左右
                                                         */
    g_fac_ms = 1000 / delay_ostickspersec;              /* 代表OS可以延时的最少单位 */ 
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;          /* 开启SYSTICK中断 */
    SysTick->LOAD = reload;                             /* 每1/delay_ostickspersec秒中断一次 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;           /* 开启SYSTICK */
#endif 
}

/**
 * @brief     延时nus
 *  @note     用时钟摘取法来做us延时
 * @param     nus: 要延时的us数
 * @note      nus取值范围: 0 ~ (2^32 / fac_us) (fac_us一般等于系统主频, 自行套入计算)
 * @retval    无
 */ 
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;    /* LOAD的值 */
    ticks = nus * g_fac_us;             /* 需要的节拍数 */
    told = SysTick->VAL;                /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;  /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;               /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
} 







