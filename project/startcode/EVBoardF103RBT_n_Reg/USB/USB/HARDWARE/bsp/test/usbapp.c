#include "usb.h"
#include "sys.h"
#include "HAL_device.h"
/********************************************************************************************************
**函数信息 ： USB_GPIO
**功能描述 ： USB IO初始化
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
void USB_GPIO(void)
{
    RCC->APB2ENR |= RCC_APB2RSTR_IOPARST; //RCC->APB2ENR|=1<<2;  //使能PORTA时钟
    GPIOA->CRH &= 0XFFF00FFF;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->BRR = 0x0020;
}
/********************************************************************************************************
**函数信息 ： usb_test
**功能描述 ： USB外设初始化
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
void usb_test(void)
{
    //	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,ENABLE); 				//USB CLK EN
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;
    printf("This is a USB Demo \r\n");

    USB_GPIO();
#ifdef USB_INTR_MODE
    //	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;		//配置USB中断向量
    //	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //	NVIC_Init(&NVIC_InitStructure);
    MY_NVIC_Init(0, 1, USB_HP_CAN1_TX_IRQn, 0); //组2，最低优先级

#endif
    USB_Init();
    printf("This is %x \r\n", *(u32*)0x40010804);
}




#define CLICK_NO		0
#define CLICK_LEFT		1
#define CLICK_RIGHT		2
#define CLICK_MID		3
/********************************************************************************************************
**函数信息 ： mouse_ctrl
**功能描述 ： 鼠标报告数据
**输入参数 ： char clickEnt(鼠标点击事件)
char x(x坐标移动相对位置)
char y(y坐标移动相对位置)
char rol(滚轮移动相对位置)
**输出参数 ：
********************************************************************************************************/
void mouse_ctrl(char clickEnt, char x, char y, char rol)
{
    USB->rEP1_FIFO = clickEnt;									//单击事件
    USB->rEP1_FIFO = x;											//x坐标相对移动位置 (-128~127)
    USB->rEP1_FIFO = y;											//y坐标相对移动位置 (-128~127)
    USB->rEP1_FIFO = rol;										//滚轮相对移动位置 (-128~127)
    USB->rEP1_CTRL = 0x84;										//send 4 bytes packet
}


#define KEY_NONE 	0x00
#define KEY_UP    	0x1A
#define KEY_DOWN  	0x16
#define KEY_LEFT  	0x04
#define KEY_RIGHT 	0x07
#define KEY_A		0x18
#define KEY_B		0x0C
#define KEY_C		0x0D
#define KEY_D		0x0E

#define KEY_L_CTRL	0x01
#define KEY_L_SHIFT	0x02
#define KEY_L_ALT	0x04
#define KEY_L_WIN	0x08
#define KEY_R_CTRL	0x10
#define KEY_R_SHIFT	0x20
#define KEY_R_ALT	0x40
#define KEY_R_WIN	0x80
/********************************************************************************************************
**函数信息 ： keyBoard_value
**功能描述 ： 键盘报告数据
**输入参数 ： char spcKey(功能键)
nomKeyx(常用按键键值)
**输出参数 ：
********************************************************************************************************/
void keyBoard_value(char spcKey, char nomKey0, char nomKey1, char nomKey2,
                    char nomKey3, char nomKey4, char nomKey5)
{
    USB->rEP1_FIFO = spcKey;					//功能键
    USB->rEP1_FIFO = 0;							//保留
    USB->rEP1_FIFO = nomKey0;					//常用按键键值,按键1为0x59 数字依次类推
    USB->rEP1_FIFO = nomKey1;					//常用按键键值
    USB->rEP1_FIFO = nomKey2;					//常用按键键值
    USB->rEP1_FIFO = nomKey3;
    USB->rEP1_FIFO = nomKey4;
    USB->rEP1_FIFO = nomKey5;
    USB->rEP1_CTRL = 0x88;						//send 8 bytes packet
}
/********************************************************************************************************
**函数信息 ： keyBoard_ctrl
**功能描述 ： 键盘报告数据防重复发送
**输入参数 ： char spcKey(功能键)
nomKeyx(常用按键键值)
**输出参数 ：
********************************************************************************************************/
void keyBoard_ctrl(char spcKey, char nomKey0, char nomKey1, char nomKey2,
                   char nomKey3, char nomKey4, char nomKey5)
{
    keyBoard_value(spcKey, nomKey0, nomKey1, nomKey2, nomKey3, nomKey4, nomKey5);	//键值写入缓冲寄存器

    while(USB->rEP1_CTRL & 0x80);
    keyBoard_value(KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE); //清键值

    while(USB->rEP1_CTRL & 0x80);
}

