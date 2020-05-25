#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "adc.h"


int main(void)
{
    u16 adcx;
    float temperate;
    uart_init(SystemCoreClock / 1000000, 115200);
    delay_init();	   	 	//��ʱ��ʼ��
    Adc_Init();		  		//ADC��ʼ��
    //��ʾ��ʾ��Ϣ
    printf("MiniBoard �ڲ��¶ȴ���������\r\n");
    while(1)
    {
        adcx = Get_Adc_Average(5);
        temperate = (float)adcx * (3.3 / 4095);		//��ѹֵ
        temperate = (temperate - 1.5) / 0.00478 + 25;	//ת��Ϊ�¶�ֵ
        printf("��ǰ���оƬ�ڲ��¶�Ϊ:%.2f��\r\n", temperate);
        delay_ms(300);
    }
}




