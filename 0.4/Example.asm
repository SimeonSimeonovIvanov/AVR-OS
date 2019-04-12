/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   11.05.2006
*/

.include "m16def.inc"
.include "AVR-OS-API.inc"

	OSInit

	OSCreateTask task1, 11
	OSCreateTask task2, 77
	OSCreateTask task3, 9
	OSCreateTask task4, 11

	OSRemoveTaskBuNumber 4

	OSAddIRQToTask 1, INT0, IRQ0ofTask1
	OSAddIRQToTask 3, INT0, IRQ0ofTask1

	sbi PIND, PIND2

	OSRun

.include "AVR-OS.inc"

task1:
	nop
	OSSendMessageToTaskC 2, 111
	nop
	nop
	nop
jmp task1

task2:
	nop
	OSReadTaskMessageCR 2, r3
	nop
	OSCreateTask task4, 44
	nop
	nop
OSExitFromTask

task3:
	nop
	nop
	DelayMsC 33
	nop
jmp task3

task4:
	nop
	nop
	OSReadTaskMessageCR 2, r1
	nop
	nop
jmp task4

IRQ0ofTask1:
	nop
	nop
	nop
ret
