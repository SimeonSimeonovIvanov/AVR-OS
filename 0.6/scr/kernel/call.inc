/*
	      The^day^of^DooM

	Create date:   15.15.2006
	Last Update:   15.05.2006
*/

OSCallF:
	OSSaveL r16
	OSSaveH r17

	pop r17 // PCH
	mov r16, r17
	call OSPushF
	pop r16 // PCL
	call OSPushF

	pop r16
	call OSPushF // R16
	pop r16
	call OSPushF // R17

	OSLoadL r16 // Load R16
	push r16    // PCL
	OSLoadH r16 // Load R17
	push r16    // PCH

	call OSPopF // R17
	mov r17, r16
	call OSPopF // R16
reti

OSRetF:
	OSSaveL r16
	call OSPopF
	push r16
	call OSPopF
	push r16
	OSLoadL r16
reti
