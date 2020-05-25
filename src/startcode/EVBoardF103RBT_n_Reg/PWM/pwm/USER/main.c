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
        delay_ms(5);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval > 400)dir = 0;
        if(led0pwmval == 0)dir = 1;
        TIM1->CCR1 = led0pwmval;
    }
}

