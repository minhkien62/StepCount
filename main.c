#include <stm32f10x.h>
#include <math.h>
#include <stdio.h>

#include "i2c.h"
#include "mpu6050.h"
#include "lcd.h"
#include "systick.h"

#define BUTTON_PAUSE (1u<<6)
#define BUTTON_RESET (1u<<7)
#define LED_RED (1u<<5)
#define LED_BLUE (1u<<4)

void GPIO_Config(void);
void TIM2_Config(void);

uint8_t mode = 0;
uint16_t stepCount = 0;
double accelBefore = 0;
int time = 0;


int main(void) {
			GPIO_Config();
			SysTick_Init();
			I2C1_Init();
			MPU6050_init();	
			lcd_init();
			TIM2_Config();
			char buf[17];

			lcd_put_cur(0, 0);
			lcd_string("Steps: 0");

			while(1) {
			}
}


void GPIO_Config(void) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		
	
			//Init LED_GREEN at PA4	  
		GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |= GPIO_CRL_MODE4_0;
		//Init LED_RED at PA5
		GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= GPIO_CRL_MODE5_0;
	
		//Init for BUTTON_PAUSE pin
		GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);  // clear bits
		GPIOA->CRL |= GPIO_CRL_CNF6_1;  									// input push-pull
		GPIOA->ODR |= GPIO_ODR_ODR6;  										// PA6 is in Pull UP mode
		//Interrupt configyre for BUTTON_PAUSE
		AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI6_PA;					// Interrupt at port A
		EXTI->IMR |= BUTTON_PAUSE; 												// Disable Int mask
		EXTI->FTSR |= BUTTON_PAUSE;												// Trigger at Falling Edge
		
		//Init for BUTTON_RESET pin
		GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);  // clear bits
		GPIOA->CRL |= GPIO_CRL_CNF7_1;  									// input push-pull
		GPIOA->ODR |= GPIO_ODR_ODR7;  										// PA6 is in Pull UP mode
		//Interrupt configyre for BUTTON_PAUSE
		AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI7_PA;					// Interrupt at port A
		EXTI->IMR |= BUTTON_RESET; 												// Disable Int mask
		EXTI->FTSR |= BUTTON_RESET;												// Trigger at Falling Edge
		
		//Enable Interrupt
		NVIC_SetPriority(EXTI9_5_IRQn, 3);								
		NVIC_EnableIRQ (EXTI9_5_IRQn);		
}

void EXTI9_5_IRQHandler(void) {	
	if (EXTI->PR & BUTTON_PAUSE) {
			if (mode == 0) {
					mode = 1;

					GPIOA->ODR |= LED_RED;
					lcd_clear();
					lcd_put_cur(0, 0);

					char buf[20];
					lcd_string("Steps: ");
					sprintf(buf, "%d", stepCount);
					lcd_string(buf);
					lcd_put_cur(1, 0);
					lcd_string("State: Pause");
				
			} else {
					GPIOA->ODR &= ~LED_RED;
					mode = 0;
					lcd_clear();
					lcd_put_cur(0, 0);

					char buf[20];
					lcd_string("Steps: ");
					sprintf(buf, "%d", stepCount);
					lcd_string(buf);
			}
			delay(100);
			EXTI->PR |= BUTTON_PAUSE;
	} else if (EXTI->PR & BUTTON_RESET) {
					stepCount = 0;
						lcd_clear();
					lcd_put_cur(0, 0);

					lcd_string("Steps: 0");
			delay(100);
			EXTI->PR |= BUTTON_RESET;
	}
}

void TIM2_Config(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
    TIM2->PSC = 3999; // 16bit
    TIM2->ARR = 399;                    			// chuy ky = 22.2 ms =1/( 72e6/(4e3x4e2) )
    TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE; 	// enable autoreload
		TIM2->EGR |= TIM_EGR_UG;  								// create an update event
		TIM2->DIER |= TIM_DIER_UIE;         			// enable TIM2 interrupt
		
		NVIC_SetPriority(TIM2_IRQn, 2);
		NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;  				// Clear the update interrupt flag
				
				if (mode == 0) {					
							float accel = getAccel();
							if (accel - accelBefore > 6000){
								stepCount++;  }
								accelBefore = accel;  							
								lcd_put_cur(0, 0);
								char buf[20];
								lcd_string("Steps: ");
								sprintf(buf, "%d", stepCount);
								lcd_string(buf);
							}
						
						if (time == 45) {
								GPIOA->ODR ^= LED_BLUE;	//dao bit o den blue
								time = 0;
						} else {
								time++;
						}
			}
				
 }
