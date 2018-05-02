/*------------------------------------------------------------------
 *  control.c -- here you can implement your control algorithm
 *		 and any motor clipping or whatever else
 *		 remember! motor input =  0-1000 : 125-250 us (OneShot125)
 *
 *  I. Protonotarios
 *  Embedded Software Lab
 *
 *  July 2016
 *------------------------------------------------------------------
 */

#include "in4073.h"

void update_motors(void)
{					
	int aeMax=500;
	motor[0] = (ae[0]+aej[0])<aeMax?(ae[0]+aej[0]):motor[0];
	motor[1] = (ae[1]+aej[1])<aeMax?(ae[1]+aej[1]):motor[1];
	motor[2] = (ae[2]+aej[2])<aeMax?(ae[2]+aej[2]):motor[2];
	motor[3] = (ae[3]+aej[3])<aeMax?(ae[3]+aej[3]):motor[3];
}

void run_filters_and_control()
{
	// fancy stuff here
	// control loops and/or filters

	// ae[0] = xxx, ae[1] = yyy etc etc
	update_motors();
}
