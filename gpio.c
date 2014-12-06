//
//  gpio.c
//
//	Proj03
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#include "gpio.h"

// All the #defines necessary.
static const uint32 GPIO_BASE = 0x40100000; // Base address of the GPIO module in the IPS
static const uint32 GPIO_DDR_BASE = GPIO_BASE + 0x18; // Base address of the DDR registers in the GPIO space
static const uint32 GPIO_SET_BASE = GPIO_BASE + 0x30; // Base address of the SET registers in the GPIO space
static const uint32 GPIO_CLR_BASE = GPIO_BASE + 0x48; // Base address of the CLR registers in the GPIO space
static const uint32 GPIO_PAR_BASE = GPIO_BASE + 0x60; // Base address of the PAR registers in the GPIO space
#define GPIO_SET(n) (*(vuint8 *)(GPIO_SET_BASE + (n))) // SET reg, n is GPIO_PORT_TC or GPIO_PORT_DD
#define GPIO_DDR(n) (*(vuint8 *)(GPIO_DDR_BASE + (n))) // DDR reg
#define GPIO_CLR(n) (*(vuint8 *)(GPIO_CLR_BASE + (n))) // CLR reg
#define GPIO_PAR(n) (*(vuint8 *)(GPIO_PAR_BASE + (n))) // PAR reg

// Initializes the gpio ports.
void gpio_port_init(gpio_port_t p_port, gpio_pin_t p_pin, gpio_funct_t p_funct, gpio_data_dir_t p_data_dir, gpio_pin_state_t p_state)
{
	// Switch to check for either port DD or port TC.
	 switch (p_port)
	 {
	 	case gpio_port_dd:
	 		GPIO_PAR(p_port) &= ~(1 << p_pin);
	 		GPIO_PAR(p_port) |= (p_funct << p_pin);
	 		break;
	 	case gpio_port_an:
	 		GPIO_PAR(p_port) &= ~(1 << p_pin);
	 		GPIO_PAR(p_port) |= (p_funct << p_pin);
	 		break;
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
	// If statement to see if the function is set to GPIO.
	if(p_funct == gpio_funct_gpio)
	{
		gpio_port_set_data_dir(p_port, p_pin, p_data_dir);
		if(p_data_dir == gpio_data_dir_out)
		{
			gpio_port_set_pin_state(p_port,p_pin,p_state);
		}
	}
}
// Gives the state of a certain gpio pin.
gpio_pin_state_t gpio_port_get_pin_state(gpio_port_t p_port, gpio_pin_t p_pin)
{
	gpio_pin_state_t state = (gpio_pin_state_t)(GPIO_SET(p_port) >>= p_pin);
	return state;
}
// Sets the data direction of a gpio port.
static void gpio_port_set_data_dir(gpio_port_t p_port, gpio_pin_t p_pin, gpio_data_dir_t p_data_dir)
{
	// If/else statements to check if the direction is input or output.
	if(p_data_dir == gpio_data_dir_in)
	{
		GPIO_DDR(p_port) &= ~(1 << p_pin);
	}
	else if (p_data_dir == gpio_data_dir_out)
	{
		GPIO_DDR(p_port) |= (1 << p_pin);
	}
}
// Sets the pin state of a gpio port.
void gpio_port_set_pin_state(gpio_port_t p_port, gpio_pin_t p_pin, gpio_pin_state_t p_state)
{
	// If statement to check if the pin state is high or low.
	if(p_state == gpio_pin_state_high)
	{
		GPIO_SET(p_port) |= (1 << p_pin);
	}
	else
	{
		GPIO_CLR(p_port) &= ~(1 << p_pin);
	}
}
