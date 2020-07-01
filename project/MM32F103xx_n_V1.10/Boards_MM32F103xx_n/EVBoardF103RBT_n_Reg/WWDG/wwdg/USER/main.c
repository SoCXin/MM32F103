#include "delay.h"
#include "sys.h"
#include "led.h"
#include "wdg.h"

int main(void)
{
    delay_init();
    LED_Init();
    LED1 = 0;
    delay_ms(300);
    WWDG_Init(0X7F, 0X5F, 3);
    while(1)
    {
        LED1 = 1;
    }
}

