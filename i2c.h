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

typedef enum
{
	i2c_mod_0 = 0,
	i2c_mod_1 = 1
} i2c_mod_t;

static int I2C_READ = 1;
static int I2C_WRITE = 0;
