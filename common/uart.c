
/*
 * uart.c
 *
 * Created: 2019-12-19 6:07:44 PM
 *  Author: pandu
 */

#include "uart.h"


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);         // Turn on transmission and reception by setting RX Tx bits
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);    // Use 8-bit character sizes
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value
}

void UART_TxChar(char c)
{
	while (! (UCSRA & (1<<UDRE))); // Wait for empty transmit buffer
	UDR = c;
}

void UART_sendString(char *str)
{
	unsigned char s=0;
	
	while (str[s]!=0) // string till null
	{
		UART_TxChar(str[s]);    // send s to UART_TxChar(s) function
		s++;
	}
}
unsigned char UART_RxChar()									/* Data receiving function */
{
	while (!(UCSRA & (1 << RXC)));					/* Wait until new data receive */
	return UDR;									/* Get and return received data */
}
