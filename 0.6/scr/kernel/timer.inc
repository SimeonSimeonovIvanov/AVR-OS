/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   11.05.2006
*/

OSRunTimerF:
	push r16
	clr r16
	out TCCR0, r16
	out TCNT0, r16
	ldi r16, (1<<OCIE0)
	out TIMSK, r16
	ldi r16, (1<<OS_TIMER_CSXX)
	out TCCR0, r16
	pop r16
ret
