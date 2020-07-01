#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "dma.h"


int main(void)
{

    uart_init(SystemCoreClock / 1000000, 115200);	 	//���ڳ�ʼ��Ϊ115200
    delay_init();	     //��ʱ��ʼ��

    dma_m8tom8_test();
    // dma_m8tom16_test();
    //dma_m8tom32_test();
    // DMA_m16tom8_test();
    //DMA_m16tom16_test();
    // 	DMA_m16tom32_test();
    while(1);
}


