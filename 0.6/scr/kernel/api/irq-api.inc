/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   16.05.2006
*/

.macro OSRunTaskIRQC
	push r16
	ldi r16, (1<<@0)
	call OSRunTaskIRQF
	pop r16
.endmacro

.macro OSAddIRQToTaskC
	cli
	push r16
	push r17
	push XL
	push XH

	ldi r16, @0      // Номер на процеса към който ще добавим прекъсване
	ldi r17, (1<<@1) // Номер на желаното прекъсване
	ldi XH, high(@2) // \ Асреса на процедурата обработваща
	ldi XL,  low(@2) // / прекъсването

	call OSSaveTaskIRQTableF

	pop XH
	pop XL
	pop r17
	pop r16
	sei
.endmacro
