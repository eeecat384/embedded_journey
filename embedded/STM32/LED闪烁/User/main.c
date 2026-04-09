#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"

int main(void)
{
	LEDInit();
	while (1){
		LEDlight();
		Delay_ms(200);
		LEDdark();
		Delay_ms(200);
	}	
}
