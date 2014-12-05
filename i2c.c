//
//  i2c.c
//
//	Proj06*
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#include "i2c.h"



void i2c_aquire_bus (i2c_mod_t const n)
{
	while (MCF_I2C_I2SR_IBB(n) = 1)
	{

	}
}


void i2c_init(i2c_mod_t const n, uint8 const freq, int const addr)
{
	if (n == i2c_mod_0)
	{
		//void gpio_port_init(gpio_port_t p_port, gpio_pin_t p_pin,
		//gpio_funct_t p_funct, gpio_data_dir_t p_data_dir,
		//gpio_pin_state_t p_state)

		gpio_port_init(gpio_port_as, gpio_pin_0, gpio_funct_primary, gpio_dir_x, gpio_state_x)
		gpio_port_init(gpio_port_as, gpio_pin_1, gpio_funct_primary, gpio_dir_x, gpio_state_x)
	}
	else
	{
		gpio_port_init(gpio_port_ub, gpio_pin_0, gpio_funct_secondary, gpio_dir_x, gpio_state_x)
		gpio_port_init(gpio_port_ub, gpio_pin_0, gpio_funct_secondary, gpio_dir_x, gpio_state_x)
	}

	MCF_I2C_I2ADR_ADR(n) = addr;
	uint8 divider = (80 * 1000) / freq;
	uint8 ic = 0x3A;
	i2c_reset(n);
	if(MCF_I2C_I2SR_IBB(n) = 1)
	{
		MCF_I2C_I2CR(n) = 0x00;
		MCF_I2C_I2CR(n) = 0xA0;
		byte dummy;
		dummy = i2x_rx_byte(n, 0, 0);
		MCF_I2C_I2SR(n) = 0x00;
		MCF_I2C_I2CR(n) = 0x00;
	}


}
//------------------------------------------------------------------------------------------------------------------------
//-- i2c_reset: When not acting as master-transmitter, the MCF52259 I2C module should be configured for slave-receiver
//-- mode. This function configures I2CR so the MCF52259 will be in slave-receiver mode, interrupts are disabled (we are
//-- going to use polling to determine when transfers complete), received data bytes will be automatically ACKed, and the
//-- I2C module is enabled.
//------------------------------------------------------------------------------------------------------------------------

void i2c_reset(i2c_mod_t const n)
{
	MCF_I2C_I2CR_IEN(n) = 1;
	MCF_I2C_I2CR_IIEN(n) = 0;
	MCF_I2C_I2CR_MSTA(n) = 0;
	MCF_I2C_I2CR_MTX(n) = 0;
	MCF_I2C_I2CR_TXAK(n) = 0;
	MCF_I2C_I2CR_RSTA(n) = 0;
}

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_rx: Reads and returns count data bytes from the slave addressed by addr. The data bytes are stored and returned
//-- in the array data. Busy waits for delay_us µs following each transferred data or address byte. This is necessary as
//-- some I2C devices require a delay between reads.
//------------------------------------------------------------------------------------------------------------------------

void i2c_rx(i2c_mod_t const n, uint8 const addr, int const count, uint32 const delay_us, dtim_t const timer, uint8 const data[])
{
		i2c_aquire_bus(n);
		i2c_tx_addr(n, addr, I2C_READ, delay_us, timer);
		MCF_I2C_I2CR_MTX(n) = 0;
		byte dummy;
		dummy = i2x_rx_byte(n, delay_us, timer);
		int i = 0;
		for(i=1, i >= count, i++)
		{
			if(i == (count-1))
			{
				MCF_I2C_I2CR_TXAK(n) = 1;
			}
			else if(i == count)
			{
				i2c_send_stop();
			}
			d
