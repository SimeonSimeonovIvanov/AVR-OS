/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   29.04.2006
    Last Update:   19.05.2006

     -------------------------------------------------------------------------
    |                LCD type: JM162A          LCD Port:  PORTx               |
    |-------------------------------------------------------------------------|
    | MCU | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3  | Bit 2  | Bit 1  | Bit 0  |
     -------------------------------------------------------------------------
    | LCD |  ---  | Pin 4 | Pin 5 | Pin 6 | Pin 14 | Pin 13 | Pin 12 | Pin 11 |
     -------------------------------------------------------------------------
    |/\/\/|  ---  |   RS  |  R/W  |   E   | Data 3 | Data 2 | Data 1 | Data 0 |
     -------------------------------------------------------------------------
*/

#define __JM162A_API__

#ifndef __DELAY_API__
	.include "scr/lib/delay/delay-api.inc"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDPutStrR
	movw ZL, @0 // Адреса на низа (LOW byte)
	mov r16, @1 // Адреса на LCD-то
	mov r17, @2 // Разположение на низа (0=FLASH; 1=SRAM)
	OSCall LCDPutStrF
.endmacro
//	-	-	-	-	-	-	-	-	-	-	-	-	-	-
.macro LCDPutStrC
	ldi ZH, high(@0*2) 
	ldi ZL, low(@0*2)
	ldi r17, @1 // Адреса на LCD-то
	OSCall LCDPutStrF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDPutCharR
	mov r16, @0 // Char
	mov r17, @1 // Адреса на LCD-то
	OSCall LCDPutCharF
.endmacro
//	-	-	-	-	-	-	-	-	-	-	-	-	-	-
.macro LCDPutCharC
	ldi r16, @0 // Char
	ldi r17, @1 // Адреса на LCD-то
	OSCall LCDPutCharF
.endmacro
//	-	-	-	-	-	-	-	-	-	-	-	-	-	-
.macro LCDPutCharRC
	mov r16, @0 // Char
	ldi r17, @1 // Адреса на LCD-то
	OSCall LCDPutCharF
.endmacro
//	-	-	-	-	-	-	-	-	-	-	-	-	-	-
.macro LCDPutCharCR
	ldi r16, @0 // Char
	mov r17, @1 // Адреса на LCD-то
	OSCall LCDPutCharF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDClear
	OSCall LCDClearDisplayF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDFunctionSet
	ldi r16, @0
	OSCall LCDFunctionSetF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDEntryModeSet
	ldi r16, @0
	OSCall LCDEntryModeSetF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDCursorOrDisplayShift
	ldi r16, @0
	OSCall LCDCursorOrDisplayShiftF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro LCDDisplay_ON_OFF_Control
	ldi r16, @0
	OSCall LCDDisplay_ON_OFF_ControlF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

