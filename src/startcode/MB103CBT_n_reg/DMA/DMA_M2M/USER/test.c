#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "dma.h"


int main(void)
{

    uart_init(SystemCoreClock / 1000000, 115200);	 	//串口初始化为115200
    delay_init();	     //延时初始化

    dma_m8tom8_test();
    // dma_m8tom16_test();
    //dma_m8tom32_test();
    // DMA_m16tom8_test();
    //DMA_m16tom16_test();
    // 	DMA_m16tom32_test();
    while(1);
}


