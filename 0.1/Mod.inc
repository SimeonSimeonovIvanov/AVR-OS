/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   19.02.2006
    Last Update:   29.04.2006
*/

#define __MOD_INC__

#ifndef __DELAY_INC__
	.include "C:\MyINC\delay\delay.inc"
#endif

Mod:
	/*
		Тука е кода записващ командата в R16
	*/
	call SendBYTE // Изпращане на 8 битова команда

	Delay1ms // Време необходимо на
	              // приемника да обработи
                      // текущата команда.
                      // Избира се опитно.
ret

// R16 - BYTE
SendBYTE:
	push r16
	push r17
	push r18

	cbi porta, pa7 // Отрицателен тактов импулс
	Delay2ms       // вкарващ в INT0 приемника
	sbi porta, pa7 // (приемане на команда)
	
	ldi r17, 8
SendBYTE_loop:
	Delay1ms

	sbrc r16, 0
	sbi porta, pa7
	sbrs r16, 0
	cbi porta, pa7
	lsr r16

	Delay1ms
	sbi porta, pa7
	dec r17
brne SendBYTE_loop

	pop r18
	pop r17
	pop r16
ret
