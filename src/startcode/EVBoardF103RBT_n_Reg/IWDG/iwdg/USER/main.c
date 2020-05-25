#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "wdg.h"


int main(void)
{
    delay_init();	    	 //��ʱ������ʼ��
    LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          //������ʼ��
    delay_ms(300);   	 //���˿��õ���
    IWDG_Init(4, 625);   //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s
    LED1 = 0;				 //����LED1
    while(1)
    {
        if(KEY_Scan(0) == WKUP_PRES)
            IWDG_Feed();//���WK_UP����,��ι��
        delay_ms(100);
    }

}

