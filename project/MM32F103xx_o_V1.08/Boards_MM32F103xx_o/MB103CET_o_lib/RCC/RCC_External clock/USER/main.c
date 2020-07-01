#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void HSE_Pll_test(unsigned int pllmul);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    /*选择外部时钟源作为倍频时钟的基础*/
    HSE_Pll_test(RCC_PLLMul_12);                                                
    
    while(1)              		
    {       
    }
}


/********************************************************************************************************
**函数信息 ：SystemClk_HSEInit (void)                        
**功能描述 ：系统时钟初始化函数，初始化之前先复位所有时钟
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SystemClk_HSEInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    
    /*mco  pa8*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   		                        
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
    /*复用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                        
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);
    
    RCC_DeInit();
    /*CR寄存器BIT16位（HSEON位）置1，作用是连接外部时钟HSE作为系统时钟*/
    RCC_HSEConfig(RCC_HSE_ON);
    while(1){
        if(RCC_WaitForHSEStartUp()!=0)
        {
            break;
        }
    }
    
    /*选择外部时钟作为系统时钟*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);	
    /*通过PA8 pin 观察频率*/    
    RCC_MCOConfig(RCC_MCO_SYSCLK);  					                        
}


/********************************************************************************************************
**函数信息 ：SystemClkPll(unsigned int pllmul)                
**功能描述 ：倍频频率作为系统工作时钟
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16 
**输出参数 ：无
********************************************************************************************************/
void SystemClkPll(unsigned int pllmul)
{
    /*使能PLL*/
    RCC_PLLCmd(ENABLE); 				                                        
    
    while(1){
        /*查询PLL ready*/
        if(RCC->CR&0x02000000) 				                                    
        {
            break;
        }
    }
    
    /*时钟等待状态*/
    FLASH_SetLatency(FLASH_Latency_2);			                                 
   
    /*通过PA8 pin 观察频率*/    
    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  	
    /*选择PLL时钟作为系统时钟*/    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	                            	
}


/********************************************************************************************************
**函数信息 ：HSE_Pll_test(unsigned int pllmul)                      
**功能描述 ：外部晶振作为时钟源
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16 
**输出参数 ：无
********************************************************************************************************/
void HSE_Pll_test(unsigned int pllmul)
{
    SystemClk_HSEInit();
    /*选择外部时钟源作为倍频时钟的基础*/
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);                              
    SystemClkPll(pllmul);
}


/********************************************************************************************************
**函数信息 ：LSE_clk()                      
**功能描述 ：内部低频时钟并通过示波器观察PA8 pin 32.786KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LSE_clk()
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    
    /*mco  pa8*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   			                    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*复用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    /*使能PWR_CR中的DBP bit*/
    PWR_BackupAccessCmd(ENABLE); 					                            
                                                                                
    RCC_LSEConfig(RCC_LSE_ON);                                                  
                                                                                
    while(1){       
        /*查询外部32.768KHz低频时钟ready*/        
        if(RCC->BDCR&0x00000002) 					                            
        {
            break;
        }
    }
    /*通过PA8 pin 观察频率*/
    RCC_MCOConfig(RCC_MCO_LSE);                                                 
}


/********************************************************************************************************
**函数信息 ：HSE_HSI_Change()                      
**功能描述 ：外部时钟与内部时钟之间的切换
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void HSE_HSI_Change()
{
    /*CSS on*/
    RCC_ClockSecuritySystemCmd(ENABLE);                                         
    SystemClk_HSEInit();
    
    /*close HESON*/
    RCC->CR &= 0xfffeffff; 				                                        
}

