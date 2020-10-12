#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "bkp.h"
int main(void)
{
    u8 i;

    delay_init();	     	//��ʱ��ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_init(SystemCoreClock / 1000000, 115200);
    i = BKP_DATA();		 //��BKPд���ݣ�����ɹ���LED����10�£�ʧ���������˸

    if(i == 0) //д������ȷ��LED��˸5��
    {
        for(i = 0; i < 10; i++)
        {
            LED1 = !LED1;
            LED2 = !LED2;
            LED3 = !LED3;
            LED4 = !LED4;
            delay_ms(400);
        }
    }
    else//д���ݲ���ȷ��LED������˸
    {
        while(1)
        {
            LED1 = !LED1;
            LED2 = !LED2;
            LED3 = !LED3;
            LED4 = !LED4;
            delay_ms(50);
        }
    }

    while(1)
    {
    }
}
























