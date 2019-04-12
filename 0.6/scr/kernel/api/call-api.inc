/*
	      The^day^of^DooM

	Create date:   15.15.2006
	Last Update:   17.05.2006
*/

.macro OSCall
	cli
	push r17
	push r16

	ldi r17, high(@0)
	ldi r16, low(@0)
	call OSCallF
.endmacro

.macro OSRet
	cli
	jmp OSRetF
.endmacro
