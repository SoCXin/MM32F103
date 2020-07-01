#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "key.h"
#include "dma.h"
#include "lcdc.h"

u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];

int main(void)
{
    u16 i;
    u8 t = 0;
    char str[20];
    float pro = 0;			//进度
    delay_init();	    	 //延时函数初始化
    uart_initwBaudRate(115200);	 //串口初始化为115200
    printf("\r\nDMA DATA:\r\n");
    LED_Init();		  	 //初始化与LED连接的硬件接口
    KEY_Init();				//按键初始化
    MYDMA_Config(DMA1_Channel4, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 100); //DMA1通道4,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.
    initLCD();
    putStr(110, 20, 2, 1, "MM32 Dev Board");
    putStr(110, 40, 2, 1, "DMA TEST");
    putStr(110, 60, 2, 1, "@MindMotion");
    putStr(110, 80, 2, 1, "2016/09/02");
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
    while(1)
    {
        t = KEY_Scan(0);
        if(t == WKUP_PRES) //KEY0按下
        {
            putStr(110, 100, 2, 1, "Start Transimit....");
            putStr(135, 120, 2, 1, " %");
            UART1->GCR |= 1 << 1;       //选择串口1的DMA方式发送
            UART1->GCR |= 1 << 4;       //使能串口1的发送
            MYDMA_Enable(DMA1_Channel4);//开始一次DMA传输！
            //等待DMA传输完成，此时我们来做另外一些事，点灯
            //实际应用中，传输数据期间，可以执行另外的任务
            while(1)
            {
                if(DMA1->ISR & (1 << 13)) //等待通道4传输完成
                {
                    DMA1->IFCR |= 1 << 13; //清除通道4传输完成标志
                    break;
                }
                pro = DMA1_Channel4->CNDTR; //得到当前还剩余多少个数据
                pro = 1 - pro / ((TEXT_LENTH + 2) * 100); //得到百分比
                pro *= 100;    //扩大100倍
                sprintf(str, "%.1f", pro);
                putStr (110, 120, 2, 1, str);
            }
            putStr (110, 120, 2, 1, "100 ");
            putStr(110, 140, 2, 1, "Transimit Finished!");
        }
        i++;
        delay_ms(10);
        if(i == 20)
        {
            LED1 = !LED1; //提示系统正在运行
            i = 0;
        }
    }
}
