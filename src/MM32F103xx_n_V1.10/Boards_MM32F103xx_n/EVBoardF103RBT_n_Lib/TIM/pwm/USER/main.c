/**
******************************************************************************
* @file    main.c
* @author  AE Team
* @version V1.1.0
* @date    28/08/2019
* @brief   This file provides all the main functions.
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
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"

int main(void)
{
    u16 led0pwmval = 0;
    u8 dir = 1;
    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM1_PWM_Init(899, 0); //����Ƶ��PWMƵ��=96000/(899+1)=106Khz
    while(1)
    {
        delay_ms(8);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval > 400)dir = 0;
        if(led0pwmval == 0)dir = 1;
        TIM_SetCompare1(TIM1, led0pwmval);
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

