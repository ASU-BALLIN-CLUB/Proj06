;***********************************************************************************************************************************
; FILE: int.s
;
; DESCRIPTION
; Functions for handling interrupt sources.
;
; AUTHORS
; Kevin R. Burger (burgerk@asu.edu)
; Computer Science & Engineering
; Arizona State University, Tempe, AZ 85287-8809
; http://www.devlang.com
;***********************************************************************************************************************************
.include "global.inc"

;===================================================================================================================================
; External Global Variable Declarations
;===================================================================================================================================

.extern ___VECTOR_RAM  ; ___VECTOR_RAM is defined in the linker command file (.lcf) to be 0x2000_0000.

;===================================================================================================================================
; Public Function Declarations
;===================================================================================================================================

.global _int_config
.global _int_inhibit_all
.global _int_mask_src
.global _int_unmask_src
.global _int_uninhibit_all

;===================================================================================================================================
; Private Global Constants
;===================================================================================================================================

; These are the INTC0 registers we will be using.
INT0_BASE      .equ IPSBAR + $000C00
INT0_ICR_BASE  .equ INT0_BASE + $0040  ; The ICRn regs are all 8-bits
INT0_IMRH      .equ INT0_BASE + $0008  ; 32-bit reg
INT0_IMRL      .equ INT0_BASE + $000C  ; 32-bit reg
INT0_IPRH      .equ INT0_BASE + $0000  ; 32-bit reg
INT0_IPRL      .equ INT0_BASE + $0004  ; 32-bit reg

;===================================================================================================================================
; Function Definitions
;===================================================================================================================================

.text

;-----------------------------------------------------------------------------------------------------------------------------------
; FUNCTION: __declspec(standard_abi) void int_config(int const p_src, int const p_level, int const p_priority,
;                                                    int_isr_t const p_isr)
;
; INPUT PARAMS
; p_src         8(a6)  The interrupt source in [1, 63].
; p_level      12(a6)  The level of the interrupt in [1, 7].
; p_priority   16(a6)  The priority of the interrupt in [0, 7].
; p_isr        20(a6)  The address of the ISR.
;
; DESCRIPTION
; Places the address of the ISR (p_isr) for the interrupt with source number p_src into the exception vector table at the proper
; offet. Configures the appropriate ICR register with the interrupt level and priority.
;
; RETURNS
; Nothing
;
; REGISTER USAGE
; d0-d1, a0
;
; EQUIVALENT C CODE
; __declspec(standard_abi) void int_config(int const p_src, int const p_level, int const p_priority, int_isr_t const p_isr)
; {
;     ___VECTOR_RAM[p_src + 64] = p_isr;
;     *(ICR_BASE + p_src) = p_level << 3 | p_priority;
;     int_unmask_src(p_src);
; }
;-----------------------------------------------------------------------------------------------------------------------------------
_int_config:
; Create stack frame. Don't allocate space for any local variables.
     link      a6, #0

; p_src is at 8(a6). The address in the exception vector table for interrupt source p_src is at offset 4 x (p_src + 64) from the
; beginning of the table. Load p_src into d0 and then calculate the offset.

; ___VECTOR_RAM[p_src + 64] = p_isr
    move.l     8(a6), d0               ; p_src -> d0
    addi.l     #64, d0                 ; p_src + 64 -> d0
    lsl.l      #2, d0                  ; 4 x (p_src + 64) -> d0
    lea        ___VECTOR_RAM, a0       ; base address of the exception vector table -> a0
    adda.l     d0, a0                  ; ___VECTOR_RAM + 4 x (p_src + 64) -> a0
    move.l     20(a6), (a0)            ; ___VECTOR_RAM[p_src + 64] = p_isr

; *(ICR_BASE + p_src) = p_level << 3 | p_priority
    lea        INT0_ICR_BASE, a0       ; base address of ICRn registers -> a0
    adda.l     8(a6), a0               ; &(ICR_BASE + p_src) -> a0
    move.l     12(a6), d0              ; p_level -> d0
    lsl.l      #3, d0                  ; p_level << 3 -> d0
    move.l     16(a6), d1              ; p_priority -> d1
    or.l       d1, d0                  ; p_level << 3 | p_priority -> d0
    move.b     d0, (a0)                ; *(ICR_BASE + p_src) = p_level << 3 | p_priority

; int_unmask_src(p_src)
    push       8(a6)                   ; Push p_src
    jsr        _int_unmask_src         ; int_unmask_src(p_src)

; Destroy stack frame and return.
    unlk       a6
    rts

;-----------------------------------------------------------------------------------------------------------------------------------
; FUNCTION: __declspec(standard_abi) void int_inhibit_all()
;
; INPUT PARAMS
; None
;
; DESCRIPTION
; Writes 111 to SR[I] to inhibit interrupts at all levels (except for the edge-sensitive level 7 requests which cannot be masked).
; Saves the current state of IMRH and IMRL in g_imrh and g_imrl and then writes 1 to IMRL[MASKALL] to inhibit all interrupt sources.
; IMRH and IMRL will be restored when int_uninhibit_all() is called.
;
; RETURNS
; Nothing
;
; REGISTER USAGE
; d0, a0, SR
;
; EQUIVALENT C CODE
; __declspec(standard_abi) void int_inhibit_all()
; {
;     MCU.SR[I] |= 0x0700;
; }
;
; REMARKS
; This is a leaf procedure so there is no need to create and destroy a stack frame.
;-----------------------------------------------------------------------------------------------------------------------------------
 _int_inhibit_all:
; SR[I] = 0
    move.w     SR, d0                  ; SR -> d0
    ori.l      #$0700, d0              ; Clear SR[I] field
    move.w     d0, SR                  ; SR[I] = 0

; return
    rts

