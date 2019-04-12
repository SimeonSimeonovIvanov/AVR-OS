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

.macro OSSendSystemMessageC
	push r16
	ldi r16, @0
	sts SYSTEM_MESSAGE, r16
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSSendMessage
#if @0!=r16
	push r16
	mov r16, @0 // Номер на процеса към който ще изпращаме съобщението
#endif
#if @1!=r17
	push r17
	mov r17, @1 // Самото съобщение
#endif
	call OSSendMessageToTaskF
#if @1!=r17
	pop r17
#endif
#if @0!=r16
	pop r16
#endif
.endmacro

.macro OSSendMessageC
	ldi r16, @0 // Номер на процеса към който ще изпращаме съобщението
	ldi r17, @1 // Самото съобщение
	call OSSendMessageToTaskF
.endmacro
//------------------------------------------------------------------------------------
/*
	@0 - Съобщението
	@1 - Номера на процеса които го е изпратил
*/
.macro OSReadMessage
	cli
#if @0!=R16
	push r16
#endif
#if @1!=R17
	push r17
#endif
	call OSReadMessageF
#if @1!=R17
	mov @1, r17
	pop r17
#endif
#if @0!=R16
	OSSaveToTempL r16
	pop r16
	OSLoadFromTempL @0
#endif
	sei
.endmacro
//------------------------------------------------------------------------------------
