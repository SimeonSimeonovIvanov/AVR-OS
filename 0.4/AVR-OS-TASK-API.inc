/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   11.05.2006
*/

//------------------------------------------------------------------------------------
.macro OSRun
	OSSendSystemMessageC NO_MESSAGE
	jmp OSTaskManagerPointOfFirstRun
.endmacro
//------------------------------------------------------------------------------------
.macro OSExitFromTask
	jmp OSExitFromTaskF
.endmacro
//------------------------------------------------------------------------------------
.macro OSRemoveTaskBuNumber
	cli
	push r16
	ldi r16, @0
	call OSRemoveTaskF
	pop r16
	sei
.endmacro
//------------------------------------------------------------------------------------
/*
	@0 - Името на процеса (label)
	@1 - Брой на машини цикли с който разполага процеса
*/
.macro OSCreateTask
	cli
	push XL
	push XH
	push YL
	push YH

	ldi XH, high(@0)
	ldi XL, low(@0)

	ldi YH, high(@1)
	ldi YL, low(@1)
	call OSCreateTaskF

	pop YH
	pop YL
	pop XH
	pop XL
	sei
.endmacro
//------------------------------------------------------------------------------------
.macro OSSaveCurrentTask
	pop ZH
	pop ZL

	push XH
	push XL
	movw XL, ZL

	lds r16, CURRENT_TASK
	push r16
	call OSGetTaskDisplacementF
	
	OSLoadFromTempH r16 // Прочитаме запазения SREG на текущия процес
	st Z+, r16          // след което го запзваме в структурата на процеса

	st Z+, XH // Записваме текущата позиция в процеса
	st Z+, XL

	pop r16
	call OSGetTaskDisplacementOfTaskRegistersF

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
	OSLoadFromTempL r16 // и R16
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
	st Z+, r26

	pop XL
	st Z+, XL
	pop XH
	st Z+, XH
	
	st Z+, YL
	st Z+, YH 
.endmacro
//------------------------------------------------------------------------------------
.macro OSLoadCurrentTask
	lds r16, CURRENT_TASK
	call OSLoadTaskF
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetCurrentTaskNumber
	lds r16, CURRENT_TASK
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetDisplacementOfTaskTableR
	push r16
	mov r16, @0
	call OSGetTaskDisplacementF
	pop r16
.endmacro

.macro OSGetDisplacementOfTaskTableC
	push r16
	ldi r16, @0
	call OSGetTaskDisplacementF
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSGetNumberOfCurrentTask
	lds r16, CURRENT_TASK
.endmacro
//------------------------------------------------------------------------------------
