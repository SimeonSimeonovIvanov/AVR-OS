/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   15.06.2006
*/

//------------------------------------------------------------------------------------
/*
	R16 - Процеса на който ще го изпращаме
	R17 - Събщението което ще изпращаме
*/
OSSendMessageToTaskF:
	push r16
	push ZL
	push ZH
	
	OSGetDisplacementOfTask r16
	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	st Z+, r17
	OSGetNumberOfCurrentTask r16
	st Z+, r16

	pop ZH
	pop ZL
	pop r16
ret
//------------------------------------------------------------------------------------
/*
		Прочита съобщението на текущия процес
	return:
		R16 - Прочетеното съобщение
		R17 - Номера на процеса който го е изпратил
*/
OSReadMessageF:
	push ZL
	push ZH

	OSGetNumberOfCurrentTask r16
	OSGetDisplacementOfTask r16

	adiw ZH:ZL, DISPLACEMENT_OF_TASK_MESSAGE_BOX
	ld r16, Z+
	ld r17, Z+

	pop ZH
	pop ZL
ret
//------------------------------------------------------------------------------------
