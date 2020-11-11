/*
 * main.c
 *
 * Created: 2019-07-22 12:29:16 PM
 * Author : pandu
 */

#define F_CPU 8000000UL
#define SREG    _SFR_IO8(0x3f)

#include <avr/io.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>




#define DHT11_PIN PIND4
#define BUZZER_PIN PIND5
#define HEART_RATE_PIN PINA2
#define VOL_IN_PIN PINA1
#define SERIAL_SWITCH PIND6

///////////////////////Common////////////////////////
#include "common/analog.h"
#include "common/uart.h"
///////////////////////Buzzer////////////////////////
#include "buzzer/buzzer.h"
//////////////////////Key Pad///////////////////////
#include "keypad/keypad.h"
///////////////////////LCD display/////////////////
#include "display/i2c_lcd.h"
#include "common/i2cmaster.h"
////////////////DHT11 Temparature/////////////////
#include "temperature/temperature.h"
////////////////////GPS module///////////////////
#include "gps/gps.h"

////////////////////Altitude/////////////////////
#include "altitude/bmp085.h"

int show_altitude_at(int x, int y);

///////////////////Compass///////////////////////
#include "compass/hmc5883l.h"
#include "compass/magn_docalibration.h"

typedef enum MenuStatus {
    LEADER, MAIN_MENU, ALTITUDE, HEART_RATE, TEMPERATURE, LOCATION, BATTERY, COMPASS
} MenuStatus;

#define NO_OF_MENU_ITEM 7
char * menu_item[NO_OF_MENU_ITEM] = {
	"Connect leader",
	"Altimeter",
	"Heart rate",
	"Temperature",
	"Location",
	"Battery",
	"Compass"
};

MenuStatus currentMenuStatus = MAIN_MENU;

void show_main_menu(char* menu_item[], int menu_size);
void show_altitude_menu(void);
void connect_to_leader(void);
void show_initial_logo(void);
void show_temperature_menu(void);
void show_heart_rate_menu(void);
void show_voltage_menu(void);
void show_compass_menu(void);
void show_gps_menu(void);

void putFillChar(char from_x, char to_x, char y);
void putAsCaption(char * str, char y);

/////////////// Voltage sensor///////////////////////
#define R1 30000.0
#define R2 7500.0
#define TOTAL_VOLTAGE 4.3
float get_voltage();

////////////////Pulse sensor///////////////////////
#include "pulse/pulse.h"
int heartReat = 0;

///////////////WI-FI///////////////////////////
#include "wifi/node_mcu.h"

//////////// Global variables /////////////////////

short global_altitude = 0;

void switch_init(void);
void switch_to_wifi(void);
void switch_to_gps(void);

const char * FORMAT = "$A%s,O%s^";


int main(void) {
	

	//Define Variables

    /************ init all **********************/
	adc_init();
	UART_init(9600);                /* initialize USART with 9600 baud rate */
	gps_init();
	switch_to_wifi();
	lcd_init(LCD_BACKLIGHT_ON);
	bmp085_init();	
	//hmc5883l_init();
	//Switch
	switch_init();
	
		
		
	
	keypad_init();
    buzzer_init();
	
	lcd_clear();
	lcd_goto_xy(0, 0);
	
    show_initial_logo();
	
	
    /* 	This code segment should work
        lcd_clear();
        lcd_goto_xy(0,0);
        lcd_puts("Hello");
    */
    while (1) {
		show_main_menu(menu_item, NO_OF_MENU_ITEM);
		
		//_delay_ms(10000);
	}
    return 0;
}

void move_cursor(int pos, bool isUp);

