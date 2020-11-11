
/*
 * uart.h
 *
 * Created: 2019-12-19 6:07:31 PM
 *  Author: pandu
 */ 
#define F_CPU 8000000UL				// Define CPU clock Frequency e.g. here its 8MHz
#include <avr/io.h>					// Include AVR std. library file
#include <avr/interrupt.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)     // convert to baudrate dec value

void UART_init(long USART_BAUDRATE);
void UART_TxChar(char c);
void UART_sendString(char *str);
unsigned char UART_RxChar();









