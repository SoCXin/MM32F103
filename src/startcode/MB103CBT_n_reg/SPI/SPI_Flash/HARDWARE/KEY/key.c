#include "key.h"
#include "delay.h"


//������ʼ������
//PA0 PC13 PB10 PB11���ó�����
void KEY_Init(void)
{


    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //ʹ��GPIOAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //RCC->APB2ENR|=1<<3;  //ʹ��GPIOBʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //RCC->APB2ENR|=1<<4;  //ʹ��GPIOCʱ��

    GPIOA->CRL &= 0XFFFFFFF0;	//PA0���ó�����	,K2(WK_UP)
    GPIOA->CRL |= GPIO_CRL_CNF0_1;

    GPIOC->CRH &= 0XFF0FFFFF;	//PC13���ó�����,K1
    GPIOC->CRH |= GPIO_CRH_CNF13_1;
    GPIOC->ODR |= GPIO_ODR_ODR13;	   	//PC13����

    GPIOB->CRH &= 0XFFFF00FF;	//PB10,PB11���ó�����,K3,K4
    GPIOB->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;
    GPIOB->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR11;	   	//PB10,PB11����

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP����
//KEY3_PRES��KEY3����
//KEY4_PRES��KEY4����
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //�������ɿ���־
    if(mode)key_up = 1; //֧������
    if(key_up && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0))
    {
        delay_ms(10);//ȥ����
        key_up = 0;
        if(KEY1 == 0)return KEY1_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
        else if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
    }
    else if(KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;// �ް�������
}



















