/*
			AVR-Os Version 0.1
		
		The^day^of^DooM

	Create date:   01.02.2006
	Last Update:   11.02.2006

					Карта на памета:
   ----------------------------------------------------------------------------
	Резервираната част на OS (процес номер 0) [3 байта]:	
		$0061 - Брой на процесите
		$0062 - Номера на процеса изпълняван в момента
		$0063 - Резервиран
   ----------------------------------------------------------------------------
	Масив на процесите - съставен от отделни
	структури по [3 байта]:
		((Номер на процеса)*3)+0 - Младши байт от адреса на процеса
		((Номер на процеса)*3)+1 - Старша част от адреса на процеса
		((Номер на процеса)*3)+2 - Байт оказващ времето за изпоолзване от OS
   ----------------------------------------------------------------------------

   I) Инициализиране на стека
   II) Създаване на процеси
   III) Пускане на OS
   (добавяне и премахване* на процеси, тяхното изпълнение)

		*при премахване на даден процес то слдващите след него (ако има)
		 си смъкват номера с един надоло.
*/

.equ NUMBER_OF_TASK=$60+100
.equ CURIENT_TASK=$61+101

OSInitF:
	push r16
	ldi r16, 1
	sts NUMBER_OF_TASK, r16
	sts CURIENT_TASK, r16
	pop r16
ret

OSCreateTaskF:
	push r19
	push r20
	push ZL
	push ZH
// - - - - - - - - - - - - - - - - - - - - - - - - - -
	ldi r20, 3   // Четем текущия брой на
	lds r19, NUMBER_OF_TASK // работещи процеси
	mul r19, r20 // омножава ме го с 3 и
	ldi r20, $60 // резултата събираме с $60
	add r0, r20  // за да получим
	clr r20      // базовото
	adc r1, r20  // отемстване
	mov ZL, r0   // (адреса) на
	mov ZH, r1   // N-тия преоцес
// - - - - - - - - - - - - - - - - - - - - - - - - - -
	st Z+, r16 // Записваме младшата и старшата
	st Z+, r17 // част от адреса на процеса
	st Z+, r18 // записваме времетраенето на процеса
// - - - - - - - - - - - - - - - - - - - - - - - - - -
	inc r19 // Записваме новия брой на
	sts NUMBER_OF_TASK, r19 // процесите.	
// - - - - - - - - - - - - - - - - - - - - - - - - - -
	pop ZH
	pop ZL
	pop r20
	pop r19
ret

OSRunF:
	lds r16, CURIENT_TASK
	lds r17, NUMBER_OF_TASK
	cp r17, r16
	breq L1
	jmp L2
L1:
	ldi r16, 1
	sts CURIENT_TASK, r16
L2:
	call OSRunTaskF
	inc r16
	sts CURIENT_TASK, r16
jmp OSRunF

OSRunTaskF:
	push XL
	push XH
	push ZL
	push ZH
	push r16

	ldi XL, 3
	mul XL, r16

	ldi r16, $60
	add r0, r16
	clr r16
	adc r1, r16
	mov XL, r0
	mov XH, r1

	ld ZL, X+
	ld ZH, X+
	ld r16, X+
	
OSRunTaskF_loop:
	push r16
	push ZL
	push ZH
	icall
	pop ZH
	pop ZL
	pop r16
	dec r16
	brne OSRunTaskF_loop

	pop r16
	pop ZH
	pop ZL
	pop XH
	pop XL
ret

OSRemoveTaskF:
	push XL
	push XH
	push YL
	push YH
	push r16
	push r17
	push r18
	push r19

	lds r19, NUMBER_OF_TASK
	cpi r19, 1
	breq OSRemoveTaskF_ret
	cp r16, r19
	breq OSRemoveTaskF_ret

	ldi r18, 3
OSRemoveTaskF_Swap_Task:
	mul r16, r18
	mov XL, r0
	mov XH, r1

	ldi r18, $60
	add XL, r18
	clr r18
	adc XH, r18

	inc r16
	
	ldi r18, 3
	mul r16, r18
	mov YL, r0
	mov YH, r1
	
	ld r17, Y+
	st X+, r17

	cp r16, r19
	brne OSRemoveTaskF_Swap_Task

	dec r19
	sts NUMBER_OF_TASK, r19
OSRemoveTaskF_ret:
	pop r19
	pop r18
	pop r17
	pop r16
	pop YH
	pop YL
	pop XH
	pop Xl
ret
