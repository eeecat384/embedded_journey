#include <stm32f10x.h>

void LEDInit(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStructure);
}

void LEDlight(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LEDdark(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}
