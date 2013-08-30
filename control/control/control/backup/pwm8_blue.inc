;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: PWM8_BLUE.inc
;;   Version: 2.5, Updated on 2010/6/8 at 12:37:18
;;  Generated by PSoC Designer 5.1.2309
;;
;;  DESCRIPTION: Assembler declarations for the PWM8 user module interface
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2010. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"


;--------------------------------------------------
; Constants for PWM8_BLUE API's.
;--------------------------------------------------

PWM8_BLUE_CONTROL_REG_START_BIT:           equ 0x01    ; Control register start bit
PWM8_BLUE_INT_REG:                         equ 0x0e1
PWM8_BLUE_INT_MASK:                        equ 0x04


;--------------------------------------------------
; Constants for PWM8_BLUE user defined values
;--------------------------------------------------

PWM8_BLUE_PERIOD:                          equ 0x00
PWM8_BLUE_PULSE_WIDTH:                     equ 0x00


;--------------------------------------------------
; Register Address Constants for PWM8_BLUE
;--------------------------------------------------

PWM8_BLUE_COUNTER_REG:                 equ 0x28   ; DR0 Count register
PWM8_BLUE_PERIOD_REG:                  equ 0x29   ; DR1 Period register
PWM8_BLUE_COMPARE_REG:                 equ 0x2a   ; DR2 Compare register
PWM8_BLUE_CONTROL_REG:                 equ 0x2b   ; Control register
PWM8_BLUE_FUNC_REG:                    equ 0x28   ; Function register
PWM8_BLUE_INPUT_REG:                   equ 0x29   ; Input register
PWM8_BLUE_OUTPUT_REG:                  equ 0x2a   ; Output register


;--------------------------------------------------
; PWM8_BLUE Macro 'Functions'
;--------------------------------------------------

   macro PWM8_BLUE_Start_M
   or    reg[PWM8_BLUE_CONTROL_REG],  PWM8_BLUE_CONTROL_REG_START_BIT
   endm

   macro PWM8_BLUE_Stop_M
   and   reg[PWM8_BLUE_CONTROL_REG], ~PWM8_BLUE_CONTROL_REG_START_BIT
   endm

   macro PWM8_BLUE_EnableInt_M
   M8C_EnableIntMask PWM8_BLUE_INT_REG, PWM8_BLUE_INT_MASK
   endm

   macro PWM8_BLUE_DisableInt_M
   M8C_DisableIntMask PWM8_BLUE_INT_REG, PWM8_BLUE_INT_MASK
   endm


; end of file PWM8_BLUE.inc