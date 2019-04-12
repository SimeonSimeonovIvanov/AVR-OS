/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   19.06.2006

				Структура на таблицата на прекъсванията:

		4 байта таблица
		2 байта адрес на обработващата подпрограма
		------------------------------------------
		БАЙТ_3
		БАЙТ_2
		БАЙТ_1
		БАЙТ_0
		Адрес_на_подпрограмата_H
		Адрес_на_подпрограмата_L

	   Към БАЙТ_0 са "прикачени" най-младшите прекъсвания на процесора а към БАЙТ_3
	най-старшите. Битове 6 и 7 битове са за индификация, започват от 00 и стигат до 11
	(от 0 до 4, точно колкото да се "адресират" и 4-те байата).

	   До днешна дата (16.06.2006) са включени прекъсвания INT0, INT1 и INT2 поради
	факта, че все още не е определено кой прекъсвания ще бъдат изцяло предоставени на
	ядрото и кой ще могат да се дават директно на процесите.
*/

//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса
	R17 - Номера на прекъсването
*/
OSRemoveIRQFromTaskF:
	push r18
	push r19
	push r20
	push r21
	push XL
	push XH

	call OSLoadTaskIRQTableF

	cpi r17, OSINT0
	breq OS_REMOVE_INT0
	cpi r17, OSINT1
	breq OS_REMOVE_INT1
	cpi r17, OSOC2
	breq OS_REMOVE_OC2
	cpi r17, OSOVF2
	breq OS_REMOVE_OVF2
	cpi r17, OSICP1
	breq OS_REMOVE_ICP1
	cpi r17, OSOC1A
	breq OS_REMOVE_OC1A

	cpi r17, OSOC1B
	breq OS_REMOVE_OC1B
	cpi r17, OSOVF1
	breq OS_REMOVE_OVF1
	cpi r17, OSOVF0
	breq OS_REMOVE_OVF0
	cpi r17, OSSPI
	breq OS_REMOVE_SPI
	cpi r17, OSURXC
	breq OS_REMOVE_URXC
	cpi r17, OSUDRE
	breq OS_REMOVE_UDRE

	cpi r17, OSUTXC
	breq OS_REMOVE_UTXC
	cpi r17, OSADCC
	breq OS_REMOVE_ADCC
	cpi r17, OSERDY
	breq OS_REMOVE_ERDY
	cpi r17, OSACI
	breq OS_REMOVE_ACI
	cpi r17, OSTWI
	breq OS_REMOVE_TWI
	cpi r17, OSINT2
	breq OS_REMOVE_INT2

	cpi r17, OSOC0
	breq OS_REMOVE_OC0
	cpi r17, OSSPMR
	breq OS_REMOVE_SPMR
	
	jmp OSRemoveIRQFromTaskRet
//------------------------------
// IRQ Byte 0
OS_REMOVE_INT0: cbr r18, OSINT0
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_INT1: cbr r18, OSINT1
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_OC2: cbr r18, 4
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_OVF2: cbr r18, 8
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_ICP1: cbr r18, 16
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_OC1A: cbr r18, 32
jmp OSRemoveIRQFromTaskRet
//------------------------------
// IRQ Byte 1
OS_REMOVE_OC1B: cbr r19, 1
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_OVF1: cbr r19, 2
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_OVF0: cbr r19, 4
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_SPI: cbr r19, 8
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_URXC: cbr r19, 16
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_UDRE: cbr r19, 32
jmp OSRemoveIRQFromTaskRet
//------------------------------
// IRQ Byte 2
OS_REMOVE_UTXC: cbr r20, 1
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_ADCC: cbr r20, 2
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_ERDY: cbr r20, 4
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_ACI: cbr r20, 8
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_TWI: cbr r20, 16
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_INT2: cbr r20, 32
jmp OSRemoveIRQFromTaskRet
//------------------------------
// IRQ Byte 3
OS_REMOVE_OC0:	cbr r21, 1
jmp OSRemoveIRQFromTaskRet

OS_REMOVE_SPMR: cbr r21, 2
//------------------------------
OSRemoveIRQFromTaskRet:
	call OSSaveTaskIRQTableF
	pop XH
	pop XL
	pop r21
	pop r20
	pop r19
	pop r18
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса
	R17 - Номера на прекъсването
	XH:XL - Адреса на процедурата обработваща прекъсването
