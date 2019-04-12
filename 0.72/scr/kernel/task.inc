/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   01.06.2007
*/

//------------------------------------------------------------------------------------
OSTaskManagerF:
// - - - - - OSSaveCurrentTask - - - - - -
	push r0
	in r0, sreg
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
	
	OSGetDisplacementOfCurrentTask
	adiw ZH:ZL, 1
	in r16, sph
	st Z+, r16
	in r16, spl
	st Z+, r16
// - - - - - - - - - - - - - - - - - - - - -
  OSTaskManagerPointOfFirstRun:
  OSTaskManagerPointOfExitFromTask:

	lds r17, TASK_COUNTER
  OSTaskManagerCrash:
  	tst r17
  breq OSTaskManagerCrash

  OSTaskManagerLoadNextTaskL:
	OSGetNumberOfCurrentTask r16
	inc r16
	cp r17, r16
	brlo OSTaskManagerGoToTaskOneL
  
  	jmp OSTaskManagerLoadTaskL

  OSTaskManagerGoToTaskOneL:
	ldi r16, 1
 
  OSTaskManagerLoadTaskL:
	OSSaveNumberOfCurrentTask r16
// - - - - - - - - - - - - - - - - - - - - -
	call OSGetDisplacementOfTaskF
	ld r17, Z+
	
	cpi r17, TASK_IS_RUN
	breq OSTaskManagerLoadTask
	cpi r17, TASK_IS_STOP
	breq OSTaskManagerLoadNextTaskL

OSTaskManagerLoadTask:
	call OSInitTaskIRQF
// - - - - - - - - - - - - - - - - -
	ld r16, Z+
	out sph, r16
	ld r16, Z+
	out spl, r16
// - - - - - - - - - - - - - - - - -
	ld r16, Z+ // Load TCNTx
	ld r16, Z+ // H:L
	out OCR0, r16 // Настроиваме таймера
	
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	out sreg, r0
	pop r0
// - - - - - - - - - - - - - - - - - - - - -
  	OSRunTimer
  reti // Прескачаме към текущия процес.
//------------------------------------------------------------------------------------
/*
	X - Адреса на процеса
	Y - Брой на машини цикли за изпълнение
*/
OSCreateTaskF:
	call OSOpenTaskArrayF

	push r16
	push r17
	push r24
	push r25
	push ZL
	push ZH

// - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
	lds r16, TASK_COUNTER
	inc r16
	call OSGetDisplacementOfTaskF
	clr r17 ; TASK_IS_RUN
	st Z+, r17

//////////////////////////////////////////////////////////////////////////////
//		Насочваме стековия указател в областа на стека като приемаме, че
// са сме "вкарали" всичките регистри (r0-r31), SREG и началния адрес
// на процеса (подготвайки го за първото му стартиране).

	movw r25:r24, ZH:ZL
	adiw r25:r24, SIZE_OF_TASK_STRUCTURE - 37

	st Z+, r25 // SPH
	st Z+, r24 // SPL
// ---------------------------------------------------------------------------
	st Z+, YH // Брой на машини цикли за
	st Z+, YL // изпълнение на процеса
	subi ZH:ZL, 5
//////////////////////////////////////////////////////////////////////////////
//   В самия край на паметта заделена за процеса (в частност областа на стека)
// записваме адреса на процеса, която ще се извлече при първоначалнто му
// стартиране (при изпълнение на reti в TaskManagerF).

	clr r24
	ldi r17, SIZE_OF_TASK_STRUCTURE
	add ZL, r17
	adc ZH, r24

	st -Z, XL
	st -Z, XH
// ---------------------------------------------------------------------------
	st -Z, r24	// Нулираме и "push"-ваме R0.

	ldi r17, (1<<sreg_i) // 	"Push"-ваме SREG и разрешаваме прекъсванията
	st -Z, r17				// за процеса при първото му стартиране.
// ---------------------------------------------------------------------------

	// Нулираме регистрите и таблицата на прекъсванията.
	ldi r17, SIZE_OF_TASK_STRUCTURE-9
  OSCreateTask_loop:
	st -Z, r24
	dec r17
	tst r17
  brne OSCreateTask_loop
// ---------------------------------------------------------------------------
	sts TASK_COUNTER, r16

	pop ZH
	pop ZL
	pop r25
	pop r24
	pop r17
	pop r16

	call OSCloseTaskArrayF
ret
//------------------------------------------------------------------------------------
/*
	R16 - Номера на процеса който
	      желаем да премахнем
  -	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	По-важни регистри:
	 R20	- flag; 0 - От текущия процес премахваме друг
					  1 - Изход от текущия процес
	 
	 R6	- temp; Съхранява номера на процеса извикал OSRemoveTaskF (R20=1)

	 R19:R18 - Съхранява преизсчисления SP на task_2 (процеса преместващ се напред)
*/
OSRemoveTaskF: //  Премахване на процес различен от текущия	
	call OSOpenTaskArrayF
	cli
	clr r20

	OSGetNumberOfCurrentTask r17
	cp r16, r17
	brne OSRemoveTask00L

// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSExitFromTaskL: // Премахване на текущия процес
	call OSOpenTaskArrayF
	cli
	// Пренасочваме стека към този на операционата за да изжегнем нежелани
	ldi r16, high(OSSPH) // презаписи в таблицата на процесите, по-специално в
	out sph, r16			// стековата им област.
	ldi r16, low(OSSPL)
	out spl, r16

	ldi r20, 1
	OSGetNumberOfCurrentTask r16
// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  OSRemoveTask00L:
	lds r6, CURRENT_TASK
	lds r17, TASK_COUNTER
	
	tst r17               // Ако броя на процеси е нула
	breq OSRemoveTask01L  // излизаме директно от функцията

	cp r16, r17           // А може и да сме искали да махнем последния процес:
	breq OSRemoveLastTask // тогава просто има намаляме броя с едно

	cp r17, r16           // Може би пък сме задали не съществуващ процес?
	brlo OSRemoveTask01L  //  (по-голям номер от текущия им брой)
	                      // Ами тогава веднага напускаме процедурата :)
	
#ifdef DISPLAY
	lds r17, OSDisplayStatus
	cp r16, r17
	brge L00
	dec r17
	sts OSDisplayStatus, r17
L00:
	lds r17, TASK_COUNTER
#endif

// - - - - - - - - - - - - - - - - - 
 OSRemoveTaskLoop:
	call OSGetDisplacementOfTaskF
	movw YL, ZL   // Y -> task_1
	adiw ZH:ZL, 1
// - - - - - - - - - - - - - - - - -
//	Настоящата част от кода се
//	грижи да изчисли и запише новата
//	стойност на SP на task_2.
//	(SP-=SIZE_OF_TASK_STRUCTURE)
	inc r16
	call OSGetDisplacementOfTaskF // Z -> task_2
	adiw ZH:ZL, 1
	ld r19, Z+
	ld r18, Z+
	subi r18, SIZE_OF_TASK_STRUCTURE
	sbci r19, 0
	
	tst r20			//  Понеже OSRemoveTaskF e функция - използва стека най-малкото
	brne sssssssss	// със инструкциите за извикаването и излизане (call && reti) то
	cp r16, r6		// се появява проблем със стека на процеса който я е извикал. А имено
	brne sssssssss	// стека на процеса бива променен и при изпълнение на reti реално се
	out sph, r19	// отива на непредсказуем адрес. Заради това когато стигнем до таблицата
	out spl, r18	// на процеса извикал OSRemoveTaskF променяме хардуерния стеков указател.
	sssssssss:		// Операцията е аналогична на по-горното пренасочване на стека към този
						// на операционата.
	
	subi ZH:ZL, 2
	st Z+, r19
	st Z+, r18
	subi ZH:ZL, 3
// - - - - - - - - - - - - - - - - - 
	ldi r18, SIZE_OF_TASK_STRUCTURE
	// task_1 = task_2;
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
	
	cp r16, r17
	brlo L11
	
	dec r16
L11:
	sts CURRENT_TASK, r16
 OSRemoveTask01L:
	tst r20
	breq OSExitFromTask00L
	
	clr r20
	sts OS_TASK_ARRAY_STATUS, r20
	
	jmp OSTaskManagerPointOfExitFromTask

 OSExitFromTask00L:
 	clr r20
	sts OS_TASK_ARRAY_STATUS, r20
reti
//------------------------------------------------------------------------------------
/*
	R16   - Номера на процеса
	XH:ZL - Новия приоритет на процеса
*/
OSChangeTaskPriorityF:
	call OSOpenTaskArrayF
	push ZL
	push zl
	
	push ZH
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, 2
	st Z+, XH
	st Z+, XL
	
	pop ZH
	pop ZL
	call OSCloseTaskArrayF
reti

//------------------------------------------------------------------------------------
OSCloseTaskArrayF:
	push r16
	clr r16
	sts OS_TASK_ARRAY_STATUS, r16
	pop r16
ret

OSOpenTaskArrayF:
	push r16
  OSTaskArrayWaitL:					////////////////////////////////
	cli									//		Изчакваме докато приключи
	lds r16, OS_TASK_ARRAY_STATUS	// предишната операция работеща
	tst r16								// с масива на процесите.
	breq OSTaskArrayEndOfWaitL
	sei
  jmp OSTaskArrayWaitL
  
  OSTaskArrayEndOfWaitL:
	ldi r16, 1
	sts OS_TASK_ARRAY_STATUS, r16
	pop r16
reti
//------------------------------------------------------------------------------------
OSGetDisplacementOfCurrentTaskF:
	push r16
	lds r16, CURRENT_TASK
	call OSGetDisplacementOfTaskF
	pop r16
ret

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

	movw ZH:ZL, r0
	subi ZH:ZL, 3
	pop r16
	pop r1
	pop r0
ret
//------------------------------------------------------------------------------------
