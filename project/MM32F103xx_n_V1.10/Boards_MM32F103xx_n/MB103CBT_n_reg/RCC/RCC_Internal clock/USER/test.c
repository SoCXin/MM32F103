#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"

extern void SystemClk_Output(void);
int main(void)
{
    //修改system_MM32L373.c文件中的系统时钟
    SystemClk_Output();
    while(1);
}

























