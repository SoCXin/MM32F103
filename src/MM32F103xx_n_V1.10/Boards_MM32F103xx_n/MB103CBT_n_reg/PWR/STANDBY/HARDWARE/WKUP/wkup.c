#include "wkup.h"
#include "led.h"
#include "delay.h"



//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{
    //�ر���������(����ʵ�����д)
    RCC->APB2RSTR |= 0X01FC; //��λ����IO��
    Sys_Standby();//�������ģʽ
}
//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
//      0:����Ĵ���
u8 Check_WKUP(void)
{
    u8 t = 0;	//��¼���µ�ʱ��
    LED1 = 0; //����DS0
    LED2 = 0; //����DS0
    LED3 = 0; //����DS0
    LED4 = 0; //����DS0
    while(1)
    {
        if(WKUP_KD)
        {
            t++;			//�Ѿ�������
            delay_ms(30);
            if(t >= 100)		//���³���3����
            {
                LED1 = 0;	 	//����DS0
                LED2 = 0;	 	//����DS0
                LED3 = 0;	 	//����DS0
                LED4 = 0;
                return 1; 	//����3s������
            }
        }
        else
        {
            LED1 = 1;
            LED2 = 1;
            LED3 = 1;
            LED4 = 1;
            return 0; //���²���3��
        }
    }
}
//�ж�,��⵽PA0�ŵ�һ��������.
//�ж���0���ϵ��жϼ��
void EXTI0_IRQHandler(void)
{
    EXTI->PR = EXTI_PR_PR0; //���LINE10�ϵ��жϱ�־λ

}
//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   //��ʹ������IO PORTAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;   //��������ʱ��

    GPIOA->CRL &= 0XFFFFFFF0; //PA0���ó�����
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    Ex_NVIC_Config(GPIO_A, 0, RTIR); //PA0�����ش���

    //(����Ƿ���������)��
    if(Check_WKUP() == 0)Sys_Standby();  //���ǿ���,�������ģʽ
    MY_NVIC_Init(2, 2, EXTI0_IRQn, 2); //��ռ2�������ȼ�2����2
}
















