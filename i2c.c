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
	uint8 ic =
	switch (divider)
	{

		case gpio_port_dd:
			GPIO_PAR(p_port) &= ~(1 << p_pin);
		 	GPIO_PAR(p_port) |= (p_funct << p_pin);
		 	break;
		case gpio_port_an:
		 	GPIO_PAR(p_port) &= ~(1 << p_pin);
		 	GPIO_PAR(p_port) |= (p_funct << p_pin);
		 	break;
		case gpio_port_ub:
		case gpio_port_tc:
			GPIO_PAR(p_port) &= ~(3 << 2*p_pin);
			GPIO_PAR(p_port) |= (p_funct << p_pin);
			break;
		case gpio_port_ta:
			GPIO_PAR(p_port) &= ~(3 << 2*p_pin);
			GPIO_PAR(p_port) |= (p_funct << p_pin);
		default:
			break;
	}
}
