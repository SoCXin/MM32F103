#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void HSI_Pll_test(unsigned int pllmul);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    /*选择内部时钟源作为倍频时钟的基础*/
    HSI_Pll_test(RCC_PLLMul_6);	                                                
    
    while(1)              		
    {
        
    }
}


/********************************************************************************************************
**函数信息 ：SystemClk_HSIInit (void)                        
**功能描述 ：系统时钟初始化函数，初始化之前先复位所有时钟
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SystemClk_HSIInit()
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
    /*CR寄存器BIT0位（HSION位）置1，作用是连接内部时钟HSI作为系统时钟*/
    RCC_HSICmd(ENABLE);
    
    /*通过PA8 pin 观察频率*/
    RCC_MCOConfig(RCC_MCO_SYSCLK); 
    /*选择内部时钟作为系统时钟*/    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);					                    
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
void HSI_Pll_test(unsigned int pllmul)
{
    
    SystemClk_HSIInit();
    /*选择内部时钟源作为倍频时钟的基础*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul);                              
    SystemClkPll(pllmul);
}

/********************************************************************************************************
**函数信息 ：LSI_clk()                      
**功能描述 ：内部低频时钟并通过示波器观察PA8 pin 40KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LSI_clk()
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
    //open lsi on
    RCC_LSICmd(ENABLE);
    
    while(1)
    {
        /*查询内部低频时钟ready*/
        if(RCC->CSR&0x00000002) 						                        
        {                                                                       
            break;                                                              
        }                                                                       
    }                                                                           
    /*通过PA8 pin 观察频率*/                                                                      
    RCC_MCOConfig(RCC_MCO_LSI);  						                        
}                                                                               
