/************************************************************************/
/*        Code for buzzer alarm                                         */
/************************************************************************/ 


#ifndef F_CPU
#define F_CPU 8000000L
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "buzzer.h"

void buzzer_init(void)
{
	DDRD |= 1 << BUZZER_PIN;
}

void buzzer_alarm(int ms)
{
	for (int i = 0; i < ms / 4; i++)
	{
		PORTD |= 1 << BUZZER_PIN;
		_delay_ms(2);
		PORTD &= ~(1 << BUZZER_PIN);
		_delay_ms(2);
	}
}










