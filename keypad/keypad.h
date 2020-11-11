/*
 * keypad.h
 *
 * Created: 2019-07-27 1:12:09 PM
 *  Author: pandu
 */
#include<stdbool.h>
#include<avr/io.h>
#include<util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif // F_CPU
typedef enum Keypad{
	BTN_1		 = 0b00010001U,
	BTN_4		 = 0b00010010U,
	BTN_7		 = 0b00010100U,
	BTN_OK		 = 0b00011000U, // *
	BTN_2		 = 0b00100001U,
	BTN_5		 = 0b00100010U,
	BTN_8		 = 0b00100100U,
	BTN_0		 = 0b00101000U,
	BTN_3		 = 0b01000001U,
	BTN_6	  	 = 0b01000010U,
	BTN_9		 = 0b01000100U,
	BTN_CANCEL	 = 0b01001000U, // #
	BTN_UP		 = 0b10000001U, // A
	BTN_DOWN	 = 0b10000010U, // B
	BTN_RIGHT	 = 0b10000100U, // C
	BTN_LEFT	 = 0b10001000U, // D
	BTN_NO_PRESS = 0b00000000U 
} Keypad;

Keypad keypad_waitForKeyPress();
bool keypad_isPressed(Keypad key);

void keypad_init(void);
char keypad_get_number(Keypad key);
char keypad_returnChar(Keypad key);
Keypad keypad_returnPressed();
Keypad keypad_returnPressedWithoutWait();