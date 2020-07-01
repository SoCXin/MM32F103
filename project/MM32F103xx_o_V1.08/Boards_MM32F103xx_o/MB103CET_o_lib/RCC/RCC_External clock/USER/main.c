#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

void HSE_Pll_test(unsigned int pllmul);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    /*ѡ���ⲿʱ��Դ��Ϊ��Ƶʱ�ӵĻ���*/
    HSE_Pll_test(RCC_PLLMul_12);                                                
    
    while(1)              		
    {       
    }
}


/********************************************************************************************************
**������Ϣ ��SystemClk_HSEInit (void)                        
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/
void SystemClk_HSEInit(void)
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
    
    RCC_DeInit();
    /*CR�Ĵ���BIT16λ��HSEONλ����1�������������ⲿʱ��HSE��Ϊϵͳʱ��*/
    RCC_HSEConfig(RCC_HSE_ON);
    while(1){
        if(RCC_WaitForHSEStartUp()!=0)
        {
            break;
        }
    }
    
    /*ѡ���ⲿʱ����Ϊϵͳʱ��*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);	
    /*ͨ��PA8 pin �۲�Ƶ��*/    
    RCC_MCOConfig(RCC_MCO_SYSCLK);  					                        
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
void HSE_Pll_test(unsigned int pllmul)
{
    SystemClk_HSEInit();
    /*ѡ���ⲿʱ��Դ��Ϊ��Ƶʱ�ӵĻ���*/
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);                              
    SystemClkPll(pllmul);
}


/********************************************************************************************************
**������Ϣ ��LSE_clk()                      
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 32.786KHz
**������� ����
**������� ����
********************************************************************************************************/
void LSE_clk()
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
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
    /*ʹ��PWR_CR�е�DBP bit*/
    PWR_BackupAccessCmd(ENABLE); 					                            
                                                                                
    RCC_LSEConfig(RCC_LSE_ON);                                                  
                                                                                
    while(1){       
        /*��ѯ�ⲿ32.768KHz��Ƶʱ��ready*/        
        if(RCC->BDCR&0x00000002) 					                            
        {
            break;
        }
    }
    /*ͨ��PA8 pin �۲�Ƶ��*/
    RCC_MCOConfig(RCC_MCO_LSE);                                                 
}


/********************************************************************************************************
**������Ϣ ��HSE_HSI_Change()                      
**�������� ���ⲿʱ�����ڲ�ʱ��֮����л�
**������� ����
**������� ����
********************************************************************************************************/
void HSE_HSI_Change()
{
    /*CSS on*/
    RCC_ClockSecuritySystemCmd(ENABLE);                                         
    SystemClk_HSEInit();
    
    /*close HESON*/
    RCC->CR &= 0xfffeffff; 				                                        
}

