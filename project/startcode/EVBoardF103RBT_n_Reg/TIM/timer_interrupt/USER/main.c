#include "delay.h"
#include "sys.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    delay_init();
    LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM3_Int_Init(4999, SystemCoreClock / 10000 - 1); //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
    while(1)
    {
        LED2 = !LED2;
        delay_ms(500);	//�ж���LED1��˸Ƶ���Ƿ�һ��
    }

}

