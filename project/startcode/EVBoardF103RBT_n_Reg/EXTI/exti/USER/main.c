#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "exti.h"


int main(void)
{
    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    EXTIX_Init();		//�ⲿ�жϳ�ʼ��
    while(1)
    {
    }
}

