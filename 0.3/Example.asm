/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   01.05.2006
*/

.include "m16def.inc"
.include "AVR-OS-API.inc"

	OSInit

	OSCreateTask task1, 88
	OSCreateTask task2, 77
	OSCreateTask task3, 31
	OSCreateTask task4, 44

	OSRemoveTaskBuNumber 4

	OSAddIRQToTask 1, INT0, IRQ0ofTask1

	OSRun

.include "AVR-OS.inc"

task1:
	nop
	OSSendMessageToTaskC 2, 111
	nop
	call task4
	nop
	nop
jmp task1

task2:
	nop
	OSReadTaskMessageCR 2, r3
	nop
	nop
	nop
OSExitFromTask

task3:
	nop
	nop
	nop
jmp task3

task4:
	nop
	nop
	nop
	nop
ret

IRQ0ofTask1:
	nop
	nop
	nop
ret
