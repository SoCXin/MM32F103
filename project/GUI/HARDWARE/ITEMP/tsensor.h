#ifndef __TSENSOR_H
#define __TSENSOR_H	
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������
//ADC ��������			   
////////////////////////////////////////////////////////////////////////////////// 	  
 		 
#define ADC_CH_TEMP  	ADC_Channel_16 //�¶ȴ�����ͨ��
   
u16  T_Get_Temp(void);  //ȡ���¶�ֵ
void T_Adc_Init(void); //ADCͨ����ʼ��
void ADC1_SingleChannel_Get(uint8_t ADC_Channel_x,unsigned int *puiADData);//�õ�ĳ��ͨ��10�β�����ƽ��ֵ 
void  T_Get_Adc_Average(uint8_t ADC_Channel_x,unsigned int *puiADData);

#endif 
