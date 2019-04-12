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
	
	OSSaveToTempL r16 // Запазваме R16 и SREG
	in r16, SREG      // за по-късно изпозване
	OSSaveToTempH r16 // в OSSaveCurrentTask


	// Прочитаме последното записано съобщения на
	OSReadSystemMessage r16      // операционата система, за да разберем какъв е бил
	cpi r16, THIS_IS_SYSTEM_TASK // процеса който е прекъснала. В случей, че е 
	breq OSThisIsSystemTask      // системен, преопускаме запазването на текущия процес
	// (иначе ще се презаше грешно).

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
	X - Адреса на процеса
	Y - Брой на машини цикли за изпълнение
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

	st Z+, XH // Текуща позиция
	st Z+, XL // в процеса

	clr r16
	st Z+, r16

	st Z+, YH // Брой на машини цикли за
	st Z+, YL // изпълнение на процеса
	
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
	R16 - Номера на процеса който
	      желаем да премахнем
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
	
	tst r17               // Ако броя на процеси е нула
	breq OSRemoveTaskRet  // излизаме директно от функцията

	cp r16, r17           // А може и да сме искали да махнем последния процес:
	breq OSRemoveLastTask // тогава просто има намаляме броя с едно

	cp r17, r16           // Може би пък сме задали не съществуващ процес?
	brlo OSRemoveTaskRet  //  (по-голям номер от текущия им брой)
	                      // Ами тогава веднага напускаме процедурата :)
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
	R16 - Номер на процеса
	retutn:
		Z - Отместването на процеса
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
