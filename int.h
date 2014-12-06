//**********************************************************************************************************************************
// FILE: int.h
//
// DECRIPTION
// Public declarations for interrupt controller functions. The functions are written in assembly language and are found in int.s.
// Note that all of the functions are declared using the standard_abi calling convention (for no good reason other than for
// educational purposes; standard ABI is generally slower than register ABT).
//
// AUTHORS
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe, AZ 85287-8809
// http://www.devlang.com
//**********************************************************************************************************************************
#ifndef INT_H
#define INT_H

//==================================================================================================================================
// Global Preprocessor Macros
//==================================================================================================================================

// Define interrupt sources, levels, and priorities.
#define ADCA_INT_SRC         (49)
#define ADCA_INT_LVL         (2)
#define ADCA_INT_PRI         (7)

#define ADCB_INT_SRC         (50)
#define ADCB_INT_LVL         (2)
#define ADCB_INT_PRI         (6)

#define DTIM_INT_SRC(timer)  (19 + (timer))
#define DTIM_INT_LVL(timer)  (6)
#define DTIM_INT_PRI(timer)  (7 - (timer))

#define GPT_INT_SRC(chan)    (44 + (chan))
#define GPT_INT_LVL(chan)    (4)
#define GPT_INT_PRI(chan)    (7 - (chan))

#define I2C0_INT_SRC         (17)
#define I2C0_INT_LVL         (5)
#define I2C0_INT_PRI         (7)

#define I2C1_INT_SRC         (62)
#define I2C1_INT_LVL         (5)
#define I2C1_INT_PRI         (6)

#define PIT_INT_SRC(timer)   (55 + (timer))
#define PIT_INT_LVL(timer)   (6)
#define PIT_INT_PRI(timer)   (3 - (timer))

#define QSPI_INT_SRC         (18)
#define QSPI_INT_LVL         (5)
#define QSPI_INT_PRI         (5)

#define UART_INT_SRC(uart)   (13 + (uart))
#define UART_INT_LVL(uart)   (5)
#define UART_INT_PRI(uart)   (4 - (uart))

//==================================================================================================================================
// Global Type Definitions
//==================================================================================================================================

// This typedef defines a function pointer type named int_isr_t which is a pointer to a void function which has no parameters.
typedef void (*int_isr_t)();

//==================================================================================================================================
// Public Function Declarations
//==================================================================================================================================

//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: __declspec(standard_abi) void int_config(int p_src, int p_level, int p_priority, int_isr_t p_isr)
//
// DESCRIPTION
// Configures interrupt source p_src to have level p_level, priority p_priority. The address of the ISR in p_isr is placed in the
// exception vector table.
//----------------------------------------------------------------------------------------------------------------------------------
__declspec(standard_abi) void int_config(int const p_src, int const p_level, int const p_priority, int_isr_t const p_isr);

//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: __declspec(standard_abi) void int_hibit_all()
//
// DESCRIPTION
// Writes 111 to SR[I] to inhibit interrupt requests from all levels.
//----------------------------------------------------------------------------------------------------------------------------------
__declspec(standard_abi) void int_inhibit_all();

//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: __declspec(standard_abi) void int_mask_src(int p_src)
//
// DESCRIPTION
// Masks interrupt requests from the interrupt source p_src by writing a 1 to the appropriate bit in either the IMRH (if p_src > 31)
// or IMRL (if p_src <= 31) standard.
//----------------------------------------------------------------------------------------------------------------------------------
__declspec(standard_abi) void int_mask_src(int const p_src);

//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: __declspec(standard_abi) void int_unmask_src(int p_src)
//
// DESCRIPTION
// Unmasks interrupt requests from the interrupt source p_src by writing a 0 to the appropriate bit in either the IMRH (if p_src >
// 31) or IMRL (if p_src <= 31) standard.
//----------------------------------------------------------------------------------------------------------------------------------
__declspec(standard_abi) void int_unmask_src(int const p_src);

//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTION: __declspec(standard_abi) void int_unhibit_all()
//
// DESCRIPTION
// Writes 000 to SR[I] to permit interrupt requests from all levels.
//----------------------------------------------------------------------------------------------------------------------------------
__declspec(standard_abi) void int_uninhibit_all();

#endif
 
