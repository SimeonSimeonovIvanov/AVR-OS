/*
			AVR-Os Version 0.6
		
		The^day^of^DooM

	Create date:   03.04.2006
	Last Update:   19.05.2006

                                  Карта на паметта:
   -------------------------------------------------------------------------------------
        Резервираната част на OS (процес номер 0) [3 байта]:	
                $61 - Брой на процесите
                $62 - Номера на процеса изпълняван в момента
                $63 - SYSTEM_MESSAGE
   -------------------------------------------------------------------------------------
        Масив на процесите - съставен от структури с рамер SIZE_OF_TASK_STRUCTURE:

                TASK_DISPLACEMENT+0  - H \    Стоп 'точка' на процеа
                TASK_DISPLACEMENT+1  - L /  (адреса в който в прекъсанто изпълнението ми)

				TASK_DISPLACEMENT+2 - SREG на процеса

                TASK_DISPLACEMENT+3  - H \    Времето с което
                TASK_DISPLACEMENT+4  - L /  разполага процеса

				TASK_DISPLACEMENT+5  - Прекъсвания който ще обработва
                TASK_DISPLACEMENT+6  - H \    Адрес на процедурата
                TASK_DISPLACEMENT+7  - L /  изпълняваща рпекъсванията

                TASK_DISPLACEMENT+8  - Пощенска кутия на процеса

                TASK_DISPLACEMENT+9 - R0..R31

                TASK_DISPLACEMENT+41 - Стек на процеса

       !!! TASK_DISPLACEMENT=((NUMBER_OF_TASK * SIZE_OF_TASK_STRUCTURE) + $61) !!!
   -------------------------------------------------------------------------------------

                                     *ВАЖНО*

      1)  При премахване на даден процес то слдващите след него (ако има)
	 си смъкват номера с един надоло.
*/

#define DISPLAY

.include "scr/kernel/api/irq-api.inc"
.include "scr/kernel/api/task-api.inc"
.include "scr/kernel/api/call-api.inc"
.include "scr/kernel/api/timer-api.inc"
.include "scr/kernel/api/message-api.inc"
#ifdef DISPLAY
.include "scr/drivers/display/display-api.inc"
#endif

// -----------------------------------------
//	Адреси на системните променливи:
.equ TASK_COUNTER=$61
.equ CURRENT_TASK=$62
.equ SYSTEM_MESSAGE=$63
.equ OS_TEMP_L=$64
.equ OS_TEMP_H=$65

// -----------------------------------------
//	Глобални константи:
.equ NULL=0

/*
					!!! НЕ ПРОМЕНЯЙ !!!

		Тука са декларирани отместванията на различните
	елементи в структурата на процеса, крайната и големина и д.р
*/
.equ DISPLACEMENT_OF_TASK_STOP_POINT=0
.equ DISPLACEMENT_OF_TASK_SREG=2
.equ DISPLACEMENT_OF_TASK_TIME=3
.equ DISPLACEMENT_OF_TASK_IRQ=5
.equ DISPLACEMENT_OF_TASK_MESSAGE_BOX=8
.equ DISPLACEMENT_OF_TASK_REGISTERS=9
.equ DISPLACEMENT_OF_TASK_STACK=41

.equ SIZE_OF_TASK_STACK=23
.equ NUMBER_OF_TASK_REGISTERS=32

.equ SIZE_OF_TASK_STRUCTURE	= 10 + 32 + 23
// -----------------------------------------

.macro OSInit
.dseg
.org $61

.eseg
.org $1

.cseg
.org $000
	jmp OSReset
.org INT0addr
	jmp OSINT0
.org INT1addr
	jmp OSINT1

.org OC2addr
.org OVF2addr
.org ICP1addr
.org OC1Aaddr
.org OC1Baddr
.org OVF1addr
.org OVF0addr
.org SPIaddr
.org URXCaddr
.org UDREaddr
.org UTXCaddr
.org ADCCaddr
.org ERDYaddr
.org ACIaddr
.org TWIaddr
.org INT2addr

.org OC0addr
		rjmp OSTaskManagerF

/*
.equ	OC0addr	= 0x0026	; Timer/Counter0 Compare Match
.equ	SPMRaddr	= 0x0028	; Store Program Memory Ready
*/
OSReset:
	ldi r16, high(ramend)
	out sph, r16
	ldi r16, low(ramend)
	out spl, r16

	clr r16
	sts CURRENT_TASK, r16
	sts TASK_COUNTER, r16
#ifdef DISPLAY
	OSCreateTaskC DisplayDriverF, 222
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
