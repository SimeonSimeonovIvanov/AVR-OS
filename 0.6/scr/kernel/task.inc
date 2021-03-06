/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   18.05.2006
*/

.include "scr/kernel/task-stack.inc"
//------------------------------------------------------------------------------------
OSTaskManagerF:
	cli
	OSStopTimer
	
	OSSaveToTempL r16 // ��������� R16 � SREG
	in r16, SREG      // �� ��-����� ���������
	OSSaveToTempH r16 // � OSSaveCurrentTask


	// ��������� ���������� �������� ��������� ��
	OSReadSystemMessage r16      // ������������ �������, �� �� �������� ����� � ���
	cpi r16, THIS_IS_SYSTEM_TASK // ������� ����� � ����������. � ������, �� � 
	breq OSThisIsSystemTask      // ��������, ����������� ����������� �� ������� ������
	// (����� �� �� ������� ������).

	OSSaveCurrentTask

  OSThisIsSystemTask:
  OSTaskManagerPointOfFirstRun:

  OSTaskManagerCrash:
	lds r18, TASK_COUNTER
	tst r18
  breq OSTaskManagerCrash

	lds r16, CURRENT_TASK
	inc r16
	cp r18, r16
	brlo OSTaskManagerGoToTaskOne
  
  	jmp OSTaskManagerIJmpToTask

  OSTaskManagerGoToTaskOne:
	ldi r16, 1
 
  OSTaskManagerIJmpToTask:
	OSSaveNumberOfCurrentTask r16
	OSSendSystemMessageC NO_MESSAGE
	OSLoadCurrentTask
	OSRunTimer
  reti
//------------------------------------------------------------------------------------
/*
	X - ������ �� �������
	Y - ���� �� ������ ����� �� ����������
*/
OSCreateTaskF:
	cli
	push r16
	push r17
	push ZL
	push ZH

	lds r16, TASK_COUNTER
	inc r16
	sts TASK_COUNTER, r16
	call OSGetDisplacementOfTaskF

	st Z+, XH // ������ �������
	st Z+, XL // � �������

	clr r16
	st Z+, r16

	st Z+, YH // ���� �� ������ ����� ��
	st Z+, YL // ���������� �� �������
	
	ldi r17, 55
  OSCreateTask_loop:
	st Z+, r16
	dec r17
	tst r17
  brne OSCreateTask_loop

	pop ZH
	pop ZL
	pop r17
	pop r16
ret
//------------------------------------------------------------------------------------
OSExitFromTaskF:
	cli
	OSGetNumberOfCurrentTask r16
	call OSRemoveTaskF
	OSSendSystemMessageC THIS_IS_SYSTEM_TASK
	sei
OSExitFromTaskLoop:
jmp OSExitFromTaskLoop
//------------------------------------------------------------------------------------
/*
	R16 - ������ �� ������� �����
	      ������ �� ���������
*/
OSRemoveTaskF:
	push r16
	push r17
	push r18
	push r19
 	push YL
	push YH
	push ZL
	push ZH

	lds r17, TASK_COUNTER
	
	tst r17               // ��� ���� �� ������� � ����
	breq OSRemoveTaskRet  // �������� �������� �� ���������

	cp r16, r17           // � ���� � �� ��� ������ �� ������ ��������� ������:
	breq OSRemoveLastTask // ������ ������ ��� �������� ���� � ����

	cp r17, r16           // ���� �� ��� ��� ������ �� ����������� ������?
	brlo OSRemoveTaskRet  //  (��-����� ����� �� ������� �� ����)
	                      // ��� ������ ������� ��������� ����������� :)
// - - - - - - - - - - - - - - - - - 
 OSRemoveTaskLoop:
	call OSGetDisplacementOfTaskF
	movw YL, ZL
	inc r16
	call OSGetDisplacementOfTaskF

	ldi r18, SIZE_OF_TASK_STRUCTURE
   OSRemoveTaskSwap:
    ld r19, Z+
	st Y+, r19
	dec r18
   brne OSRemoveTaskSwap

    cp r16, r17
 brne OSRemoveTaskLoop
// - - - - - - - - - - - - - - - - -
 OSRemoveLastTask:
	dec r17
	sts TASK_COUNTER, r17
	lds r16, CURRENT_TASK
	tst r16
	breq OSRemoveTaskRet

	dec r16
	cp r17, r16
	brlo L1

	jmp L2
L1:
	clr r16
L2:
	sts CURRENT_TASK, r16	
 OSRemoveTaskRet:
	pop ZH
	pop ZL
	pop YH
	pop YL
	pop r19
	pop r18
	pop r17
	pop r16
ret
//------------------------------------------------------------------------------------
OSGetDisplacementOfTaskRegistersF:
	call OSGetDisplacementOfTaskF
	
	push r16
	ldi r16, DISPLACEMENT_OF_TASK_REGISTERS
	
	add ZL, r16
	clr r16
	adc ZH, r16

	pop r16
ret
//------------------------------------------------------------------------------------
/*
	R16 - ����� �� �������
	retutn:
		Z - ������������ �� �������
*/
OSGetDisplacementOfTaskF:
	push r0
	push r1
	push r16

	ldi ZL, SIZE_OF_TASK_STRUCTURE
	mul r16, ZL

	ldi r16, $61
	add r0, r16
	clr r16
	adc r1, r16

	movw ZL, r0

	pop r16
	pop r1
	pop r0
ret
//------------------------------------------------------------------------------------
