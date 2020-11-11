/*
 * pulse.c
 *
 * Created: 2019-07-31 1:00:48 PM
 *  Author: pandu
 */ 

#include "pulse.h"


/*************** threshold ***************/
double m=0,c=0; // gradient and slope
int prevx=0, num=0;
double sumit(int data_points[],int length){
	
	double sum=0;
	for(int i=1;i<=length;i++)sum+=data_points[i];

	return sum;
}

double xysum(int data_points[],int length){
	
	double sum=0;
	for(int i=1;i<=length;i++)sum+=(i)*data_points[i];

	return sum;
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


int randNew(){
	return num;
}



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


void pulse_regression(int data_points[],double n){
	double squarex= (n)*(n+1)*(2*n+1)/6.0;
	double xbar = (n+1)/2.0;
	double ybar = sumit(data_points,n)/n;

	m=(xysum(data_points,n)- n*xbar*ybar)/( squarex - n*xbar*xbar );
	c=ybar-m*xbar;
}
/*************** end threshold ***************/
