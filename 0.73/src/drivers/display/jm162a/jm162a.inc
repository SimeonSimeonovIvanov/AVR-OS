/*
	 The^day^of^DooM

    Avr Studio 4

    Create date:   29.04.2006
    Last Update:   07.07.2006
*/

#ifndef __DELAY_INC__
	.include "C:\ATMEL\MyINC\ATmega16\delay\delay.inc"
#endif

#ifndef __JM162A_INC__
#define __JM162A_INC__

#if XTAL == 4
.equ JM162A_DELAY_TIME=27
#else
.equ JM162A_DELAY_TIME=43
#endif

.equ LCD_E=4
.equ LCD_RW=5
.equ LCD_RS=6

.equ LCD_DDRx=DDRA
.equ LCD_PINx=PINA
.equ LCD_PORTx=PORTA

.equ LCD_D0=PA0
.equ LCD_D1=PA1
.equ LCD_D2=PA2
.equ LCD_D3=PA3

.equ LCD_PORTx_FREE_PIN=PC7

JM162AInitF:
	in r16, LCD_DDRx
	ori r16, (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_E)|(1<<LCD_D3)|(1<<LCD_D2)|(1<<LCD_D1)|(1<<LCD_D0)
	out LCD_DDRx, r16
	ldi r16, 30
	call DelayMsF
	JM162AFunctionSet 0b00001100
	JM162ADisplay_ON_OFF_Control 0b00000111
	JM162AClear
	JM162AEntryModeSet 0b00000010
ret

JM162AClearDisplayF: ;+
	push r16
	ldi r16, 0b00000001
	call JM162ASendByteF
	ldi r16, 2
	call DelayMsF
	pop r16
	;call JM162WaitBusyF
ret

JM162AReturnToHomeF: ;+
	push r16
	ldi R16, 0b00000010
	call JM162ASendByteF
	ldi r16, 2
	call DelayMsF
	pop r16
ret

/*
	R16 - X
	R17 - Y
*/
JM162AGoToXYF:
	tst r17
	breq JM162A_Line0
JM162A_Line1:
	push r17
	ldi r17, 0x40
	add r16, r17
	pop r17
JM162A_Line0:
	call JM162ASetDDRAMAddressF
ret

/*
	R16:
	 Bit(0): Entire shift ON(1)/OFF(0)
	 Bit(1): Decrement(0)/Increment(1) mode
*/
JM162AEntryModeSetF: ;+
	push r16
	sbr r16, 4
	andi r16, 0b00000111
	call JM162ASendByteF
	pop r16
ret

/*
	R16:
	 Bit(0): Blink ON(1)/OFF(0)
	 Bit(1): Cursor ON(1)/OFF(0)
	 Bit(2): Display ON(1)/OFF(0)
*/
JM162ADisplay_ON_OFF_ControlF: ;+
	push r16
	sbr R16, 8
	andi R16, 0b00001111
	call JM162ASendByteF
	pop r16
ret

/*
	R16:
	 Bit(2):
	 Bit(3):
*/
JM162ACursorOrDisplayShiftF:
	sbr r16, 16
	andi r16, 0b00011100
	call JM162ASendByteF
ret

/*
	R16:
	 Bit(2): 5x8(0) or 5x11(1) Dots
	 Bit(3): 1(0) or 2(1) - line mode
	 Bit(4): 4(0) or 8(1) bit interface
*/
JM162AFunctionSetF:
	sbr r16, 32
	andi r16, 0b00111100
	call JM162ASendByteF
ret

//	R16 - Addres
JM162ASetDDRAMAddressF:
	push r16
	sbr r16, 128
	call JM162ASendByteF
	pop r16
ret

//	R16 - Data
JM162AWriteDataToRAM:
	sbi LCD_PORTx, LCD_RS
	call JM162ASendByteF
ret

JM162ASendByteF: ;+
	push r16
	push r17

	swap r16
	call JM162AWriteToPortF
	swap r16
	call JM162AWriteToPortF

	ldi r16, JM162A_DELAY_TIME
	call DelayUsF

	in r16, LCD_PORTx
	andi r16, (1<<LCD_PORTx_FREE_PIN)
	out LCD_PORTx, R16
	
	pop r17
	pop r16
ret


JM162AWriteToPortF:
	push r16

	andi r16, 0x0F
	in r17, LCD_PORTx
	andi r17, (1<<LCD_PORTx_FREE_PIN)|(1<<LCD_RS)|(1<<LCD_RW)

	or r16, r17
	out LCD_PORTx, r16

	sbi LCD_PORTx, LCD_E
	nop
	nop
	cbi LCD_PORTx, LCD_E

	pop r16
ret

/*
JM162WaitBusyF:
	push r16
JM162AWaitBusyLoop:
	call JM162AReadBusyFlagAndAddressCounterF
	sbrc r16, 7
jmp JM162AWaitBusyLoop
	pop r16
ret

JM162AReadBusyFlagAndAddressCounterF:
	push r17

	in r16, LCD_DDRx
	andi r16, 0b10110000
	out LCD_DDRx, r16

	
	call JM162AReadPortF
	mov r17, r16
	swap r17
	call JM162AReadPortF
	or r16, r17

	in r17, LCD_DDRx
	ori r17, 0x7f
	out LCD_DDRx, r17
	pop r17	
ret

JM162AReadPortF:
	sbi LCD_PORTx, LCD_E
	in r16, LCD_PINx
	andi r16, 0x0f
	nop
	cbi LCD_PORTx, LCD_E
ret
*/
#endif
