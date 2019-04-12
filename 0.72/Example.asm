/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   02.02.2007
*/

.include "m16def.inc"
.include "scr/AVR-OS-API.inc"

	OSInit

	OSCreateTaskC task1, 222
	OSCreateTaskC task2, 11
	OSCreateTaskC task3, 22
	OSCreateTaskC task4, 111

	OSDisplayGotoXY 3,0
	OSDisplayPutStr AVR_OS_MESSAGE
	OSDisplayGotoXY 1,1

	;OSAddIRQToTaskC 1, OSSPI, TaskINT0
	;OSAddIRQToTaskC 1, OSINT0, TaskINT0
	;OSRemoveIRQFromTaskC 1, OSSPI
	;sbi PIND, PIND2

	OSRun

	.include "scr/AVR-OS.inc"

task1:
	ldi r16, 10
	call OSMallocF

	OSTaskSendMessageC 3, 65
	OSTaskReadMessage r16, 17
	tst r17
	breq task1

	OSRemoveTaskByNumberC 2
OSExit

task2:
	OSMallocC 7
	OSCreateTaskC task5, 234
	OSTaskSendMessageC 1, 1
task2_end:
jmp task2_end

task3:
	OSOpenDisplay
task3L:
	OSTaskReadMessage r17, r16
	call OSDisplayPutCharF
	delaymsc 66
	ldi r16, 66
	call OSDisplayPutCharF
	delaymsc 66
jmp task3L

.include "c:\atmel\myinc\atmega16\fdd_motor.inc"
task4:
	call fdd_motor_init
fdd_motor_loop:
	call fdd_motor_one_run_right
jmp fdd_motor_loop

task5:
task5_tx:
	USARTPutCharC 1
	DelayMsC 111
	USARTPutCharC 3
	DelayMsC 111
	USARTPutCharC 0
	DelayMsC 111
	USARTPutCharC 1
	DelayMsC 111
	USARTPutCharC 2
	DelayMsC 111
jmp task5_tx

TaskINT0:
ret
