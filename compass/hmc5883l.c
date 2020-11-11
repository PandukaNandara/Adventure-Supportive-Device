/*
hmc5883l lib 0x01

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "hmc5883l.h"

//path to i2c fleury lib
#include HMC5883L_I2CFLEURYPATH

static double hmc5883l_scale = 0;

/*
 * init the hmc5883l
 */
void hmc5883l_init() {
	#if HMC5883L_I2CINIT == 1
	//init i2c
	i2c_init();
	_delay_us(10);
	#endif

	//set scale
	hmc5883l_scale = 0;
	uint8_t regValue = 0x00;
	#if HMC5883L_SCALE == HMC5883L_SCALE088
		regValue = 0x00;
		hmc5883l_scale = 0.73;
	#elif HMC5883L_SCALE == HMC5883L_SCALE13
		regValue = 0x01;
		hmc5883l_scale = 0.92;
	#elif HMC5883L_SCALE == HMC5883L_SCALE19
		regValue = 0x02;
		hmc5883l_scale = 1.22;
	#elif HMC5883L_SCALE == HMC5883L_SCALE25
		regValue = 0x03;
		hmc5883l_scale = 1.52;
	#elif HMC5883L_SCALE == HMC5883L_SCALE40
		regValue = 0x04;
		hmc5883l_scale = 2.27;
	#elif HMC5883L_SCALE == HMC5883L_SCALE47
		regValue = 0x05;
		hmc5883l_scale = 2.56;
	#elif HMC5883L_SCALE == HMC5883L_SCALE56
		regValue = 0x06;
		hmc5883l_scale = 3.03;
	#elif HMC5883L_SCALE == HMC5883L_SCALE81
		regValue = 0x07;
		hmc5883l_scale = 4.35;
	#endif

	//setting is in the top 3 bits of the register.
	regValue = regValue << 5;
    i2c_start_wait(HMC5883L_ADDR | I2C_WRITE);
    i2c_write(HMC5883L_CONFREGB);
    i2c_write(regValue);
    i2c_stop();

	//set measurement mode
	i2c_start_wait(HMC5883L_ADDR | I2C_WRITE);
	i2c_write(HMC5883L_MODEREG);
	i2c_write(HMC5883L_MEASUREMODE);
	i2c_stop();
}

/*
 * get raw data
 */
void hmc5883l_getrawdata(int16_t *mxraw, int16_t *myraw, int16_t *mzraw) {
	uint8_t i = 0;
	uint8_t buff[6];

	i2c_start_wait(HMC5883L_ADDR | I2C_WRITE);
	i2c_write(HMC5883L_DATAREGBEGIN);
	i2c_stop();
	i2c_start_wait(HMC5883L_ADDR | I2C_READ);
	for(i=0; i<6; i++) {
		if(i==6-1)
			buff[i] = i2c_readNak();
		else
			buff[i] = i2c_readAck();
	}
	i2c_stop();

	*mxraw = (int16_t)((buff[0] << 8) | buff[1]);
	*mzraw = (int16_t)((buff[2] << 8) | buff[3]);
	*myraw = (int16_t)((buff[4] << 8) | buff[5]);
}

/*
 * get scaled data
 */
void hmc5883l_getdata(double *mx, double *my, double *mz) {
	int16_t mxraw = 0;
	int16_t myraw = 0;
	int16_t mzraw = 0;
	hmc5883l_getrawdata(&mxraw, &myraw, &mzraw);

	#if HMC5883L_CALIBRATED == 1
	float mxt = mxraw - HMC5883L_OFFSETX;
	float myt = myraw - HMC5883L_OFFSETY;
	float mzt = mzraw - HMC5883L_OFFSETZ;
	*mx = HMC5883L_GAINX1 * mxt + HMC5883L_GAINX2 * myt + HMC5883L_GAINX3 * mzt;
	*my = HMC5883L_GAINY1 * mxt + HMC5883L_GAINY2 * myt + HMC5883L_GAINY3 * mzt;
	*mz = HMC5883L_GAINZ1 * mxt + HMC5883L_GAINZ2 * myt + HMC5883L_GAINZ3 * mzt;
	#else
	*mx = mxraw * hmc5883l_scale;
	*my = myraw * hmc5883l_scale;
	*mz = mzraw * hmc5883l_scale;
	#endif



}
