/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   11.05.2007
*/

//------------------------------------------------------------------------------------
/*
	R16 - От кого е пратено
	R17 - До кого е пратено
	R18 - Самото съобщението
*/
OSSendMessageF:
	push ZL
	push ZH

	tst r16
	brne OSSendMessageToTaskL

	; Send message to kernel
	sts SYSTEM_MESSAGE_FROM, r16 ; From...
	sts SYSTEM_MESSAGE, r18   ; Message...
	jmp OSSendMessageRet

OSSendMessageToTaskL:
	call OSOpenTaskArrayF

	push r16
	mov r16, r17
	call OSGetDisplacementOfTaskF
	pop r16

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	st Z+, r18 ; Message...
	st Z+, r16 ; From...
	
	call OSCloseTaskArrayF
OSSendMessageRet:
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
/*
	R16 - На кого
	return:
		R16 - Oт кого
		R17 - Самото съобщение
*/
OSReadMessageF:
	push ZL
	push ZH

	tst r16
	brne OSReadTaskMessageL

	; Read kernel message
	lds r16, SYSTEM_MESSAGE_FROM ; From...
	lds r17, SYSTEM_MESSAGE   ; Message...
	jmp OSReadMessageRet

OSReadTaskMessageL:
	call OSOpenTaskArrayF

	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	ld r16, Z+ ; Message...
	ld r17, Z+ ; From...

	call OSCloseTaskArrayF
OSReadMessageRet:
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
