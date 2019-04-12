/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   29.04.2006
    Last Update:   30.06.2006

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

#ifndef __DELAY_INC__
	.include "C:\MyINC\delay\delay-api.inc"
#endif

#ifndef __JM162A_API__
#define __JM162A_API__

#define DISPLEY_X_SIZE 16
#define DISPLEY_Y_SIZE 2

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro JM162AInit
	call JM162AInitF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro JM162AClear
	call JM162AClearDisplayF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro JM162AFunctionSet
	ldi r16, @0
	call JM162AFunctionSetF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro JM162AEntryModeSet
	ldi r16, @0
	call JM162AEntryModeSetF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro JM162ACursorOrDisplayShift
	ldi r16, @0
	call JM162ACursorOrDisplayShiftF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.macro JM162ADisplay_ON_OFF_Control
	ldi r16, @0
	call JM162ADisplay_ON_OFF_ControlF
.endmacro
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
