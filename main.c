//**********************************************************************************************************************************
// FILE: main.c
//**********************************************************************************************************************************
#include <stdio.h> // For printf()
#include "i2c.h"
#include "oct_nunchuk.h"
#include "pit.h"
//==================================================================================================================================
// Private Preprocessor Macros
//==================================================================================================================================
#define forever while (1)
//==================================================================================================================================
// Static Function Definitions
//==================================================================================================================================
static void hw_init();
static void console_update();
static void sw_init();
//==================================================================================================================================
// Private Global Variables
//==================================================================================================================================
static volatile int g_console_update;
//==================================================================================================================================
// Function Definitions
//==================================================================================================================================
//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: g_console_update()
//
// DESCRIPTION
// Called by PIT0 ISR at 1 Hz. Set the g_console_update flag to 1 to cause the main loop to read the Nunchuk data and display it.
//----------------------------------------------------------------------------------------------------------------------------------
static void console_update()
{
 g_console_update = 1;
}
//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: hw_init()
//
// DESCRIPTION
// Initializes the MCF52259 hardware peripherals.
//----------------------------------------------------------------------------------------------------------------------------------
static void hw_init()
{
 int_inhibit_all();
 oct_nunchuk_init(i2c_mod_1, dtim_3, pit_timer_1);
 pit_init(pit_timer_0, pit_freq_1_hz, console_update);
 int_uninhibit_all();
}
//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: main()
//
// DESCRIPTION/ Where it all begins. Initialize the hardware and software. Drop into an infinite loop displaying the Nunchuk state information
// on the console.
//----------------------------------------------------------------------------------------------------------------------------------
int main()
{

	hw_init();
	sw_init();forever {
	// g_console_update will be set to 1 at a 1 Hz rate.
	while (!g_console_update) {}
	// Retrieve the Nunchuk data from the oct_nunchuk module and display it on the CodeWarrior console.
	 printf("%d %d %d %d %d %d %d\n",
	 oct_nunchuk_pos_x(),
	 oct_nunchuk_pos_y(),
	  oct_nunchuk_accel_x(), oct_nunchuk_accel_y(),
	 oct_nunchuk_accel_z(),
	 oct_nunchuk_button_c(),
	 oct_nunchuk_button_z()
	 );
	 // Clear g_console_update so we will get stuck in the while loop above until the next PIT0 interrupt.
	 g_console_update = 0;
	 }
}
//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: sw_init()
//
// DESCRIPTION
// Software initialization. Set g_console_update to 1 so we will display the Nunchuk data right away.
//----------------------------------------------------------------------------------------------------------------------------------
static void sw_init()
{
  g_console_update = 1;
}
