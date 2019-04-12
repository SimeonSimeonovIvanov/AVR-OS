/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   19.05.2006
*/

// - - - - - - - - - - - - - -
//  Номера на прекъсванията:
//    !!! Не променяй !!!
.equ	OSINT0	=	0b00000001
.equ	OSINT1	=	0b00000010
.equ	OSOC2	=	0b00000100
.equ	OSOVF2	=	0b00001000
.equ	OSICP1	=	0b00010000
.equ	OSOC1A	=	0b00100000

.equ	OSOC1B	=	0b01000001
.equ	OSOVF1	=	0b01000010
.equ	OSOVF0	=	0b01000100
.equ	OSSPI	=	0b01001000
.equ	OSURXC	=	0b01010000
.equ	OSUDRE	=	0b01100000

.equ	OSUTXC	=	0b10000001
.equ	OSADCC	=	0b10000010
.equ	OSERDY	=	0b10000100
.equ	OSACI	=	0b10001000
.equ	OSTWI	=	0b10010000
.equ	OSINT2	=	0b10100000

.equ	OSOC0	=	0b11000001
.equ	OSSPMR	=	0b11000010
// - - - - - - - - - - - - - -

/*
	Подава се номера на прекъсването, след това
	той се изпраща като съобщение към процеса
*/

.macro OSRunTaskIRQC
	OSSaveL r16
	ldi r16, (@0)
	call OSRunTaskIRQF
.endmacro

.macro OSRemoveIRQFromTaskC
	push r16
	push r17
	ldi r16, @0
	ldi r17, @1
	call OSRemoveIRQFromTaskF
	pop r17
	pop r16
.endmacro

.macro OSAddIRQToTaskC
	push r16
	push r17
	push XL
	push XH

	ldi r16, @0
	ldi r17, @1
	ldi XH, high(@2)
	ldi XL, low(@2)
	call OSAddIRQToTaskF

	pop XH
	pop XL
	pop r17
	pop r16
.endmacro
