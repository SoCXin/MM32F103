#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dma.h"

u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

int main(void)
{
    u16 i;
    u8 t = 0;

    uart_init(SystemCoreClock / 1000000, 115200);	 	//串口初始化为115200
    delay_init();	   	 	//延时初始化
    LED_Init();		  		//初始化与LED连接的硬件接口
    KEY_Init();				//按键初始化
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 10); //DMA1通道4,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.
    //显示提示信息
    printf("\r\nDMA DATA:\r\n");

    for(i = 0; i < (TEXT_LENTH + 2) * 100; i++) //填充ASCII字符集数据
    {
        if(t >= TEXT_LENTH) //加入换行符
        {
            SendBuff[i++] = 0x0d;
            SendBuff[i] = 0x0a;
            t = 0;
        }
        else SendBuff[i] = TEXT_TO_SEND[t++]; //复制TEXT_TO_SEND语句
    }

    UART1->GCR |= 1 << 1;       //选择串口1的DMA方式发送
    UART1->GCR |= 1 << 4;       //使能串口1的发送
    MYDMA_Enable(DMA1_Channel4);//开始一次DMA传输！
    while(1)
    {
        if(DMA1->ISR & (1 << 13)) //等待通道4传输完成
        {
            DMA1->IFCR |= 1 << 13; //清除通道4传输完成标志
            break;
        }
    }
    printf("\r\nUART DMA TEST OK!\r\n ");

    while(1);

}


