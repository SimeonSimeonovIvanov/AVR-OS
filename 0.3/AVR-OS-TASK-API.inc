/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   30.04.2006
*/

/*
	Адреси на системните променливи:
*/
.equ TASK_COUNTER=$61
.equ CURRENT_TASK=$62

//------------------------------------------------------------------------------------
.macro OSRun
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
	pop XH // Изважда позицията на
	pop XL // текущия процес от стека
	/*                 !!!ВНИМАНИЕ!!!     
	   ще работи коректно само ако е извикан при получаване
	   на прекъсване от системния таймер !!! 
	*/

	push r16
	lds r16, CURRENT_TASK
	call OSSaveTaskF
	pop r16
.endmacro
//------------------------------------------------------------------------------------
.macro OSLoadCurrentTask
	push r16
	lds r16, CURRENT_TASK
	call OSLoadTaskF
	pop r16
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
