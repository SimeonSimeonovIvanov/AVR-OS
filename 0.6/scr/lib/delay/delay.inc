/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   27.01.2005
    Last Update:   19.05.2006

	XTAL == 4MHz
*/

#define __DELAY_INC__

DelayMsF:
	OSPush r16
Delay_ms_loop:
	Delay1ms
	dec R16
brne Delay_ms_loop
	OSPop r16
OSRet

Delay1msF:
	OSPush r16
	ldi r16, 4
	out TCCR2, r16
	clr r16
	out TCNT2, r16
Delay1msF_loop1:
	in r16, TCNT2
	cpi r16, 51
brlo Delay1msF_loop1
	OSPop r16
OSRet
