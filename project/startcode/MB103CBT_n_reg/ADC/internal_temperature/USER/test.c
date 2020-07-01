#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "adc.h"


int main(void)
{
    u16 adcx;
    float temperate;
    uart_init(SystemCoreClock / 1000000, 115200);
    delay_init();	   	 	//延时初始化
    Adc_Init();		  		//ADC初始化
    //显示提示信息
    printf("MiniBoard 内部温度传感器测试\r\n");
    while(1)
    {
        adcx = Get_Adc_Average(5);
        temperate = (float)adcx * (3.3 / 4095);		//电压值
        temperate = (temperate - 1.5) / 0.00478 + 25;	//转换为温度值
        printf("当前测得芯片内部温度为:%.2f℃\r\n", temperate);
        delay_ms(300);
    }
}