*/
OSAddIRQToTaskF:
	push r18
	push r19
	push r20
	push r21

	push XL
	push XH
	call OSLoadTaskIRQTableF
	pop XH
	pop XL

	cpi r17, OSINT0
	breq OS_ADD_INT0
	cpi r17, OSINT1
	breq OS_ADD_INT1
	cpi r17, OSOC2
	breq OS_ADD_OC2
	cpi r17, OSOVF2
	breq OS_ADD_OVF2
	cpi r17, OSICP1
	breq OS_ADD_ICP1
	cpi r17, OSOC1A
	breq OS_ADD_OC1A

	cpi r17, OSOC1B
	breq OS_ADD_OC1B
	cpi r17, OSOVF1
	breq OS_ADD_OVF1
	cpi r17, OSOVF0
	breq OS_ADD_OVF0
	cpi r17, OSSPI
	breq OS_ADD_SPI
	cpi r17, OSURXC
	breq OS_ADD_URXC
	cpi r17, OSUDRE
	breq OS_ADD_UDRE

	cpi r17, OSUTXC
	breq OS_ADD_UTXC
	cpi r17, OSADCC
	breq OS_ADD_ADCC
	cpi r17, OSERDY
	breq OS_ADD_ERDY
	cpi r17, OSACI
	breq OS_ADD_ACI
	cpi r17, OSTWI
	breq OS_ADD_TWI
	cpi r17, OSINT2
	breq OS_ADD_INT2

	cpi r17, OSOC0
	breq OS_ADD_OC0
	cpi r17, OSSPMR
	breq OS_ADD_SPMR
	
	jmp OSAddIRQToTaskRet
//------------------------------
// IRQ Byte 0
OS_ADD_INT0: or r18, r17
jmp OSAddIRQToTaskRet

OS_ADD_INT1: or r18, r17
jmp OSAddIRQToTaskRet

OS_ADD_OC2: or r18, r17
jmp OSAddIRQToTaskRet

OS_ADD_OVF2: or r18, r17
jmp OSAddIRQToTaskRet

OS_ADD_ICP1: or r18, r17
jmp OSAddIRQToTaskRet

OS_ADD_OC1A: or r18, r17
jmp OSAddIRQToTaskRet
//------------------------------
// IRQ Byte 1
OS_ADD_OC1B: or r19, r17
jmp OSAddIRQToTaskRet

OS_ADD_OVF1: or r19, r17
jmp OSAddIRQToTaskRet

OS_ADD_OVF0: or r19, r17
jmp OSAddIRQToTaskRet

OS_ADD_SPI: or r19, r17
jmp OSAddIRQToTaskRet

OS_ADD_URXC: or r19, r17
jmp OSAddIRQToTaskRet

OS_ADD_UDRE: or r19, r17
jmp OSAddIRQToTaskRet
//------------------------------
// IRQ Byte 2
OS_ADD_UTXC: or r20, r17
jmp OSAddIRQToTaskRet

OS_ADD_ADCC: or r20, r17
jmp OSAddIRQToTaskRet

OS_ADD_ERDY: or r20, r17
jmp OSAddIRQToTaskRet

OS_ADD_ACI: or r20, r17
jmp OSAddIRQToTaskRet

OS_ADD_TWI: or r20, r17
jmp OSAddIRQToTaskRet

OS_ADD_INT2: or r20, r17
jmp OSAddIRQToTaskRet
//------------------------------
// IRQ Byte 3
OS_ADD_OC0:	or r21, r17
jmp OSAddIRQToTaskRet

OS_ADD_SPMR: or r21, r17
//------------------------------
OSAddIRQToTaskRet:
	call OSSaveTaskIRQTableF
	pop r21
	pop r20
	pop r19
	pop r18
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса
*/
OSInitTaskIRQF:
	push r16
	call OSLoadTaskIRQTableF
//------------------------------
// Тука се нулират всички прекъсвания
// на процесора 
// (без прекъсването на системния таймер де ;))
// (недовършена част)
	clr r16
	out GICR, r16
//------------------------------
	tst r18
	breq OS_INIT_BYTE_1
	
	sbrs r18, 0
	jmp OS_INIT_INT1
OS_INIT_INT0:
	in r16, GICR
	sbr r16, (1<<INT0)
	out GICR, r16
	
	sbrs r18, 1
	jmp OS_INIT_OC2
OS_INIT_INT1:
	in r16, GICR
	sbr r16, (1<<INT1)
	out GICR, r16

	sbrs r18, 2
	jmp OS_INIT_OVF2
OS_INIT_OC2:

	sbrs r18, 3
	jmp OS_INIT_OVF2
OS_INIT_OVF2:

	sbrs r18, 4
	jmp OS_INIT_OC1A
OS_INIT_ICP1:

	sbrs r18, 5
	jmp OS_INIT_BYTE_1
OS_INIT_OC1A:
//------------------------------
OS_INIT_BYTE_1:
	tst r19
	breq OS_INIT_BYTE_2

	sbrs r19, 0
	jmp OS_INIT_OVF1
