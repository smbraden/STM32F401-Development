/*
Description:	Echos user's keyboard input via UART interface
				
Author:			Sonja Braden
	
IDE:			Keil uVision 5

uVision 
Dependencies:	CMSIS Core, STM32F4xx/STM32F401xE Startup files
*/

#include "stm32f401xe.h"
#include "pinDefines.h"
#include "UART.h"

// Global variables
volatile uint32_t msTicks = 0;	// store millisecond ticks. Won't overflow for ~50 days
volatile uint8_t data = 'U';

// Function prototypes
void initSysTick(void);
void SysTick_Handler(void);
void delay_ms(uint32_t);
void USART6_IRQnHandler(void);

int main(void) {
	
	configUART();
	printString("Hello World!\n");
	initSysTick();

	
	while (1) {
	
	}
}

//----------- USART6 interrupt handler---------//


void USART6_IRQnHandler(void) {

	data = USART6->DR;						// Copy new data into the buffer.
	USART6->DR = data;						// echo the character
	while(!(USART6->SR & USART_SR_TC)){}	// wait until char sent

	// "RXNE bit set by hardware when the content of the 
	// RDR shift register has been transferred to the USART_RDR register"
}


//-----------SysTick Functipons----------------------//

// Configures SysTick to generate 1 ms interrupts
void initSysTick(void) {
	
	// 1 interrupt per millisecond
	SysTick_Config(SystemCoreClock / 1000);	
	// generating 1 interupt per (SystemCoreClock / 1000) 'ticks'

}


// SysTick interrupt Handler
// Will only response to its interrupt if initSysTick() is called first 
void SysTick_Handler(void)  {
	msTicks++;	
}


// Can only be called if initSysTick() is called first
void delay_ms(uint32_t delayTime) {
	
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < delayTime) {}
}




