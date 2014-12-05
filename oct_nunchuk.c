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
	oct_nunchuk_tx_cmd(OCT_NUNCHUK_REG_F0, 0x55);
	oct_nunchuk_tx_cmd(OCT_NUNCHUK_REG_FB, 0x00);
	//pit_init(pit_timer_t const n, uint8 const p_prescaler, uint16 const p_pmr ,int_isr_t const p_callback)
	pit_init(g_pit_timer, 2, 999, oct_nunchuk_read);
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

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_read: Called at OCT_NUNCHUK_REFRESH_FREQ_HZ Hz by the PIT timer interrupt service routine. We read the
//-- Nunchuk state information using the Nunchuk protocol. Call user callback functions as appropriate.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_read()
{
	uint8 temp = 0;
	oct_nunchuk_tx_cmd(OCT_NUNCHUK_REG_NONE, 0x00);
	uint8 data[6] = {0};
	i2c_rx(g_i2c_mod, OCT_NUNCHUK_I2C_ADDR, 6, OCT_NUNCHUK_I2C_DELAY_US, data, g_dtim_timer);
	g_pos_x = (int)data[0];
	g_pos_y = (int)data[1];
	g_accel_x = (int)(data[2] << 2 | (data[5] & 0x0C) >> 2);
	g_accel_y = (int)(data[3] << 2 | (data[5] & 0x30) >> 4);
	g_accel_z = (int)(data[4] << 2 | (data[5] & 0xC0) >> 6);
	temp = OCT_NUNCHUK_BUTTON_C;
	temp &= (data[5]);
	if(temp == 0)
	{
		g_button_c = 1;
	}
	else
	{
		g_button_c = 0;
	}
	temp =  OCT_NUNCHUK_BUTTON_Z;
	temp &= (data[5]);
	if(temp == 0)
	{
		g_button_z = 1;
	}
	else
	{
		g_button_z = 0;
	}
	if(g_pos_x < OCT_NUNCHUK_STICK_LEFT_MAX && g_callback_stick_left != 0)
	{
		g_callback_stick_left();
	}
	else if(g_pos_x > OCT_NUNCHUK_STICK_RIGHT_MIN && g_callback_stick_right !=0)
	{
		g_callback_stick_right();
	}
	if(g_pos_y < OCT_NUNCHUK_STICK_DOWN_MAX && g_callback_stick_down != 0)
	{
		g_callback_stick_down();
	}
	else if(g_pos_y < OCT_NUNCHUK_STICK_UP_MIN && g_callback_stick_up != 0)
	{
		g_callback_stick_up();
	}
	if(g_button_c != 0 && g_callback_button_c !=0)
	{
		g_callback_button_c();
	}
	if(g_button_z != 0 && g_callback_button_z !=0)
	{
		g_callback_button_z();
	}
}
//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_reset: Initialize all of the callback function pointers to null.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_reset()
{
	g_callback_button_c = 0;
	g_callback_button_z = 0;
	g_callback_stick_down = 0;
	g_callback_stick_left = 0;
	g_callback_stick_right = 0;
	g_callback_stick_up = 0;
}

//------------------------------------------------------------------------------------------------------------------------
//-- oct_nunchuk_tx_cmd: Transmits command cmd to Nunchuk register reg. If reg is OCT_NUNCHUK_REG_NONE (because the
//-- command is not sent to a Nunchuk register, then we only transmit 1 byte, which is the command. For reasons I do not
//-- understand, we have to delay after transmitting. I discovered by trial-and-error that 300 µs (2 ×
//-- OCT_NUNCHUK_I2C_DELAY_US) works well.
//------------------------------------------------------------------------------------------------------------------------

void oct_nunchuk_tx_cmd(uint8 const reg, uint8 const cmd)
{
	if(reg != OCT_NUNCHUK_REG_NONE)
	{
		tx_data[0] = reg;
		tx_data[1] = cmd;
		tx_count = 2;
	}
	else
	{
		tx_data[0] = cmd;
		tx_count = 1;
	}
	i2c_tx(g_i2c_mod, OCT_NUNCHUK_I2C_ADDR, tx_count, OCT_NUNCHUK_I2C_DELAY, g_dtim_timer, tx_data);
	//dtim_busy_delay_us(dtim_t const p_timer, uint32 const p_usecs)
	dtim_busy_delay_us(g_dtim_timer, 2*OCT_NUNCHUK_DELAY_US);
}
















