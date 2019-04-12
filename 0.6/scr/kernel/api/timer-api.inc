/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   11.05.2006
*/

.equ OS_TIMER_CSXX=CS01

.macro OSRunTimer
	call OSRunTimerF
.endmacro

.macro OSStopTimer
	push r16
	clr r16
	out TCCR0, r16
	pop r16
.endmacro
