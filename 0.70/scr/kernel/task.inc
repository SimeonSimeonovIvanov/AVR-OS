/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   02.07.2006
*/

//------------------------------------------------------------------------------------
OSTaskManagerF:
	cli
	OSStopTimer

	OSSaveToTempL r16 // Запазваме R16 и SREG
	in r16, SREG      // за по-късно изпозване
	OSSaveToTempH r16 // в OSSaveCurrentTask

// - - - - - OSSaveCurrentTask - - - - - -
	push ZH
	push ZL
	OSGetNumberOfCurrentTask r16
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_SREG
	
	OSLoadH r16 // Load and
	st Z+, r16  // save SREG
	st Z+, r0
	st Z+, r1
	st Z+, r2
	st Z+, r3
	st Z+, r4
	st Z+, r5
	st Z+, r6
	st Z+, r7
	st Z+, r8
	st Z+, r9
	st Z+, r10
	st Z+, r11
	st Z+, r12
	st Z+, r13
	st Z+, r14
	st Z+, r15
	OSLoadL r16
	st Z+, r16
	st Z+, r17
	st Z+, r18
	st Z+, r19
	st Z+, r20
	st Z+, r21
	st Z+, r22
	st Z+, r23
	st Z+, r24
	st Z+, r25
	st Z+, XL
	st Z+, XH
	st Z+, YL
	st Z+, YH
	movw XL, ZL
	pop r16
	st X+, r16
	pop r16
	st X+, r16

	OSGetNumberOfCurrentTask r16
	call OSGetDisplacementOfTaskF

	pop r16    // Load and
	st Z+, r16 // save PCH
	pop r16    // Load and
	st Z+, r16 // save PCL
	
	in r16, SPH
	st Z+, r16
	in r16, SPL
	st Z+, r16
	
// - - - - - - - - - - - - - - - - - - - - -
  OSTaskManagerPointOfFirstRun:
  OSTaskManagerPointOfExitFromTask:

  OSTaskManagerCrash:
	lds r18, TASK_COUNTER
	tst r18
  breq OSTaskManagerCrash

	lds r16, CURRENT_TASK
	inc r16
	cp r18, r16
	brlo OSTaskManagerGoToTaskOne
  
  	jmp OSTaskManagerRet

  OSTaskManagerGoToTaskOne:
	ldi r16, 1
 
  OSTaskManagerRet:
	OSSaveNumberOfCurrentTask r16
	OSSendSystemMessageC NO_MESSAGE

// - - - - - - - - - - - - - - - - - - - - -
	cli
	call OSInitTaskIRQF
	call OSGetDisplacementOfTaskF
// - - - - - - - - - - - - - - - - -
	ld r17, Z+ // Load PCH
	ld r16, Z+ // Load PCL

	ld r19, Z+   // Насочваме SP към
	out SPH, r19 // стека на текущия
	ld r18, Z+   // процес
	out SPL, r18
	
	push r16 // Зареждаме стека с адреса на
	push r17 // който е бил прекъснат процеса
// - - - - - - - - - - - - - - - - -
	ld r17, Z+ // Load TCNTx
	ld r16, Z+ // H:L
	out OCR0, r16 // Настроиваме таймера
	
	adiw ZH:ZL, 8 // !!! При условие, че DISPLACEMENT_OF_TASK_SREG=14 !!!
	ld r16, Z+
	out SREG, r16
	ld r0, Z+
	ld r1, Z+
	ld r2, Z+
	ld r3, Z+
	ld r4, Z+
	ld r5, Z+
	ld r6, Z+
	ld r7, Z+
	ld r8, Z+
	ld r9, Z+
	ld r10, Z+
	ld r11, Z+
	ld r12, Z+
	ld r13, Z+
	ld r14, Z+
	ld r15, Z+
	ld r16, Z+
	ld r17, Z+
	ld r18, Z+
	ld r19, Z+
	ld r20, Z+
	ld r21, Z+
	ld r22, Z+
	ld r23, Z+
	ld r24, Z+
	ld r25, Z+
	ld XL, Z+
	OSSaveL XL	
	ld XH, Z+
	OSSaveH XH	
	ld YL, Z+
	ld YH, Z+
	ld XL, Z+
	ld XH, Z+
	movw ZL, XL
	OSLoadL XL
	OSLoadH XH
