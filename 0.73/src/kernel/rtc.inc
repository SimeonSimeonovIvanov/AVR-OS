/*
	      The^day^of^DooM

	Create date:   01.02.2009
	Last Update:   02.02.2009
*/
OSRTCInterruptF:
	push r16
	push r17
	push r24
	push r25
	
	lds r24, OS_RTC_MS_L
	lds r25, OS_RTC_MS_H

	adiw r24, 1
	ldi r16, high(1000)
	cpi r24, low(1000)
	cpc r25, r16
	brne OSRTCInterruptIsNot1SL
	clr r24 clr r25
	
	lds r16, OS_RTC_SS
	inc r16
	cpi r16, 60
	brne OSRTCInterruptIsNot1ML
	clr r16
	
	lds r17, OS_RTC_HH
	inc r17
	cpi r17, 24
	brne OSRTCInterruptIsNot24HL
	clr r17
	
  OSRTCInterruptIsNot24HL:
	sts OS_RTC_HH, r17

  OSRTCInterruptIsNot1ML:
	sts OS_RTC_SS, r16

  OSRTCInterruptIsNot1SL:
  	sts OS_RTC_MS_L, r24
  	sts OS_RTC_MS_H, r25

	pop r25
	pop r24
	pop r17
	pop r16
reti
