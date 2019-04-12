/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   29.04.2006
    Last Update:   19.05.2006

	XTAL == 4MHz
*/

#ifndef __DELAY_API__
	#define __DELAY_API__
#endif

.macro DelayMsR
	OSPush r16
	mov r16, @0
	OSCall DelayMsF
	OSPop r16
.endmacro

.macro DelayMsC
	OSPush r16
	ldi r16, @0
	OSCall DelayMsF
	OSPop r16
.endmacro

.macro Delay1ms
	OSCall Delay1msF
.endmacro
