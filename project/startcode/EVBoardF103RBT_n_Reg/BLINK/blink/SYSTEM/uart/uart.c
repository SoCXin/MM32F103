#include "sys.h"
#include "uart.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB

#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    while((UART1->CSR & 0x0001) == 0); //ѭ������,ֱ���������
    UART1->TDR = (ch & (uint16_t)0x00FF);
    return ch;
}

#else
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
    while((UART1->CSR & 0x0001) == 0); //ѭ������,ֱ���������
    UART1->TDR = (ch & (uint16_t)0x00FF);
    return ch;
}

#endif



void uart_initwBaudRate(u32 bound)
{

    u32 tempBaud;

    //-------------------------------------------------
    RCC->APB2ENR |= 1 << 2; //ʹ��GPIOA��ʱ��   #define RCC_APB2Periph_UART1            ((uint32_t)0x00004000) #define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
    /*
    APB2����ʱ��ʹ�ܼĴ�����RCC_APB2ENR��
    ƫ�Ƶ�ַ��0x18
    ��λֵ��0x0000 0000
    ���ʣ��޵ȴ����ڣ��֣����ֺ��ֽڷ���
    ע��������ʱ��û������ʱ��������ܶ�������Ĵ�������ֵ
    λ2
    IOPAEN��IO�˿�Aʱ��ʹ��
    ������á�1�����塯0��
    0��IO�˿�Aʱ�ӹرգ�
    1��IO�˿�Aʱ�ӿ�����
    */
    RCC->APB2ENR |= 1 << 14; //ʹ�ܴ���ʱ��
    /*
    λ14
    UART1EN��UART1ʱ��ʹ��
    ������á�1�����塯0��
    0��UART1ʱ�ӹرգ�
    1��UART1ʱ�ӿ�����
    */
    GPIOA->CRH &= 0XFFFFF00F; //IO״̬����
    GPIOA->CRH |= 0X000008B0; //IO״̬����
    /*
    8.2.2 �˿����ø߼Ĵ�����GPIOx_CRH��(x=A..E)
    ƫ�Ƶ�ַ��0x04
    ��λֵ��0x4444 4444
    ����PA9Ϊ B = 10 11  MODE9[1:0] = 11[5:4] �����ģʽ������ٶ�50MHz, CNF9[1:0] = 10[7:6] �����ù����������ģʽ
    ����PA10 Ϊ8 = 10 00 MODE10[1:0] = 00[9:8] ������ģʽ����λ���״̬��, CNF10[1:0] = 10[7:6] ��10������/��������ģʽ

    MODEy[1:0]:�˿�x��ģʽλ��y=8��15��
    ���ͨ����Щλ������Ӧ��I/O�˿ڣ���ο���15�˿�λ���ñ�
    00������ģʽ����λ���״̬��
    01�����ģʽ������ٶ�10MHz
    10�����ģʽ������ٶ�20MHz
    11�����ģʽ������ٶ�50MHz

    CNFy[1:0]:�˿�x����λ��8��15��
    ���ͨ����Щλ������Ӧ��I/O�˿ڣ���ο���15�˿�λ���ñ�
    ������ģʽ��MODE[1:0]==00��:
    00��ģ������ģʽ
    01����������ģʽ
    10������/��������ģʽ
    11������
    �����ģʽ��MODE[1:0]>00��:
    00��ͨ���������ģʽ
    01��ͨ�ÿ�©���ģʽ
    10�����ù����������ģʽ
    11�����ù��ܿ�©���ģʽ

    */
    RCC->APB2RSTR |= 1 << 14; //��λ����1
    RCC->APB2RSTR &= ~(1 << 14); //ֹͣ��λ
    //-------------------------------------------------
    //����������
    // 	UART1->BRR=mantissa; // ����������
    /* Determine the uart_baud*/
    tempBaud = (SystemCoreClock / 16) / (bound);
    /* Write to UART BRR */
    UART1->BRR = tempBaud;
    UART1->FRA = (SystemCoreClock / 16) % (bound);
    UART1->CCR |= 0X30; //1λֹͣ,��У��λ.
    //-------------------------------------------------
    //ʹ�ܽ����ж�
    UART1->GCR = 0X19;			//�շ�ʹ��	UART1->CCR|=1<<5;    //���ջ������ǿ��ж�ʹ��
    /*
    23.5.5 UART �ж�ʹ�ܼĴ���(UART_IER)
    ƫ�Ƶ�ַ��0x10
    ��λֵ��0x0000
    λ1
    RXIEN:���ջ����ж�ʹ��λ
    1=�ж�ʹ��
    0=�жϽ�ֹ
    23.5.6 UART �ж�����Ĵ���(UART_ICR)
    ƫ�Ƶ�ַ��0x14
    ��λֵ��0x0000
    λ1
    RXICLR: �����ж����λ
    1=�ж����
    0=�ж�û�����
    */
}



