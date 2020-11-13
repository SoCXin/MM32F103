#include  "sys.h"
#include "controller.h"
#include "alarm.h"

void Controller_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA,PC�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 //IO���ٶ�Ϊ10MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��
	 GPIO_SetBits(GPIOA,GPIO_Pin_4);						 //PA4 �����
    
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ0MHz
	 GPIO_SetBits(GPIOA,GPIO_Pin_2); 						 //����� 
}

void Controller_RelayAction(u8 id, u8 op)
{
	 switch(id)
	 {
		 case MASK_SLOT1:
			 if (op == kOn)
			 {
				  SLOT0 = SLOT_ON; //�͵�ƽ��
			 }else if(op ==kOff)
			 {
				  SLOT0 = SLOT_OFF;
			 }
			 break;
		 case MASK_SLOT2:
			 if (op == kOn)
				 SLOT1 = SLOT_ON;
			 else
				 SLOT1 = SLOT_OFF;
			 break;
		 case (MASK_SLOT1 | MASK_SLOT2):
			 if (op == kOn)
			 {
				 SLOT0 = SLOT_ON;
				 SLOT1 = SLOT_ON;
			 }
			 else
			 {
				 SLOT0 = SLOT_OFF;
				 SLOT1 = SLOT_OFF;
			 }
			 break;
		 default:
			 break;
	 }
		 
}
