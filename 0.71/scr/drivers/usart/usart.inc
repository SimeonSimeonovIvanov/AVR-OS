/*
		ATMEL
	
	Devicve: ATmega16

	 The^day^of^DooM

    Create date:   11.06.2006
    Last Update:   11.06.2006
*/

/*
.org URXCaddr		; Rx Complete
	rjmp RXdone
.org UDREaddr		; Data Register Empty
	rjmp TXempty
.org UTXCaddr		; Tx Complete
	rjmp TXdone
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
USART_Init:
USARTInitF:
	push r16
	; Set baud rate
	ldi r16, high(BAUD_RATE)
	out UBRRH, r16
	ldi r16, low(BAUD_RATE)
	out UBRRL, r16

	; Enable reciver and transmitter
	ldi r16, (1<<RXEN)|(1<<TXEN); |(1<<RXCIE)|(1<<TXCIE) ; enable RXDone and TXDone INT
	out UCSRB, r16

	; Set frame format: 8 data, 2 stop bit
	ldi r16, (1<<URSEL)|(1<<USBS)|(3<<UCSZ0)
	out UCSRC, r16
	
	USARTClose
	pop r16
ret
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
USART_Transmit:
USART_putchar:
	USARTOpen
	; Wait for empty transmit buffer
	sbis UCSRA, UDRE
	rjmp USART_Transmit
	; Put data (r16) into buffer, sends the data
	out UDR, r16
	USARTClose
ret
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
USART_Receive:
USART_getchar:
	USARTOpen
	; Wait for data to be received
	sbis UCSRA, RXC
	rjmp USART_Receive
	; Get and return received data from buffer
	in r16, UDR
	USARTClose
ret
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TXdone: reti
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TXempty: reti
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RXdone: in r16, UDR
reti
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
USARTOpenF:
	USARTSetStatusC USART_IS_OPEN
ret
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
USARTCloseF:
	USARTSetStatusC USART_IS_CLOSE
ret
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
