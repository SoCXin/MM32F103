#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us延时倍乘数*/		
static u8  fac_us=0; 
/*ms延时倍乘数,在ucos下,代表每个节拍的ms数*/
static u16 fac_ms=0; 

void delay_init(void);
void LED_Init(void);
void delay_ms(u16 nms);
u8 BKP_DATA(void);
void WriteToBackupReg(u16 FirstBackupData);
u8 CheckBackupReg(u16 FirstBackupData);

u16 BKPDataReg[10] =
{
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10
}; 
u16 testdata[10] = 
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a
};

#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))		

#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))		

#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	

/********************************************************************************************************
**函数信息 ：int main(void)                     
**功能描述 ：
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)    
{
    u8 flag=0;
    delay_init();
    LED_Init();
    /*往BKP写数据，如果成功则LED满闪，失败则快速闪烁*/
    flag = BKP_DATA();		 
    
    if(flag==0)	
    {
        while(1)             
        {
            LED1_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            /*BKP数据读写成功，慢闪*/
            delay_ms(5000);
        }
    }
    else
    {
        while(1)             
        {
            LED1_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            /*BKP数据读写失败，快闪*/
            delay_ms(500);
        }
    }
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
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    LED1_OFF();
    LED3_OFF();
    LED4_OFF();
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
**函数信息 ：BKP_DATA(void)         
**功能描述 : BKP数据读写测试，判断写和读的数据是否一致
**输入参数 ：FirstBackupData
**输出参数 ：i
********************************************************************************************************/
u8 BKP_DATA(void)
{
    u8 i;
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    
    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);
    
    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();
    /*往BKP寄存器里写数据*/   
    WriteToBackupReg(0x55);
    /*判断数据写数据是否正确*/
    i=CheckBackupReg(0x55);
    return i;
}


/********************************************************************************************************
**函数信息 ：WriteToBackupReg(u16 FirstBackupData)           
**功能描述 : 往BKP数据寄存器里写数据
**输入参数 ：FirstBackupData
**输出参数 ：无 
********************************************************************************************************/
void WriteToBackupReg(u16 FirstBackupData)
{
    uint32_t index = 0;
    
    for (index = 0; index < 10; index++)
    {
        /*往对应寄存器里写数据*/
        BKP_WriteBackupRegister(BKPDataReg[index], testdata[index]);            
    }  
}


/********************************************************************************************************
**函数信息 ：CheckBackupReg(u16 FirstBackupData)                 
**功能描述 : 判断写数据是否正确	
**输入参数 ：FirstBackupData
**输出参数 ：写错误：index+1；写成功：0
********************************************************************************************************/
u8 CheckBackupReg(u16 FirstBackupData)
{
    u32 index = 0;
    
    for (index = 0; index < 10; index++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[index]) != testdata[index])
        {
            return (index + 1);
        }
    }
    
    return 0;
}
