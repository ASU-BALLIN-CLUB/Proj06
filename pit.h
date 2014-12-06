//
//  pit.h
//
//	Proj02
//  Lab Partners: Sean Slamka, Aydin Balci
//  Email: sslamka@asu.edu, abalci@asu.edu
//  CSE325 Embedded Microprocessor Systems
//  Fall 2014
//

#ifndef ____pit__
#define ____pit__

#include "support_common.h"

#include"gpio.h"
#include"global.h"
#include"int.h"
#include"uc_dipsw.h"
#include"gpt.h"
#include"oct_rgb_led.h"
#include"pit.h"
#include"pwm.h"
#include"uc_pushb.h"
#include"uc_led.h"

typedef enum {
 pit_timer_0 = 0, //
 pit_timer_1 = 1 //
} pit_timer_t;

__declspec(interrupt) void pit0_isr();
__declspec(interrupt) void pit1_isr();
static int_isr_t g_pit_callbacks[2] = {0};
static int_isr_t g_pit_isrs[2] = {pit0_isr, pit1_isr};
void pit_init(pit_timer_t const n, uint8 const p_prescaler, uint16 const p_pmr, int_isr_t const p_callback);
void pit_disable(pit_timer_t const n);
void pit_enable(pit_timer_t const n);

//static uint32 const MCF_PIT_BASE = 0x40150000;
//#define MCF_PIT_PMR(timer)   *(volatile uint16 *) (MCF_PIT_BASE + 0x02 + ((timer) << 4))
//#define MCF_PIT_PCNTR(timer) *(volatile uint16 *) (MCF_PIT_BASE + 0x04 + ((timer) << 4))
//#define MCF_PIT_PCSR(timer)  *(volatile uint16 *) (MCF_PIT_BASE + 0x00 + ((timer) << 4))



#endif
