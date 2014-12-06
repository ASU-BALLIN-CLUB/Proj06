//
//  gpio.h
//
//	Proj03
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#ifndef ____gpio__
#define ____gpio__
#include "support_common.h"

//Header file for gpio function
typedef enum {
 gpio_data_dir_in = 0, // Configure pin in a DDR reg to be an Input pin
 gpio_data_dir_out = 1, // Configure pin in a DDR reg to be an Output pin
 gpio_data_dir_x = 2 // Don't care. Used when the pin is not being configured for the GPIO function
} gpio_data_dir_t;

typedef enum {
 gpio_funct_gpio = 0x00, // Configure a pin's function in the PAR register for the GPIO function
 gpio_funct_primary = 0x01, // Configure a pin's function in the PAR register for the Primary function
 gpio_funct_secondary = 0x02, // Configure a pin's function in the PAR register for the Secondary function
 gpio_funct_tertiary = 0x03 // Configure a pin's function in the PAR register for the Tertiary function
} gpio_funct_t;

typedef enum {
 gpio_pin_0 = 0, // Pin 0 of a GPIO port
 gpio_pin_1 = 1, // Pin 1 of a GPIO port
 gpio_pin_2 = 2, // Pin 2 of a GPIO port
 gpio_pin_3 = 3, // Pin 3 of a GPIO port
 gpio_pin_4 = 4, // Pin 4 of a GPIO port
 gpio_pin_5 = 5, // Pin 5 of a GPIO port
 gpio_pin_6 = 6, // Pin 6 of a GPIO port
 gpio_pin_7 = 7 // Pin 7 of a GPIO port
} gpio_pin_t;

typedef enum {
 gpio_pin_state_high = 1, // Configures pin's output state in a SET register to be High
 gpio_pin_state_low = 0, // Configures pin's output state in a SET register to be Low
 gpio_pin_state_x = 2 // Don't care. Used when the pin is not being configured for the GPIO function
} gpio_pin_state_t;

typedef enum {
 gpio_port_dd = 0x14, // Offset to the DD register from the beginning of a GPIO reg's address space
 gpio_port_tc = 0x0F, // Offset to the TC register from the beginning of a GPIO reg's address space
 gpio_port_ta = 0x0E, // Offset to the TA register from the beginning of a GPIO reg's address space
 gpio_port_an = 0x0A, // Offset to the AN register from the beginning of a GPIO reg's address
 gpio_port_ub = 0x12
} gpio_port_t;


void gpio_port_init(gpio_port_t p_port, gpio_pin_t p_pin, gpio_funct_t p_funct, gpio_data_dir_t p_data_dir, gpio_pin_state_t p_state);
gpio_pin_state_t gpio_port_get_pin_state(gpio_port_t p_port, gpio_pin_t p_pin);
static void gpio_port_set_data_dir(gpio_port_t p_port, gpio_pin_t p_pin, gpio_data_dir_t p_data_dir);
void gpio_port_set_pin_state(gpio_port_t p_port, gpio_pin_t p_pin, gpio_pin_state_t p_state);


#endif /* defined(__gpio__) */
