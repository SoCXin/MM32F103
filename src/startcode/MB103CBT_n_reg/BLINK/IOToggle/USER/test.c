#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    delay_init();	     	//��ʱ��ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        delay_ms(400);
    }
}

























