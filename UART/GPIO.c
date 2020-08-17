#include "GPIO.h"


void configLED(uint8_t LED_PIN, GPIO_TypeDef* GPIOx) {
	
	// enable clock for given GPIOx peripheral
	if 		(GPIOx == GPIOA)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	else if (GPIOx == GPIOB)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	else if (GPIOx == GPIOC)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	else if (GPIOx == GPIOD)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	else if (GPIOx == GPIOE)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	
	// Set LED pin to push-pull low-speed output.
	GPIOx->MODER &= ~(0x3UL << (LED_PIN*2));		// input mode (reset state)
	GPIOx->MODER |= (0x1UL << (LED_PIN*2));		// output mode
	GPIOx->OTYPER &= ~(1UL << LED_PIN);			// Push-pull output
	GPIOx->PUPDR &= ~(3UL << (LED_PIN*2));		// No pull-up/pull-down resistors
}

void configButton(uint8_t BUTTON_PIN, GPIO_TypeDef* GPIOx) {

	//Initialize the GPIOC pins.
	//PC13 should be set to 'input' mode with pull-up.
	GPIOx->MODER &= ~(0x3UL << (BUTTON_PIN*2));	// input mode (reset state)
	GPIOx->PUPDR &= ~(0x3UL << (BUTTON_PIN*2));	// no pull-up or pull-down (reset)
	GPIOx->PUPDR |= (0x1UL << (BUTTON_PIN*2));	// pull-up activated
	
}

