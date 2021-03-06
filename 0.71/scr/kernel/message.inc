/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   21.07.2006
*/

//------------------------------------------------------------------------------------
/*
	R16 - �� ���� � �������
	R17 - �� ���� � �������
	R18 - ������ �����������
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
	;OSGetDisplacementOfTask r17
	push r16
	mov r16, r17
	call OSGetDisplacementOfTaskF
	pop r16

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	st Z+, r18 ; Message...
	st Z+, r16 ; From...

OSSendMessageRet:
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
/*
	R16 - �� ����
	return:
		R16 - O� ����
		R17 - ������ ���������
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
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	ld r16, Z+ ; Message...
	ld r17, Z+ ; From...

OSReadMessageRet:
	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
