/*
 * tempa.c
 *
 * Created: 2019-07-31 10:30:37 AM
 *  Author: pandu
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
	
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

#ifndef DHT11_PIN
#define DHT11_PIN PIND4
#endif

typedef struct DH11_Data{
	uint8_t c;
	uint8_t I_RH;
	uint8_t D_RH;
	uint8_t I_Temp;
	uint8_t D_Temp;
	uint8_t CheckSum;
} DH11_Data;


DH11_Data dht11_get_data();

uint8_t dht11_receive_data();

void dht11_request(void);

void dht11_response(void);

bool dht11_validate(DH11_Data data);

int dht11_get_temperature(DH11_Data data);

int dht11_get_humidity(DH11_Data data);
