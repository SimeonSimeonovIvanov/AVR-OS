/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   16.05.2006
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
	lds @0, SYSTEM_MESSAGE
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
.macro OSSendMessageC
	cli
	push r16
	push r17

	ldi r16, @0 // Номер на процеса към който ще изпращаме съобщението
	ldi r17, @1 // Самото съобщение
	call OSSendMessageToTaskF

	pop r17
	pop r16
	sei
.endmacro
//------------------------------------------------------------------------------------
.macro OSReadMessage
	cli
#if @0!=R16
	push r16
#endif
	call OSReadMessageF
#if @0!=R16
	OSSaveToTempL r16
	pop r16
	OSLoadFromTempL @0
#endif
	sei
.endmacro
//------------------------------------------------------------------------------------
