#include "UART.h"
#include "pinDefines.h"



//-------- UART Functions --------------------//


void transmitByte(char ch)
{
	while(!(USART6->SR & USART_SR_TC)){}	// Wait for empty flag
	USART6->DR = ch;						// Set data to Transmit Data Register
}
// "TC: Transmission complete
// Bit is set by hardware if the transmission of a data frame is complete and if TXE is set."



void printString(const char msg[]) {
	
	uint8_t i = 0;
	while(msg[i]) {
		transmitByte(msg[i]);
		i++;
	}
}

// This is specific to USART6 with PC6 and PC7
void configUART(void) {
	
	// Off while configuring
	USART6->CR1 &= ~(USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );
		
	//-----------------Eanble UART Clock------------------------------------//
	
	// clock enable UART peripheral
	RCC->APB2ENR &= ~RCC_APB2ENR_USART6EN;
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	// clock enable GPIO
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
		
	//-----------------Configure UART GPIO--------------------------------//
	
	// configure pin PC6 transmitter (TX6) USART6
	GPIOC->MODER	&=	~(0x3UL << (2 * TX6));			// Reset
	GPIOC->OTYPER	&=	~(0x1UL << TX6);				// Output push-pull (reset state)
	GPIOC->OSPEEDR	&=	~(0x3UL << (2 * TX6));			// Reset
	GPIOC->PUPDR	&=	~(0x3UL << (2 * TX6));			// No pull-up or pull-down
	
	// configure pin PC7 reciever (RX6) USART6	
	// Besides "Alt Function", these should be default
	GPIOC->MODER	&=	~(0x3UL << (2 * RX6)); 			// Reset
	GPIOC->MODER	|=	(0x2UL << (2 * RX6));			// Alternate Function mode
	GPIOC->PUPDR	&=	~(0x3UL << (2 * RX6));			// No pull-up or pull-down
	

	// GPIO alternate function configuration					
    GPIOC->AFR[0] &= ~(GPIO_AFRL_AFRL6 & GPIO_AFRL_AFRL7);		// AFR[0] is for the lower pins' register
    GPIOC->AFR[0] |=  GPIO_AFRL_AFRL6_3 | GPIO_AFRL_AFRL7_3;	// UART6 Alt function bits: AF8 = 0b1000
	
	//--------------------Redundant default settings---------------------//
	
	// set UART word length to 8	M[1:0] = 00: 1 Start bit, 8 data bits, n stop bits
	USART6->CR1 &= ~USART_CR1_M;
	// No parity 					PCE[0] = 0: Parity control disabled
	USART6->CR1 &= ~USART_CR1_PCE;
	// 1 stop bit STOP[0] = 0		STOP[1:0] = 00: 1 stop bit
	USART6->CR2 &= ~USART_CR2_STOP;
	
	//--------------------Baud rate(*)-----------------------------//
	
	USART6->CR1 |= ~USART_CR1_OVER8;	// Select oversampling by 16 (OVER8=0)
	
	/*	Equation 1: Baud rate for standard USART (SPI mode included)
		For baud of 9600 with OVER8 = 0 at 16MHz:
	
		USARTDIV = fck/(8 * (2 - OVER8) * baud) = 16,000,000/(8 * 2 * 9600) ~= 104.16667
		
		binary:	0000 0110 1000 . 0010
		hex:	Mantissa = 0x068	Fraction = 0x2 	
	*/
	
	USART6->BRR = (0x068UL << USART_BRR_DIV_Mantissa) | (0x2UL << USART_BRR_DIV_Fraction);
	
	// Equivalently: USART6->BRR = (0x682A << USART_BRR_DIV_FRACTION_Pos)
	
	//--------------------------------------------------------------//
	
	// Enable USART interrupts whenever ORE=1 or RXNE=1 in the USART_ISR 
	USART6->CR1 |= USART_CR1_RXNEIE;

	NVIC_SetPriority(USART6_IRQn, 0x0UL);
	NVIC_EnableIRQ(USART6_IRQn);

	// Enable the USART 
	// UE, TE, RE ("UART Enable", "Reciever Enable", "Transmitter Enable" bits) 
	USART6->CR1 |= (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE);
	
}
