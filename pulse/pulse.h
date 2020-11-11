/*
 * pulse.h
 *
 * Created: 2019-07-31 1:01:10 PM
 *  Author: pandu
 */ 


#ifndef PULSE_H_
#define PULSE_H_

extern int prevx, num;
extern double m, c;

double pulse_sumit(int data_points[],int length);
double pulse_xysum(int data_points[],int length);
void pulse_regression(int data_points[],double n);

#endif /* PULSE_H_ */