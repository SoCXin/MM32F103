#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "lcdc.h"
#include "rtc.h"

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    u8 t;
    char str[20];
    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 	//串口初始化为115200
    LED_Init();				//初始化与LED连接的硬件接口
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1,  "RTC TEST");
    putStr(110, 60, 2, 1,  "@MindMotion");
    putStr(110, 80, 2, 1,  "2016/09/02");

    while(RTC_Init())		//RTC初始化	，一定要初始化成功
    {
        putStr(110, 100, 2, 1, "RTC ERROR!   ");
        delay_ms(800);
        putStr(110, 100, 2, 1, "RTC Trying...");
    }
    putStr(110, 100, 2, 1, "    -  -     ");
    putStr(110, 140, 2, 1, "  :  :  ");
    while(1)
    {
        if(t != calendar.sec)
        {
            t = calendar.sec;
            sprintf(str, "%d", calendar.w_year);
            putStr(110, 100, 2, 1, str);

            sprintf(str, "%d", calendar.w_month);
            putStr(150, 100, 2, 1, str);

            sprintf(str, "%d", calendar.w_date);
            putStr(170, 100, 2, 1, str);
            switch(calendar.week)
            {
                case 0:
                    putStr(110, 120, 2, 1, "Sunday   ");
                    break;
                case 1:
                    putStr(110, 120, 2, 1, "Monday   ");
                    break;
                case 2:
                    putStr(110, 120, 2, 1, "Tuesday  ");
                    break;
                case 3:
                    putStr(110, 120, 2, 1, "Wednesday");
                    break;
                case 4:
                    putStr(110, 120, 2, 1, "Thursday ");
                    break;
                case 5:
                    putStr(110, 120, 2, 1, "Friday   ");
                    break;
                case 6:
                    putStr(110, 120, 2, 1, "Saturday ");
                    break;
            }
            sprintf(str, "%d", calendar.hour);
            putStr(110, 140, 2, 1, str);

            sprintf(str, "%d", calendar.min);
            putStr(135, 140, 2, 1, str);

            sprintf(str, "%d", calendar.sec);
            putStr(160, 140, 2, 1, str);
            LED1 = !LED1;
        }
        delay_ms(10);
    }
}




