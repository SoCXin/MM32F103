#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    unsigned int uiCnt = 0;

    delay_init();	     //��ʱ��ʼ��
    uart_init(SystemCoreClock / 1000000, 115200); //���ڳ�ʼ��
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM1_Int_Init(10000 - 1, SystemCoreClock / 10000 - 1); //10Khz�ļ���Ƶ�ʣ�������10000Ϊ1s
    LED1 = 1;
    while(1)
    {
        for(uiCnt = 0; ; uiCnt++)
        {
            /* �ȴ���ʱ����� */
            Tim1_UPStatusOVCheck_test();
            /* ��ӡ��ǰ��ֵ */
            printf("0x%x\r\n", uiCnt);
            LED1 = !LED1;
        }
    }
}






















