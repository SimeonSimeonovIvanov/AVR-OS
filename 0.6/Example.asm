/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   19.05.2006
*/

.include "m16def.inc"
.include "scr/AVR-OS-API.inc"

	OSInit

	OSCreateTaskC task1, 111
	OSCreateTaskC task2, 77
	OSCreateTaskC task3, 9
	OSCreateTaskC task4, 11

	OSRemoveTaskBuNumberC 4

	//OSAddIRQToTaskC 2, INT0, IRQ0ofTask1
	//OSAddIRQToTaskC 3, INT0, IRQ0ofTask1

	//sbi PIND, PIND2

	OSRun

.include "scr/AVR-OS.inc"

task1:
	nop
	OSCall call_and_ret
	nop
	OSSendMessageC 2, 111
	nop
	nop
	nop
jmp task1

task2:
	nop
	OSReadMessage r16
	nop
	OSCreateTaskC task4, 44
	nop
	nop
OSExitFromTask

task3:
	nop
	nop
	OSPushC 3
	nop
	OSPop r18
	nop
jmp task3

task4:
	nop
	nop
	OSReadMessage r1
	nop
	nop
jmp task4

call_and_ret:
	nop
	nop
	nop
OSRet

IRQ0ofTask1:
	nop
	nop
	nop
ret