void show_main_menu(char* menu_item[], int menu_size){
	
	char curssor_pos = 1;
	char actual_pos = 1;
	lcd_clear();
	lcd_goto_xy(0, 0);
	
	putAsCaption("MENU", 0);
	
	
	for(char i = 1; i <= 3 ; i++){
		lcd_puts_at(menu_item[i + actual_pos - 2], 1, i);
	}
	
	lcd_putc_at(0, 1, '>');
	Keypad key;
	while(key != BTN_OK){
		key = keypad_waitForKeyPress();
		switch(key){
			case BTN_DOWN:{
				if(actual_pos == 1){
					move_cursor(++curssor_pos, false);
					actual_pos++;
				}else if(actual_pos == menu_size - 1){
					move_cursor(++curssor_pos, false);
					actual_pos++;
				}else if(actual_pos >= menu_size){
					break;
				}else{
					for(char i = 0; i < 3 ; i++){
						lcd_clear_selected(1, DSP_WIDTH - 1, i + 1);
						lcd_puts_at(menu_item[i + actual_pos - 1], 1, i + 1);
					}
					actual_pos++;
				}
				break;
			}
			case BTN_UP:{
				if(actual_pos >= menu_size){
					actual_pos--;
					move_cursor(--curssor_pos, true);
				}else if(actual_pos == 2){
					actual_pos--;
					move_cursor(--curssor_pos, true);

				}else if(actual_pos <= 1){
					lcd_goto_xy(1,0);
					break;
				}else{
					actual_pos--;
					for(char i = 0; i < 3 ; i++){
						lcd_clear_selected(1, DSP_WIDTH - 1, i + 1);
						lcd_puts_at(menu_item[i + actual_pos - 2], 1, i + 1);
					}
				}
				break;	
			}
			case BTN_OK:{
			//	buzzer
				buzzer_alarm(100);
			}
			default:break;
		}
		lcd_goto_xy(0, 0);
		lcd_put_int(actual_pos);
		_delay_ms(500);
	}
	switch(actual_pos){
		case 1: connect_to_leader(); break;
		case 2:  show_altitude_menu(); break; //ALTITUDE;
		case 3:  show_heart_rate_menu(); break; //HEART_RATE;
		case 4:  show_temperature_menu(); break; //return TEMPERATURE;
		case 5:  show_gps_menu();break;
		case 6:  show_voltage_menu(); break; //return BATTERY;
		case 7:  show_compass_menu(); break; //return COMPASS;
		default: show_main_menu(menu_item, NO_OF_MENU_ITEM); //return MAIN_MENU;
	}
}

void move_cursor(int pos, bool isUp){
	if(isUp){
		lcd_putc_at(0, pos, '>');
		lcd_putc_at(0, pos + 1, ' ');
	}else{
		lcd_putc_at(0, pos, '>');
		lcd_putc_at(0, pos - 1, ' ');
	}
}

void show_altitude_menu(){
    lcd_clear();
    lcd_goto_xy(0, 0);
    
    putAsCaption("ALTITUDE", 0);
     lcd_puts_at("Altitude:", 0, 1);
    int true_alti = show_altitude_at(9, 1);
	
    if (global_altitude == 0) { // Showing default menu of altitude
        lcd_puts_at("New level:", 0, 2);
        short set_alti = 0;
        char count = 0;
        Keypad key = keypad_waitForKeyPress();
        while (1) {
			
			_delay_ms(100);
            switch (key) {
                case BTN_0:case BTN_1:
                case BTN_2:case BTN_3:
                case BTN_4:case BTN_5:
                case BTN_6:case BTN_7:
                case BTN_8:case BTN_9: {
                    char num = keypad_get_number(key);
                    lcd_put_int(num);
                    set_alti = set_alti * 10 + num;
                    count++;
                    key = keypad_waitForKeyPress();
                    break;
                }
                case BTN_CANCEL: {
					
                    if (set_alti != 0) {
                        lcd_perform_backspace();
						
                        set_alti /= 10;
						key = keypad_waitForKeyPress();
                    } else {
						show_main_menu(menu_item, NO_OF_MENU_ITEM);
                    }
                    break;
                }
                case BTN_OK: {
					if (set_alti == 0) {
                        buzzer_alarm(200);
						key = keypad_waitForKeyPress();
                    } else {
                        global_altitude = set_alti;
						lcd_clear_line(3);
                        lcd_puts_at("All set!", 0, 3);
                        _delay_ms(2000);
                        show_altitude_menu();
                    }
                    break;
                }
                default:
                    buzzer_alarm(300);
                    key = keypad_waitForKeyPress();
            }
        }
    } else {
        while (!keypad_isPressed(BTN_CANCEL)) {
            lcd_puts_at("Altitude:", 0, 1);
            int alitude = show_altitude_at(9, 1);
            lcd_goto_xy(0, 2);
            lcd_puts("Destination:");
            lcd_put_int(global_altitude);
            lcd_goto_xy(0, 3);
            lcd_puts("Difference:");
            int diffence = global_altitude - alitude;
			//lcd_clear_selected(10, DSP_WIDTH, 3);
			char temp[5];
			sprintf(temp, "%4d", diffence);
			lcd_puts(temp);
            //lcd_put_int_with_width(diffence, 4);
            _delay_ms(500);
        }
		show_main_menu(menu_item, NO_OF_MENU_ITEM);
    }
}


