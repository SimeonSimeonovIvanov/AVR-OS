/*
	      The^day^of^DooM

	Create date:   19.05.2006
	Last Update:   22.09.2006
*/

#if DISPLAY_TYPE == JM162A
	.include "scr/drivers/display/jm162a/jm162a.inc"
#endif

#if DISPLAY_TYPE == JM162B
#endif

//--------------------------------------------------------------------------
OSDisplayInitF:
	clr r16
	sts OSDisplayStatus, r16
	sts OSDisplayX, r16
	sts OSDisplayY, r16
#if DISPLAY_TYPE == JM162A
	call JM162AInitF
#endif
ret
//--------------------------------------------------------------------------
/*
	ZH:ZL - ������ �� ���� � FLASH-a
*/
OSDisplayPutStrF:
	push r16
	push ZL
	push ZH

  OSPutStr_loop:
  	lpm r16, Z+
	tst r16	
  	breq OSPutStrRet
	call OSDisplayPutCharF
  jmp OSPutStr_loop

OSPutStrRet:
	pop ZH
	pop ZL
	pop r16
ret
//--------------------------------------------------------------------------
/*
	R16 - ����� ����� �� ���������
*/
OSDisplayPutCharF:
	push r16
	push r17
	push r16

/*
	gotoxy(x,y);
	putchar(ch);

	if(++x>=DISPLEY_X_SIZE) {
		x=0;
		y++;
	}

	if(y>=DISPLEY_Y_SIZE) {
		x=0;
		y=0;
		clrscr();
	}
*/
#if DISPLAY_TYPE == JM162A
	lds r16, OSDisplayX
	lds r17, OSDisplayY
	call JM162AGoToXYF ; gotoxy(r16,r17)
#endif
	pop r16
#if DISPLAY_TYPE == JM162A
	call JM162AWriteDataToRAM ; putchar(r16)
	lds r16, OSDisplayX
#endif

#if DISPLAY_TYPE != UNKNOWN_DISPLEY_TYPE
	inc r16
	cpi r16, DISPLEY_X_SIZE
	brlo L01
	inc r17
	clr r16
L01:
	cpi r17, DISPLEY_Y_SIZE
	brlo L02
	clr r16
	clr r17
	OSDisplayClear ; clrscr()
L02:
	sts OSDisplayX, r16
	sts OSDisplayY, r17
#endif
// -----------------------------------------
	pop r17
	pop r16
ret
//--------------------------------------------------------------------------
OSCloseDisplayF:
	push r16
	clr r16
	sts OSDisplayStatus, r16
	pop r16
ret
//--------------------------------------------------------------------------
OSOpenDisplayF:
	push r16
	push r17

	OSGetNumberOfCurrentTask r16
/*
	OSDisplayStatus ������ �� ��������:
		0  - ��������, ������� � ��������
		>0 - ������ �� ������� ����� �� � ����������

	� ������, �� � ���� OSDisplayStatus=OSGetNumberOfCurrentTask
	��� � �� ����� �� ���� �� ������� ���������� �� ��������� ����
	��� �������, �� � �������� �� ������ � ����� ����� �� OSDisplayStatus
*/
  OSOpenDisplayWaitL:
  	cli
  	lds r17, OSDisplayStatus
	/* ���� �� � ���� �� ������� ������? */
	cp r16, r17
	breq OSDisplayIsFreeL
	
	/* ���� �� ��� � ��������? */
	tst r17
	breq OSDisplayIsFreeL
	sei
  jmp OSOpenDisplayWaitL

OSDisplayIsFreeL:
	sts OSDisplayStatus, r16 ; ��������� ��� ������ � �������� �������
	sei

	pop r17
	pop r16
ret
// --------------------------------------------------------------------------
