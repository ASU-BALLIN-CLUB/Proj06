;***********************************************************************************************************************************
; FILE: gpio.inc
;
; DESCRIPTION
; Global assembly language declarations for stuff declared in gpio.h.
;
; AUTHORS
; Kevin R. Burger (burgerk@asu.edu)
; Computer Science & Engineering
; Arizona State University, Tempe, AZ 85287-8809
; http://www.devlang.com
;***********************************************************************************************************************************

;===================================================================================================================================
; Global Type Definitions
;===================================================================================================================================

; Enumerated Type: gpio_data_dir_t
gpio_data_dir_in:   .equ 0             ; Input pin
gpio_data_dir_out:  .equ 1             ; Output pin
gpio_data_dir_x:    .equ 2             ; Don't care

; Enumerated Type: gpio_funct_t
gpio_funct_gpio:       .equ 0x00       ; Quaternary (GPIO) function
gpio_funct_primary:    .equ 0x01       ; Primary function
gpio_funct_secondary:  .equ 0x02       ; Secondary function
gpio_funct_tertiary:   .equ 0x03       ; Tertiary function

; Enumerated Type: gpio_pin_t
gpio_pin_0:  .equ 0
gpio_pin_1:  .equ 1
gpio_pin_2:  .equ 2
gpio_pin_3:  .equ 3
gpio_pin_4:  .equ 4
gpio_pin_5:  .equ 5
gpio_pin_6:  .equ 6
gpio_pin_7:  .equ 7

; Enumerated Type: gpio_pin_state_t
gpio_pin_state_high:  .equ 1           ; High
gpio_pin_state_low:   .equ 0           ; Low
gpio_pin_state_x:     .equ 2           ; Don't care

; Enumerated Type: gpio_port_t
gpio_port_an:  .equ 0x0A
gpio_port_as:  .equ 0x0B
gpio_port_dd:  .equ 0x14
gpio_port_nq:  .equ 0x08
gpio_port_qs:  .equ 0x0C
gpio_port_ta:  .equ 0x0E
gpio_port_tc:  .equ 0x0F
gpio_port_te:  .equ 0x00
gpio_port_tf:  .equ 0x01
gpio_port_tg:  .equ 0x02
gpio_port_th:  .equ 0x03
gpio_port_ti:  .equ 0x04
gpio_port_tj:  .equ 0x06
gpio_port_ua:  .equ 0x11
gpio_port_ub:  .equ 0x12
gpio_port_uc:  .equ 0x13

;==================================================================================================================================
; Public Function Declarations
;==================================================================================================================================

.extern _gpio_port_get_pin_state
.extern _gpio_port_init
.extern _gpio_port_set_pin_state
