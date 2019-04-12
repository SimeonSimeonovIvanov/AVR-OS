/*
			AVR-Os Version 0.70
		
		The^day^of^DooM

	Create date:   03.04.2006
	Last Update:   02.07.2006

   -------------------------------------------------------------------------------------
        Масив на процесите - съставен от структури с рамер SIZE_OF_TASK_STRUCTURE:

                TASK_DISPLACEMENT+0  - PCH
                TASK_DISPLACEMENT+1  - PCL

                TASK_DISPLACEMENT+2  - SPH
                TASK_DISPLACEMENT+3  - SPL

                TASK_DISPLACEMENT+4  - TCNTxH
                TASK_DISPLACEMENT+5  - TCNTxL

                TASK_DISPLACEMENT+6  - IRQ_TABLE_BYTE_3
                TASK_DISPLACEMENT+7  - IRQ_TABLE_BYTE_2
                TASK_DISPLACEMENT+8  - IRQ_TABLE_BYTE_1
                TASK_DISPLACEMENT+9  - IRQ_TABLE_BYTE_0
                TASK_DISPLACEMENT+10  - IRQH
                TASK_DISPLACEMENT+11  - IRQL

                TASK_DISPLACEMENT+12  - Message
                TASK_DISPLACEMENT+13  - From...

                TASK_DISPLACEMENT+14 - SREG
				
                TASK_DISPLACEMENT+15 - R31:R0


       !!! TASK_DISPLACEMENT=((NUMBER_OF_TASK * SIZE_OF_TASK_STRUCTURE) - $3) !!!
   -------------------------------------------------------------------------------------

                                     *ВАЖНО*

      1)  При премахване на даден процес то слдващите след него (ако има)
	 си смъкват номера с един надоло.
*/

#define USART
#define DISPLAY
#define DISPLAY_DEBUG

.include "scr/kernel/api/irq-api.inc"
.include "scr/kernel/api/task-api.inc"
.include "scr/kernel/api/timer-api.inc"
.include "scr/kernel/api/message-api.inc"

#ifdef USART
.include "scr/drivers/usart/usart-api.inc"
#endif

#ifdef DISPLAY
.include "scr/drivers/display/display-api.inc"
#endif

// -----------------------------------------
//	Глобални константи:
.equ	NULL=0
// -----------------------------------------
//	Адреси на системните променливи:

.equ	TASK_COUNTER=ramend
.equ	CURRENT_TASK=ramend-1
.equ	SYSTEM_MESSAGE=ramend-2

.equ	OS_TEMP_H=ramend-3
.equ	OS_TEMP_L=ramend-4

.equ	USART_STATUS=ramend-5

.equ	OSDisplayStatus=ramend-6
.equ	OSDisplayX=ramend-7
.equ	OSDisplayY=ramend-8

.equ	OSSPH=ramend-9
.equ	OSSPL=ramend-10
// -----------------------------------------



/*
					!!! НЕ ПРОМЕНЯЙ !!!

		Тука са декларирани отместванията на различните
	елементи в структурата на процеса, крайната и големина и д.р
*/

.equ DISPLACEMENT_OF_TASK_IRQ=6
.equ DISPLACEMENT_OF_TASK_MESSAGE_BOX=12
.equ DISPLACEMENT_OF_TASK_SREG=14

.equ SIZE_OF_TASK_STRUCTURE	= 100
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
	jmp OSOC2F
.org OVF2addr
	jmp OSOVF2F
.org ICP1addr
	jmp OSICP1F
.org OC1Aaddr
	jmp OSOC1AF
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
		jmp OSTaskManagerF
.org SPMRaddr
	jmp OSSPMRF
OSReset:
	ldi r16, high(OSSPH)
	out sph, r16
	ldi r16, low(OSSPL)
	out spl, r16

	clr r16
	sts CURRENT_TASK, r16
	sts TASK_COUNTER, r16

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
