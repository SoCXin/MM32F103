#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������
//�������� ��������
//////////////////////////////////////////////////////////////////////////////////

//������ʼ������
//PA15��PC10 ���ó�����
void KEY_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //RCC->APB2ENR|=1<<2;  //ʹ��GPIOAʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //RCC->APB2ENR|=1<<4;  //ʹ��GPIOCʱ��

    GPIOA->CRL &= 0XFFFFFFF0;	//PA0���ó�����	,K2(WK_UP)
    GPIOA->CRL |= GPIO_CRL_CNF0_1;

    GPIOC->CRH &= 0XFFFFF0FF;	//PC10���ó�����,K1
    GPIOC->CRH |= GPIO_CRH_CNF10_1;
    GPIOC->ODR |= GPIO_ODR_ODR10;	   	//PC13����

    GPIOA->CRH &= 0X0FFFFFFF;	//PA15���ó�����
    GPIOA->CRH |= GPIO_CRH_CNF15_1;
    GPIOA->ODR |= GPIO_ODR_ODR15;	   	//PA15����

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP����
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //�������ɿ���־
    if(mode)key_up = 1; //֧������
    if(key_up && (KEY3 == 0 || KEY4 == 0 || WK_UP == 1))
    {
        delay_ms(10);//ȥ����
        key_up = 0;
        if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
    }
    else if(KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;// �ް�������
}


















