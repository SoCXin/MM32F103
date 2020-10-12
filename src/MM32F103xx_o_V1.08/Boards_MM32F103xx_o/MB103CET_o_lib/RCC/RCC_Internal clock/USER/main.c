#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void HSI_Pll_test(unsigned int pllmul);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    /*ѡ���ڲ�ʱ��Դ��Ϊ��Ƶʱ�ӵĻ���*/
    HSI_Pll_test(RCC_PLLMul_6);	                                                
    
    while(1)              		
    {
        
    }
}


/********************************************************************************************************
**������Ϣ ��SystemClk_HSIInit (void)                        
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/
void SystemClk_HSIInit()
{
    
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    /*mco  pa8*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   			                    
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;  
    /*�����������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		                    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);
    /*CR�Ĵ���BIT0λ��HSIONλ����1�������������ڲ�ʱ��HSI��Ϊϵͳʱ��*/
    RCC_HSICmd(ENABLE);
    
    /*ͨ��PA8 pin �۲�Ƶ��*/
    RCC_MCOConfig(RCC_MCO_SYSCLK); 
    /*ѡ���ڲ�ʱ����Ϊϵͳʱ��*/    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);					                    
}

/********************************************************************************************************
**������Ϣ ��SystemClkPll(unsigned int pllmul)                
**�������� ����ƵƵ����Ϊϵͳ����ʱ��
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16 
**������� ����
********************************************************************************************************/
void SystemClkPll(unsigned int pllmul)
{
    /*ʹ��PLL*/
    RCC_PLLCmd(ENABLE); 				                                        
    
    while(1){
        /*��ѯPLL ready*/
        if(RCC->CR&0x02000000) 				                                    
        {
            break;
        }
    }
    
    /*ʱ�ӵȴ�״̬*/
    FLASH_SetLatency(FLASH_Latency_2);			                                 
   
    /*ͨ��PA8 pin �۲�Ƶ��*/    
    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  	
    /*ѡ��PLLʱ����Ϊϵͳʱ��*/    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	                            	
}

/********************************************************************************************************
**������Ϣ ��HSE_Pll_test(unsigned int pllmul)                      
**�������� ���ⲿ������Ϊʱ��Դ
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16 
**������� ����
********************************************************************************************************/
void HSI_Pll_test(unsigned int pllmul)
{
    
    SystemClk_HSIInit();
    /*ѡ���ڲ�ʱ��Դ��Ϊ��Ƶʱ�ӵĻ���*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul);                              
    SystemClkPll(pllmul);
}

/********************************************************************************************************
**������Ϣ ��LSI_clk()                      
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 40KHz
**������� ����
**������� ����
********************************************************************************************************/
void LSI_clk()
{
    
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    /*mco  pa8*/
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  				                
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*�����������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 			                
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);
    //open lsi on
    RCC_LSICmd(ENABLE);
    
    while(1)
    {
        /*��ѯ�ڲ���Ƶʱ��ready*/
        if(RCC->CSR&0x00000002) 						                        
        {                                                                       
            break;                                                              
        }                                                                       
    }                                                                           
    /*ͨ��PA8 pin �۲�Ƶ��*/                                                                      
    RCC_MCOConfig(RCC_MCO_LSI);  						                        
}                                                                               