int show_altitude_at(int x, int y) {
	
    int val;
    char printbuff[10];
    itoa(val, printbuff, 10);
    lcd_goto_xy(x, y);
    lcd_puts(printbuff);
	return val;
}

void putFillChar(char from_x, char to_x, char y) {
    for (char i = from_x; i < to_x; i++)
        lcd_putc_at(i, y, 0b11111111);
}

void putAsCaption(char * str, char y) {
	char str_len = strlen(str);
	char firstset = (DSP_WIDTH - str_len) / 2;
	putFillChar(0, firstset, y);
	lcd_puts(str);
	putFillChar(firstset + str_len, DSP_WIDTH, y);
}

void show_initial_logo(void){
    lcd_clear();
    putFillChar(0, DSP_WIDTH, 0);
    putAsCaption("GENTEK", 1);
    putAsCaption("AS01", 2);
	putAsCaption("LEADER", 3);
    _delay_ms(5000);
    lcd_clear();
	currentMenuStatus = MAIN_MENU;
}

float get_voltage(){
	float value = (float)adc_read(VOL_IN_PIN);
	return  ((value * 5.0 * (R1 + R2))/ (1023.0 * R2));
}

void show_heart_rate_menu(void){
	lcd_clear();
	lcd_goto_xy(0, 0);
	putAsCaption("Heart Rate", 0);
	 
	 
	/*  timing data */
	const double sampling_rate = 0.1000 ;	   	// actually this is the _delay_ms val
	const int time_limit = 10 ;  				 //in seconds
	const int se=time_limit/(sampling_rate*2);
	/* /timing data */
	lcd_puts_at("Press any key", 0, 1);
	lcd_puts_at("to start measure", 0, 2);
	Keypad key;
	while((key = keypad_waitForKeyPress()) != BTN_CANCEL){
		
		int data_points[se+1];
		char val[10];
		data_points[0]=0;
		int thresh=300;
		int count=0;
		prevx=0;
		num=0;
		int count2=0;
		
		
		lcd_clear_line(1);
		lcd_clear_line(2);
		
		lcd_goto_xy(0,1);
		lcd_puts("Place your");
		
		lcd_goto_xy(0,2);
		lcd_puts("fingertip");
		_delay_ms(2000);
		
		
		lcd_clear_line(1);
		lcd_clear_line(2);
		
		lcd_goto_xy(0,1);
		lcd_puts("Keep placing");
		lcd_goto_xy(0,2);
		lcd_puts("your fingertip");
		_delay_ms(3000);
				
		lcd_clear_line(1);
		lcd_clear_line(2);
		lcd_clear_line(3);
		
		for(int i=0;i<time_limit/(sampling_rate*2);i++){
			data_points[i+1]=adc_read(HEART_RATE_PIN);
				if(i==0)
				{
					num=data_points[i+1];
				}
			
			itoa(data_points[i+1],val,10);

			lcd_goto_xy(0,1);
			lcd_puts("Measuring>>>");
			int k=16*(data_points[i+1]-200)/375;
			
			itoa(k,val,10);
			_delay_ms(sampling_rate*1000);
		}
		pulse_regression(data_points,se);
		for(int i = 0;i<time_limit/(sampling_rate*2);i++){
			thresh=(i+2)* m + c;

			int a=data_points[i+1];//current value
			char temp[11]="";
			char ccount[3];
			
			if(a>thresh){
				count+=1; //peak counting]
				if(a>data_points[i] && a>data_points[i+2]){
					count2+=1;
				}
				
			}
		}
		//displaying the heart rate
		int x = count2*(60/time_limit);	//calculating the pulse value
		itoa(x,val,10);
			
		
		lcd_goto_xy(0,1);
		lcd_puts("Heart Rate:-");
		
		

		strcat(val," BMP");
		
		lcd_goto_xy(4,2);
		lcd_puts(val);
		_delay_ms(3000);
		lcd_puts_at("Press again.", 0, 3);
		key = keypad_waitForKeyPress();
	}
	show_main_menu(menu_item, NO_OF_MENU_ITEM);
}

