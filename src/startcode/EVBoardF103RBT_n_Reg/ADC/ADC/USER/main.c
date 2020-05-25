#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "lcdc.h"
#include "adc.h"

int main(void)
{

    unsigned char vol[20];
    unsigned char val[20];
    u16 adcx = 0;
    float fValue;
    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 //串口初始化为115200
    LED_Init();		  	 //初始化与LED连接的硬件接口

    /*配置ADC1为单次转换模式,通道1使能*/
    Adc_Init();
    initLCD();

    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "ADC TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 120, 2, 1, "ADC_CH1_VAL:");
    putStr(110, 140, 2, 1, "ADC_CH1_VOL:0.000V");

    while(1)
    {
        /*读取当前转换数据*/
        adcx = Get_Adc(); //读取PA1电平值
        fValue = ((float)adcx / 4095) * 3.3;
        sprintf(val, "%d", adcx);
        sprintf(vol, "%.3f", fValue);
        putStr(205, 120, 2, 1, "    ");
        putStr(205, 120, 2, 1, val);
        putStr(205, 140, 2, 1, vol);
        delay_ms(300);
    }



}


