#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "i2c.h"
int main(void)
{

    delay_init();	     	//��ʱ��ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_init(SystemCoreClock / 1000000, 115200);
    I2CMasterTest();
    while(1)
    {
    }
}

























