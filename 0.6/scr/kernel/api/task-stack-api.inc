/*
	      The^day^of^DooM

	Create date:   15.05.2006
	Last Update:   19.05.2006
*/

.macro OSPush
	cli
#if @0!=r16
	push r16
	mov r16, @0
#endif
	call OSPushF
#if @0!=r16
	pop r16
#endif
	sei
.endmacro

.macro OSPushC
	cli
#if @0!=r16
	push r16
	ldi r16, @0
#endif
	call OSPushF
#if @0!=r16
	pop r16
#endif
	sei
.endmacro

.macro OSPop
	cli
#if @0!=r16
	push r16
#endif
	call OSPopF
#if @0!=r16
	mov @0, r16
	pop r16
#endif
	sei
.endmacro
