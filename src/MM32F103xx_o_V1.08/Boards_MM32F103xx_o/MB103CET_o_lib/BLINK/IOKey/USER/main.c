#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0; 

void delay_init(void);
void delay_ms(u16 nms);
void LED_Init(void);
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))		

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))		

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键1
#define WK_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

/********************************************************************************************************
**函数信息 ：main(void)                       
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 t=0;
	
    delay_init();
    /*初始化与LED连接的硬件接口*/
    LED_Init();
    /*初始化与按键连接的硬件接口*/    
    KEY_Init();          	                                                    
                                                                            
    while(1)                                                                
    {   
        /*得到键值*/        
        t=KEY_Scan(0);		                                                        
        switch(t)                                                               
        {	
            /*K1默认不连PC13,默认连接reset复位按键，所以按下K1会复位*/        
            case KEY1_PRES:                                                             
                LED1_TOGGLE();  
                break;
            case KEY3_PRES:				
                LED3_TOGGLE();
                break;
            case KEY4_PRES:				
                LED4_TOGGLE();
                break;
            default:
                delay_ms(10);	
        } 
    }
}

/********************************************************************************************************
**函数信息 ：LED_Init(void)                        
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    LED1_OFF();
    LED3_OFF();
    LED4_OFF();
}

/********************************************************************************************************
**函数信息 ：void delay_init(void)                         
**功能描述 ：初始化延迟函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void delay_init(void)
{
    /*选择外部时钟  HCLK/8*/
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /*为系统时钟的1/8*/    
    fac_us=SystemCoreClock/8000000;				                               
    /*非OS下,代表每个ms需要的systick时钟数 */
    fac_ms=(u16)fac_us*1000;					                              
}

/********************************************************************************************************
**函数信息 ：void delay_ms(u16 nms)    
**功能描述 ：延时nms
**输入参数 ：nms
**输出参数 ：
**    备注 ：SysTick->LOAD为24位寄存器,所以,最大延时为:nms<=0xffffff*8*1000/SYSCLK,72M条件下,nms<=1864 
********************************************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    /*时间加载(SysTick->LOAD为24bit)*/    
    SysTick->LOAD=(u32)nms*fac_ms;
    /*清空计数器*/    
    SysTick->VAL =0x00;	
    /*开始倒数*/    
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
    do{
        temp=SysTick->CTRL;
    }
    /*等待时间到达*/
    while((temp&0x01)&&!(temp&(1<<16)));		                            
    /*关闭计数器*/
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
    /*清空计数器*/    
    SysTick->VAL =0X00;       					                               
}


/********************************************************************************************************
**函数信息 ：void KEY_Init(void)    
**功能描述 ：按键初始化函数 
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void KEY_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    /*使能GPIOA,GPIOB,GPIOC时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
    
    /*PC13，K1*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    /*设置成上拉输入*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*初始化GPIOC13*/    
    GPIO_Init(GPIOC, &GPIO_InitStructure);			                            
      
    /*PA0,K2（WK_UP）*/    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	
    /*设置成下拉输入*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    /*初始化GPIOA0*/    
    GPIO_Init(GPIOA, &GPIO_InitStructure);			                            
    
    /*PB10,PB11,K3,K4*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;
    /*设置成上拉输入*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*初始化GPIOB.10,11*/    
    GPIO_Init(GPIOB, &GPIO_InitStructure);				                          
}


/********************************************************************************************************
**函数信息 ：u8 KEY_Scan(u8 mode)   
**功能描述 ：按键处理函数 
**输入参数 ：mode  mode:0,不支持连续按;1,支持连续按;
**输出参数 ：0,没有按键按下;KEY1_PRES,KEY1按下;WKUP_PRES,WK_UP按下;KEY3_PRES,KEY3按下;KEY4_PRES，KEY4按下
**    备注 ：
********************************************************************************************************/
u8 KEY_Scan(u8 mode)
{	 
    /*按键按松开标志*/
    static u8 key_up=1;
    /*支持连按*/ 
    if(mode)key_up=1;  
    if(key_up&&(KEY1==0||WK_UP==1||KEY3==0||KEY4==0))
    {
        /*去抖动*/
        delay_ms(10);
        key_up=0;
        if(KEY1==0)return KEY1_PRES;
        else if(WK_UP==1)return WKUP_PRES; 
        else if(KEY3==0)return KEY3_PRES;
        else if(KEY4==0)return KEY4_PRES;
    }else if(KEY1==1&&KEY3==1&&KEY4==1&&WK_UP==0)key_up=1;
    /*无按键按下*/    
    return 0;
}
