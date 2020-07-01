#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "sleep.h"
int main(void)
{

    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_init(SystemCoreClock / 1000000, 115200);
    printf("sleep mode ! \r\n");
    sleep_test();

    //����K2��������
    while(1)//����ѭ��
    {
        printf("Wake Up ! \r\n");
        delay_ms(1000);
    }
}





























