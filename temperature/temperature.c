/*
 * temperature.c
 *
 * Created: 2019-07-31 10:29:11 AM
 *  Author: pandu
 */ 

#include "temperature.h"

uint8_t slope=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

void dht11_request(void){
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(18);	/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
	//_delay_ms(20);	/* wait for 20ms */
}

void dht11_response(void){
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}


DH11_Data dht11_get_data(){
		dht11_request();
		/* send start pulse */
		dht11_response();
		/* receive response */
		DH11_Data data;
		data.I_RH = dht11_receive_data();	/* store first eight bit in I_RH */
		data.D_RH = dht11_receive_data();	/* store next eight bit in D_RH */
		data.I_Temp = dht11_receive_data();	/* store next eight bit in I_Temp */
		data.D_Temp = dht11_receive_data();	/* store next eight bit in D_Temp */
		data.CheckSum = dht11_receive_data();/* store next eight bit in CheckSum */
		
		return data;
}


uint8_t dht11_receive_data(){
	for (char q = 0; q < 8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		slope = (slope<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		slope = (slope<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return slope;
}

bool dht11_validate(DH11_Data data){
	return (data.I_RH + data.D_RH + data.I_Temp + data.D_Temp) == data.CheckSum;
}

int dht11_get_temperature(DH11_Data data){
	return (int)round(data.I_Temp + (float)(data.D_Temp) / 10);
}

int dht11_get_humidity(DH11_Data data){
	return (int)round(data.I_RH + (float)(data.D_RH) / 10);
}






