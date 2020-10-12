#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

#define TIMPSC          374                                                     //set Timer prescaler
#define TIMARR          255                                                     //set Timer period
#define PWMWAVELENGTH   80                                                      //PWM_WAVE_list length
#define DELAYTIME       50                                                      //set delay period

u8 PWMListAddr;
u16 delaynum ;  

u8 PWM_Wave[PWMWAVELENGTH] = {
    1,1,1,2,2,2,2,3,3,4,4,5,6,7,8,9,10,12,14,17,19,23,25,29,
    33,39,44,52,59,70,80,94,107,125,143,167,191,223,255,255,
    255,223,191,167,143,125,107,94,80,70,59,52,44,39,33,29,
    25,23,19,17,14,12,10,9,8,7,6,5,4,4,3,3,2,2,2,2,1,1,1,1
};

void initGPIO_TIM3(void);
void initTIM3(u16 psc, u16 arr);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
int main(void)
{
    initGPIO_TIM3();
    initTIM3(TIMPSC, TIMARR);
    
    while(1){
    }
}


/********************************************************************************************************
**函数信息 ：void initGPIO_TIM3()                          
**功能描述 ：TIM3 GPIO初始化配置
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void initGPIO_TIM3(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      
    /*TIM3_CH1*/    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);   			
    
}


/********************************************************************************************************
**函数信息 ：void initTIM3(u16 psc, u16 arr)                         
**功能描述 ：TIM3 初始化配置
**输入参数 ：u16 psc:set TIM prescaler(0-65535),u16 arr:set TIM period(0-65535)
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void initTIM3(u16 psc, u16 arr)
{ 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /*TIM3 interrupt source*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
    /*Set interrupt priority*/    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;   
    /*IRQ channel is enabled*/    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    /*Initialize NVIC register*/    
    NVIC_Init(&NVIC_InitStructure);                                             
            
    /*Set auto-load register value*/    
    TIM_TimeBaseStructure.TIM_Period = arr;
    /*Set the prescaler value*/    
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 
    /*Set the clock segmentation*/    
    TIM_TimeBaseStructure.TIM_ClockDivision=0;
    /*Set the counting mode*/    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    /*Initialize the TIM3 register*/    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                             
                                                                                
    TIM_OCStructInit(&TIM_OCInitStructure);     
    /*Configure PWM mode*/    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    /*Output enabled*/    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    /*Initial pulse width is set 20*/    
    TIM_OCInitStructure.TIM_Pulse = 20;  
    /*Less than CCR1 value is low*/    
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    /*Initialize TIM3_OC1 register*/    
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                                    
          
    /*Preload enable*/          
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);                           
         
    /*Enable TIM Reload Register ARR*/         
    TIM_ARRPreloadConfig(TIM3, ENABLE); 
    /*TIM3 update interrupt is enabled*/    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                                  
    /*Enable TIM3*/                                                                           
    TIM_Cmd(TIM3, ENABLE);                                                      
}


/********************************************************************************************************
**函数信息 oid TIM3_IRQHandler(void)                          
**功能描述 ：TIM3 中断函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void TIM3_IRQHandler(void)
{
    /*Judge update interrupt flag*/
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){                                            
        delaynum ++;
        /*delay DELAYTIME * 1ms*/
        if(delaynum >= DELAYTIME){ 
            /*Modify the timer compare register*/            
            TIM_SetCompare1(TIM3, PWM_Wave[PWMListAddr]);
            /*Look up the table to the next member variable*/            
            PWMListAddr++; 
            /*Check the table to find the last one, redirected to the header*/            
            if( PWMListAddr >=  PWMWAVELENGTH){                                 
                PWMListAddr = 0;
            }
            /*Reset cycle count flag*/
            delaynum = 0;                                                       
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}


