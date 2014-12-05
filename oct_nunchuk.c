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

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_init: Initializes the MCF52259 hardware so we can read the Nunchuk state information. We use DMA timer
//-- p_dtim_timer to implement a busy delay for OCT_NUNCHUK_I2C_DELAY_US µs following the transfer and receipt of data
//-- bytes to and from the Nunchuk. We use PIT timer p_pit_timer to generate periodic interrupts at
//-- OCT_NUNCHUK_REFRESH_FREQ_HZ Hz. At each interrupt oct_nunchuk_read() will be called to read the Nunchuk state.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_init(i2c_mod_t const p_i2c_mod, dtim_t const p_dtim_timer, pit_timer_t const p_pit_timer)
{
	g_i2c_mod = p_i2c_mod;
	g_dtim_timer = p_dtim_timer;
	g_pit_timer = p_pit_timer;
	oct_nunchuk_reset();
	dtim_init(g_dtim_timer);
	i2c_init(g_i2c_mod, 100, 8);
	oct_nunchuk_tx_cmd(g_i2c_mod, OCT_NUNCHUK_REG_F0, 0x55);
	oct_nunchuk_tx_cmd(g_i2c_mod, OCT_NUNCHUK_REG_FB, 0x00);
	//pit_init(pit_timer_t const n, uint8 const p_prescaler, uint16 const p_pmr ,int_isr_t const p_callback)
	pit_init(g_pit_timer, OCT_NUNCHUK_REFRESH_FREQ_HZ, oct_nunchuk_read);
	pit_enable(g_pit_timer);
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_on_button_c: Request callbacks on button C presses. Saves the user's callback function pointer in
//-- g_callback_button_c. The callback function is called from oct_nunchuk_read() when button C is pressed.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuck_on_button_c(int_isr_t const g_callback)
{
	g_callback_button_c = g_callback;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_on_button_z: Request callbacks on button Z presses. Saves the user's callback function pointer in
//-- g_callback_button_z. The callback function is called from oct_nunchuk_read() when button Z is pressed.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_on_button_z(int_isr_t const g_callback)
{
	g_callback_button_z = g_callback;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_on_stick_down: Request callbacks when the joystick is moved to the down position. Saves the user's
//-- callback function pointer in g_callback_stick_down. The callback function is called from oct_nunchuk_read() when the
//-- stick is moved down.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_on_stick_down(int_isr_t const g_callback)
{
	g_callback_stick_down = g_callback;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_on_stick_left: Request callbacks when the joystick is moved to the left position. Saves the user's
//-- callback function pointer in g_callback_stick_left. The callback function is called from oct_nunchuk_read() when the
//-- stick is moved left.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_on_stick_left(int_isr_t const g_callback)
{
	g_callback_stick_left = g_callback;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_on_stick_right: Request callbacks when the joystick is moved to the right position. Saves the user's
//-- callback function pointer in g_callback_stick_right. The callback function is called from oct_nunchuk_read() when the
//-- stick is moved right.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_on_stick_right(int_isr_t const g_callback)
{
	g_callback_stick_right = g_callback;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_on_stick_up: Request callbacks when the joystick is moved to the up position. Saves the user's
//-- callback function pointer in g_callback_stick_up. The callback function is called from oct_nunchuk_read() when the
//-- stick is moved up.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_on_stick_up(int_isr_t const g_callback)
{
	g_callback_stick_right = g_callback;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_pos_x: Returns the joystick x-axis (left/right) position.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuck_pos_x()
{
	return g_pos_x;
}


//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_pos_y: Returns the joystick y-axis (up/down) position.
//------------------------------------------------------------------------------------------------------------------------

int oct_nunchuck_pos_y()
{
	return g_pos_y;
}
















