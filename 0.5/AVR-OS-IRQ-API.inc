/*
	      The^day^of^DooM

	Create date:   30.04.2006
	Last Update:   01.05.2006
*/

.macro OSRunTaskIRQC
	push r17
	ldi r17, (1<<@0)
	call OSRunTaskIRQF
	pop r17
.endmacro

.macro OSAddIRQToTask
	cli
	push r16
	push r17
	push XL
	push XH

	ldi r16, @0      // ����� �� ������� ��� ����� �� ������� ����������
	ldi r17, (1<<@1) // ����� �� �������� ����������
	ldi XH, high(@2) // \ ������ �� ����������� �����������
	ldi XL,  low(@2) // / ������������

	call OSSaveTaskIRQTableF

	pop XH
	pop XL
	pop r17
	pop r16
	sei
.endmacro