;-----------------------------------------------------------------------------------------------------------------------------------
; FUNCTION: __declspec(standard_abi) void int_mask_src(int const p_src)
;
; INPUT PARAMS
; p_src  8(a6)  Interrupt source
;
; DESCRIPTION
; Writes a 1 to the IMRH or IMRL register to mask interrupts from the interrupt with source number p_src. If p_src > 31 we write 1
; to register IMRH. If p_src <= 31 we write 1 to register IMRL.
;
; RETURNS
; Nothing
;
; REGISTER USAGE
; d0-d1, a0
;
; EQUIVALENT C CODE
; __declspec(standard_abi) void int_mask_src(int const p_src)
; {
;     if (p_src > 31) {
;         INT0_IMRH |= 1 << (p_src - 32);
;     } else {
;         INT0_IMRL |= (1 << p_src) & 0xFFFFFFFE;
;     }
; }
;-----------------------------------------------------------------------------------------------------------------------------------
 _int_mask_src:
; Create stack frame.
    link       a6, #0

; if (p_src > 31) {
    move.l     8(a6), d0             ; p_src -> d0
    cmpi.l     #31, d0               ; is d0 <= 31?
    ble        @false_clause         ; Yes, skip to false-clause.

; INT0_IMRH |= 1 << (p_src - 32)
    subi.l     #32, d0               ; p_src - 32 -> d0
    moveq.l    #1, d1                ; 1 -> d1
    lsl.l      d0, d1                ; 1 << (p_src - 32) -> d1
    lea        INT0_IMRH, a0         ; &INT0_IMRH -> a0
    or.l       d1, (a0)              ; INT0_IMRH |= 1 << (p_src - 32)
    bra        @end_if               ; Skip over false-clause.

@false_clause:
; INT0_IMRL |= (1 << p_src) & 0xFFFFFFFE
    move.l     8(a6), d0             ; p_src -> d0
    moveq.l    #1, d1                ; 1 -> d1
    lsl.l      d0, d1                ; 1 << p_src -> d1
    andi.l     #$FFFFFFFE, d1        ; (1 << p_src) & 0xFFFFFFFE (clears bit 0) -> d1
    lea        INT0_IMRL, a0         ; &INT0_IMRL -> a0
    or.l       d1, (a0)              ; INT0_IMRL |= (1 << p_src) & 0xFFFFFFFE

@end_if:
; Destroy stack frame and return.
    unlk       a6
    rts

;-----------------------------------------------------------------------------------------------------------------------------------
; FUNCTION: __declspec(standard_abi) void int_unmask_src(int const p_src)
;
; INPUT PARAMS
; p_src  8(a6)  Interrupt source
;
; DESCRIPTION
; Writes a 0 to the IMRH or IMRL register to unmask interrupts from the interrupt with source number p_src.
;
; RETURNS
; Nothing
;
; REGISTER USAGE
; d0-d1, a0
;
; EQUIVALENT C CODE
; __declspec(standard_abi) void int_unmask_src(int const p_src)
; {
;     if (p_src > 31) {
;         INT0_IMRH &= ~(1 << (p_src - 32));
;     } else {
;         IMRL &= ~(1 << p_src) & 0xFFFFFFFE;
;     }
; }
;-----------------------------------------------------------------------------------------------------------------------------------
_int_unmask_src:
; Create stack frame.
    link       a6, #0

; if (p_src > 31) {
    move.l     8(a6), d0             ; p_src -> d0
    cmpi.l     #31, d0               ; is d0 <= 31?
    ble        @false_clause         ; Yes, skip to false-clause.

; INT0_IMRH &= ~(1 << (p_src - 32))
    subi.l     #32, d0               ; p_src - 32 -> d0
    moveq.l    #1, d1                ; 1 -> d1
    lsl.l      d0, d1                ; 1 << (p_src - 32) -> d1
    not.l      d1                    ; ~(1 << (p_src - 32)) -> d1
    lea        INT0_IMRH, a0         ; &INT0_IMRH -> a0
    and.l      d1, (a0)              ; INT0_IMRH &= ~(1 << (p_src - 32))
    bra        @end_if               ; Skip over false-clause.

@false_clause:
; IMRL &= ~(1 << p_src) & 0xFFFFFFFE
    move.l     8(a6), d0             ; p_src -> d0
    moveq.l    #1, d1                ; 1 -> d1
    lsl.l      d0, d1                ; 1 << p_src -> d1
    not.l      d1                    ; ~(1 << p_src) -> d1
    andi.l     #$FFFFFFFE, d1        ; ~(1 << p_src) & 0xFFFFFFFE (clears bit 0) -> d1
    lea        INT0_IMRL, a0         ; &INT0_IMRL -> a0
    and.l      d1, (a0)              ; INT0_IMRL &= ~(1 << p_src) & 0xFFFFFFFE

@end_if:
; Destroy stack frame and return.
    unlk       a6
    rts

;-----------------------------------------------------------------------------------------------------------------------------------
; FUNCTION: __declspec(standard_abi) void int_uninhibit_all()
;
; INPUT PARAMS
; None
;
; DESCRIPTION
; Writes 000 to SR[I] to permits interrupts at all levels. Restores IMRH and IMRL.
;
; RETURNS
; Nothing
;
; REGISTER USAGE
; d0, a0, SR
;
; EQUIVALENT C CODE
; __declspec(standard_abi) void int_uninhibit_all()
; {
;     MCU.SR &= 0xF8FF;
; }
;
; REMARKS
; This is a leaf procedure so there is no need to create and destroy a stack frame.
;-----------------------------------------------------------------------------------------------------------------------------------
 _int_uninhibit_all:
; SR[I] = 0
    move.w     SR, d0
    andi.l     #$F8FF, d0
    move.w     d0, SR

; return
    rts
