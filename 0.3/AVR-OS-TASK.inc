/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   30.04.2006
*/

//------------------------------------------------------------------------------------
OSTaskManagerF:
	cli                             // Забраняваме прекъсванията,
	push r16                        // съхраняваме R16 и спираме таймера
	clr r16                         // в противен случен най-вероятно
	out TCCR0, r16                  // ще зациклим в OSTaskManagerF...
// -------------------------------
	OSReadSystemMessage             // Тука се обработват съобщенията
	cpi r16, THIS_IS_SYSTEM_TASK    // получени от последния изпълняван процес.
	pop r16
	breq OSThisIsSystemTask
// -------------------------------
	OSSaveCurrentTask

  OSThisIsSystemTask:
  OSTaskManagerPointOfFirstRun:
  
  OSTaskManagerCheckTaskCounter:
  	push r16
	push r18
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
	sts CURRENT_TASK, r16
	OSLoadCurrentTask
	pop r18
	pop r16

	OSSendSystemMessageC NO_MESSAGE
	OSRunTimer
	sei
  ijmp
//------------------------------------------------------------------------------------
/*
	X - Адреса на процеса
	Y - Брой на машини цикли за изпълнение
*/
OSCreateTaskF:
	push r16
	push ZL
	push ZH

	lds r16, TASK_COUNTER
	inc r16
	sts TASK_COUNTER, r16
	call OSGetTaskDisplacementF

	in r16, SREG
	st Z+, r16
	st Z+, XH // Текуща позиция
	st Z+, XL // в процеса

	st Z+, YH // Брой на машини цикли за
	st Z+, YL // изпълнение на процеса

	ldi r16, NULL
	st Z+, r16
	st Z+, r16
	st Z+, r16

	OSSendSystemMessageC THIS_IS_SYSTEM_TASK

	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
OSExitFromTaskF:
	cli
	push r16
	lds r16, CURRENT_TASK
	call OSRemoveTaskF
	pop r16
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
	call OSGetTaskDisplacementF
	movw YL, ZL
	inc r16
	call OSGetTaskDisplacementF

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
	lds r16, CURRENT_TASK
	cp r16, r17
	brge OSRemoveTask_L1
	jmp OSRemoveTask_L2
 OSRemoveTask_L1:
    dec r16
 OSRemoveTask_L2:
	dec r17
	sts TASK_COUNTER, r17
	sts CURRENT_TASK, r16
 OSRemoveTaskRet:
 	OSSendSystemMessageC THIS_IS_SYSTEM_TASK
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
/*
	R16 - Номер на процеса
	X - Текуща позиция в процеса

		 С тази процедура запазваме текущата позиция в процеса и SREG-a му.
		Работи с номера на процеса и приема текущата позиция в X регистъра.
*/
OSSaveTaskF:
	push r16
	push ZL
	push ZH
	
	call OSGetTaskDisplacementF
	
	in r16, SREG
	st Z+, r16

	st Z+, XH // Текуща позиция
	st Z+, XL // в процеса
	
	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номер на процеса
	return:
		Z - Текуща позиция в процеса
		
		Функцията е с двойна употреба:
		 1) Възтановява SREG на желания процес, зарежда броя на машни
		   цикли отделени за него в системня таймер.
		 2) Връща в Z регистъра текущата позиция в процеса с цел
		   прескачане към нея чрез инструкцията IJMP
*/
OSLoadTaskF:
	push r17
	push XL
	push XH
	push YL
	push YH

	call OSInitTaskIRQF
	call OSGetTaskDisplacementF
		
	ld r17, Z+
	out SREG, r16

	ld XH, Z+ // Текуща позиция
	ld XL, Z+ // в процеса

	ld YH, Z+ // Резервирани машини
	ld YL, Z+ // цикли за процеса

	out OCR0, YL
	
	movw ZL, XL

	pop YH
	pop YL
	pop XH
	pop XL
	pop r17
ret

//------------------------------------------------------------------------------------
/*
	R16 - Номер на процеса
	retutn:
		Z - Отместването на процеса

		Тази функция връща адреса в SRAM на който е запазена
	текущата информация на процеса. Работи с номера на желания процес
*/
OSGetTaskDisplacementF:
	push r0
	push r1
	push r17

	ldi r17, SIZE_OF_TASK_STRUCTURE
	mul r16, r17

	ldi r17, $5F
	add r0, r17
	clr r17
	adc r1, r17

	movw ZL, r0

	pop r17
	pop r1
	pop r0
ret
