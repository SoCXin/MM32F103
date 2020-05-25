#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
extern u8  TIM2CH1_CAPTURE_STA;		//���벶��״̬
extern u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ

int main(void)
{
    u32 temp = 0;
    uart_initwBaudRate(115200);
    delay_init();	   	 			//��ʱ��ʼ��
    LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM2_Cap_Init(0XFFFF, SystemCoreClock / 1000000 - 1);		//��1Mhz��Ƶ�ʼ���
    while(1)
    {
        delay_ms(10);
        LED0_PWM_VAL++;
        if(LED0_PWM_VAL == 300)LED0_PWM_VAL = 0;
        if(TIM2CH1_CAPTURE_STA & 0X80) //�ɹ�������һ�θߵ�ƽ
        {
            temp = TIM2CH1_CAPTURE_STA & 0X3F;
            temp *= 65536;					//���ʱ���ܺ�
            temp += TIM2CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
            printf("HIGH:%d us\r\n", temp);	//��ӡ�ܵĸߵ�ƽʱ��
            TIM2CH1_CAPTURE_STA = 0;			//������һ�β���
        }
    }
}
