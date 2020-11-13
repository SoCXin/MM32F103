#include "tsensor.h"
#include "delay.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������
//ADC ��������			   
////////////////////////////////////////////////////////////////////////////////// 	  
 
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3	
void T_Adc_Init(void)  //ADCͨ����ʼ��
{
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��GPIOA,ADC1ͨ��ʱ��
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //��Ƶ����6ʱ��Ϊ72M/6=12MHz

   	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
 
	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
	/* Initialize the ADC_ContinuousConvMode member */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	 /* Initialize the ADC_ExternalTrigConv member */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	ADC1->ADCHS&=0xffffffe00;	/*��������ͨ��*/


	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 0, 0); 

	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	ADC1->ADCFG|=0x04;

 
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1


}
void  T_Get_Adc_Average(uint8_t ADC_Channel_x,unsigned int *puiADData)
{
	unsigned int  uiCnt=0;
	/*ADCR�Ĵ�����ADSTλʹ�ܣ��������ת��*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
	
	do
	{
		/*����ǰ���ݼĴ���ֵ*/
		*puiADData=ADC1->ADDATA;
		uiCnt++;
	}
	while( (((*puiADData>>21)&0x01)==0) &&  (uiCnt<100)); 
	/*1.�ж��Ƿ�ʱ 2.�ж��Ƿ�����Ч���� 3.�ж��Ƿ�Ϊ��ǰͨ����*/
	if((uiCnt<100)&&(((*puiADData>>21)&0x01)==1)&&(((*puiADData>>16)&0x0f)==ADC_Channel_x))
	{	
		*puiADData&=0xfff;
	}
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	/*ADCR�Ĵ�����ADSTλʧ�ܣ����ת������*/
	ADC_SoftwareStartConvCmd(ADC1, DISABLE); 
	
} 	   

