#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dma.h"

u8 DST_Buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //UART DMA接受到数据存放数组


int main(void)
{
    u16 i;

    uart_init(SystemCoreClock / 1000000, 115200);	 	//串口初始化为115200
    delay_init();	   	 	//延时初始化
    LED_Init();		  		//初始化与LED连接的硬件接口
    KEY_Init();				//按键初始化
    //显示提示信息
    printf("Please enter 10 numbers!\r\n");
    MYDMA_Config(DMA1_Channel5, (u32)&UART1->RDR, (u32)DST_Buffer, 10); //DMA1通道4,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.

    UART1->GCR |= 1 << 1;       //选择串口1的DMA方式发送
    UART1->GCR |= 1 << 3;       //使能串口1的   接收
    MYDMA_Enable(DMA1_Channel5);//开始一次DMA传输！
    while(1)
    {
        if(DMA1->ISR & (1 << 17)) //等待通道4传输完成
        {
            DMA1->IFCR |= 1 << 17; //清除通道4传输完成标志
            break;
        }
    }
    for(i = 0; i < 10; i++)
    {
        printf("DST_Buffer[%d]==%d\r\n", i, (DST_Buffer[i] - 0x30));
    }
    printf("\r\nUART DMA TEST OK!\r\n ");

    while(1);

}


