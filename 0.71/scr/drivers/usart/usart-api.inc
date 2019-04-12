/*
	      The^day^of^DooM

	Create date:   11.06.2006
	Last Update:   11.06.2006
*/

.equ USART_IS_OPEN=0
.equ USART_IS_CLOSE=1
.equ BAUD_RATE=0x0025 // 9600; Max: 250 kbps
// ----------------------------------------------------------------------------
.macro USARTInit
	call USARTInitF
.endmacro
// ----------------------------------------------------------------------------
.macro USARTPutChar
#if @0!=r16
	push r16
	mov r16, @0
#endif
	call USART_putchar
#if @0!=r16
	pop r16
#endif
.endmacro
// ----------------------------
.macro USARTPutCharC
	push r16
	ldi r16, @0
	call USART_putchar
	pop r16
.endmacro
// ----------------------------------------------------------------------------
.macro USARTGetChar
#if @0!=r16
	push r16
#endif
	call USART_getchar
#if @0!=r16
	mov @0, r16
	pop r16
#endif
.endmacro
// ----------------------------------------------------------------------------
.macro USARTOpen
	call USARTOpenF
.endmacro
// ----------------------------------------------------------------------------
.macro USARTClose
	call USARTCloseF
.endmacro
// ----------------------------------------------------------------------------
.macro USARTGetStatus
	lds @0, USART_STATUS
.endmacro
// ----------------------------------------------------------------------------
.macro USARTSetStatusC
	push r16
	ldi r16, @0
	sts USART_STATUS, r16
	pop r16
.endmacro
// ----------------------------------------------------------------------------
