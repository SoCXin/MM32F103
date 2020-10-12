#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

/*us��ʱ������*/		
static u8  fac_us=0; 
/*ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��*/
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

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����1
#define WK_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//��ȡ����4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

/********************************************************************************************************
**������Ϣ ��main(void)                       
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u8 t=0;
	
    delay_init();
    /*��ʼ����LED���ӵ�Ӳ���ӿ�*/
    LED_Init();
    /*��ʼ���밴�����ӵ�Ӳ���ӿ�*/    
    KEY_Init();          	                                                    
                                                                            
    while(1)                                                                
    {   
        /*�õ���ֵ*/        
        t=KEY_Scan(0);		                                                        
        switch(t)                                                               
        {	
            /*K1Ĭ�ϲ���PC13,Ĭ������reset��λ���������԰���K1�Ḵλ*/        
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
**������Ϣ ��void delay_init(void)                         
**�������� ����ʼ���ӳٺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void delay_init(void)
{
    /*ѡ���ⲿʱ��  HCLK/8*/
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    /*Ϊϵͳʱ�ӵ�1/8*/    
    fac_us=SystemCoreClock/8000000;				                               
    /*��OS��,����ÿ��ms��Ҫ��systickʱ���� */
    fac_ms=(u16)fac_us*1000;					                              
}

/********************************************************************************************************
**������Ϣ ��void delay_ms(u16 nms)    
**�������� ����ʱnms
**������� ��nms
**������� ��
**    ��ע ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nms<=0xffffff*8*1000/SYSCLK,72M������,nms<=1864 
********************************************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    /*ʱ�����(SysTick->LOADΪ24bit)*/    
    SysTick->LOAD=(u32)nms*fac_ms;
    /*��ռ�����*/    
    SysTick->VAL =0x00;	
    /*��ʼ����*/    
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	                               
    do{
        temp=SysTick->CTRL;
    }
    /*�ȴ�ʱ�䵽��*/
    while((temp&0x01)&&!(temp&(1<<16)));		                            
    /*�رռ�����*/
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	                           
    /*��ռ�����*/    
    SysTick->VAL =0X00;       					                               
}


/********************************************************************************************************
**������Ϣ ��void KEY_Init(void)    
**�������� ��������ʼ������ 
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void KEY_Init(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    /*ʹ��GPIOA,GPIOB,GPIOCʱ��*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
    
    /*PC13��K1*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    /*���ó���������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*��ʼ��GPIOC13*/    
    GPIO_Init(GPIOC, &GPIO_InitStructure);			                            
      
    /*PA0,K2��WK_UP��*/    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;	
    /*���ó���������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    /*��ʼ��GPIOA0*/    
    GPIO_Init(GPIOA, &GPIO_InitStructure);			                            
    
    /*PB10,PB11,K3,K4*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;
    /*���ó���������*/    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    /*��ʼ��GPIOB.10,11*/    
    GPIO_Init(GPIOB, &GPIO_InitStructure);				                          
}


/********************************************************************************************************
**������Ϣ ��u8 KEY_Scan(u8 mode)   
**�������� ������������ 
**������� ��mode  mode:0,��֧��������;1,֧��������;
**������� ��0,û�а�������;KEY1_PRES,KEY1����;WKUP_PRES,WK_UP����;KEY3_PRES,KEY3����;KEY4_PRES��KEY4����
**    ��ע ��
********************************************************************************************************/
u8 KEY_Scan(u8 mode)
{	 
    /*�������ɿ���־*/
    static u8 key_up=1;
    /*֧������*/ 
    if(mode)key_up=1;  
    if(key_up&&(KEY1==0||WK_UP==1||KEY3==0||KEY4==0))
    {
        /*ȥ����*/
        delay_ms(10);
        key_up=0;
        if(KEY1==0)return KEY1_PRES;
        else if(WK_UP==1)return WKUP_PRES; 
        else if(KEY3==0)return KEY3_PRES;
        else if(KEY4==0)return KEY4_PRES;
    }else if(KEY1==1&&KEY3==1&&KEY4==1&&WK_UP==0)key_up=1;
    /*�ް�������*/    
    return 0;
}
