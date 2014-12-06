//
//  oct_nunchuck.h
//
//	Proj06*
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//
#ifndef OCT_NUNCHUCK_H_
#define OCT_NUNCHUCK_H_

#include "support_common.h"
#include "dtim.h"
#include "gpio.h"
#include "i2c.h"
#include "int.h"
#include "pit.h"

//Define OCT_NUNCHUK_BUTTON_C 0x02 -- Button C state is in bit 1 of last rx byte
#define OCT_NUNCHUK_BUTTON_C 				(0x02)
//Define OCT_NUNCHUK_BUTTON_Z 0x01 -- Button Z state is in bit 0 of last rx byte
#define OCT_NUNCHUK_BUTTON_z 				(0x01)
//Define OCT_NUNCHUK_I2C_ADDR 0x52 -- The Wii Nunchuk I2C address
#define OCT_NUNCHUK_I2C_ADDR 				(0x52)
//Define OCT_NUNCHUK_I2C_DELAY_US 150 -- We have to delay 150 µs after I2C byte xfers/rcvs
#define OCT_NUNCHUK_I2C_DELAY_US 			(150)
//Define OCT_NUNCHUK_REFRESH_FREQ_HZ 4 -- We will read the Nunchuk state every 250 ms
#define OCT_NUNCHUK_REFRESH_FREQ_HZ 4 		(4)
//Define OCT_NUNCHUK_REG_F0 0xF0
#define OCT_NUNCHUK_REG_F0 					(0xF0)
//Define OCT_NUNCHUK_REG_FB 0xFB
#define OCT_NUNCHUK_REG_FB 					(0xFB)
//Define OCT_NUNCHUK_REG_NONE 0x00
#define OCT_NUNCHUK_REG_NONE 				(0x00)
//Define OCT_NUNCHUK_STICK_LEFT_MAX 30 -- We define joystick left as less than 30
#define OCT_NUNCHUK_STICK_LEFT_MAX	 		(30)
//Define OCT_NUNCHUK_STICK_RIGHT_MIN 225 -- We define joystick right as greater than 225
#define OCT_NUNCHUK_STICK_RIGHT_MIN 		(225)
//Define OCT_NUNCHUK_STICK_DOWN_MAX 30 -- We define joystick down as less than 30
#define OCT_NUNCHUK_STICK_RIGHT_MAX 		(30)
//Define OCT_NUNCHUK_STICK_UP_MIN 225 -- We define joystick up as greater than 225
#define OCT_NUNCHUK_STICK_UP_MIN			(225)
//Define g_callback_button_c -- Callback function which is called when button C is pressed
static int_isr_t g_callback_button_c = 0;
//Define g_callback_button_z -- Callback function which is called when button Z is pressed
static int_isr_t g_callback_button_z = 0;
//Define g_callback_stick_down -- Callback function which is called when the stick is moved
static int_isr_t g_callback_stick_down = 0;
//Define g_callback_stick_left -- Callback function which is called when the stick is moved
static int_isr_t g_callback_stick_left = 0;
//Define g_callback_stick_right -- Callback function which is called when the stick is moved to the right position
static int_isr_t g_callback_stick_right = 0;
//Define g_callback_stick_up -- Callback function which is called when the stick is moved to the up position
static int_isr_t g_callback_stick_up = 0;
//Define g_accel_x -- x-axis acceleration
static int g_accel_x;
//Define g_accel_y -- y-axis acceleration
static int g_accel_y;
//Define g_accel_z -- z-axis acceleration
static int g_accel_z;
//Define g_button_c -- 1 if button C is pressed, 0 if button C is not pressed
static int g_button_c;
//Define g_button_z -- 1 if button Z is pressed, 0 if button Z is not pressed
static int g_button_z;
//Define g_pos_x -- Joystick x-axis (left/right) position
static int g_pos_x;
//Define g_pos_y -- Joystick y-axis (up/down) position
static int g_pos_y;
//Define g_i2c_mod -- The I2C module used to communicate with the Nunchuk
static int g_i2c_mod;
//Define g_dtim_timer -- The DMA timer to use to implement busy delays
static int g_dtim_timer;
//Define g_pit_timer -- The PIT timer to use to read the Nunchuk state at 4 Hz
static int g_pit_timer;


int oct_nunchuk_accel_x();
int oct_nunchuk_accel_y();
int oct_nunchuk_accel_z();
int oct_nunchuk_button_c();
int oct_nunchuk_button_z();
void oct_nunchuk_init(i2c_mod_t const p_i2c_mod, dtim_t const p_dtim_timer, pit_timer_t const p_pit_timer);
void oct_nunchuck_on_button_c(int_isr_t const g_callback);
void oct_nunchuk_on_button_z(int_isr_t const g_callback);
void oct_nunchuk_on_stick_down(int_isr_t const g_callback);
void oct_nunchuk_on_stick_left(int_isr_t const g_callback);
void oct_nunchuk_on_stick_right(int_isr_t const g_callback);
void oct_nunchuk_on_stick_up(int_isr_t const g_callback);
int oct_nunchuck_pos_x();
int oct_nunchuck_pos_y();
void oct_nunchuk_read();
void oct_nunchuk_reset();
void oct_nunchuk_tx_cmd(uint8 const reg, uint8 const cmd);




#endif
