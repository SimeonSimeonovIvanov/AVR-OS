/*
	      The^day^of^DooM

	Create date:   15.05.2006
	Last Update:   17.05.2006
*/

//------------------------------------------------------------------------------------
OSPushF:
	push ZL
	push ZH

	push r16
	call OSGetTaskStackF
	
	inc r16    // Увеличаваме и записваме
	st Z, r16 // новия брой елементи в стека
	
	add ZL, r16 // Изчисляваме отместването
	clr r16     // на
	adc ZH, r16 // последния елемент

	pop r16     // Възтановяваме и
	st Z, r16   // записваме елемента

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
OSPopF:
	push ZL
	push ZH
	
	call OSGetTaskStackF
	push r16
	dec r16
	st Z, r16
	pop r16

	add ZL, r16 // Изчисляваме отместването
	clr r16     // на
	adc ZH, r16 // последния елемент

	ld r16, Z   // прочитаме посления елемент

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
/*
	return:
		R16 - Текущия брой елементи в стека
		  Z - Отместването на стека
*/
OSGetTaskStackF:
	OSGetNumberOfCurrentTask r16
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_STACK
	nop
	ld r16, Z // Зареждаме текущия брой на елементи в стека
ret
//------------------------------------------------------------------------------------
