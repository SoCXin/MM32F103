#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "wkup.h"

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬰��°���WAKE UP
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{

    uart_init(SystemCoreClock / 1000000, 115200);	 	//���ڳ�ʼ��Ϊ115200
    printf("\r\n�밴��wake up����3�벻��\r\n");
    delay_init();	   	 	//��ʱ��ʼ��
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    LED1 = 0;
    WKUP_Init();			//��ʼ��WK_UP������ͬʱ����Ƿ�����������
    while(1)
    {
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        delay_ms(250);//��ʱ250ms
        printf("�Ѵ�standbyģʽ����\r\n");
    }
}




