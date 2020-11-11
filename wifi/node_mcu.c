
/*
 * node_mcu.c
 *
 * Created: 2019-12-19 6:12:09 PM
 *  Author: pandu
 */ 


#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "../common/uart.h"
#include "node_mcu.h"



char* wifi_receiveStringFromWifi(){
	static char str[NODE_STRING_SIZE];
	char num;
	while (UART_RxChar() != 255);
	for (char i = 0; i < NODE_STRING_SIZE; i++)
	{
		if(num = UART_RxChar())
		str[i] = num;
		if(num == '\0')
		break;
	}
	return str;
}

unsigned char wifi_readCharFromWifi(){
	cli();
	return UART_RxChar();
}

void wifi_sendStringToWifi(char * str){
	_delay_ms(50);
	UART_sendString(str);      // send string
	_delay_ms(1200);
}

void wifi_waitTillChar(char ch){
	while(wifi_readCharFromWifi() != ch);
}

void wifi_sendCharToWifi(char c){
	UART_TxChar(c);
}
