#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
/********************************************************************************************************
**������Ϣ ��PVD_IRQHandler(void)
**�������� ����ѹ������������õ���ֵ��ѹ�󣬽�����жϣ���תLED
**������� ����
**������� ����
********************************************************************************************************/
void PVD_IRQHandler(void)
{
    if((EXTI->PR & 0x10000) != RESET) //
    {
        /* LED��ת */
        LED1 = !LED1;
        LED2 = !LED2;
        LED3 = !LED3;
        LED4 = !LED4;
        /* Clear the Key Button EXTI line pending bit */
        EXTI->PR = 0x10000;
    }
}
int main(void)
{

    delay_init();	     	//��ʱ��ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    MY_NVIC_Init(3, 3, PVD_IRQn, 2); //����PVD�ж�
    EXTI->IMR |= 1 << 16; //����16���ж���
    EXTI->RTSR |= 1 << 16; //����16���ж��������ش���
    EXTI->FTSR |= 1 << 16; //����16���ж����½��ش���
    PWR->CR |= 0x00000800; //����3.0VΪ��ѹ��ֵ
    PWR->CR |= 1 << 4; //ʹ��PVD
    while(1)
    {
    }
}

























