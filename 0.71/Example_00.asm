/*
	      The^day^of^DooM

	Create date:   05.12.2006
	Last Update:   05.12.2006
*/

.include "m16def.inc"
.include "scr/AVR-OS-API.inc"

	OSInit
	
	OSCreateTaskC task1, 255

	OSRun

.include "scr/AVR-OS.inc"

task1:
	OSOpenDisplay

	ldi ZH, high(test_str*2)
	ldi ZL, low(test_str*2)
	call strlen

	clr r17	
	add ZL, r16
	adc ZH, r17
	mov r17, r16

	inc r17
  task1_l0:
	dec r17
    breq task1_exit

	subi ZH:ZL, 1
	lpm r16, Z
  	call OSDisplayPutCharF
  jmp task1_l0

task1_exit:
	OSCloseDisplay
OSExit

/*
	Z - input str
	r16 - string lenght
*/
strlen:
	push ZL
	push ZH
	push r17
	clr r16

  strlen_l0:
	lpm r17, Z+
	tst r17
	breq strlen_ret
	inc r16
  jmp strlen_l0

  strlen_ret:
	pop r17
	pop ZH
	pop ZL
ret

test_str: .DB "AVR-OS 0.71",0
