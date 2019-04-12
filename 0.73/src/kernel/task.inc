/*
		The^day^of^DooM

	Create date:	30.04.2006 \ Last UpDate For 0.72
	Last Update:	01.06.2007 /
	Full Change:	15.01.2008 - And Last Create 0.73
	Last Update:	02.02.2009
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

	lds ZH, OSCurrentTaskPtr
	lds ZL, OSCurrentTaskPtr-1
	adiw ZH:ZL, 1
	in r16, sph
	st Z+, r16
	in r16, spl
	st Z+, r16
	subi ZH:ZL, 3

  OSTaskManagerPointOfExitFromTask:
	clr r17
	lds XH, OSTaskListRoot
	lds XL, OSTaskListRoot-1
  OSTaskManagerCrash:
	cp XL, r17
	cpc XH, r17
  breq OSTaskManagerCrash

  OSTaskManagerLoadNextTaskL:
	adiw ZH:ZL, TASK_ID+1
	ld YL, Z+
	ld YH, Z
	movw ZH:ZL, YH:YL
	
	cp ZL, r17
	cpc ZH, r17	
	breq OSTaskManagerGoToTaskOneL
  	jmp OSTaskManagerLoadTaskL

  OSTaskManagerGoToTaskOneL:
  	movw ZH:ZL, XH:XL

  OSTaskManagerLoadTaskL:
  	sts OSCurrentTaskPtr, ZH
	sts OSCurrentTaskPtr-1, ZL

	ld r17, Z+
	cpi r17, TASK_IS_RUN
	breq OSTaskManagerLoadTask
	subi ZH:ZL, 1
	cpi r17, TASK_IS_STOP
	breq OSTaskManagerLoadNextTaskL
  jmp OSTaskManagerCrash

  OSTaskManagerLoadTask:
	ld r16, Z+
	out sph, r16
	ld r16, Z+
	out spl, r16
// - - - - - - - - - - - - - - - - -
	ld r16, Z+ // Load TCNTx
	ld r17, Z+ // H:L
	
	out OCR2, r16
	clr r16
	out TCNT2, r16

	adiw ZH:ZL, TASK_ID-5
	ld r16, Z
	sts CURRENT_TASK, r16	
	call OSInitTaskIRQF

	ldi r16, 1<<OCIE2 | 1<<OCIE1A
	out TIMSK, r16

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
  reti // Прескачаме към текущия процес.
//------------------------------------------------------------------------------------
/*
	X - Адреса на процеса
	Y - Брой на машини цикли за изпълнение
*/
OSCreateTaskF:
	call OSOpenTaskArrayF
	
	push r2
	push r3
	push r16
	push r17
	push r18
	push r24
	push r25
	push XL
	push XH
	push YL
	push YH
	push ZL
	push ZH
	
	clr r16
	// if(!(temp = (LPTASK)malloc(sizeof(TASK))) {
	ldi r24, low(100)
	ldi r25, high(100)
  	call malloc

	cp ZL, r16
	cp ZH, r16
	brne initTaskStructL
	jmp OSCreateTaskRetL
	//}

	////////////////////////////////////////////////////////////////////////////////////
  initTaskStructL:
	st Z+, r16 ;TASK_IS_RUN
	////////////////////////////////////////////////////////////////////////
	//		Насочваме стековия указател в областа на стека като приемаме, че
	// са сме "вкарали" всичките регистри (r0-r31), SREG и началния адрес
	// на процеса (подготвайки го за първото му стартиране).
	movw r25:r24, ZH:ZL
	adiw r25:r24, SIZE_OF_TASK_STRUCTURE - 37

	st Z+, r25 // SPH
	st Z+, r24 // SPL
	st Z+, YL // Брой на машини цикли за
	st Z+, YH // изпълнение на процеса
	subi ZH:ZL, 5
	
	//////////////////////////////////////////////////////////////////////////////
	//   В самия край на паметта заделена за процеса (в частност областа на стека)
	// записваме адреса на процеса, която ще се извлече при първоначалнто му
	// стартиране (при изпълнение на reti в TaskManagerF).
	ldi r17, SIZE_OF_TASK_STRUCTURE
	add ZL, r17
	adc ZH, r16
	st -Z, XL
	st -Z, XH

	st -Z, r16			 // Нулираме и "push"-ваме R0.
	ldi r17, (1<<sreg_i) // "Push"-ваме SREG и разрешаваме прекъсванията
	st -Z, r17			 // за процеса при първото му стартиране.
	////////////////////////////////////////////////////
	// Нулираме регистрите и таблицата на прекъсванията.
	ldi r17, SIZE_OF_TASK_STRUCTURE-9
  OSCreateTask_loop:
	st -Z, r16
	dec r17
	tst r17
  brne OSCreateTask_loop
	subi ZH:ZL, 5		// Възстановяваме указателя сочещ паметта заделена за процеса.
	////////////////////////////////////////////////////////////////////////////////////

	ldi r18, 1
	lds XH, OSTaskListRoot
	lds XL, OSTaskListRoot-1
	// if(!*root) {
	cp XL, r16
	cpc XH, r16
	brne findFreeTaskIDL
	
	sts OSTaskListRoot, ZH
	sts OSTaskListRoot-1, ZL
	adiw ZH:ZL, TASK_ID
	st Z, r18
	jmp OSCreateTaskRetL
	// }

