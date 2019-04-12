/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   12.05.2006
*/

//------------------------------------------------------------------------------------
OSINT0:
	cli
#if LCD && LCD_DEBUG
	//LCDClear
	LCDPutStrC INT0_Str, 0x4C, 0
  INT0_loop:
	sbis PIND, PIND2
  jmp INT0_loop
#endif
	OSRunTaskIRQC INT0
reti
//------------------------------------------------------------------------------------
OSINT1:
	cli
#if LCD && LCD_DEBUG
	//LCDClear
	LCDPutStrC INT1_Str, 0x4C, 0
  INT1_loop:
	sbis PIND, PIND2
  jmp INT1_loop 
#endif
	OSRunTaskIRQC INT1
reti
//------------------------------------------------------------------------------------
OSRunTaskIRQF:
	push r16
	push XL
	push XH
	push ZL
	push ZH

	OSGetNumberOfCurrentTask
	call OSLoadTaskIRQTableF

	and r16, r17
	tst r16
	breq OSRunTaskIRQF_ret

	icall
OSRunTaskIRQF_ret:
	pop ZH
	pop ZL
	pop XH
	pop XL
	pop r16
ret
//------------------------------------------------------------------------------------
OSInitTaskIRQF:
	push r16
	push r17
	push XL
	push XH
	push ZL
	push ZH

	call OSLoadTaskIRQTableF

	clr r17
	tst r16
	breq OSInitTaskIRQF_ret

	sbrc r16, INT0
	sbr r17, (1<<INT0)

	sbrc r16, INT1
	sbr r17, (1<<INT1)

OSInitTaskIRQF_ret:
	out GICR, r17
	pop ZH
	pop ZL
	pop XH
	pop XL
	pop r17
	pop r16
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса
	return:
		R16 - Маска на прекъсванията
		Z - Адреса на процедурата прикачена към прекъсването
*/
OSLoadTaskIRQTableF:
	push XL
	push XH

	call OSGetTaskDisplacementF	
	adiw ZH:ZL, 5
	movw XL, ZL
	nop

	ld r16, X+
	ld ZH, X+
	ld ZL, X+
	
	pop XH
	pop XL
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номер на процеса
	R17 - IRQ
	  X - Процедура на на прекъсването
*/
OSSaveTaskIRQTableF:
	push r16
	push ZL
	push ZH

	call OSGetTaskDisplacementF
	adiw ZH:ZL, 5
	nop

	st Z+, r17
	st Z+, XH
	st Z+, XL
	
	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
