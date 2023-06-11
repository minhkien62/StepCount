#include <stm32f10x.h>
#include "systick.h"

uint32_t msTick = 0;

void SysTick_Init(void) {
	SysTick->LOAD = SystemCoreClock / (1000-1); //configured the SysTick to count in 1ms
	/* Select Core Clock & Enable SysTick & Enable Interrupt */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk |SysTick_CTRL_ENABLE_Msk;
		
	NVIC_SetPriority(SysTick_IRQn, 1);
		NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void) {
		SysTick->CTRL |= SysTick_CTRL_COUNTFLAG;
		msTick++;
}
 
void delay(uint32_t ms) {
		ms += msTick;
		while(msTick < ms);
	
}
