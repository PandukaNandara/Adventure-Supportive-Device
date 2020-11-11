/*
 * gps.c
 *
 * Created: 2020-01-12 12:30:51 PM
 *  Author: pandu
 */ 

#include "gps.h"

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>


char Latitude_Buffer[15], Longitude_Buffer[15], Time_Buffer[15];
char degrees_buffer[degrees_buffer_size];                      /* save latitude or longitude in degree */
char GGA_Buffer[Buffer_Size];                                  /* save GGA string */
uint8_t GGA_Pointers[20];                                      /* to store instances of ',' */
char GGA_CODE[3];

volatile uint16_t GGA_Index, CommaCounter;

bool IsItGGAString = false;
bool flag1 = false;
bool flag2 = false;

void gps_init(void){
	GGA_Index=0;
	memset(GGA_Buffer, 0, Buffer_Size);
	memset(degrees_buffer,0,degrees_buffer_size);
	sei();
}


void gps_get_latitude(uint16_t lat_pointer){
	cli();
	uint8_t lat_index;
	uint8_t index = lat_pointer+1;
	lat_index=0;
		
	/* parse Latitude in GGA string stored in buffer */
	for(;GGA_Buffer[index]!=',';index++){
		Latitude_Buffer[lat_index]= GGA_Buffer[index];
		lat_index++;
	}
		
	Latitude_Buffer[lat_index++] = GGA_Buffer[index++];
	Latitude_Buffer[lat_index]= GGA_Buffer[index];		/* get direction */
	gps_convert_to_degrees(Latitude_Buffer);
	sei();
}

void gps_get_longitude(uint16_t long_pointer){
	cli();
	uint8_t long_index;
	uint8_t index = long_pointer+1;
	long_index=0;

	
	/* parse Longitude in GGA string stored in buffer */
	for( ; GGA_Buffer[index]!=','; index++){
		Longitude_Buffer[long_index]= GGA_Buffer[index];
		long_index++;
	}
	
	Longitude_Buffer[long_index++] = GGA_Buffer[index++];
	Longitude_Buffer[long_index]   = GGA_Buffer[index]; /* get direction */
	gps_convert_to_degrees(Longitude_Buffer);
	sei();
}

void gps_convert_time_to_UTC()
{
	unsigned int hour, min, sec;
	uint32_t Time_value;

	Time_value = atol(Time_Buffer);               /* convert string to integer */
	hour = (Time_value / 10000);                  /* extract hour from integer */
	min = (Time_value % 10000) / 100;             /* extract minute from integer */
	sec = (Time_value % 10000) % 100;             /* extract second from integer*/

	sprintf(Time_Buffer, "%d:%d:%d", hour,min,sec);
}

void gps_convert_to_degrees(char *raw){
	
	
	double value;
	float decimal_value,temp;
	
	int32_t degrees;
	
	float position;
	value = atof(raw);                             /* convert string into float for conversion */
	
	/* convert raw latitude/longitude into degree format */
	decimal_value = (value/100);
	degrees = (int)(decimal_value);
	temp = (decimal_value - (int)decimal_value)/0.6;
	position = (float)degrees + temp;
	
	dtostrf(position, 6, 4, degrees_buffer);       /* convert float value into string */
}

double get_distance(double lon1, double lat1, double lon2, double lat2)
{
	double pi = M_PI;
	double R= 6371000;
	double dlon, dlat;


	//for testing purposes
	//-----------defining latitudes-------------
	//lat1=06.7976;
	//lon1=79.9021;
	//-------------------------------------------


	//converting to radian values
	dlon = (lon1 > lon2) ? (lon1 - lon2) * (pi / 180) : (lon2 - lon1) * (pi / 180);
	dlat = (lat1 > lat2) ? (lat1 - lat2) * (pi / 180) : (lat2 - lat1) *(pi / 180);

	lat1 = lat1 * (pi / 180);
	lat2 = lat2 * (pi / 180);
	//-------------------------------------------
	double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double distance = R * c;
	return distance > 0 ? distance : 0;
	
}

ISR (USART_RXC_vect)
{
	uint8_t oldsrg = SREG;
	cli();
	char received_char = UDR;
	
	if(received_char =='$'){                                                    /* check for '$' */
		GGA_Index = 0;
		CommaCounter = 0;
		IsItGGAString = false;
	}
	else if(IsItGGAString == true){                                             /* if true save GGA info. into buffer */
		if(received_char == ',' ) GGA_Pointers[CommaCounter++] = GGA_Index;     /* store instances of ',' in buffer */
		GGA_Buffer[GGA_Index++] = received_char;
	}
	else if(GGA_CODE[0] == 'R' && GGA_CODE[1] == 'M' && GGA_CODE[2] == 'C'){    /* check for GGA string */
		IsItGGAString = true;
		GGA_CODE[0] = 0; GGA_CODE[1] = 0; GGA_CODE[2] = 0;
	}
	else{
		GGA_CODE[0] = GGA_CODE[1];  GGA_CODE[1] = GGA_CODE[2]; GGA_CODE[2] = received_char;
	}
	SREG = oldsrg;
}