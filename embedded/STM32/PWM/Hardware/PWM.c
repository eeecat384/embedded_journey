#include "stm32f10x.h"                  // Device header

void PWMInit()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1分频
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 20000 - 1;				//重装值ARR
	TIM_InitStructure.TIM_Prescaler = 72 - 1;			//预分频PSC
	TIM_InitStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	//频率 = 72M(时钟频率) / (PSC*ARR*分频)
	
	TIM_OCInitTypeDef TIMOCInitStructure;
	TIM_OCStructInit(&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIMOCInitStructure.TIM_Pulse = 0;						//捕获比较CCR 取值0x0000到0xFFFF
	TIM_OC1Init(TIM2, &TIMOCInitStructure);
	//PWM频率：	Freq = 72M(时钟频率) / (PSC*ARR*分频)
	//PWM占空比：	Duty = CCR / ARR
	//PWM分辨率：	Reso = 1 / ARR

	TIM_Cmd(TIM2, ENABLE);
	
}
