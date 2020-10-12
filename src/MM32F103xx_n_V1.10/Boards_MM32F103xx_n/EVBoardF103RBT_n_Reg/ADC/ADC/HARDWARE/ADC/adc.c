#include "adc.h"
#include "delay.h"


//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3
void  Adc_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //使能GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //使能ADC1时钟
    GPIOA->CRL &= 0XFFFFFF0F; //PA1 anolog输入

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1复位
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //复位结束

    /*将ADC配置成单次软件触发转换模式*/
    ADC1->ADCFG |= 7 << 4; //16分频
    ADC1->ADCFG |= 0x7 << 10;
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //单次非周期模式,数据右对齐
    ADC1->ADCR |= ADCR_ADMD_PERIOD;
    ADC1->ADCHS = ADCHS_CHEN1; //使能通道1
    ADC1->ADCFG |= ADCFG_ADEN;//ADC1使能
    ADC1->ADCR |= ADCR_ADST;//开始转换

}
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(void)
{
    u32 temp;
    u16 i, j, val[3];


    for(i = 0; i < 3; i++)
    {
        ADC1->ADCR |= ADCR_ADST;//开始转换
        do
        {
            temp = ADC1->ADDR1;
        }
        while(((temp >> 21) & 0x01) == 0x00);
        val[i] = temp;
    }

    for(i = 0; i < 3; i++)
    {
        for(j = i + 1; j < 3 - i; j++)
        {
            if(val[i] > val[j])
            {
                temp = val[i];
                val[i] = val[j];
                val[j] = temp;
            }
        }
    }
    return val[1] & 0xfff;
}




























