/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   21.07.2006
*/

//------------------------------------------------------------------------------------
/*
	���������� ���������:
*/
.equ NO_MESSAGE=NULL
//------------------------------------------------------------------------------------
/*
	@0 - �� ���� � �������
	@1 - ������ �����������
*/
.macro OSTaskSendMessageC
	OSGetNumberOfCurrentTask r16
	ldi r17, @0
	ldi r18, @1
	OSSendMessage r16 r17 r18
.endmacro
// - - - - - - - - - - - - - - - - - - - -
/*
	@0 - �� ���� � �������
	@1 - �� ���� � �������
	@2 - ������ �����������
*/
.macro OSSendMessage
#if @0!=r16
	push r16
	mov r16, @0
#endif
#if @1!=r17
	push r17
	mov r17, @1
#endif
#if @1!=r18
	push r18
	mov r18, @2
#endif
	call OSSendMessageF
#if @1!=r18
	pop r18
#endif
#if @1!=r17
	pop r17
#endif
#if @0!=r16
	pop r16
#endif
.endmacro
//------------------------------------------------------------------------------------
/*
	return:
		@0 - O� ����
		@1 - ������ ���������
*/
.macro OSTaskReadMessage
#if @0!=R16
	push r16
#endif
#if @1!=R17
	push r17
#endif

	OSGetNumberOfCurrentTask r16
	call OSReadMessageF

#if @1!=R17
	mov @0, r17
	pop r17
#endif
#if @0!=R16
	mov @0, r16
	pop r16
#endif
.endmacro
// - - - - - - - - - - - - - - - - - - - -
/*
	@0 - �� ����
	return:
		@1 - O� ����
		@2 - ������ ���������
*/
.macro OSReadMessage
#if @0!=R16
	#if @1!=R16 && @2!=R16
		push r16
	#endif	
	mov  r16, @0
#endif

#if @1!=R16
	push r16
#endif
#if @2!=R17
	push r17
#endif
	call OSReadMessageF
#if @2!=R17
	mov @2, r17
	pop r17
#endif
#if @1!=R16
	mov @1, r16
	pop r16
#endif

#if @0!=R16 && @1!=R16 && @2!=R16
	pop r16
#endif
.endmacro
//------------------------------------------------------------------------------------
