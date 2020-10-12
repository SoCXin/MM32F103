#include "HAL_conf.h"
#include "HAL_device.h"
#include "HAL_can.h"
#include "stdio.h"

#define CAN_ID_STD                   0
#define CAN_ID_EXT                   1
#define CAN_DATA_FRAME       					0
#define CAN_REMOTE_FRAME              1
#define CAN_500K                     500000
#define CAN_250K                     250000
#define TX_CANID 									 0x172
typedef enum {
  StandardFrame_SingleFilter=0,
  ExtendedFrame_SingleFilter=1,
  StandardFrame_DoubleFilter=2,
  ExtendedFrame_DoubleFilter=3,
}CAN_Mode;

typedef struct
{
  uint32_t CANID;  		
  uint32_t CANIDtype;
  
  uint8_t RTR;     
  
  uint8_t DLC;     
  uint8_t Data[8]; 
}CanTxMsg;
CanTxMsg CAN_TX_Config = {TX_CANID,CAN_ID_STD,CAN_DATA_FRAME,8,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
CanPeliRxMsg     CanPeliRxMsgStructure;
char printBuf[100];
unsigned char Time_Flag_10ms = 0 ;
unsigned char Time_Flag_100ms = 0 ;
unsigned char Time_Flag_1s = 0 ;

void uart_initwBaudRate(u32 bound);
void UartSendGroup(u8* buf,u16 len);
void CANM_Init(void);
void CAN_Config(u32 CAN_Pre,CAN_Mode ID,u32 idCode1,u32 idCode2,u32 mask1,u32 mask2);
void Send_CANFrame(CanTxMsg* TxMessage);
void Tim2_UPCount_test(u16 Prescaler,u32 Period);
void Tim2_UPStatusOVCheck_test(void);
/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� �������󣬽��յ���ȷ���ĺ󴮿ڴ�ӡ����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{   
  u8 i = 0;
  uart_initwBaudRate(9600);
  CANM_Init();
  CAN_Config(CAN_250K,ExtendedFrame_DoubleFilter,0x172,0x325,0x00,0x00);		//CAN����˫�˲���
  Tim2_UPCount_test(47,1000); //1msʱ��
  while(1)              				
  {
    if(Time_Flag_1s)																	//ÿ���ѯ����ӡһ��
    {
      Time_Flag_1s = 0;
      CAN_Peli_Receive(&CanPeliRxMsgStructure);			//�������һ�γɹ���CANͨ�ŵ���Ϣ��CanPeliRxMsgStructure�ṹ����
      UartSendGroup((u8*)printBuf, sprintf(printBuf,"CANID:0x%x  Data",CanPeliRxMsgStructure.ID));
      for(i=0;i<8;i++)															
      {
        UartSendGroup((u8*)printBuf, sprintf(printBuf,"%x",CanPeliRxMsgStructure.Data[i]));
      }
      UartSendGroup((u8*)printBuf, sprintf(printBuf,"\r\n"));//����
    }
  }			
  
}

/********************************************************************************************************
**������Ϣ ��void uart_initwBaudRate(u32 bound)     
**�������� ��UART��ʼ��
**������� ��bound
**������� ��
**    ��ע ��
********************************************************************************************************/
void uart_initwBaudRate(u32 bound)
{
  /*GPIO�˿�����*/
  GPIO_InitTypeDef GPIO_InitStructure;
  UART_InitTypeDef UART_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /*UART1_TX   GPIOA.9*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  /*�����������*/    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		                     
  GPIO_Init(GPIOA, &GPIO_InitStructure);				                        
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
  
  /*UART1_RX  GPIOA.10��ʼ��*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  /*��������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	                 
  GPIO_Init(GPIOA, &GPIO_InitStructure);				                     
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
  
  /*UART ��ʼ������*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	                   
  
  /*���ڲ�����*/
  UART_InitStructure.UART_BaudRate = bound;
  /*�ֳ�Ϊ8λ���ݸ�ʽ*/    
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  /*һ��ֹͣλ*/    
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  /*����żУ��λ*/    
  UART_InitStructure.UART_Parity = UART_Parity_No;
  /*��Ӳ������������*/    
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  /*�շ�ģʽ*/
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                
  /*��ʼ������1*/
  UART_Init(UART1, &UART_InitStructure); 	
  /*�������ڽ����ж�*/    
  UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
  /*ʹ�ܴ���1*/    
  UART_Cmd(UART1, ENABLE);                    			                    
}
/********************************************************************************************************
**������Ϣ ��void UartSendByte(u8 dat)      
**�������� ��UART��������
**������� ��dat
**������� ��
**    ��ע ��
********************************************************************************************************/
void UartSendByte(u8 dat)
{
  UART_SendData( UART1, dat);
  while(!UART_GetFlagStatus(UART1,UART_FLAG_TXEPT));
}


/********************************************************************************************************
**������Ϣ ��void UartSendGroup(u8* buf,u16 len)     
**�������� ��UART��������
**������� ��buf,len
**������� ��
**    ��ע ��
********************************************************************************************************/
void UartSendGroup(u8* buf,u16 len)
{
  while(len--)
    UartSendByte(*buf++);
}
/********************************************************************************************************
**������Ϣ ��void CANM_Init(void)
**�������� ��CAN��ʼ��
**������� ��None
**������� ��None
**    ��ע ��
********************************************************************************************************/
void CANM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
#if 1
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   									//CAN RX PB8
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   									//CAN TX PB9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_9);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_9);	
#else
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;   								//CAN RX PA11
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   								//CAN RX PA12
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_9);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_9);	
#endif
}

