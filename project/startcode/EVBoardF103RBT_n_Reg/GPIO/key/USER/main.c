#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"


int main(void)
{
    u8 t = 0;
    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    LED1 = 0;					//����LED
    while(1)
    {
        t = KEY_Scan(0);		//�õ���ֵ
        switch(t)
        {
            case KEY3_PRES:
                LED1 = !LED1;
                break;
            case KEY4_PRES:
                LED2 = !LED2;
                break;
            case WKUP_PRES:
                LED2 = !LED2;
                LED1 = !LED1;
                break;
            default:
                delay_ms(10);
        }
    }
}

