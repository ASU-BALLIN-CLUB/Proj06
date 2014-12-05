//
//  dtim.c
//
//	Proj03
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//


#include "dtim.h"

// The base address in the peripheral register range of the DTIM module registers.
static uint32 const DTIM_BASE = 0x40000400;
// The addresses of the DTIM registers for timers n = 0, 1, 2, and 3.
#define DTIM_DTCN(timer) *(volatile uint32 *) (DTIM_BASE + 0x0C + ((timer) << 6))
#define DTIM_DTCR(timer) *(volatile uint32 *) (DTIM_BASE + 0x08 + ((timer) << 6))
#define DTIM_DTER(timer) *(volatile uint8 *) (DTIM_BASE + 0x03 + ((timer) << 6))
#define DTIM_DTMR(timer) *(volatile uint16 *) (DTIM_BASE + 0x00 + ((timer) << 6))
#define DTIM_DTRR(timer) *(volatile uint32 *) (DTIM_BASE + 0x04 + ((timer) << 6))
#define DTIM_DTXMR(timer) *(volatile uint8 *) (DTIM_BASE + 0x02 + ((timer) << 6))
// static int_isr_t g_dtim_callbacks[4] = {0};
// static int_isr_t g_dtim_irsrs[4] = {dtim0_isr, dtim1_isr, dtim2_isr, dtim3_isr};

// Function which takes a timer and unsigned 32 bit integer input and tells
// if the input is small enough to be entered as microseconds rather than miliseconds.
// Then calls the microsecond function if it is small enough.
void dtim_busy_delay_ms(dtim_t const p_timer, uint32 const p_msecs)
{
	if(p_msecs < 4294968)
	{
		dtim_busy_delay_us(p_timer, p_msecs * 1000);
	}
}
// The timer delay function with microseconds rather than miliseconds.
void dtim_busy_delay_us(dtim_t const p_timer, uint32 const p_usecs)
{
	DTIM_DTER(p_timer) = 0x03;
	DTIM_DTCN(p_timer) = 0;
	DTIM_DTRR(p_timer) = (uint32)(p_usecs - 1);
	DTIM_DTMR(p_timer) |= 0x0001;
	while(~DTIM_DTER(p_timer) & 0x02)
	{
	}
}
// Initializes the Timers.
void dtim_init(dtim_t const p_timer)
{
	DTIM_DTMR(p_timer) |= 0x0001;
	DTIM_DTMR(p_timer) &= ~(0x0001);
	DTIM_DTMR(p_timer) = 0x4F02;
	DTIM_DTXMR(p_timer) = 0x40;
}

void dtim_init_irq(dtim_t const p_timer, uint32 const p_usecs, int_isr_t const p_callback)
{
	DTIM_DTMR(p_timer) |= (0x01);
	DTIM_DTMR(p_timer) &= (0xFFFE);
	DTIM_DTMR(p_timer) = 0x4F1A;
	DTIM_DTXMR(p_timer) = 0x40;
	DTIM_DTCN(p_timer) = 0x0;
	DTIM_DTRR(p_timer) = (uint32)(p_usecs - 1);
	DTIM_DTER(p_timer) |= (0x02);
	g_dtim_callbacks[p_timer] = p_callback;
	int_config(DTIM_INT_SRC(p_timer), DTIM_INT_LVL(p_timer), DTIM_INT_PRI(p_timer), g_dtim_irsrs[p_timer]);
	DTIM_DTMR(p_timer) |= (0x01);
}

__declspec(interrupt) void dtim0_isr()
{
	DTIM_DTER(0) |= (0x02);
	if(g_dtim_callbacks[0] != 0)
	{
		(*g_dtim_callbacks[0])();
	}
}

__declspec(interrupt) void dtim1_isr()
{
	DTIM_DTER(1) |= (0x02);
	if(g_dtim_callbacks[1] != 0)
	{
		(*g_dtim_callbacks[1])();
	}
}

__declspec(interrupt) void dtim2_isr()
{
	DTIM_DTER(2) |= (0x02);
	if(g_dtim_callbacks[2] != 0)
	{
		(*g_dtim_callbacks[2])();
	}
}

__declspec(interrupt) void dtim3_isr()
{
	DTIM_DTER(3) |= (0x02);
	if(g_dtim_callbacks[3] != 0)
	{
		(*g_dtim_callbacks[3])();
	}
}
