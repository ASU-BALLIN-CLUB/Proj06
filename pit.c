//
//  pit.c
//
//	Proj02
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#include "pit.h"

//static uint32 const MCF_PIT_BASE = 0x40150000;


//#define MCF_PIT_PMR(timer)   *(volatile uint16 *) (MCF_PIT_BASE + 0x02 + ((timer) << 4))
//#define MCF_PIT_PCNTR(timer) *(volatile uint16 *) (MCF_PIT_BASE + 0x04 + ((timer) << 4))
//#define MCF_PIT_PCSR(timer)  *(volatile uint16 *) (MCF_PIT_BASE + 0x00 + ((timer) << 4))


void pit_init(pit_timer_t const n, uint8 const p_prescaler, uint16 const p_pmr ,int_isr_t const p_callback)
{
	pit_disable(n);
	switch(n){
		case pit_timer_0:
			MCF_PIT0_PCSR |= MCF_PIT_PCSR_PRE(p_prescaler);
			MCF_PIT0_PCSR &= ~MCF_PIT_PCSR_DOZE;
			MCF_PIT0_PCSR |= MCF_PIT_PCSR_DBG;
			MCF_PIT0_PCSR |= MCF_PIT_PCSR_OVW;
			MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIE;
			MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIF;
			MCF_PIT0_PCSR |= MCF_PIT_PCSR_RLD;
			MCF_PIT0_PMR  |= MCF_PIT_PMR_PM(p_pmr);
			int_config(PIT_INT_SRC(n), PIT_INT_LVL(n), PIT_INT_PRI(n), g_pit_isrs[n]);
			break;
		case pit_timer_1:
			MCF_PIT1_PCSR |= MCF_PIT_PCSR_PRE(p_prescaler);
			MCF_PIT1_PCSR &= ~MCF_PIT_PCSR_DOZE;
			MCF_PIT1_PCSR |= MCF_PIT_PCSR_DBG;
			MCF_PIT1_PCSR |= MCF_PIT_PCSR_OVW;
			MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIE;
			MCF_PIT1_PCSR |= MCF_PIT_PCSR_PIF;
			MCF_PIT1_PCSR |= MCF_PIT_PCSR_RLD;
			MCF_PIT1_PMR  |= MCF_PIT_PMR_PM(p_pmr);
			int_config(PIT_INT_SRC(n), PIT_INT_LVL(n), PIT_INT_PRI(n), g_pit_isrs[n]);
			break;
	}
	g_pit_callbacks[n] = p_callback;
	pit_enable(n);
}

void pit_disable(pit_timer_t const n)
{
	MCF_PIT_PCSR(n) &= 0xFFFE;
}

void pit_enable(pit_timer_t const n)
{
	MCF_PIT_PCSR(n) |= 0x0001;
}

__declspec(interrupt) void pit0_isr()
{
	MCF_PIT_PCSR(0) |= (0x04);
	uc_led_on((gpio_pin_t)uc_led_1);
	if(g_pit_callbacks[0] != 0)
	{
		(*g_pit_callbacks[0])();
	}
	uc_led_off((gpio_pin_t)uc_led_1);
}

__declspec(interrupt) void pit1_isr()
{
	MCF_PIT_PCSR(1) |= (0x04);
	if(g_pit_callbacks[1] != 0)
	{
		(*g_pit_callbacks[1])();
	}
}
