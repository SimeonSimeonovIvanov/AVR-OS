/*
		The^day^of^DooM

	Create date:	03.04.2006 \ Last UpDate For 0.72
	Last Update:	02.02.2007 /
	Full Change:	15.01.2008 - And Last Create 0.73
	Last Update:	16.01.2008
	Last Update:	02.02.2009


                TASK_DISPLACEMENT+0  - STATUS

                TASK_DISPLACEMENT+1  - SPH
                TASK_DISPLACEMENT+2  - SPL

                TASK_DISPLACEMENT+3  - TCNTxH
                TASK_DISPLACEMENT+4  - TCNTxL

                TASK_DISPLACEMENT+5  - IRQ_TABLE_BYTE_3
                TASK_DISPLACEMENT+6  - IRQ_TABLE_BYTE_2
                TASK_DISPLACEMENT+7  - IRQ_TABLE_BYTE_1
                TASK_DISPLACEMENT+8  - IRQ_TABLE_BYTE_0
                TASK_DISPLACEMENT+9  - IRQH
                TASK_DISPLACEMENT+10 - IRQL

                TASK_DISPLACEMENT+11  - Message
                TASK_DISPLACEMENT+12  - From...

                TASK_DISPLACEMENT+13  - NUMBER_OF_CURRENT_IRQ

                TASK_DISPLACEMENT+14  - TASK_ID
                TASK_DISPLACEMENT+15  - Ptr To Next Task (L)
                TASK_DISPLACEMENT+16  - Ptr To Next Task (H)
*/

//#define USART
#define DISPLAY
#define DISPLAY_DEBUG

.include "src/kernel/api/irq-api.inc"
.include "src/kernel/api/task-api.inc"
.include "src/kernel/api/timer-api.inc"
.include "src/kernel/api/message-api.inc"

#ifdef USART
.include "src/drivers/usart/usart-api.inc"
#endif

#ifdef DISPLAY
.include "src/drivers/display/display-api.inc"
#endif

// ----------------------------------------------------------------
//	Глобални константи:
.equ	NULL=0

.equ	TASK_IS_RUN=NULL
.equ	TASK_IS_STOP=NULL+1
// -------------------------------------------------------------
//
//					!!! НЕ ПРОМЕНЯЙ !!!
//
//		Тука са декларирани отместванията на различните
//	елементи в структурата на процеса, крайната и големина и д.р

.equ	DISPLACEMENT_OF_TASK_IRQ			= 5
.equ	DISPLACEMENT_OF_TASK_MESSAGE_BOX	= 11
.equ	NUMBER_OF_CURRENT_IRQ				= 13
.equ	TASK_ID								= 14

.equ SIZE_OF_TASK_STRUCTURE	= 100
// ----------------------------------------------------------------
//	Адреси на системните променливи:
.equ	TASK_COUNTER			= ramend
.equ	CURRENT_TASK			= ramend-1

.equ	OS_TASK_ARRAY_STATUS	= ramend-2

.equ	SYSTEM_MESSAGE			= ramend-3
.equ	SYSTEM_MESSAGE_FROM		= ramend-4

.equ	OS_TEMP_H				= ramend-5
.equ	OS_TEMP_L				= ramend-6

.equ 	USART_STATUS			= ramend-7

.equ	OSDisplayStatus			= ramend-8
.equ	OSDisplayX				= ramend-9
.equ	OSDisplayY				= ramend-10

.equ	OSTaskListRoot			= ramend-11
.equ	OSCurrentTaskPtr		= ramend-13

.equ	OS_RTC_MS_H				= ramend-15
.equ	OS_RTC_MS_L				= ramend-16
.equ	OS_RTC_SS				= ramend-17
.equ	OS_RTC_MM				= ramend-18
.equ	OS_RTC_HH				= ramend-19

.equ OS_STACK_BOTTOM			= SRAM_START		+ 24
.equ MM_IS_OPEN_HEAP			= OS_STACK_BOTTOM	+ 1
.equ MM_HEAP_TABLE				= MM_IS_OPEN_HEAP	+ 2
// -----------------------------------------
.macro OSRun
	cli
	lds ZH, OSTaskListRoot
	lds ZL, OSTaskListRoot-1
	jmp OSTaskManagerLoadTaskL
.endmacro
// -----------------------------------------
.macro OSInit
.cseg
.org $000
	jmp OSReset
.org INT0addr
	jmp OSINT0F
.org INT1addr
	jmp OSINT1F
.org OC2addr
	jmp OSTaskManagerF ;OSOC2F
.org OVF2addr
	jmp OSOVF2F
.org ICP1addr
	jmp OSICP1F
.org OC1Aaddr
	jmp OSRTCInterruptF ;OSOC1AF
.org OC1Baddr
	jmp OSOC1BF
.org OVF1addr
	jmp OSOVF1F
.org OVF0addr
	jmp OSOVF0F
.org SPIaddr
	jmp OSSPIF
#ifdef USART
.org URXCaddr		; Rx Complete
	rjmp RXdone
.org UDREaddr		; Data Register Empty
	rjmp TXempty
.org UTXCaddr		; Tx Complete
	rjmp TXdone
#endif
.org ADCCaddr
	jmp OSADCCF
.org ERDYaddr
	jmp OSERDYF
.org ACIaddr
	jmp OSACIF
.org TWIaddr
	jmp OSTWI
.org INT2addr
	jmp OSINT2F
.org OC0addr
	jmp OVF1addr
.org SPMRaddr
	jmp OSSPMRF

OSReset:	
	ldi r16, high(OS_STACK_BOTTOM)
	out sph, r16
	ldi r16, low(OS_STACK_BOTTOM)
	out spl, r16

	call mmInit

	clr r25
	ldi r24, 24 // ~
	call malloc
	
	;clr r16
	;sts CURRENT_TASK, r16
	;sts TASK_COUNTER, r16
	;sts OS_TASK_ARRAY_STATUS, r16

	; Init RTC
	; F = 1 KHz
	; T = t1 + t2 = 500us + 500us = 1ms
	ldi r16, low(999)
	;ldi r16, low(1998)
	out OCR1AL, r16
	ldi r16, high(999)
	;ldi r16, low(1998)
	out OCR1AH, r16
	ldi r16, 1<<WGM12 | 1<<CS11
	out TCCR1B, r16

	; Init system timer (task manager)
	ldi r16, 1<<WGM21 | 1<<CS22 | 1<<CS20
	out TCCR2, r16

	ldi r16, 1<<OCIE1A
	out TIMSK, r16

#ifdef USART
	USARTInit
#endif

#ifdef DISPLAY
	OSDisplayInit
#endif
.endmacro
// -------------------------
.macro OSSaveH
	sts OS_TEMP_H, @0
.endmacro

.macro OSSaveToTempH
	sts OS_TEMP_H, @0
.endmacro
// -------------------------
.macro OSSaveL
	sts OS_TEMP_L, @0
.endmacro

.macro OSSaveToTempL
	sts OS_TEMP_L, @0
.endmacro
// -------------------------
.macro OSLoadH
	lds @0, OS_TEMP_H
.endmacro

.macro OSLoadFromTempH
	lds @0, OS_TEMP_H
.endmacro
// -------------------------
.macro OSLoadL
	lds @0, OS_TEMP_L
.endmacro

.macro OSLoadFromTempL
	lds @0, OS_TEMP_L
.endmacro
// -------------------------