void show_temperature_menu(void){
	lcd_clear();
	putAsCaption("TEMPERATURE", 0);
	char data[10];
	lcd_puts_at("Humidity->", 0, 1);
	lcd_puts_at("Temperature->", 0, 2);
	int count = 0;
	while(!keypad_isPressed(BTN_CANCEL))
	{
		_delay_ms(500);	
		DH11_Data x = dht11_get_data();
		if (!dht11_validate(x))
		{
			lcd_goto_xy(0,1);
			lcd_puts("Error reading");
			lcd_goto_xy(0,2);
			lcd_puts("data");
		}else{
			itoa(dht11_get_humidity(x),data,10);
			lcd_goto_xy(10,1);
			lcd_puts(data);
			lcd_putc('%');
			itoa(dht11_get_temperature(x), data,10);
			lcd_goto_xy(13,2);
			lcd_puts(data);
			lcd_putc('C');
		}
		_delay_ms(800);
	}
	
	show_main_menu(menu_item, NO_OF_MENU_ITEM);
}

void show_voltage_menu(void){
	lcd_clear();
	putAsCaption("BATTERY", 0);
	lcd_puts_at("Battery status:", 0, 1);
	
	char temp[5];
	while(!keypad_isPressed(BTN_CANCEL)){
		float voltage = get_voltage();
		
		char percentage = (char)(voltage / TOTAL_VOLTAGE) * 100;
		
		itoa(percentage, temp, 10);
		lcd_puts_at(temp, 0, 2);
		lcd_puts_at("%", 3, 2);
		
		_delay_ms(100);
	}
	show_main_menu(menu_item, NO_OF_MENU_ITEM);
}

void show_compass_menu(void){
	lcd_clear();
	putAsCaption("COMPASS", 0);
	
	int16_t mxraw = 0;
	int16_t myraw = 0;
	int16_t mzraw = 0;
	double mx = 0;
	double my = 0;
	double mz = 0;
	int set_direction = 0;
	Keypad key;
	
	while( (key =keypad_returnPressed()) != BTN_CANCEL){
		
		
		hmc5883l_getrawdata(&mxraw, &myraw, &mzraw);
		hmc5883l_getdata(&mx, &my, &mz);
		
		float heading = 0;
		heading = atan2((double)myraw,(double)mxraw)*57.29578;
			
		heading -= 60;
		if(heading < 0)
			heading = 360 + heading;
				
			
		lcd_goto_xy(0, 1);
		lcd_puts("Azimuth:");
		char temp[4];
		sprintf(temp, "%03d", (int)heading);
		lcd_puts(temp);
		
		lcd_goto_xy(0, 2);
		lcd_puts("Set direction:");
		if(key == BTN_OK)
			set_direction = heading;
		
		int temp_direction = heading - set_direction;
		
		sprintf(temp, "%+04d", (int)temp_direction);
		
		lcd_goto_xy(3, 3);
		lcd_puts(temp);
		_delay_ms(500);
	}
	show_main_menu(menu_item, NO_OF_MENU_ITEM);
}

