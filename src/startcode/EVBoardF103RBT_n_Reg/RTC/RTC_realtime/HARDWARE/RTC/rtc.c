#include "delay.h"
#include "uart.h"
#include "rtc.h"


_calendar_obj calendar;//ʱ�ӽṹ��

//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������
u8 RTC_Init(void)
{
    //����ǲ��ǵ�һ������ʱ��
    u16 temp = 0;
    RCC->APB1ENR |= RCC_APB1RSTR_PWRRST;   //ʹ�ܵ�Դʱ��
    RCC->APB1ENR |= RCC_APB1RSTR_BKPRST;   //ʹ�ܱ���ʱ��
    PWR->CR |= PWR_CR_DBP;         //ȡ��������д����
    if(BKP->DR1 != 0X5050) //��һ������
    {
        RCC->BDCR |= RCC_BDCR_LSEON;       //�����ⲿ��������
        while((!(RCC->BDCR & RCC_BDCR_LSERDY)) && temp < 250) //�ȴ��ⲿʱ�Ӿ���
        {
            temp++;
        };
        if(temp >= 500)return 1; //��ʼ��ʱ��ʧ��,����������
        RCC->BDCR |= RCC_BDCR_RTCSEL_LSE; //LSE��ΪRTCʱ��
        delay_us(1);//С��ʱ���ȴ�ͬ��
        RCC->BDCR |= RCC_BDCR_RTCEN; //RTCʱ��ʹ��
        while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������
        while(!(RTC->CRL & (RTC_CRL_RSF))); //�ȴ�RTC�Ĵ���ͬ��
        RTC->CRH |= RTC_CRH_SECIE;  		 //�������ж�
        while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������

        RTC->CRL |= RTC_CRL_CNF;         //��������
        RTC->PRLH = 0X0000;
        RTC->PRLL = 0x7fff;        //ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767
        RTC->CRL &= ~(RTC_CRL_CNF);         //���ø���
        while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������
        while(!(RTC->CRL & (RTC_CRL_RSF))); //�ȴ�RTC�Ĵ���ͬ��
        RTC_Set(2016, 9, 14, 10, 42, 55); //����ʱ��
        while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������

        BKP->DR1 = 0X5050;
    }
    else //ϵͳ������ʱ
    {
        while(!(RTC->CRL & (RTC_CRL_RSF))); //�ȴ�RTC�Ĵ���ͬ��
        RTC->CRH |= RTC_CRH_SECIE;  		 //�������ж�
        while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������
    }
    MY_NVIC_Init(0, 0, RTC_IRQn, 2); //���ȼ�����
    RTC_Get();//����ʱ��
    return 0; //ok
}
//RTCʱ���ж�
//ÿ�봥��һ��
void RTC_IRQHandler(void)
{
    if(RTC->CRL & RTC_CRL_SECF) //�����ж�
    {
        RTC_Get();//����ʱ��
    }
    if(RTC->CRL & RTC_CRL_ALRF) //�����ж�
    {
        RTC->CRL &= ~(RTC_CRL_ALRF);		//�������ж�
    }
    RTC->CRL &= 0X0FFA;       //�������������жϱ�־
    while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������
}
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//year:���
//����ֵ:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0) //�����ܱ�4����
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1; //�����00��β,��Ҫ�ܱ�400����
            else return 0;
        }
        else return 1;
    }
    else return 0;
}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�
u8 const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; //���������ݱ�
//ƽ����·����ڱ�
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//syear,smon,sday,hour,min,sec��������ʱ����
//����ֵ�����ý����0���ɹ���1��ʧ�ܡ�
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if(syear < 1970 || syear > 2099)return 1;
    for(t = 1970; t < syear; t++)	//��������ݵ��������
    {
        if(Is_Leap_Year(t))seccount += 31622400; //�����������
        else seccount += 31536000;			 //ƽ���������
    }
    smon -= 1;
    for(t = 0; t < smon; t++)	 //��ǰ���·ݵ����������
    {
        seccount += (u32)mon_table[t] * 86400; //�·����������
        if(Is_Leap_Year(syear) && t == 1)seccount += 86400; //����2�·�����һ���������
    }
    seccount += (u32)(sday - 1) * 86400; //��ǰ�����ڵ����������
    seccount += (u32)hour * 3600; //Сʱ������
    seccount += (u32)min * 60;	 //����������
    seccount += sec; //�������Ӽ���ȥ

    //����ʱ��
    RCC->APB1ENR |= RCC_APB1RSTR_PWRRST;   //ʹ�ܵ�Դʱ��
    RCC->APB1ENR |= RCC_APB1RSTR_BKPRST;   //ʹ�ܱ���ʱ��
    PWR->CR |= PWR_CR_DBP;         //ȡ��������д����
    //���������Ǳ����!
    RTC->CRL |= RTC_CRL_CNF; //��������
    RTC->CNTL = seccount & 0xffff;
    RTC->CNTH = seccount >> 16;
    RTC->CRL &= ~(RTC_CRL_CNF); //���ø���

    while(!(RTC->CRL & (RTC_CRL_RTOFF))); //�ȴ�RTC�Ĵ����������

    RTC_Get();//������֮�����һ������
    return 0;
}
//�õ���ǰ��ʱ�䣬���������calendar�ṹ������
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;
    timecount = RTC->CNTH; //�õ��������е�ֵ(������)
    timecount <<= 16;
    timecount += RTC->CNTL;

    temp = timecount / 86400; //�õ�����(��������Ӧ��)
    if(daycnt != temp) //����һ����
    {
        daycnt = temp;
        temp1 = 1970;	//��1970�꿪ʼ
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp >= 366)temp -= 366; //�����������
                else break;
            }
            else temp -= 365;	 //ƽ��
            temp1++;
        }
        calendar.w_year = temp1; //�õ����
        temp1 = 0;
        while(temp >= 28) //������һ����
        {
            if(Is_Leap_Year(calendar.w_year) && temp1 == 1) //�����ǲ�������/2�·�
            {
                if(temp >= 29)temp -= 29; //�����������
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //ƽ��
                else break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1;	//�õ��·�
        calendar.w_date = temp + 1;  	//�õ�����
    }
    temp = timecount % 86400;     		//�õ�������
    calendar.hour = temp / 3600;     	//Сʱ
    calendar.min = (temp % 3600) / 60; 	//����
    calendar.sec = (temp % 3600) % 60; 	//����
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date); //��ȡ����
    return 0;
}
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//year,month,day������������
//����ֵ�����ں�
u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
    u16 temp2;
    u8 yearH, yearL;

    yearH = year / 100;	yearL = year % 100;
    // ���Ϊ21����,�������100
    if (yearH > 19)yearL += 100;
    // ����������ֻ��1900��֮���
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if (yearL % 4 == 0 && month < 3)temp2--;
    return(temp2 % 7);
}
















