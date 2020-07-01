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
    printf("wwdg test\r\n\r\n");
    WWDG_Init(0X7F, 0X5F, 3); //������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8
    while(1)
    {
        LED2 = !LED2;
        delay_ms(300);
    }


}
