/********************************************************************************************************
**������Ϣ ��void CAN_Config(u32 CAN_Pre,CAN_Mode ID,u32 idCode1,u32 idCode2,u32 mask1,u32 mask2)
**�������� ��CAN����
**������� ��CAN_Pre:CANͨ������
CAN_250K  CAN_500K
fMode:CANͨ��ģʽ
StandardFrame_SingleFilter=0:��׼֡�����˲���ģʽ
StandardFrame_SingleFilter=1:��չ֡�����˲���ģʽ
ExtendedFrame_SingleFilter=2:��׼֡��˫�˲���ģʽ
ExtendedFrame_DoubleFilter=3:��չ֡��˫�˲���ģʽ
idCode1�����մ���1
idCode2�����մ���2
mask1�����մ�������λ1
mask2�����մ�������λ2
**������� ��None
**    ��ע ��
********************************************************************************************************/
void CAN_Config(u32 CAN_Pre,CAN_Mode ID,u32 idCode1,u32 idCode2,u32 mask1,u32 mask2)
{
  CAN_Peli_InitTypeDef          CAN_Peli_InitStructure;
  RCC_ClocksTypeDef             RCC_Clocks;
  u32 idCodeTemp1,idMaskTemp1;
  u32 idCodeTemp2,idMaskTemp2;
  CAN_Peli_FilterInitTypeDef    CAN_Peli_FilterInitStructure;
  
  CAN_ResetMode_Cmd(CAN1,ENABLE);// ���븴λģʽ
  CAN_Mode_Cmd(CAN1, CAN_PELIMode);//CAN����Peliģʽ
  
  RCC_GetClocksFreq(&RCC_Clocks);
  
  CAN_Peli_StructInit(&CAN_Peli_InitStructure);
  
  CAN_AutoCfg_BaudParam( &CAN_Peli_InitStructure, RCC_Clocks.PCLK1_Frequency, CAN_Pre);
  
  CAN_Peli_InitStructure.SAM = RESET;//������
  CAN_Peli_InitStructure.STM = DISABLE;//DISABLE;//�Բ�ģʽ
  CAN_Peli_InitStructure.LOM = DISABLE;//ֻ��ģʽ
  //	CAN_Peli_InitStructure.EWLR = 0x96;
  CAN_Peli_Init(&CAN_Peli_InitStructure);
  
  switch(ID)
  {
  case StandardFrame_SingleFilter:
    idCodeTemp1 = idCode1<<(3 + 18);
    idMaskTemp1 = mask1<<(3 + 18);
    CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Singal;
    CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1>>16)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp1>>8)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId3 = (idCodeTemp1>>0)&0xff;
    
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = ((idMaskTemp1>>16)&0xff)|0x1f;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = ((idMaskTemp1>>8)&0xff)|0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = ((idMaskTemp1>>0)&0xff)|0xff;
    break;
  case ExtendedFrame_SingleFilter:
    idCodeTemp1 = idCode1<<3;
    idMaskTemp1 = mask1<<3;
    CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Singal;
    CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1>>16)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp1>>8)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId3 = idCodeTemp1&0xff;
    
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = ((idMaskTemp1>>16)&0xff);
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = ((idMaskTemp1>>8)&0xff);
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = ((idMaskTemp1>>0)&0xff);
    break;        
  case StandardFrame_DoubleFilter:
    idCodeTemp1 = idCode1<<(3 + 18);
    idMaskTemp1 = mask1<<(3 + 18);
    idCodeTemp2 = idCode2<<(3 + 18);
    idMaskTemp2 = mask2<<(3 + 18);
    CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Double;
    CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1>>16)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp2>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId3 = (idCodeTemp2>>16)&0xff;
    
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = ((idMaskTemp1>>16)&0xff)|0x1f;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = (idMaskTemp2>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = ((idMaskTemp2>>16)&0xff)|0x1f;
    break; 
  case ExtendedFrame_DoubleFilter:
    idCodeTemp1 = idCode1<<(3);
    idMaskTemp1 = mask1<<(3);
    idCodeTemp2 = idCode2<<(3);
    idMaskTemp2 = mask2<<(3);
    CAN_Peli_FilterInitStructure.AFM = CAN_FilterMode_Double;
    CAN_Peli_FilterInitStructure.CAN_FilterId0 = (idCodeTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId1 = (idCodeTemp1>>16)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId2 = (idCodeTemp2>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterId3 = (idCodeTemp2>>16)&0xff;
    
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId0 = (idMaskTemp1>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId1 = (idMaskTemp1>>16)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId2 = (idMaskTemp2>>24)&0xff;
    CAN_Peli_FilterInitStructure.CAN_FilterMaskId3 = (idMaskTemp2>>16)&0xff;
    break; 
  default:
    break;
  }
  CAN_Peli_FilterInit(&CAN_Peli_FilterInitStructure);
  CAN_Peli_ITConfig(CAN_IT_RI,ENABLE);
  CAN_ResetMode_Cmd(CAN1,DISABLE);//�˳���λģʽ,���빤��ģʽ
}
/********************************************************************************************************
**������Ϣ ��void CANM_Init(void)
**�������� ��CAN��ʼ��
**������� ��None
**������� ��None
**    ��ע ��
********************************************************************************************************/
void Send_CANFrame(CanTxMsg* TxMessage)
{
  CanPeliTxMsg     CanPeliTxMsgStructure;
  uint32_t ID=0;
  u32 i;
  if(TxMessage->CANIDtype)  //��չ֡
  {
    ID=TxMessage->CANID <<3;
    
    CanPeliTxMsgStructure.FF   = 0x01; 
    CanPeliTxMsgStructure.IDLL = (ID >> 0)&0xff;
    CanPeliTxMsgStructure.IDLH = (ID >> 8)&0xff;
    CanPeliTxMsgStructure.IDHL = (ID >> 16)&0xff;
    CanPeliTxMsgStructure.IDHH = (ID >> 24)&0xff;
  }
  else												//��׼֡
  {
    ID=TxMessage->CANID << 21;
    
    CanPeliTxMsgStructure.FF   = 0x00;
    CanPeliTxMsgStructure.IDHL = (ID >>16)&0xff;
    CanPeliTxMsgStructure.IDHH = (ID >>24)&0xff;
  }
  CanPeliTxMsgStructure.DLC  = TxMessage->DLC;
  CanPeliTxMsgStructure.RTR  = TxMessage->RTR;
  for(i = 0;i < 8; i ++ )
  {
    CanPeliTxMsgStructure.Data[i] = *(TxMessage->Data + i);
  }
  CAN_Peli_Transmit(&CanPeliTxMsgStructure);
}
/********************************************************************************************************
**������Ϣ ��void TIM2_IRQHandler(void)                  
**�������� ����ʱ��2�жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void TIM2_IRQHandler(void)
{
  static unsigned int cnt;
  cnt++;
  if(cnt>=1000)
  {
    cnt = 0;
    Time_Flag_1s = 1;
  }
  /*�����ֶ�����жϱ�־λ*/
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
}

/********************************************************************************************************
**������Ϣ ��void Tim2_UPCount_test(u16 Prescaler,u32 Period)                 
**�������� �����ö�ʱ��2���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim2_UPCount_test(u16 Prescaler,u32 Period)
{
  TIM_TimeBaseInitTypeDef TIM_StructInit;
  NVIC_InitTypeDef NVIC_StructInit;
  
  /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK1(PCLK1δ��Ƶʱ����Ƶ,������PCLK1��Ƶ���),��ѡ����ʱ��Դ*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  /*ARR�Ĵ���ֵ*/
  TIM_StructInit.TIM_Period=Period;   
  /*Ԥ��Ƶֵ*/    
  TIM_StructInit.TIM_Prescaler=Prescaler;                                     
  /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/  
  /*������Ƶֵ*/    
  TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;   
  /*����ģʽ*/    
  TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                          
  TIM_StructInit.TIM_RepetitionCounter=0;
  
  TIM_TimeBaseInit(TIM2, &TIM_StructInit);
  
  /*����ʱ��2�����ж�*/
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  TIM_Cmd(TIM2, ENABLE);	
  
  /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  
  /*���ö�ʱ��2�ж�ͨ�������ȼ� */
  NVIC_StructInit.NVIC_IRQChannel=TIM2_IRQn;
  NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
  NVIC_StructInit.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_StructInit.NVIC_IRQChannelSubPriority=1;
  
  NVIC_Init(&NVIC_StructInit);
  
  
}
/********************************************************************************************************
**������Ϣ ��Tim2_UPStatusOVCheck_test(void)                       
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim2_UPStatusOVCheck_test(void)
{
  /*�ȴ��¼����±�־λ*/
  while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET);
  /*���¼���־λ*/
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

