#include "stm32f10x.h"                  // Device header

void Timer_Init(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 10000 - 1;
	TIM_InitStructure.TIM_Prescaler = 7200 - 1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef TIM_NVIC_Initstructure;
	TIM_NVIC_Initstructure.NVIC_IRQChannel = TIM2_IRQn;
	TIM_NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;
	TIM_NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 2;
	TIM_NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&TIM_NVIC_Initstructure);
	
	TIM_Cmd(TIM2, ENABLE);

}

/*
void TIM2_IRQHandler(){
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
*/
