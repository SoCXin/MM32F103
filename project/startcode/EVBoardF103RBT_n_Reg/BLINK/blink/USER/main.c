#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

int main(void)
{

    delay_init();	     	//��ʱ��ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    LED2 = !LED1;
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        delay_ms(300);
    }
}

























