#include "sys.h"
#include "uart_nvic.h"
#include "delay.h"


void uart_nvic_init(u32 bound)
{

    u32 tempBaud;

    //-------------------------------------------------
    RCC->APB2ENR |= 1 << 2; //使能PORTA口时钟   #define RCC_APB2Periph_UART1            ((uint32_t)0x00004000) #define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
    /*
    APB2外设时钟使能寄存器（RCC_APB2ENR）
    偏移地址：0x18
    复位值：0x0000 0000
    访问：无等待周期，字，半字和字节访问
    注：当外设时钟没有启动时，软件不能读出外设寄存器的数值
    位2
    IOPAEN：IO端口A时钟使能
    由软件置’1’或清’0’
    0：IO端口A时钟关闭；
    1：IO端口A时钟开启。
    */
    RCC->APB2ENR |= 1 << 14; //使能串口时钟
    /*
    位14
    UART1EN：UART1时钟使能
    由软件置’1’或清’0’
    0：UART1时钟关闭；
    1：UART1时钟开启。
    */
    GPIOA->CRH &= 0XFFFFF00F; //IO状态设置
    GPIOA->CRH |= 0X000008B0; //IO状态设置
    /*
    8.2.2 端口配置高寄存器（GPIOx_CRH）(x=A..E)
    偏移地址：0x04
    复位值：0x4444 4444
    配置PA9为 B = 10 11  MODE9[1:0] = 11[5:4] ：输出模式，最大速度50MHz, CNF9[1:0] = 10[7:6] ：复用功能推挽输出模式
    配置PA10 为8 = 10 00 MODE10[1:0] = 00[9:8] ：输入模式（复位后的状态）, CNF10[1:0] = 10[7:6] ：10：上拉/下拉输入模式

    MODEy[1:0]:端口x的模式位（y=8…15）
    软件通过这些位配置相应的I/O端口，请参考表15端口位配置表
    00：输入模式（复位后的状态）
    01：输出模式，最大速度10MHz
    10：输出模式，最大速度20MHz
    11：输出模式，最大速度50MHz

    CNFy[1:0]:端口x配置位（8…15）
    软件通过这些位配置相应的I/O端口，请参考表15端口位配置表。
    在输入模式（MODE[1:0]==00）:
    00：模拟输入模式
    01：浮空输入模式
    10：上拉/下拉输入模式
    11：保留
    在输出模式（MODE[1:0]>00）:
    00：通用推挽输出模式
    01：通用开漏输出模式
    10：复用功能推挽输出模式
    11：复用功能开漏输出模式

    */
    RCC->APB2RSTR |= 1 << 14; //复位串口1
    RCC->APB2RSTR &= ~(1 << 14); //停止复位
    //-------------------------------------------------
    //波特率设置
    // 	UART1->BRR=mantissa; // 波特率设置
    /* Determine the uart_baud*/
    //		    tempBaud = (72000000 / 16) /(bound);

    tempBaud = (SystemCoreClock / 16) / (bound);
    /* Write to UART BRR */
    UART1->BRR = tempBaud;
    UART1->FRA = (SystemCoreClock / 16) % (bound);
    UART1->CCR |= 0X30; //1位停止,无校验位.
    //-------------------------------------------------
    //使能接收中断
    UART1->GCR = 0X19;			//收发使能	UART1->CCR|=1<<5;    //接收缓冲区非空中断使能
    /*
    23.5.5 UART 中断使能寄存器(UART_IER)
    偏移地址：0x10
    复位值：0x0000
    位1
    RXIEN:接收缓冲中断使能位
    1=中断使能
    0=中断禁止
    23.5.6 UART 中断清除寄存器(UART_ICR)
    偏移地址：0x14
    复位值：0x0000
    位1
    RXICLR: 接收中断清除位
    1=中断清除
    0=中断没有清除
    */

    //使能接收中断
    UART1->GCR = 0X19;			//收发使能	UART1->CCR|=1<<5;    //接收缓冲区非空中断使能
    UART1->IER = 0X2;			//接收中断使能
    /*
    23.5.5 UART 中断使能寄存器(UART_IER)
    偏移地址：0x10
    复位值：0x0000
    位1
    RXIEN:接收缓冲中断使能位
    1=中断使能
    0=中断禁止
    */
    UART1->ICR = 0X2;			//清接收中断
    /*
    23.5.6 UART 中断清除寄存器(UART_ICR)
    偏移地址：0x14
    复位值：0x0000
    位1
    RXICLR: 接收中断清除位
    1=中断清除
    0=中断没有清除
    */
    MY_NVIC_Init(3, 3, UART1_IRQn, 2); //组2，最低优先级
}

u8 UART_RX_BUF[UART_REC_LEN];     //接收缓冲,最大UART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART_RX_STA = 0;     //接收状态标记

void UART1_IRQHandler(void)
{
    u8 res;
    if((UART1->ISR & 0x0002) != (uint16_t)RESET)	//接收到数据
    {
        res = (uint16_t)(UART1->RDR & (uint16_t)0x00FF);
        UART1->ICR |= 2;//清接收中断 //		UART_ClearITPendingBit(UART1,UART_IT_RXIEN);

        if((UART_RX_STA & 0x8000) == 0) //接收未完成
        {
            if(UART_RX_STA & 0x4000) //接收到了0x0d
            {
                if(res != 0x0a)UART_RX_STA = 0; //接收错误,重新开始
                else UART_RX_STA |= 0x8000;	//接收完成了
            }
            else  //还没收到0X0D
            {
                if(res == 0x0d)UART_RX_STA |= 0x4000;
                else
                {
                    UART_RX_BUF[UART_RX_STA & 0X3FFF] = res;
                    UART_RX_STA++;
                    if(UART_RX_STA > (UART_REC_LEN - 1))UART_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}