/*
	last = *root;
	while(last) {
		if(id == last->id) { id++; last = *root; }
		else last = last->next;
	}
*/
	clr r16
  findFreeTaskIDL:
  	cp XL, r16
	cpc XH, r16
	breq findFreeTaskIDEndL

	adiw XH:XL, TASK_ID
	ld r17, X+
	
	cp r18, r17
	breq getNextIDL
	ld r2, X+
	ld r3, X
	movw XH:XL, r3:r2
	jmp findFreeTaskIDL

   getNextIDL:
  	inc r18
	lds XH, OSTaskListRoot
	lds XL, OSTaskListRoot-1
  jmp findFreeTaskIDL
  findFreeTaskIDEndL:

/*
	last = *root;
	while(last->next) last = last->next;
	last->next = temp;
*/
	clr r2
	lds XH, OSTaskListRoot
	lds XL, OSTaskListRoot-1
  findLastTaskL:
	adiw XH:XL, TASK_ID+1
	ld r16, X+
	ld r17, X
	cp r16, r2
	cpc r17, r2
  breq findLastTaskEndL
	movw XH:XL, r17:r16
  jmp findLastTaskL

  findLastTaskEndL:
	subi XH:XL, 1
	st X+, ZL
	st X+, ZH
	adiw ZH:ZL, TASK_ID
	st Z, r18

  OSCreateTaskRetL:
  	pop ZH
	pop ZL
	pop YH
	pop YL
	pop XH
	pop XL
	pop r25
	pop r24
	pop r18
	pop r17
	pop r16
	pop r3
	pop r2
	call OSCloseTaskArrayF
ret
//------------------------------------------------------------------------------------
//	R16 - Task ID
OSRemoveTaskF:
	call OSOpenTaskArrayF
	
	// temp = *root;
	lds XH, OSTaskListRoot
	lds XL, OSTaskListRoot-1
	// if(NULL == *root) return;
	clr r17
	cp XL, r17
	cpc XH, r17
	breq OSRemoveTaskRet

	// current = NULL;
	clr YL
	clr YH

	clr r19
	findTaskForRemoveL:
		adiw XH:XL, TASK_ID
		ld r17, X+

		cp r16, r17
		breq else
		movw ZH:ZL, XH:XL
		jmp endIf
    	else:
		movw YH:YL, XH:XL
		endIf:

		// X = X->next;
		ld r2, X+
		ld r3, X
		movw XH:XL, r3:r2
		
		cp XL, r19
		cpc XH, r19
	brne findTaskForRemoveL

		cp YL, r19
		cpc YH, r19
	breq findTaskForRemoveL

	subi YH:YL, TASK_ID+1
	subi ZH:ZL, TASK_ID+1
	movw r25:r24, YH:YL

	// if(NULL == current) return;
	cp YL, r19
	cpc YH, r19
	breq OSRemoveTaskRet
	
	// r3:r2 = current->next;
	adiw YH:YL, TASK_ID+1
	ld r2, Y+
	ld r3, Y
	subi YH:YL, TASK_ID+2

	lds XH, OSTaskListRoot
	lds XL, OSTaskListRoot-1
	cp YL, XL
	cpc YH, XH
	breq llllll
	// previous->next = current->next;
	adiw ZH:ZL, TASK_ID+1
	st Z+, r2
	st Z, r3
	jmp lllllll
	llllll:
	//*root = current->next
	sts OSTaskListRoot, r3
	sts OSTaskListRoot-1, r2
	lllllll:
	
	call free
	lds r17, OSCurrentTaskPtr
	lds r16, OSCurrentTaskPtr-1
	cp r24, r16
	cpc r25, r17
	brne  OSRemoveTaskRet
	
	ldi r16, high(OS_STACK_BOTTOM)
	out sph, r16
	ldi r16, low(OS_STACK_BOTTOM)
	out spl, r16
	call OSCloseTaskArrayF
	jmp OSTaskManagerPointOfExitFromTask
	
  OSRemoveTaskRet:
  	call OSCloseTaskArrayF
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
	cli								//		Изчакваме докато приключи
	lds r16, OS_TASK_ARRAY_STATUS	// предишната операция работеща
	tst r16							// с масива на процесите.
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
	call OSGetCurrentTaskIDF
	call OSGetPtrToTaskF
	pop r16
ret

OSGetCurrentTaskIDF:
	push ZL
	push ZH
	clr r16
	lds ZH, OSCurrentTaskPtr
	lds ZL, OSCurrentTaskPtr-1
	cp ZL, r16
	cpc ZH, r16
	breq OSGetCurrentTaskIDRetL
	adiw ZH:ZL, TASK_ID
	ld r16, Z
  OSGetCurrentTaskIDRetL:
  	pop ZH
	pop ZL
ret

/*
	R16 - Номер на процеса
	retutn:
		Z - Отместването на процеса
*/
OSGetPtrToTaskF:
OSGetDisplacementOfTaskF:
	push r17
	push YL
	push YH

	lds ZH, OSTaskListRoot
	lds ZL, OSTaskListRoot-1

  OSGetPtrToTaskLoop:
	clr r17
	cp ZL, r17
	cpc ZH, r17
	breq OSGetPtrToTaskRetL
	
	adiw ZH:ZL, TASK_ID
	ld r17, Z
	subi ZH:ZL, TASK_ID

	cp r16, r17
    breq OSGetPtrToTaskRetL

	movw YH:YL, ZH:ZL
	adiw YH:YL, TASK_ID + 1
	ld ZL, Y+
	ld ZH, Y
  jmp OSGetPtrToTaskLoop

  OSGetPtrToTaskRetL:
  	pop YH
	pop YL
	pop r17
ret
//------------------------------------------------------------------------------------