#define TIME_OUT 9000

const char * MSG_FORMAT = "$%s,%s,%03d^";

void connect_to_leader(void){
	lcd_clear();
	putAsCaption("LEADER", 0);
	
	switch_to_wifi();
	
	//node_sendCharToNode(NODE_START_NODE);
	
	Keypad key;
	
	
	//bool wasThereNoClient;
	
	lcd_puts_at("1", 0, 0);
	
	
	if(wifi_readCharFromWifi() == NODE_NO_CLIENT){
		lcd_puts_at("Looking For", 0, 1);
		lcd_puts_at("Client...", 0, 2);
		//wasThereNoClient = true;
	}
	//else
		//wasThereNoClient = false;
		
	bool firstTimeClear = false;
	
	while((key = keypad_returnPressed()) != BTN_CANCEL){
		
		switch_to_wifi();
		
		
		//lcd_puts_at("2", 0, 0);
		
		char value = wifi_readCharFromWifi();
		
		//lcd_clear_line(1);
		//lcd_clear_line(2);
		//
		
		lcd_puts_at("Reading data", 0, 1);
		
		while((value = wifi_readCharFromWifi())!= NODE_START_CHAR);
		
		lcd_clear_line(1);
		
		int i = 1;
		char data[NODE_STRING_SIZE];
		int data_pos = 0;
		
		////Begin a new msg
		char client_id[3];
		char lati[LATI_STR_SIZE];
		char lon[LON_STR_SIZE];
		char heart[HEART_STR_SIZE];
		int lati_point = 0;
		int lon_point = 0;
		int heart_point = 0;
		int client_point = 0;
		if(value == NODE_START_CHAR){ 
			
			while(value != NODE_END_CHAR && data_pos != NODE_STRING_SIZE){
				value = wifi_readCharFromWifi();
			
				data[data_pos++] = value;
				
			}
			
			data[data_pos] = '\0';
			
			//lcd_puts_at(data, 0, 0);
			
			int count_temp = 0;
			for(int i = 0; i < data_pos - 1; i++){
				
				if(data[i] == ','){
					count_temp++;
					continue;
				}
				
				if(data[i] == NODE_END_CHAR)
					break;
				
				else{
					if(count_temp == 0)
						client_id[client_point++] = data[i];
					if(count_temp == 1)
						lati[lati_point++] = data[i];
					if(count_temp == 2)
						lon[lon_point++] = data[i];
					if(count_temp == 3)
						heart[heart_point++] = data[i];
				}
				
			}
			
			
			memset(data, 0, data_pos);
			
			lati[lati_point] = '\0';
			lon[lon_point] = '\0';
			heart[heart_point] = '\0';
			data_pos = 0;
			
			lcd_clear_line(1);
			lcd_clear_line(2);
			lcd_clear_line(3);
			
			
			float lati_client = atof(lati);
			float lon_client = atof(lon);
			
			int bpm_client = atoi(heart);
			
			switch_to_gps();
			//lcd_puts_at("7", 0, 0);
			_delay_ms(1000); //This is must for GPS
			gps_get_latitude(GGA_Pointers[1]);         // Extract Latitude
			//lcd_puts_at("Pass4", 0, 3);
			
			
			float lati_server = atof(degrees_buffer);
			
			memset(degrees_buffer,0,degrees_buffer_size);
			
			gps_get_longitude(GGA_Pointers[3]);    // Extract Longitud/e
			//lcd_goto_xy(5,2);
			//lcd_puts(lon);            // display longitude in degree
			
			
			
			float lon_server = atof(degrees_buffer);
			
			//double distance = get_distance(06.7976, 79.9021, (double)lati, (double)lon);
			
			double distance = get_distance(lati_client, lon_client, lati_server, lon_server);
			
			distance = distance > 0 ? distance : -1 * distance;
			
			if(distance > 1000)
				distance = 0;
			
			
			//lcd_puts_at("8", 0, 0);
			
			memset(degrees_buffer,0, degrees_buffer_size);
			char temp[16];
			char distance_str[6];
			itoa(distance, distance_str, 10);
			
			sprintf(temp, "Client Id:%s", client_id);
			lcd_puts_at(temp, 0, 1);
			
			if(distance < 7){	
				sprintf(temp, "Distance:%sm", distance_str);
				lcd_puts_at(temp, 0, 2);
			}else{
				
				sprintf(temp, "Client away! :%sm", distance_str);
				lcd_puts_at(temp, 0, 2);
				
				buzzer_alarm(1000);
				_delay_ms(500);
				buzzer_alarm(1000);
				_delay_ms(500);
				buzzer_alarm(1000);
				_delay_ms(5000);
			}
			
			char temp_bmp[14];
			if(bpm_client <= 100 && bpm_client >= 60){
				
				sprintf(temp_bmp, "Client BPM:%d", (int) bpm_client);
				lcd_puts_at(temp_bmp, 0, 3);            // display longitude in degree
			}
			
			else if(bpm_client > 100){
				
				lcd_puts_at("Client BPM high!", 0, 3);
				_delay_ms(1000);
				buzzer_alarm(1000);
				_delay_ms(100);
				buzzer_alarm(100);
				_delay_ms(100);
				buzzer_alarm(1000);
				_delay_ms(100);
				buzzer_alarm(100);
				_delay_ms(100);
				buzzer_alarm(1000);
				
			}else{
				lcd_puts_at("Client BPM low!", 0, 3);
				lcd_put_int(bpm_client);
				_delay_ms(100);
				buzzer_alarm(100);
				_delay_ms(100);
				buzzer_alarm(1000);
				_delay_ms(100);
				buzzer_alarm(100);
				_delay_ms(100);
				buzzer_alarm(1000);
				_delay_ms(100);
				buzzer_alarm(100);
				_delay_ms(100);
				buzzer_alarm(1000);
			}
			
			memset(lati, 0, LATI_STR_SIZE);
			memset(lon, 0, LON_STR_SIZE);
			memset(heart, 0, HEART_STR_SIZE);
		}
	}
	
	
}



