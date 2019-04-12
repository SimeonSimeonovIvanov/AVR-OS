/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   30.04.2006
*/

.equ SYSTEM_MESSAGE=$63
//------------------------------------------------------------------------------------
/*
	Стандартни съобщения:
*/
.equ NO_MESSAGE=0
.equ FIRST_RUN_OF_OS=1
.equ THIS_IS_SYSTEM_TASK=FIRST_RUN_OF_OS

//------------------------------------------------------------------------------------
.macro OSReadSystemMessage
	call OSReadSystemMessageF
.endmacro
//------------------------------------------------------------------------------------
.macro OSSendSystemMessageR
	mov r16, @0
	call OSSendSystemMessageF
.endmacro

.macro OSSendSystemMessageC
	ldi r16, @0
	call OSSendSystemMessageF
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

.macro OSReadTaskMessageCR
	cli
	push r16
	ldi r16, @0
	call OSReadTaskMessageF
	mov @1, r16
	pop r16
	sei
.endmacro
