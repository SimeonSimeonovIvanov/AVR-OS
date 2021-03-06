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
		���� � ���� �������� ��������� � R16
	*/
	call SendBYTE // ��������� �� 8 ������ �������

	Delay1ms // ����� ���������� ��
	              // ��������� �� ��������
                      // �������� �������.
                      // ������ �� ������.
ret

// R16 - BYTE
SendBYTE:
	push r16
	push r17
	push r18

	cbi porta, pa7 // ����������� ������ ������
	Delay2ms       // ������� � INT0 ���������
	sbi porta, pa7 // (�������� �� �������)
	
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
