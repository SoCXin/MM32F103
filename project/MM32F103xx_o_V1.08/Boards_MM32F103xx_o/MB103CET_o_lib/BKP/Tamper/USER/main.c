#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

u16 BKPDataReg[10] =
{
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10
}; 

void LED_Init(void);
void BKP_DATA(void);
void NVIC_Configuration(void);
void WriteToBackupReg(u16 FirstBackupData);
u8 CheckBackupReg(u16 FirstBackupData);

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))		

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))		

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，LED点亮，触碰PC13，LED2点亮
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{ 
    /*设置中断优先级分组2*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /*初始化与LED连接的硬件接口*/    
    LED_Init();		  		                                                    
    BKP_DATA();
    while(1)
    {
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
**函数信息 KP_DATA(void)           
**功能描述 : 开启侵入检测，并往BKP数据寄存器写数据，并判断是否正确
**输入参数 ：无
**输出参数 ：无 
********************************************************************************************************/
void BKP_DATA(void)
{
    NVIC_Configuration();
    
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    
    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);
    /* Disable Tamper pin */
    BKP_TamperPinCmd(DISABLE);
    
    /* Disable Tamper interrupt */
    BKP_ITConfig(DISABLE);
    
    /* Tamper pin active on low level */
    BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);
    
    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();
    
    /* Enable Tamper interrupt */
    BKP_ITConfig(ENABLE);
    
    /* Enable Tamper pin */
    BKP_TamperPinCmd(ENABLE);
    
    /* Write data to Backup DRx registers */
    WriteToBackupReg(0xA53C);
    
    /* Check if the written data are correct */
    if(CheckBackupReg(0xA53C) == 0x00)
    {
        /* Turn on LED1 */
        LED1_ON();//数据读写正确
    }
    else
    {
        /* Turn on LED3 */
        LED3_ON();//数据读写错误
    }   
}


/********************************************************************************************************
**函数信息 ：NVIC_Configuration(void)        
**功能描述 : 侵入检测中断初始化
**输入参数 ：无
**输出参数 ：无 
********************************************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Enable TAMPER IRQChannel */
    NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
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
        BKP_WriteBackupRegister(BKPDataReg[index], FirstBackupData);
    }  
}


/********************************************************************************************************
**函数信息 ：TAMPER_IRQHandler(void)                    
**功能描述 ：判断PC13是否产生侵入检测，PC13引脚一旦发生电平改变，就会进入中断
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
u8 CheckBackupReg(u16 FirstBackupData)
{
    u32 index = 0;
    index=BKP->DR1;
    for (index = 0; index < 10; index++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[index]) != FirstBackupData)
        {
            return (index + 1);
        }
    }
    
    return 0;
}
