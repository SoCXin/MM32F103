#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

extern void SystemClk_Output(void);
int main(void)
{
    //�޸�system_MM32L373.c�ļ��е�ϵͳʱ��
    SystemClk_Output();
    while(1);
}

























