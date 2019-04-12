/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   13.05.2006
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
	push ZL // Запазваме Z регистъра
	push ZH // за да го запишем по-късно

	OSGetNumberOfCurrentTask r16
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
	OSLoadFromTempL r16
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
	st Z+, XH
	st Z+, XL
	st Z+, YH
	st Z+, YL

	pop XH    //
	st Z+, XH //
	pop XL    //
	st Z+, XL //
// - - - - - - - - - - - - - - -
	OSGetNumberOfCurrentTask r16
	call OSGetTaskDisplacementF
	
	OSLoadFromTempH r16 // Прочитаме запазения SREG на текущия процес
	st Z+, r16          // след което го запзваме в структурата на процеса

	pop XH    // Изваждаме и записваме
	st Z+, XH // в процеса
	pop XL    // текущата позиция
	st Z+, XL // -=<>=-
.endmacro
//------------------------------------------------------------------------------------
.macro OSLoadCurrentTask
	OSGetNumberOfCurrentTask r16
	call OSInitTaskIRQF
	call OSGetTaskDisplacementF

	ld r16, Z+    // Възтановява SREG
	out SREG, r16 // на процеса

	ld XH, Z+ // Зареждаме текущата позиция
	ld XL, Z+ // на процеса в
	push XL   // стековия указател
	push XH   // -=<>=-

	ld YH, Z+ // Резервирани машини
	ld YL, Z+ // цикли за процеса
	out OCR0, YL
// - - - - - - - - - - - - - - - - - - - - - -
	OSGetNumberOfCurrentTask r16
	call OSGetTaskDisplacementOfTaskRegistersF
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
	ld XH, Z+
	ld XL, Z+
	ld YH, Z+
	ld YL, Z+

	OSSaveToTempH XH
	OSSaveToTempL XL
	movw XL, ZL
	nop
	ld ZH, X+
	ld ZL, X+
	OSLoadFromTempH XH
	OSLoadFromTempL XL
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
	lds @0, CURRENT_TASK
.endmacro
//------------------------------------------------------------------------------------
