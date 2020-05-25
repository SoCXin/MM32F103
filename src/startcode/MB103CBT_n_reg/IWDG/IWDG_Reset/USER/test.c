#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "wdg.h"
#include "key.h"
int main(void)
{
    delay_init();	     	//��ʱ��ʼ��
    KEY_Init();
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    LED1 = 1;
    uart_init(SystemCoreClock / 1000000, 115200); //���ڳ�ʼ��
    printf("uart ok!\r\n");
    IWDG_Init(4, 625);

    while(1)//���°���ι����LED1����������Ҳ����ӡ�����򲻶ϸ�λ�����ڲ��ϴ�ӡ��LED1ҲϨ��
    {
        if(KEY_Scan(0) == WKUP_PRES)
        {
            IWDG_Feed();
            LED1 = 0;
        }
        delay_ms(10);
    }
}

























