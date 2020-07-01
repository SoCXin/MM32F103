#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    u16 led0pwmval = 0;
    u8 dir = 1;

    delay_init();	     //��ʱ��ʼ��
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM3_PWM_Init(899, 0); //����Ƶ

    while(1)//����ѭ��
    {
        delay_ms(4);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval > 500)dir = 0;
        if(led0pwmval == 0)dir = 1;
        TIM3->CCR1 = led0pwmval; //����Ƚ�ֵ
    }
}






















