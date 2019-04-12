/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   11.05.2006
*/

//------------------------------------------------------------------------------------
/*
	Стандартни съобщения:
*/
.equ NO_MESSAGE=0
.equ FIRST_RUN_OF_OS=1
.equ THIS_IS_SYSTEM_TASK=FIRST_RUN_OF_OS

//------------------------------------------------------------------------------------
.macro OSReadSystemMessage
	lds r16, SYSTEM_MESSAGE
.endmacro
//------------------------------------------------------------------------------------
.macro OSSendSystemMessageR
	sts SYSTEM_MESSAGE, @0
.endmacro

.macro OSSendSystemMessageC
	push r16
	ldi r16, @0
	sts SYSTEM_MESSAGE, r16
	pop r16
.endmacro
//------------------------------------------------------------------------------------


.macro OSSendMessageToTaskC
	cli
	push r16
	push r17

	ldi r16, @0
	ldi r17, @1
	call OSSendMessageToTaskF

	pop r17
	pop r16
	sei
.endmacro
//------------------------------------------------------------------------------------
.macro OSReadTaskMessageCR
	cli
	push r16
	ldi r16, @0
	call OSReadTaskMessageF
	mov @1, r16
	pop r16
	sei
.endmacro
//------------------------------------------------------------------------------------
