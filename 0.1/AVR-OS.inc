/*
			AVR-Os Version 0.1
		
		The^day^of^DooM

	Create date:   01.02.2006
	Last Update:   11.02.2006

					����� �� ������:
   ----------------------------------------------------------------------------
	������������� ���� �� OS (������ ����� 0) [3 �����]:	
		$0061 - ���� �� ���������
		$0062 - ������ �� ������� ���������� � �������
		$0063 - ����������
   ----------------------------------------------------------------------------
	����� �� ��������� - �������� �� �������
	��������� �� [3 �����]:
		((����� �� �������)*3)+0 - ������ ���� �� ������ �� �������
		((����� �� �������)*3)+1 - ������ ���� �� ������ �� �������
		((����� �� �������)*3)+2 - ���� ������� ������� �� ����������� �� OS
   ----------------------------------------------------------------------------

   I) �������������� �� �����
   II) ��������� �� �������
   III) ������� �� OS
   (�������� � ����������* �� �������, ������� ����������)

		*��� ���������� �� ����� ������ �� ��������� ���� ���� (��� ���)
		 �� ������� ������ � ���� ������.
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
	ldi r20, 3   // ����� ������� ���� ��
	lds r19, NUMBER_OF_TASK // �������� �������
	mul r19, r20 // �������� �� �� � 3 �
	ldi r20, $60 // ��������� �������� � $60
	add r0, r20  // �� �� �������
	clr r20      // ��������
	adc r1, r20  // ����������
	mov ZL, r0   // (������) ��
	mov ZH, r1   // N-��� �������
// - - - - - - - - - - - - - - - - - - - - - - - - - -
	st Z+, r16 // ��������� �������� � ��������
	st Z+, r17 // ���� �� ������ �� �������
	st Z+, r18 // ��������� ������������� �� �������
// - - - - - - - - - - - - - - - - - - - - - - - - - -
	inc r19 // ��������� ����� ���� ��
	sts NUMBER_OF_TASK, r19 // ���������.	
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
