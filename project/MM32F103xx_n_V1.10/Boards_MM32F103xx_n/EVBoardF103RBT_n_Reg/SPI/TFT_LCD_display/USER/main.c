#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "lcdc.h"
int main(void)
{
    u8 x = 0;
    u16 color;
    LED_Init();
    delay_init();
    initLCD();
    while(1)
    {
        switch(x)
        {
            case 0: lcdFillColor(Black); color = Black; break;
            case 1: lcdFillColor(White); color = White; break;
            case 2: lcdFillColor(Blue); color = Blue; break;
            case 3: lcdFillColor(DarkGrey); color = DarkGrey; break;
            case 4: lcdFillColor(Magenta); color = Magenta; break;
            case 5: lcdFillColor(Green); color = Green; break;
            case 6: lcdFillColor(Cyan); color = Cyan; break;

            case 7: lcdFillColor(Yellow); color = Yellow; break;
            case 8: lcdFillColor(Purple); color = Purple; break;
            case 9: lcdFillColor(Olive); color = Olive; break;
            case 10: lcdFillColor(LightGrey); color = LightGrey; break;
            case 11: lcdFillColor(Maroon); color = Maroon; break;
        }
        lcdSetBackColor(color);
        putStr(110, 20, 2, 1, "MM32 Dev Board");
        putStr(110, 40, 2, 1, "TFTLCD TEST");
        putStr(110, 60, 2, 1, "@MindMotion");
        putStr(110, 80, 2, 1, "2016/09/02");

        x++;
        if(x == 12)x = 0;
        LED1 = !LED1;
        delay_ms(1000);
    }

}





























