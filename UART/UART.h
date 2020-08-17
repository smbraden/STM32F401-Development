#ifndef UART_H
	#define UART_H

#include "stm32f401xe.h"



// Prototypes
void configUART(void);
void printString(const char[]);
void transmitByte(char);


#endif
