/*
			AVR-Os Version 0.1
		
		The^day^of^DooM

	Create date:   01.02.2006
	Last Update:   11.02.2006

					Карта на памета:
   ----------------------------------------------------------------------------
	Резервираната част на OS (процес номер 0) [3 байта]:	
		$0061 - Брой на процесите
		$0062 - Номера на процеса изпълняван в момента
		$0063 - Резервиран
   ----------------------------------------------------------------------------
	Масив на процесите - съставен от отделни
	структури по [3 байта]:
		((Номер на процеса)*3)+0 - Младши байт от адреса на процеса
		((Номер на процеса)*3)+1 - Старша част от адреса на процеса
		((Номер на процеса)*3)+2 - Байт оказващ времето за изпоолзване от OS
   ----------------------------------------------------------------------------

   I) Инициализиране на стека
   II) Създаване на процеси
   III) Пускане на OS
   (добавяне и премахване* на процеси, тяхното изпълнение)

		*при премахване на даден процес то слдващите след него (ако има)
		 си смъкват номера с един надоло.
*/

.include "m16def.inc"
.include "C:\MyINC\LCD\JM162A\JM162A-API.inc"
.include "AVR-OS-API.inc"

.cseg
	OSIRQ
	jmp RESET
.org OVF0addr
	jmp OVF0

RESET:
	OSInit
	OSCreateTask task1, 4 // Read LPT and run LED
	OSCreateTask task2, 1 // Put 'I L*** ****'
	OSCreateTask task3, 1 // Put 'By ****'
	OSCreateTask task6, 1 // 0 1
	OSCreateTask task4, 15 // Put count
	OSCreateTask fdd_motor_task, 16
	OSCreateTask task7, 1 // 1 0
	OSCreateTask task5, 1 // clear LCD

	call LCDInitF
	call fdd_motor_init
	
	sbi DDRB, 1
	sbi PORTB, 1

	ldi R16, 1
	out TIMSK, R16
	ldi R16, 5
	out TCCR0, R16
	sei
	OSRun

.include "AVR-OS.inc"
.include "C:\MyINC\LCD\JM162A\jm162a.inc"
.include "task6.asm"
.include "task7.asm"
.include "fdd_motor_task.asm"

task1:
	//sbi DDRB, 1
	//sbic PINB, 0
	//sbi PORTB, 1
	//sbis PINB, 0
	//cbi PORTB, 1
ret

task2:
	ldi ZL, LOW(2*xxxx_str)
	ldi ZH, HIGH(2*xxxx_str)
	ldi R17, 0
	ldi R16, 0x00
	call LCDPutStrF
ret

task3:
	ldi ZL, LOW(2*by_str)
	ldi ZH, HIGH(2*by_str)
	ldi R17, 0
	ldi R16, 0x46
	call LCDPutStrF
ret

task4:
	ldi R17, 48
	mov R16, R24
	add R16, R17
	ldi R17, 0x41
	call LCDPutCharF
ret

task5:
	DelayMsC 33
	call LCDClearDisplayF
ret

OVF0:
	push R16
	in R16, SREG
	push R16
	
	inc R24
	cpi R24, 10
	brne OVF0_ret
	clr R24
OVF0_ret:
	pop R16
	out SREG, R16
	pop R16
reti

xxxx_str:
	.db "I Xxxx Xxxx",0x00
by_str:
	.db "By Simo",0x00
