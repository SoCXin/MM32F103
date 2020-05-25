#include "adc.h"
#include "delay.h"



void  Adc_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //使能ADC1时钟

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1复位
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //复位结束

    /*将ADC配置成单次软件触发转换模式*/
    ADC1->ADCFG |= ADCFG_ADCPRE_8 | ADCFG_ADCPRE_10; //8分频
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //单次非周期模式,数据右对齐
    ADC1->ADCR |= ADCR_ADMD_PERIOD;

    ADC1->ADCHS &= 0xffffffe00;	/*屏蔽所有通道*/
    ADC1->ADCHS = ADCHS_SENSOREN;//使能通道8，温度传感器

    ADC1->ADCFG |= ADCFG_TVSEN; //开启温度传感器

    ADC1->ADCFG |= ADCFG_ADEN;//ADC1使能

}


/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get()
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(void)
{
    u16 puiADData;

    ADC1->ADCR |= 0x100;

    while(!(ADC1->ADSTA & 0x01));

    puiADData = ADC1->ADDATA & 0xfff;

    ADC1->ADSTA |= 0x1;
    /*ADCR寄存器的ADST位失能，软件转换结束*/
    return puiADData;
}

/********************************************************************************************************
**函数信息 ：Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
**功能描述 ：获取几次ADC1采样值的平均值
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 Get_Adc_Average(uint8_t times)
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;
    for(t = 0; t < times; t++)
    {
        temp_val += ADC1_SingleChannel_Get();
        for(delay = 0; delay < 100; delay++);
    }
    return temp_val / times;
}
























