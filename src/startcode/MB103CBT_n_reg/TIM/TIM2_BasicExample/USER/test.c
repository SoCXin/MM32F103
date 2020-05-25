#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
unsigned int ucTim2Flag;//�жϼ�����־λ
int main(void)
{

    unsigned int uiCnt = 0;
    delay_init();	     //��ʱ��ʼ��
    uart_init(SystemCoreClock / 1000000, 115200); //���ڳ�ʼ��
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    TIM2_Int_Init(1000 - 1, SystemCoreClock / 1000000 - 1); //1Mhz�ļ���Ƶ�ʣ�������1000Ϊ1ms,�����жϷ�ʽ����
    LED1 = 1;
    while(1)
    {
        if(ucTim2Flag > 999)
        {
            ucTim2Flag = 0;

            /* ��ӡ��ǰuiCnt��ֵ */
            printf("Time=0x%xs\r\n", uiCnt);
            uiCnt++;
            LED1 = !LED1;

        }
    }
}






















