/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   02.07.2006
*/

.include "m16def.inc"
.include "scr/AVR-OS-API.inc"

	OSInit

	OSCreateTaskC task1, 255
	OSCreateTaskC task2, 233
	OSCreateTaskC task3, 255

	;OSRemoveTaskByNumberC 2
	;OSAddIRQToTaskC 1, OSSPI, TaskINT0
	;OSAddIRQToTaskC 1, OSINT0, TaskINT0
	;OSRemoveIRQFromTaskC 1, OSSPI
	;sbi PIND, PIND2

	OSRun

	.include "scr/AVR-OS.inc"

task1:
	nop
	OSDisplayGoToXY 0, 0
	OSDisplayPutStr AVR_OS_MESSAGE
	nop
jmp task1

task2:
	USARTPutCharC 1
	USARTPutCharC 3
	USARTPutCharC 0
OSExit

task3:
task3_tx:
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
	USARTPutCharC 0
	DelayMsC 111
	USARTPutCharC 0
	DelayMsC 111
	USARTPutCharC 2
	DelayMsC 111
	USARTPutCharC 1
	DelayMsC 111
	USARTPutCharC 2
	DelayMsC 111
	USARTPutCharC 1
	DelayMsC 111
	USARTPutCharC 0
	DelayMsC 111
jmp task3_tx

TaskINT0: cli
#if DISPLAY && DISPLAY_DEBUG
	OSDisplayClear
	OSDisplayPutStr INT0_Str
  INT0_loop:
	sbis PIND, PIND2
  jmp INT0_loop
#endif
ret

TaskINT1: cli
#if DISPLAY && DISPLAY_DEBUG
	ClearDisplay
	OSDisplayPutStr INT1_Str
  INT1_loop:
	sbis PIND, PIND2
  jmp INT1_loop
#endif
ret