OS_INIT_OC1B:

	sbrs r19, 1
	jmp OS_INIT_OVF0
OS_INIT_OVF1:

	sbrs r19, 2
	jmp OS_INIT_SPI
OS_INIT_OVF0:

	sbrs r19, 3
	jmp OS_INIT_URXC
OS_INIT_SPI:

	sbrs r19, 4
	jmp OS_INIT_UDRE
OS_INIT_URXC:

	sbrs r20, 5
	jmp OS_INIT_BYTE_2
OS_INIT_UDRE:

//------------------------------
OS_INIT_BYTE_2:
	tst r20
	breq OS_INIT_BYTE_3

	sbrs r20, 0
	jmp OS_INIT_ADCC
OS_INIT_UTXC:

	sbrs r20, 1
	jmp OS_INIT_ERDY
OS_INIT_ADCC:

	sbrs r20, 2
	jmp OS_INIT_ACI
OS_INIT_ERDY:

	sbrs r20, 3
	jmp OS_INIT_TWI
OS_INIT_ACI:

	sbrs r20, 4
	jmp OS_INIT_INT2
OS_INIT_TWI:

	sbrs r20, 5
	jmp OS_INIT_BYTE_3
OS_INIT_INT2:
	in r16, GICR
	sbr r16, (1<<INT2)
	out GICR, r16
//------------------------------
OS_INIT_BYTE_3:
	tst r21
	breq OSInitTaskIRQFRet

	sbrs r21, 0
	jmp OS_INIT_SPMR
OS_INIT_OC0:

	sbrs r21, 1
	jmp OSInitTaskIRQFRet
OS_INIT_SPMR:

OSInitTaskIRQFRet:
	pop r16
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номер на процеса
	R21:R20:R19:R18 - Таблицата на прекъсванията
	XH:XL - Адреса на процедурата обработваща прекъсването
*/
OSSaveTaskIRQTableF:
	push ZL
	push ZH

	OSGetDisplacementOfTask r16
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_IRQ
	
	st Z+, r21
	st Z+, r20
	st Z+, r19
	st Z+, r18
	st Z+, XH
	st Z+, XL
	
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
OSINT0F: cli
	OSRunTaskIRQC OSINT0
reti

OSINT1F: cli
	OSRunTaskIRQC OSINT1
reti

OSOC2F: cli
	OSRunTaskIRQC OSOC2
reti

OSOVF2F: cli
	OSRunTaskIRQC OSOVF2
reti

OSICP1F: cli
	OSRunTaskIRQC OSICP1
reti

OSOC1AF: cli
	OSRunTaskIRQC OSOC1A
reti

OSOC1BF: cli
	OSRunTaskIRQC OSOC1B
reti

OSOVF1F: cli
	OSRunTaskIRQC OSOVF1
reti

OSOVF0F: cli
	OSRunTaskIRQC OSOVF0
reti

OSSPIF: cli
	OSRunTaskIRQC OSSPI 
reti

OSADCCF: cli
	OSRunTaskIRQC OSADCC
reti

OSERDYF: cli
	OSRunTaskIRQC OSERDY
reti

OSACIF: cli
	OSRunTaskIRQC OSACI
reti

OSTWIF: cli
	OSRunTaskIRQC OSTWI
reti

OSINT2F: cli
	OSRunTaskIRQC OSINT2
reti

OSSPMRF: cli
	OSRunTaskIRQC OSSPMR
reti
//------------------------------------------------------------------------------------
/*
	R16 - Номера на прекъсването
*/
OSRunTaskIRQF:
	OSSaveH r17
	mov r17, r16
	OSLoadL r16
	push r16
	OSGetNumberOfCurrentTask r16
	OSSendMessage r16, r17
	OSLoadH r17

	OSSaveL XL
	OSSaveH XH
	push r18
	push r19
	push r20
	push r21
	call OSLoadTaskIRQTableF
	pop r21
	pop r20
	pop r19
	pop r18
	pop r16

	tst XH
	brne OSICallTaskIRQ
	tst XL
	brne OSICallTaskIRQ

	OSLoadL XL
	OSLoadH XH
	jmp OSRunTaskIRQFRet

OSICallTaskIRQ:
	push XL
	push XH
	OSLoadL XL
	OSLoadH XH
	reti
OSRunTaskIRQFRet:
reti
//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса
	return:
		XH:XL - Адреса на процедурата обработваща прекъсването
		R21:R20:R19:R18 - Таблицата на прекъсванията
*/
OSLoadTaskIRQTableF:
	push ZL
	push ZH

	OSGetDisplacementOfTask r16
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_IRQ
	
	ld r21, Z+
	ld r20, Z+
	ld r19, Z+
	ld r18, Z+
	ld XH, Z+
	ld XL, Z+
	
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------