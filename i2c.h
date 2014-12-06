//
//  i2c.h
//
//	Proj06*
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#ifndef I2C_H_
#define I2C_H_

#include "support_common.h"
#include "dtim.h"
#include "gpio.h"
#include "int.h"
#include "oct_nunchuk.h"
#include "pit.h"

typedef enum
{
	i2c_mod_0 = 0,
	i2c_mod_1 = 1
} i2c_mod_t;

static int I2C_READ = 1;
static int I2C_WRITE = 0;

void i2c_aquire_bus (i2c_mod_t const n);
void i2c_init(i2c_mod_t const n, uint8 const freq, int const addr);
void i2c_reset(i2c_mod_t const n);
void i2c_rx(i2c_mod_t const n, uint8 const addr, int const count, uint32 const delay_us, dtim_t const timer, uint8 const data[]);\
uint8 i2c_rx_byte(i2c_mod_t const n, uint32 const delay_us, dtim_t const timer);
void i2c_send_stop(i2c_mod_t n);
void i2c_tx(i2c_mod_t const n, uint8 const addr, int const count, uint32 const delay_us, dtim_t const timer, uint8 data[]);
void i2c_tx_addr(i2c_mod_t const n, uint8 const addr, int const rw, uint32 const delay_us, dtim_t const timer);
void i2c_tx_byte(i2c_mod_t const n, uint8 const tx_byte, uint32 const delay_us, dtim_t const timer);
bool_t i2c_tx_complete(i2c_mod_t const n);


#endif
