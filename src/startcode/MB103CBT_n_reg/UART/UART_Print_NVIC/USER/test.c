#include "sys.h"
#include "uart_nvic.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    u8 t;
    u8 len;
    u16 times = 0;

    delay_init();	     	//��ʱ��ʼ��
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_nvic_init(SystemCoreClock / 1000000, 115200);	 //���ڳ�ʼ��Ϊ115200
    while(1)
    {
        if(UART_RX_STA & 0x8000)

        {
            len = UART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
            printf("\r\n�����͵���ϢΪ:\r\n");
            for(t = 0; t < len; t++)
            {
                while((UART1->CSR & 0x1) == 0); //ѭ������,ֱ���������   while((UART1->SR&0X40)==0);//�ȴ����ͽ���
                UART1->TDR = UART_RX_BUF[t];

            }
            printf("\r\n\r\n");//���뻻��
            UART_RX_STA = 0;
        }
        else
        {
            times++;
            if(times % 5000 == 0)
            {
                printf("\r\n����ʵ��\r\n");
            }
            if(times % 200 == 0)printf("����������,�Իس�������\r\n");
            if(times % 30 == 0)LED1 = !LED1; //��˸LED,��ʾϵͳ��������.
            delay_ms(10);
        }
    }
}


























