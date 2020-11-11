
/*
 * node_mcu.h
 *
 * Created: 2019-12-19 6:11:41 PM
 *  Author: pandu
 */ 


#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "../common/uart.h"

#define NODE_STRING_SIZE 20
#define NODE_SUCCESS 'Z'
#define NODE_START_CHAR '$'
#define NODE_END_CHAR '^'
#define NODE_CONNECT_TO_SERVER 'C'
#define INCOMPLETED_CHAR 'I'


#define ALREADY_CONNECTED '&'
#define CONNCETED '*'
#define NOT_CONNCETED '('
#define NODE_CONNECTING 'P'

#define NODE_NO_CLIENT 'N'


#define LATI_STR_SIZE 8
#define LON_STR_SIZE 9
#define HEART_STR_SIZE 3

char * wifi_receiveStringFromWifi();
unsigned char wifi_readCharFromWifi();
void wifi_sendStringToWifi(char * str);
void wifi_waitTillChar(char ch);
void wifi_sendCharToWifi(char c);
