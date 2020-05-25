#include "adc.h"
#include "delay.h"



void  Adc_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //使能ADC1时钟

    RCC->APB2RSTR |= RCC_APB2RSTR_ADC1RST; //ADC1复位
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST); //复位结束

    /*将ADC配置成单次软件触发转换模式*/
    ADC1->ADCFG |= 7 << 4; //16分频
    ADC1->ADCFG |= 0x7 << 10;
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD | ADCR_ADMD_CONTINUE | ADCR_ALIGN_LEFT); //单次非周期模式,数据右对齐
    ADC1->ADCR |= ADCR_ADMD_PERIOD;

    ADC1->ADCHS &= 0xffffffe00;	/*屏蔽所有通道*/
    ADC1->ADCHS = ADCHS_SENSOREN;//使能通道8，温度传感器

    ADC1->ADCFG |= ADCFG_TVSEN; //开启温度传感器

    ADC1->ADCFG |= ADCFG_ADEN;//ADC1使能

}
//获得ADC值
u16 Get_Adc(void)
{

    u32 temp;
    u16 i, j, val[3];

    for(i = 0; i < 3; i++) //开始转换
    {


        ADC1->ADCR |= ADCR_ADST;
        do
        {
            temp = ADC1->ADDATA;
        }
        while(((temp >> 21) & 0x01) == 0);
        val[i] = temp & 0xfff;

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

    ADC1->ADCR &= ~ADCR_ADST; //结束转换
    return val[1] & 0xfff;
}




























