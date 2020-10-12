/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main firmware functions.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/
#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "pwm.h"
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��ʹ�öŰ������Ӱ����ϵ�PB7��PA6(Ain4)
**������� ��
**������� ��
**���ú��� ��uart_printf("0x%x\r\n",Uart1RxTest(UART1));   uart_printf("0x%x",sizeof(u32));
********************************************************************************************************/
extern u16 period;
extern u16 duty ;
extern u8 CollectFlag ;
extern u32 SystemCoreClock;
int main(void)
{

    //PB7��Ϊ���벶�����ţ�����PB7��PA6
    delay_init();	    	 //��ʱ������ʼ��

    uart_initwBaudRate(115200);
    TIM3_PWM_Init(1000 - 1, SystemCoreClock / 1000000 - 1); //1KHZ����
    TIM_SetCompare1(TIM3, 200);		//����ռ�ձ�
    TIM4_PWMINPUT_INIT(0xffff, SystemCoreClock / 1000000 - 1); //pwm�����ʼ����1M��Ƶ�ʲ�׽
    printf("������MiniBoard�ϵ�PB7��PA6(Ain4)\r\n");
    while(1)
    {
        delay_ms(500);
        if(CollectFlag == 1)
        {
            printf("duty  	= %d%% \r\n", duty * 100 / period);				//��ӡռ�ձ�
            printf("cycle 	= %dKHz\r\n", 1000 / period); //��ӡ������һ�ֽз�
            printf("period  = %dus\r\n", period);   //��ӡ����
            CollectFlag = 0;
        }
    }

}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/


