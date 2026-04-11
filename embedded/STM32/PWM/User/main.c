#include "stm32f10x.h"                
#include "OLED.h"
#include "PWM.h"

uint16_t num;

int main(void)
{
	OLED_Init();
	PWMInit();
	int CRR = ;
	
	while (1)
	{
		
		OLED_ShowNum(1, 1, num, 10);
	}
}
