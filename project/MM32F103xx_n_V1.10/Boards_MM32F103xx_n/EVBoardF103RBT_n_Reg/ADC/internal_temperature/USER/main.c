#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "adc.h"
#include "lcdc.h"


int main(void)
{
    u16 average = 0;
    float temperate;
    unsigned char tem[20];

    delay_init();
    uart_initwBaudRate(9600);
    LED_Init();
    Adc_Init();
    initLCD();

    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "Temperature TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
    putStr(110, 120, 2, 1, "TEMP_VAL:");
    putStr(220, 120, 2, 1, "C");
    while(1)
    {
        average = Get_Adc();
        temperate = (float)average * (3.3 / 4095);
        temperate = (temperate - 1.5) / 0.00478 + 25;
        sprintf(tem, "%.1f", temperate);
        putStr(185, 120, 2, 1, tem);
        delay_ms(800);

    }


}