void show_gps_menu(void){
	switch_to_gps();
	lcd_clear();
	putAsCaption("LOCATION", 0);

	bool firstTime = true;
	while(!keypad_isPressed(BTN_CANCEL)){
		lcd_goto_xy(0,0);
		if(firstTime){
			lcd_puts_at("Getting Location", 0, 1);
			lcd_puts_at("Please wait", 0, 2);
		}
		
		switch_to_gps();
		char data[NODE_STRING_SIZE], str_lati[20], str_lon[20];
		
		_delay_ms(1000); // This delay is must for GPS
		
		gps_get_latitude(GGA_Pointers[1]);         // Extract Latitude
		if(firstTime){
			lcd_clear_line(1);
			lcd_clear_line(2);
		}
		firstTime = false;
		lcd_goto_xy(0,1);
		lcd_puts("Lati:");
		lcd_puts(degrees_buffer);            /* display longitude in degree */
		
		memset(degrees_buffer, 0, degrees_buffer_size);
		
		lcd_goto_xy(0, 2);
		lcd_puts("Long:");
		
		gps_get_longitude(GGA_Pointers[3]);    // Extract Longitude
		
		lcd_puts(degrees_buffer);            /* display latitude in degree */
		
		memset(degrees_buffer,0,degrees_buffer_size);
	}
	switch_to_wifi();
	show_main_menu(menu_item,NO_OF_MENU_ITEM);
}


void switch_init(void){
	DDRD |= 1 << SERIAL_SWITCH;
}

void switch_to_gps(void){
	sei();
	PORTD |= 1 << SERIAL_SWITCH;
}

void switch_to_wifi(void){
	cli();
	PORTD &= ~(1 << SERIAL_SWITCH);
}


