//
//  oct_nunchuk.c
//
//	Proj06*
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#include "oct_nunchuk.h"


//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_accel_x: Returns the joystick x-axis acceleration.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuk_accel_x()
{
	return g_accel_x;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_accel_y: Returns the joystick y-axis acceleration.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuk_accel_y()
{
	return g_accel_y;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_accel_z: Returns the joystick z-axis acceleration.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuk_accel_z()
{
	return g_accel_z;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_button_c: Returns 1 if button C is pressed and 0 if it is not.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuk_button_c()
{
	return g_button_c;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_button_z: Returns 1 if button Z is pressed and 0 if it is not.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuk_button_z()
{
	return g_button_z;
}



















