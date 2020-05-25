#include "timer.h"
#include "led.h"

//定时器2通道1输入捕获配置
//arr：自动重装值
//psc：时钟预分频数
void TIM2_Cap_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= RCC_APB1RSTR_TIM2RST;   	//TIM2 时钟使能
    RCC->APB2ENR |= RCC_APB2RSTR_IOPARST;    	//使能PORTA时钟

    GPIOA->CRL &= 0XFFFFFFF0;	//PA0 清除之前设置
    GPIOA->CRL |= 0x8;	//PA0 输入
    GPIOA->ODR |= ~GPIO_ODR_ODR0;		//PA0 下拉

    TIM2->ARR = arr;  		//设定计数器自动重装值
    TIM2->PSC = psc;  		//预分频器

    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;		//CC1S=01 	选择输入端 IC1映射到TI1上
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_0; 		//IC1F=0001 配置输入滤波器 以Fck_int采样，2个事件后有效

    TIM2->CCER |= TIM_CCER_CC1E; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

    TIM2->DIER |= TIM_DIER_CC1IE;   	//允许捕获中断
    TIM2->DIER |= TIM_DIER_UIE;   	//允许更新中断
    TIM2->CR1 |= TIM_CR1_CEN;    	//使能定时器2
    MY_NVIC_Init(2, 0, TIM2_IRQn, 2); //抢占2，子优先级0，组2
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
//[5:0]:捕获高电平后溢出的次数
u8  TIM2CH1_CAPTURE_STA = 0;	//输入捕获状态
u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
//定时器2中断服务程序
void TIM2_IRQHandler(void)
{
    u16 tsr;
    tsr = TIM2->SR;
    if((TIM2CH1_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(tsr & 0X01) //溢出
        {
            if(TIM2CH1_CAPTURE_STA & 0X40) //已经捕获到高电平了
            {
                if((TIM2CH1_CAPTURE_STA & 0X3F) == 0X3F) //高电平太长了
                {
                    TIM2CH1_CAPTURE_STA |= 0X80; //标记成功捕获了一次
                    TIM2CH1_CAPTURE_VAL = 0XFFFF;
                }
                else TIM2CH1_CAPTURE_STA++;
            }
        }
        if(tsr & 0x02) //捕获1发生捕获事件
        {
            if(TIM2CH1_CAPTURE_STA & 0X40)		//捕获到一个下降沿
            {
                TIM2CH1_CAPTURE_STA |= 0X80;		//标记成功捕获到一次高电平脉宽
                TIM2CH1_CAPTURE_VAL = TIM2->CCR1;	//获取当前的捕获值.
                TIM2->CCER &= ~(TIM_CCER_CC1P);			//CC1P=0 设置为上升沿捕获
            }
            else  								//还未开始,第一次捕获上升沿
            {
                TIM2CH1_CAPTURE_VAL = 0;
                TIM2CH1_CAPTURE_STA = 0X40;		//标记捕获到了上升沿
                TIM2->CNT = 0;					//计数器清空
                TIM2->CCER |= TIM_CCER_CC1P; 				//CC1P=1 设置为下降沿捕获
            }
        }
    }
    TIM2->SR = 0; //清除中断标志位
}


















