#include "dma.h"
#include "delay.h"



u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ���
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    RCC->AHBENR |= RCC_AHBENR_DMAEN;			//����DMA1ʱ��
    delay_ms(5);				//�ȴ�DMAʱ���ȶ�
    DMA_CHx->CPAR = cpar; 	 	//DMA1 �����ַ
    DMA_CHx->CMAR = (u32)cmar; 	//DMA1,�洢����ַ
    DMA1_MEM_LEN = cndtr;      	//����DMA����������
    DMA_CHx->CNDTR = cndtr;    	//DMA1,����������
    DMA_CHx->CCR = 0X00000000;	//��λ
    DMA_CHx->CCR &= ~DMA_CCR1_DIR;  		//�Ӵ洢����
    DMA_CHx->CCR &= ~DMA_CCR1_CIRC;  		//��ͨģʽ
    DMA_CHx->CCR &= ~DMA_CCR1_PINC; 		//�����ַ������ģʽ
    DMA_CHx->CCR |= DMA_CCR1_MINC; 	 	//�洢������ģʽ
    DMA_CHx->CCR &= ~DMA_CCR1_PSIZE_0; 	 	//�������ݿ��Ϊ8λ
    DMA_CHx->CCR &= ~DMA_CCR1_MSIZE_0; 		//�洢�����ݿ��8λ
    DMA_CHx->CCR |= DMA_CCR1_PL_0; 		//�е����ȼ�
    DMA_CHx->CCR &= ~DMA_CCR1_MEM2MEM; 		//�Ǵ洢�����洢��ģʽ

    MY_NVIC_Init(3, 3, DMA1_Channel5_IRQn, 1); //��2��������ȼ�
    DMA_CHx->CCR |= 0x2; //����������ж�
}
//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
    DMA_CHx->CCR &= ~(DMA_CCR1_EN);     //�ر�DMA����
    DMA_CHx->CNDTR = DMA1_MEM_LEN; //DMA1,����������
    DMA_CHx->CCR |= DMA_CCR1_EN;        //����DMA����
}



























