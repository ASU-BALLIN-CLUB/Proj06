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

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_acquire_bus: Busy waits for the I2C bus to become idle.
//------------------------------------------------------------------------------------------------------------------------


void i2c_aquire_bus (i2c_mod_t const n)
{
	while (MCF_I2C1_I2SR & MCF_I2C_I2SR_IBB)
	{

	}
}
//------------------------------------------------------------------------------------------------------------------------
//-- i2c_init: Initializes ColdFire I2C module n so: (1) The I2C clock frequency is freq with freq in [21, 4000] KHz;
//-- (2) The MCF52259 is configured for slave-receiver mode; (3) The MCF52259 I2C address is addr; and (4) I2C
//-- interrupts are disabled.
//------------------------------------------------------------------------------------------------------------------------


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
		gpio_port_init(gpio_port_ub, gpio_pin_1, gpio_funct_secondary, gpio_dir_x, gpio_state_x)
	}

	MCF_I2C_I2ADR_ADR(n) = addr;
	uint8 divider = (80 * 1000) / freq;
	uint8 ic = 0x3A;
	MCF_I2C_I2FDR_IC(n) = ic;
	i2c_reset(n);
	if(MCF_I2C_I2SR_IBB(n) = 1)
	{
		MCF_I2C_I2CR(n) = 0x00;
		MCF_I2C_I2CR(n) = 0xA0;
		unit8 dummy;
		dummy = i2x_rx_byte(n, 0, 0);
		MCF_I2C_I2SR(n) = 0x00;
		MCF_I2C_I2CR(n) = 0x00;
		MCF_I2C_I2CR(n) = 0x80;
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
	MCF_I2C_I2CR_TXAK(n) = 0;
	uint8 dummy;
	dummy = i2x_rx_byte(n, delay_us, timer);
	int i = 0;
	for(i=1, i <= count, i++)
	{
		if(i == (count-1))
		{
			MCF_I2C_I2CR_TXAK(n) = 1;
		}
		else if(i == count)
		{
			i2c_send_stop(n);
		}
		data[i-1] = i2c_rx_byte(n, delay_us, timer);
	}
	i2c_reset(n);
}

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_rx_byte: Receives one data byte from the slave-transmitter and returns it. Delays for delay_us µs following the
//-- transfer. If delay_us is 0 we do not delay.
//------------------------------------------------------------------------------------------------------------------------

uint8 i2c_rx_byte(i2c_mod_t const n, uint32 const delay_us, dtim_t const timer)
{
	uint8 rx_byte;
	rx_byte = MCF_I2C_I2DR(n);
	MCF_I2C_I2SR_IFF(n) = 0;
	if(delay_us != 0)
	{
		dtim_busy_delay_us(timer, delay_us);
		//void dtim_busy_delay_us(dtim_t const p_timer, uint32 const p_usecs)
	}
	return rx_byte;
}

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_send_stop: Sends the stop bit prior to ending the communication.
//------------------------------------------------------------------------------------------------------------------------

void i2c_send_stop(i2c_mod_t n)
{
	MCF_I2C_I2CR_MSTA(n) = 0;
}//end function

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_tx: Transmits count data bytes (stored in the array data) to the slave with address addr. Busy delay for delay_us
//-- µs following each transferred byte as some devices require a delay between data bytes.
//------------------------------------------------------------------------------------------------------------------------

void i2c_tx(i2c_mod_t const n, uint8 const addr, int const count, uint32 const delay_us, dtim_t const timer, uint8 data[])
{
	i2c_aquire_bus(n);
	i2c_tx_addr(n, addr, I2C_WRITE, delay_us, timer);
	int i = 0;
	for(i=0, i < count, i++)
	{
		i2c_tx_byte(n, data[i], delay_us, timer);
	}
	i2c_send_stop(n);
	i2c_reset(n);
}

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_tx_addr: Begins a transmission to the slave with address addr. This function will place the MCF52259 I2C module
//-- into master-transmitter mode, transmit the slave address addr, and transmit the R/W bit set to rw. Note: The I2C bus
//-- should be acquired by calling i2c_acquire_bus() before calling this function.
//------------------------------------------------------------------------------------------------------------------------

void i2c_tx_addr(i2c_mod_t const n, uint8 const addr, int const rw, uint32 const delay_us, dtim_t const timer)
{
	MCF_I2C_I2CR_MTX(n) = 1;
	MCF_I2C_I2CR_MSTA(n) = 1;
	uint8 tx_byte;
	if(rw == I2C_READ)
	{
		tx_byte = 0x53;
	}
	else
	{
		tx_byte = 0x52;
	}
	i2c_tx_byte(n, tx_byte, delay_us, timer);
}

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_tx_byte: Transmits one byte (either 8-bits of data or a 7-bit address followed by the R/W bit) to a slave. Busy
//-- delays for delay_us µs following the transfer as some slaves require a delay between bytes. After the ninth
//-- SCL clock pulse, the slave will either ACK or NACK the byte. Note: I2C is exquisitely sensitive to timing, so
//-- interrupts from all levels are inhibited while the transfer takes place.
//------------------------------------------------------------------------------------------------------------------------

void i2c_tx_byte(i2c_mod_t const n, uint8 const tx_byte, uint32 const delay_us, dtim_t const timer)
{
	flag = false;
	int_inhibit_all();
	MCF_I2C_I2DR(n) = tx_byte;
	while(flag == false)
	{
		flag = i2c_tx_complete(n);
	}
	MCF_I2C_I2SR_IIF(n) = 0;
	int_unihibit_all();
	if(delay_us != 0)
	{
		dtim_busy_delay_us(timer, delay_us);
	}
}

//------------------------------------------------------------------------------------------------------------------------
//-- i2c_tx_complete: Returns true if the transfer of a data or address byte has completed or false if it is still in
//-- progress. Note: You might think that you would check for transfer complete by examining I2SR[ICF] but if you read
//-- §29.4.3 the IMRM tells you that I2SR[IIF] is also set when a transfer completes. Continuing, the second sentence of
//-- the second paragraph states, "Polling should monitor IIF rather than ICF, because the operation is different when
//-- arbitration is lost". Whatever.
//------------------------------------------------------------------------------------------------------------------------

bool_t i2c_tx_complete(i2c_mod_t const n)
{
	if(MCF_I2C_I2SR_IIF == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}





