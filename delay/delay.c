/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2023-02-25
 * @brief       ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
 *              �ṩdelay_init��ʼ�������� delay_us��delay_ms����ʱ����
 * @license     Copyright (c) 2022-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F767������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220719
 * ��һ�η���
 * V1.1 20230225
 * �޸�SYS_SUPPORT_OS���ִ���, Ĭ�Ͻ�֧��UCOSII 2.93.01�汾, ����OS��ο�ʵ��
 * �޸�delay_init����ʹ��8��Ƶ,ȫ��ͳһʹ��MCUʱ��
 * �޸�delay_usʹ��ʱ��ժȡ����ʱ, ����OS
 * �޸�delay_msֱ��ʹ��delay_us��ʱʵ��.
 *
 ****************************************************************************************************
 */


#include "delay.h"


static uint32_t g_fac_us = 0;  /* us��ʱ������ */

void delay_init(uint16_t sysclk)
{
#if SYS_SUPPORT_OS                                      /* �����Ҫ֧��OS */
    uint32_t reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);/* SYSTICKʹ���ⲿʱ��Դ,Ƶ��ΪHCLK */
    g_fac_us = sysclk;                                  /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ�� */
#if SYS_SUPPORT_OS                                      /* �����Ҫ֧��OS. */
    reload = sysclk;                                    /* ÿ���ӵļ������� ��λΪM */
    reload *= 1000000 / delay_ostickspersec;            /* ����delay_ostickspersec�趨���ʱ��,reloadΪ24λ
                                                         * �Ĵ���,���ֵ:16777216,��168M��,Լ��0.09986s����
                                                         */
    g_fac_ms = 1000 / delay_ostickspersec;              /* ����OS������ʱ�����ٵ�λ */ 
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;          /* ����SYSTICK�ж� */
    SysTick->LOAD = reload;                             /* ÿ1/delay_ostickspersec���ж�һ�� */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;           /* ����SYSTICK */
#endif 
}

/**
 * @brief     ��ʱnus
 *  @note     ��ʱ��ժȡ������us��ʱ
 * @param     nus: Ҫ��ʱ��us��
 * @note      nusȡֵ��Χ: 0 ~ (2^32 / fac_us) (fac_usһ�����ϵͳ��Ƶ, �����������)
 * @retval    ��
 */ 
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;    /* LOAD��ֵ */
    ticks = nus * g_fac_us;             /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;  /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;               /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
} 







