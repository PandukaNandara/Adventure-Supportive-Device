# Faculty of Information Technology 💻
# University of Moratuwa
## IN 1900 - First Year Project 🤖
### Adventure Supportive Device 🧗‍♀️🧗‍♂️
#### An Open source project built with with C + Atmel
![Build Status](https://img.shields.io/github/directory-file-count/Wisipsy/Adventure-Supportive-Device)
![Build Status](https://img.shields.io/github/downloads/Wisipsy/Adventure-Supportive-Device/total)

This project is to support a set of people varied from the whole population. There are many
circumstances that an average hiker will come across. So as a group we have chosen the “travelers/
adventure seekers /hikers” as our targeted customer base. Now people are encouraged to take more
challenges by participating more and more hiking events. So, the increased number of events based on
adventures can be seen in Sri Lanka as well. Our main goal is to support a hiker with the modern
technology in the best way possible.
Hiking is the most popular activity among the adventure travelers according to a report by
the Adventure Travel Trade Association. People tend to move towards those experience more and
more in these days. Globally, day by day clubs are setting up and the participants believe that the
most important components of an adventure experience were being in a natural environment,
learning, meaningful human experiences, and experiencing a new culture.
As our main problem we have considered hikers getting stranded. We have focused on
health issues also. Furthermore, the main dilemmas will be explained. Our main aim is to build a
secure portable system to the hikers/adventure seekers. Our aim will also be introduced briefly.
The way we can send his/her individual information to other teammates. In addition, how we can
help this traveler by giving the relevant information about the trail ahead will be another objective.
How we are going to overcome those problems, will be discussed under the objectives.
In the mean time we will discuss how we use the technology of sensors such as heartrate
and altitude sensors. Inputs and outputs for our microcontroller will also be sketched in this
proposal. Through an atmega32 micro controller we will build a reliable device for this special
aspiration. Certain resources will be required as heartrate sensor, modules and other resources will
be discussed. Cost estimation will also be submitted.
A device that can sense the environment and display the relevant information. Secure health
measuring system that update automatically. System that can connect travelers and inform them
purposely. Tallying these systems and making one efficient device will be a great impact on the people
(hikers /adventure seekers/ travelers).

There are seprated folder for each module. Each codes are seprated and independently. In this project, we have used following modules and connected to the port at ```Atmega32```.
| Module | PORT(S) |
| ------ | ------ |
| altitude | ```PC0, PC1``` |
| buzzer | ```PD5``` |
| compass | ```PC0, PC1``` |
| display | ```PC0, PC1``` |
| gps | ```PD0, PD1``` |
| pulse | ```PA2``` |
| temperature |  ```PD4``` |
| wifi | ```PD0, PD1``` |
| Keypad | ```PB0 - PB7``` |

You can use them based on your requirement.

##### Functions for display module.

### Altitude Sensor
connected to 
We have used ``bmp085`` module for measuring altitude. The orginal auther for this code is [Davide Gironi](http://davidegironi.blogspot.com/). **You can find his more AVR libraries in [here](https://code.google.com/archive/p/davidegironi/downloads).**
##### Functions for display module.
```c
void bmp085_init();
int32_t bmp085_getpressure();
double bmp085_getaltitude();
double bmp085_gettemperature();
double convert_row_to_meter(double x);
```
### Buzzer module

We have used ``bmp085`` module for measuring altitude. 
##### Functions for display module.
```c
void buzzer_init(void);
void buzzer_alarm(int ms);
```
### Altitude Sensor
connected to 
We have used ``hmc5883l`` module for measuring altitude.  The orginal auther for this code is [Davide Gironi](http://davidegironi.blogspot.com/). 
##### Functions for display module.
```c
void hmc5883l_init();
void hmc5883l_getrawdata(int16_t *mxraw, int16_t *myraw, int16_t *mzraw);
void hmc5883l_getdata(double *mx, double *my, double *mz);
```

### Display Module
we have used [```i2C module```](https://www.electronicscomp.com/i2c-module-16x2-lcd-india#:~:text=I2C%20Module%20has%20a%20inbuilt,of%20either%200x27%20or%200x3F.&text=With%20this%20I2C%20interface%20module,display%20via%20only%202%20wires.) to connect [```16X4 display module```](https://www.winstar.com.tw/products/character-lcd-display-module/lcd-16x4.html) module. The original auther for this module code is [Vassilis Serasidis](https://github.com/Serasidis).
##### Functions for display module.
```c
void lcd_init (unsigned char command);
void lcd_sendCommand(unsigned char cmd);	//Sends Command to LCD
void lcd_sendData(unsigned char data);
void lcd_putc(unsigned char ch);		//Sends Char to LCD
void lcd_goto_xy(uint8_t x, uint8_t y);
void lcd_puts_at(char* str, uint8_t x, uint8_t y);
void lcd_cursorOn(void);					//Underline cursor ON
void lcd_cursorOnBlink(void);			//Underline blinking cursor ON
void lcd_cursorOFF(void);				//Cursor OFF
void lcd_clear(void);					//Clears LCD
void lcd_puts(char*);					//Outputs string to LCD
void lcd_puts_P(const char*);		//copies flash string to LCD at x,y
void lcd_putc_at(uint8_t x, uint8_t y, char val);

uint8_t lcd_get_current_x();
uint8_t lcd_get_current_y();
void lcd_perform_backspace(void);
void lcd_put_int(int x);
void lcd_put_int_with_width(int x, int width);
void lcd_clear_selected(int from, int to, int y);
void lcd_clear_line(int y);
```
### GPS module

We have used ```NEO 6M GPS Module``` to measure longitude and latitude.
##### Functions for display module.
```c
void gps_init(void);
void gps_get_latitude(uint16_t lat_pointer);
void gps_get_longitude(uint16_t long_pointer);
void gps_convert_time_to_UTC();
void gps_convert_to_degrees(char *);
double get_distance(double lon1, double lat1, double lon2, double lat2);
```
### keypad
For keypad we have defined a ```enum``` to make the code type safe.

```c
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
```
##### Functions for display module.
```c
Keypad keypad_waitForKeyPress();
bool keypad_isPressed(Keypad key);

void keypad_init(void);
char keypad_get_number(Keypad key);
char keypad_returnChar(Keypad key);
Keypad keypad_returnPressed();
Keypad keypad_returnPressedWithoutWait();
```

### Pulse sensor

We have used [```Pulse sensor```](https://pulsesensor.com/) to measure the heart rate of a user. 
**Warning! This code may not work.** 💔

##### Functions for display module.
```c
double pulse_sumit(int data_points[],int length);
double pulse_xysum(int data_points[],int length);
void pulse_regression(int data_points[],double n);
```

### Temperature Sensor

We have used ```DHT11``` module to measure temperature of the context.
##### Functions for display module.
```c
DH11_Data dht11_get_data();
uint8_t dht11_receive_data();
void dht11_request(void);
void dht11_response(void);
bool dht11_validate(DH11_Data data);
int dht11_get_temperature(DH11_Data data);
int dht11_get_humidity(DH11_Data data);
```

### Wifi Module

We have used ```NodeMCU V3 ESP8266DH``` module to connect two devices.

##### Functions for display module.
```c
char * wifi_receiveStringFromWifi();
unsigned char wifi_readCharFromWifi();
void wifi_sendStringToWifi(char * str);
void wifi_waitTillChar(char ch);
void wifi_sendCharToWifi(char c);
```
