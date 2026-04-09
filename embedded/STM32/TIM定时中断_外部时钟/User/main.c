#include "stm32f10x.h"                
#include "OLED.h"
#include "Timer.h"

uint16_t num;

int main(void)
{
	OLED_Init();
	Timer_Init();
	
	while (1)
	{
		OLED_ShowNum(1, 1, num, 10);
		OLED_ShowNum(2, 1, TIM_GetCounter(TIM2),  10);
	}
}

void TIM2_IRQHandler(){
	num ++;
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
