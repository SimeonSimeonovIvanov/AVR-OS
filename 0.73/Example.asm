/*
		The^day^of^DooM

	Create date:	16.04.2006 \ Last UpDate For 0.72
	Last Update:	02.02.2007 /
	Full Change:	15.01.2008 - And Last Create 0.73
	Last Update:	16.01.2008
	Last Update:	02.02.2009
*/

.include "m16def.inc"
.include "src/AVR-OS-API.inc"

	OSInit

	OSCreateTaskC task1, 22
	OSCreateTaskC task2, 11
	OSCreateTaskC task3, 16
	OSCreateTaskC task4, 22

	OSCreateTaskC task6, 157

	
	OSDisplayGotoXY 3,0
	OSDisplayPutStr AVR_OS_MESSAGE
	OSDisplayGotoXY 1,1

	;OSAddIRQToTaskC 1, OSSPI, TaskINT0
	;OSRemoveIRQFromTaskC 1, OSSPI

	OSRun

	.include "src/AVR-OS.inc"

task1:
	ldi r16, 1
	call OSReadMessageF

	cpi r16, 2
	brne task1 ;Is From (task2)
	cpi r17, 3 ;Is Message (3)
	brne task1

	OSRemoveTaskByNumberC 2
OSExit

task2:
	clr r25
	ldi r24, 11
	call malloc
	;OSCreateTaskC task5, 234
task2_end:
	ldi r16, 2 ;From
	ldi r17, 1 ;To
	ldi r18, 3 ;Message
	call OSSendMessageF
jmp task2_end

task3:
	call OSGetCurrentTaskIDF
	ldi r17, OSINT0
	ldi XH, high(TaskINT0)
	ldi XL, low(TaskINT0)
	call OSAddIRQToTaskF
	sbi PIND, PIND2 // Disable INT0 (for debug).
	sbi DDRB, 1
task3L:
	sbi PORTB, 1
	OSOpenDisplay
	OSDisplayGoToXY 1, 0
	ldi r16, 'A'
	call OSDisplayPutCharF
	delaymsc 66
	OSDisplayGoToXY 1, 0
	ldi r16, ' '
	call OSDisplayPutCharF
	cbi PORTB, 1
	delaymsc 66
	OSCloseDisplay
jmp task3L

.include "c:\atmel\myinc\atmega16\fdd_motor.inc"
task4:
	call fdd_motor_init
fdd_motor_loop:
	call fdd_motor_one_run_right
jmp fdd_motor_loop

task5:
task5_tx:
/*	USARTPutCharC 1
	DelayMsC 111
	USARTPutCharC 3
	DelayMsC 111
	USARTPutCharC 0
	DelayMsC 111
	USARTPutCharC 1
	DelayMsC 111
	USARTPutCharC 2
	DelayMsC 111*/
jmp task5_tx

TaskINT0:
ret

task6:
	lds r24, 9
	clr r25
	call malloc
	tst ZL
	cpi ZH, 0
	brne task6L0
	OSExit

  task6L0:
  	
  task6LoopL0:
  	cli
	lds r16, OS_RTC_SS
	lds r17, OS_RTC_MM
	lds r18, OS_RTC_HH
	sei

	lds r19, ':'
	st Z+, r18
	st Z+, r19
	st Z+, r17
	st Z+, r19
	st Z+, r16
	clr r19
	st Z+, r19
	
	subi ZH:ZL, 6

	OSOpenDisplay
	OSDisplayGoToXY 3, 1
	call OSDisplayPutStrF
	OSCloseDisplay
jmp task6LoopL0