// - - - - - - - - - - - - - - - - - - - - -
	OSRunTimer
  reti // Изпразваме стека като прескачаме на текущия процес
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
// --------------------------------------
	st Z+, XH // PCH
	st Z+, XL // PCL
// --------------------------------------
	movw XL, ZL
	ldi r16, SIZE_OF_TASK_STRUCTURE-3
	add XL, r16
	clr r16
	adc XH, r16
	st Z+, XH // SPH
	st Z+, XL // SPL
// --------------------------------------
	st Z+, YH // Брой на машини цикли за
	st Z+, YL // изпълнение на процеса
// --------------------------------------
	ldi r17, SIZE_OF_TASK_STRUCTURE-6
  OSCreateTask_loop:
	st Z+, r16
	dec r17
	tst r17
  brne OSCreateTask_loop

	pop ZH
	pop ZL
	pop r17
	pop r16
reti
//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса който
	      желаем да премахнем
*/
OSRemoveTaskF:
	cli
	push r16
	push r17
	push r18
	push r19
	push r20
 	push YL
	push YH
	push ZL
	push ZH

	clr r20
	jmp OSExitFromTask01
OSExitFromTaskF:
	cli
	OSStopTimer
	ldi r20, 1
	ldi r16, high(OSSPH)
	out sph, r16
	ldi r16, low(OSSPL)
	out spl, r16
	OSGetNumberOfCurrentTask r16
OSExitFromTask01:
	lds r17, TASK_COUNTER
	
	tst r17               // Ако броя на процеси е нула
	breq OSRemoveTaskRet  // излизаме директно от функцията

	cp r16, r17           // А може и да сме искали да махнем последния процес:
	breq OSRemoveLastTask // тогава просто има намаляме броя с едно

	cp r17, r16           // Може би пък сме задали не съществуващ процес?
	brlo OSRemoveTaskRet  //  (по-голям номер от текущия им брой)
	                      // Ами тогава веднага напускаме процедурата :)

#ifdef DISPLAY
	OSSaveL r17
	lds r17, OSDisplayStatus
	cp r16, r17
	brge L00
	dec r17
	sts OSDisplayStatus, r17
L00:
	OSLoadL r17
#endif
// - - - - - - - - - - - - - - - - - 
 OSRemoveTaskLoop:
	call OSGetDisplacementOfTaskF
	movw YL, ZL // Y -> task_1
	inc r16
	call OSGetDisplacementOfTaskF // Z -> task_2
// - - - - - - - - - - - - - - - - - 
//	 Настоящата част от кода се
//	грижи да изчисли и запише новата
//	стойност на SP на task_2.
//	(SP-=SIZE_OF_TASK_STRUCTURE)
	adiw ZH:ZL, 2
	ld r19, Z+
	ld r18, Z+

	subi r18, SIZE_OF_TASK_STRUCTURE	 
	sbci r19, 0

	subi ZH:ZL, 2
	st Z+, r19
	st Z+, r18
	subi ZH:ZL, 4
// - - - - - - - - - - - - - - - - - 
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
	tst r20
	breq OSExitFromTask02
	jmp OSTaskManagerPointOfExitFromTask
OSExitFromTask02:
	pop ZH
	pop ZL
	pop YH
	pop YL
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
ret
//------------------------------------------------------------------------------------
/*
	R16   - Номера на процеса
	XH:ZL - Новия приоритет на процеса
*/
OSChangeTaskPriorityF:
	cli
	push ZL
	push ZH
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, 4
	st Z+, XH
	st Z+, XL
	pop ZH
	pop ZL
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

	movw ZL, r0
	subi ZH:ZL, $3	
	pop r16
	pop r1
	pop r0
ret
//------------------------------------------------------------------------------------
