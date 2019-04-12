/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   29.04.2006
    Last Update:   19.05.2006

     -------------------------------------------------------------------------
    |                LCD type: JM162A          LCD Port:  PORTx               |
    |-------------------------------------------------------------------------|
    | MCU | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3  | Bit 2  | Bit 1  | Bit 0  |
     -------------------------------------------------------------------------
    | LCD |  ---  | Pin 4 | Pin 5 | Pin 6 | Pin 14 | Pin 13 | Pin 12 | Pin 11 |
     -------------------------------------------------------------------------
    |/\/\/|  ---  |   RS  |  R/W  |   E   | Data 3 | Data 2 | Data 1 | Data 0 |
     -------------------------------------------------------------------------
*/

#define __JM162A_INC__

#ifndef __DELAY_INC__
	.include "scr/lib/delay/delay.inc"
#endif

.equ LCD_E=4
.equ LCD_RW=5
.equ LCD_RS=6

.equ LCD_DDRx=DDRA
.equ LCD_PINx=PINA
.equ LCD_PORTx=PORTA

DisplayDriverF:
	ldi R16, 0x7F
	out LCD_DDRx, R16
	DelayMsC 55
	LCDFunctionSet 0b00001100
	LCDDisplay_ON_OFF_Control 0b00000111
	LCDClear
	LCDEntryModeSet 0b00000010

	LCDPutStrC AVR_OS_MESSAGE, 0

DisplayDriver_loop:
/*
	clr r17
  	
	ldi ZL, low(2*AVR_OS_MESSAGE)
	ldi ZH, high(2*AVR_OS_MESSAGE)
 
  	movw XL, ZL
	adiw XH:XL, 16
  LCDDriverReadVRAM:
	lpm r16, Z+
	OSCall LCDPutCharF

	movw YL, ZL
	nop
	movw ZL, XL
	nop
	lpm r16, Z+
	
	ldi r23, $40
	add r17, r23
	OSCall LCDPutCharF
	sub r17, r23

	movw ZL, YL

	inc r17
	cpi r17, 16
  brlo LCDDriverReadVRAM
*/
	LCDPutStrC AVR_OS_MESSAGE, 0
  	//DelayMsC 33
	//LCDClear
jmp DisplayDriver_loop
/*
	Z - Адреса на низа
	R16 - Адреса на LCD-то
	R17 - Разположение на низа (0=FLASH; 1=SRAM)
*/
LCDPutStrF:
LCDPutStrNextChar:
	lpm r16, Z+
	tst r16
	breq LCDPutStrRet
	OSCall LCDPutCharF
	inc r17
jmp LCDPutStrNextChar
LCDPutStrRet:
OSRet

/*
	R16 - Char
	R17 - Addres
*/
LCDPutCharF:
	OSPush r16
	mov R16, R17
	OSCall LCDSetDDRAMAddressF
	OSPop r16
	OSCall LCD_Write_Data_To_RAM
OSRet

LCDClearDisplayF:
	clr R17
	ldi R16, 0b00000001
	OSCall LCDSendByteF
	DelayMsC 2
OSRet

LCDReturnToHomeF:
	clr R17
	ldi R16, 0b00000010
	OSCall LCDSendByteF
OSRet

/*
	R16:
	 Bit(0): Entire shift ON(1)/OFF(0)
	 Bit(1): Decrement(0)/Increment(1) mode
*/
LCDEntryModeSetF:
	clr R17
	sbr R16, 4
	andi R16, 0b00000111
	OSCall LCDSendByteF
OSRet

/*
	R16:
	 Bit(0): Blink ON(1)/OFF(0)
	 Bit(1): Cursor ON(1)/OFF(0)
	 Bit(2): Display ON(1)/OFF(0)
*/
LCDDisplay_ON_OFF_ControlF:
	clr R17
	sbr R16, 8
	andi R16, 0b00001111
	OSCall LCDSendByteF
OSRet

/*
	R16:
	 Bit(2):
	 Bit(3):
*/
LCDCursorOrDisplayShiftF:
	clr r17
	sbr r16, 16
	andi r16, 0b00011100
	OSCall LCDSendByteF
OSRet

/*
	R16:
	 Bit(2): 5x8(0) or 5x11(1) Dots
	 Bit(3): 1(0) or 2(1) - line mode
	 Bit(4): 4(0) or 8(1) bit interface
*/
LCDFunctionSetF:
	clr r17
	sbr r16, 32
	andi r16, 0b00111100
	OSCall LCDSendByteF
OSRet

//	R16 - Addres
LCDSetDDRAMAddressF:
	mov r2, r16
	mov r3, r17
	clr r17
	sbr r16, 128
	OSCall LCDSendByteF
	mov r17, r3
	mov r16, r2
OSRet

//	R16 - Data
LCD_Write_Data_To_RAM:
	mov r2, r16
	mov r3, r17
	ldi r17, 64
	OSCall LCDSendByteF
	mov r17, r3
	mov r16, r2
OSRet

LCDSendByteF:
	cli
	swap r16
	push r16

	andi R16, 0x0F
	or r16, r17
	out LCD_PORTx, r16
	nop
	sbi LCD_PORTx, LCD_E
	nop
	nop
	cbi LCD_PORTx, LCD_E
	
	pop r16
	swap r16

	andi R16, 0x0F
	or r16, r17
	out LCD_PORTx, r16
	nop
	sbi LCD_PORTx, LCD_E
	nop
	nop
	cbi LCD_PORTx, LCD_E

	clr r16
	out LCD_PORTx, R16
OSRet
