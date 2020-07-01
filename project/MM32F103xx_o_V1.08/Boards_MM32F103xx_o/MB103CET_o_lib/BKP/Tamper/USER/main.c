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
**������Ϣ ��int main (void)                          
**�������� ��������LED����������PC13��LED2����
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{ 
    /*�����ж����ȼ�����2*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /*��ʼ����LED���ӵ�Ӳ���ӿ�*/    
    LED_Init();		  		                                                    
    BKP_DATA();
    while(1)
    {
    }	
}


/********************************************************************************************************
**������Ϣ ��LED_Init(void)                        
**�������� ��LED��ʼ��
**������� ����
**������� ����
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
**������Ϣ �BKP_DATA(void)           
**�������� : ���������⣬����BKP���ݼĴ���д���ݣ����ж��Ƿ���ȷ
**������� ����
**������� ���� 
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
        LED1_ON();//���ݶ�д��ȷ
    }
    else
    {
        /* Turn on LED3 */
        LED3_ON();//���ݶ�д����
    }   
}


/********************************************************************************************************
**������Ϣ ��NVIC_Configuration(void)        
**�������� : �������жϳ�ʼ��
**������� ����
**������� ���� 
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
**������Ϣ ��WriteToBackupReg(u16 FirstBackupData)           
**�������� : ��BKP���ݼĴ�����д����
**������� ��FirstBackupData
**������� ���� 
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
**������Ϣ ��TAMPER_IRQHandler(void)                    
**�������� ���ж�PC13�Ƿ���������⣬PC13����һ��������ƽ�ı䣬�ͻ�����ж�
**������� ����
**������� ����
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
