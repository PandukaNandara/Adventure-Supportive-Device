/*
 * gps.h
 *
 * Created: 2020-01-12 12:31:00 PM
 *  Author: Sahan Amrsha
 */ 


#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../common/uart.h"

#define Buffer_Size 150
#define degrees_buffer_size 20
#define F_CPU  8000000UL
#define SREG    _SFR_IO8(0x3f)

extern char Latitude_Buffer[15],Longitude_Buffer[15],Time_Buffer[15];
extern char degrees_buffer[degrees_buffer_size];                      /* save latitude or longitude in degree */
extern char GGA_Buffer[Buffer_Size];                                  /* save GGA string */
extern uint8_t GGA_Pointers[20];                                      /* to store instances of ',' */
extern char GGA_CODE[3];

extern volatile uint16_t GGA_Index, CommaCounter;

extern bool	IsItGGAString;
extern bool flag1;
extern bool flag2;

void gps_init(void);
void gps_get_latitude(uint16_t lat_pointer);
void gps_get_longitude(uint16_t long_pointer);

void gps_convert_time_to_UTC();
void gps_convert_to_degrees(char *);

double get_distance(double lon1, double lat1, double lon2, double lat2);