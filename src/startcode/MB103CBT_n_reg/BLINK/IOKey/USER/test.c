#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
int main(void)
{
    u8 t = 0;

    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    while(1)
    {
        t = KEY_Scan(0);		//�õ���ֵ
        switch(t)
        {
            case KEY1_PRES:
                LED1 = !LED1;
                break;
            case WKUP_PRES:
                LED2 = !LED2;
                break;
            case KEY3_PRES:
                LED3 = !LED3;
                break;
            case KEY4_PRES:
                LED4 = !LED4;
                break;
            default:
                delay_ms(10);
        }
    }
}

























