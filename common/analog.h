/*
 * analog.h
 *
 * Created: 2019-07-29 6:43:02 PM
 *  Author: pandu
 */ 


#ifndef ANALOG_H_
#define ANALOG_H_

#ifndef F_CPU
#define F_CPU 8000000UL 
#endif

#include<avr/io.h>
#include<util/delay.h>

void adc_init(void);
int adc_read(short channel);

#endif /* ANALOG_H_ */