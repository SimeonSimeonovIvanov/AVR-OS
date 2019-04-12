/*
	      The^day^of^DooM

	Create date:   26.01.2007
	Last Update:   29.05.2007

							!!! Важно !!!
	
	1) Поради технически особености техните параметри са променение.
	   (нямат нищо общо със познатите от езика C)

	Принципа е прост:
		1) Когато искаме да освободим памет, с рамери до 256 байта,
		   зареждаме r16 с размера памет и извикваме OSMllocF.
		   Тя автоматично си заделя парчето памет като му записва
		   адреса и размер на специялно място в таблицата на процеса.
		   Обаче ако записа (адрес и размер) е изцяло 0 - не е намерила
		   памет. За удобство тя връща адреса и в Z a размера, евентуално 0,
		   в R16

		2) Когато искаме да освободим заетия блок просто
		   извикваме OSFreeF. Тя просто записва 0 за адрес и размер в
		   таблицата на паметта на текущия процес. Не я интересува дали
		   изобщо е бил освобождавал памет.

		3) По всяко време можем да получим адреса и размера на освободената
		   памет чрез OSGetPtrToDSF.
*/

// -------------------------------------------------------------------------------------------------------------
OSFreeF:
	push r16
	lds r16, CURRENT_TASK
	call OSKernelFreeF	
	pop r16
ret

/*
	R16 - номера на процеса
*/
OSKernelFreeF:
	call OSOpenTaskArrayF
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	clr r16
	st Z+, r16
	st Z+, r16
	st Z+, r16
	call OSCloseTaskArrayF
ret
// -------------------------------------------------------------------------------------------------------------
/*
	R16 - Размера на желаната памет.
	Return:
		R16		- Рамзера на блока. Ако е нула то не сме заделили памет.
		ZH:ZL 	- Адреса на блока. =//=. Индексира се положително (lds Rx, Z+)
*/
OSMallocF:
	push r17
	lds r17, CURRENT_TASK
	call OSKernelMallocF
	pop r17
ret
// -------------------------------------------------------------------------------------------------------------
/*
	register char size_temp = r16;		 // r2 - Променлива пазеща размера на желаната памет
	register char size_of_current_block; // r3 - Променлива в която зареждаме размера на текущия блок на taskN
	register char task_number_temp;		 // r6 - Променлсива пазеща номера на процеса
	reister int temp,  // R25:R24 - За общо ползване особено в аритметиката
				temp0, // R27:R26 - Kрая на масива на процесите

				temp1; // R29:R28 - Начален адрес от който се започва търсенето на свободен блок памет.
			
				temp3, // R15:R14 \  Началния и крайния адрес на
				temp4, // R13:R12 / блока който ще заделяме

				temp5, // R11:R10 \  Начална и крайния адрес на
				temp6, // R9:R8   / памета която ползва taskN

	В общи линии идеята е следната:

		1) Търсенето на памет започва от края на паметта, temp1
		2) Имаме два указателя покзващи началото и края на блока който ще заделяме, temp4:temp3
		4) Търсенето протича на два основни етапа: търсене на блок и проверка дали е свободен
		    (проверява заделената памет на всички процеси - дали има застъпване)
		5) Напрактика започваме търсенето от края на памета към началото и, изплзвайки temp1
		   за индексиране.
		6) При всякя dec на temp1 се изчислява новата позиция на temp3:temp4
		   (temp4=temp1; temp3=temp1-размера на блока). След това се проверява
		   дали блока не застъпва парче заета памет, като се обхождат всички процеси.
		   Използва се флаг (r4) който съдържа броя на процесите и при проверката на всеки следващ
		   се намалява с 1. Ако стане нула - блока в свобдоен и се освобождава. Иначе се отива на 6)
		   докато се намери блок или 


	R16 - Желания размер памет.
	R17 - Процеса на който ще заделяме памет.
*/

OSKernelMallocF:
	call OSOpenTaskArrayF
	//////////////////////////////////////////////
	mov r2, r16 // Init size_temp
	mov r6, r17 // Init task_number_temp
	//////////////////////////////////////////////
	// Init temp0
	ldi r25, 100
	lds r24, TASK_COUNTER
	mul r25, r24
	movw r27:r26, r1:r0
	adiw r27:r26, 0x31
	adiw r27:r26, 0x31
	//////////////////////////////////////////////
	// temp 1 = MALLOC_SRAM_END
	ldi r29, high(MALLOC_SRAM_END_H)
	ldi r28, low(MALLOC_SRAM_END_L)
	//////////////////////////////////////////////
  OSKernelMallocSerachFreeBlockL:
	// temp4 = temp1
	mov r13, r29
	mov r12, r28
	////////////////////////////////////////////////
	// temp3 = temp4 - size;
	mov r15, r13
	mov r14, r12
	clr r0
	sub r14, r2
	sbc r15, r0
	////////////////////////////////////////////////
	// if(temp3 < temp0) break; - Блока памет който
	cp r14, r26  // искаме да заделиме е навлязъл
	cpc r15, r27 // в масива на процесите, не сме
	             //намерили свободна памет.
	brlo OSKernelMallocThisIsNotFreeBlockForAllTaskL
	////////////////////////////////////////////////
  	lds r16, TASK_COUNTER
	mov r4, r16
	OSKernelMallocTestThisBlockInAllTaskL:
	   call OSGetDisplacementOfTaskF
	   adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	   ld r11, Z+ // \  Init 
	   ld r10, Z+ // / temp5
	   ld r3,  Z+ // Init size_of_current_block
	   tst r16
	   breq OSKernelMallocThisIsFreeBlockForThisTaskL
	   //////////////////////////////////////////////
	   // Init temp6
	   clr r0
	   mov r9, r11
	   mov r8, r10
	   add r8, r3
	   adc r9, r0
	   //////////////////////////////////////////////
	   // if(temp3>=temp6 || temp5>=temp4) {
		cp r14, r8
		cpc r15, r9
	   brge OSKernelMallocThisIsFreeBlockForThisTaskL

		cp r10, r12
		cpc r11, r13
	   brlo OSKernelMallocSerachFreeBlockNextAddressL

	   //////////////////////////////////////////////
	   // Паметта на текущия процес няма застъпване
	   // с паметта която искаме да заделиме.
	   // Когато r4==0 сме намерили свободен блок
	   // за всичките процеси. Инае продължаваме
	   // търсенето като отместваме адреса на
	   // блока с единица надоло
	   OSKernelMallocThisIsFreeBlockForThisTaskL:
	   	 dec r4
	   // }
	   //////////////////////////////////////////////
	   dec r16
	brne OSKernelMallocTestThisBlockInAllTaskL
	OSKernelMallocTestThisBlockInAllTaskBreakL:
	
	tst r4
	breq OSKernelMallocThisIsFreeBlockForAllTaskL

	OSKernelMallocSerachFreeBlockNextAddressL:
	subi r29:r28, 1 // --temp1
  jmp OSKernelMallocSerachFreeBlockL

  OSKernelMallocThisIsFreeBlockForAllTaskL:
	mov r29, r15
	mov r28, r14
   jmp OSKernelMallocRetL

  OSKernelMallocThisIsNotFreeBlockForAllTaskL: // Не сме намерили свободен блок памет
	clr r2
	clr r29
	clr r28

 OSKernelMallocRetL:
 	mov r16, r6
	call OSGetDisplacementOfTaskF
 	adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	st Z+, r29
	st Z+, r28
	st Z+, r2

	mov r16, r12 // Ако е различно от нула - успели сме да заделиме памет
	movw ZH:ZL, r29:r28 // Същото като горното

	call OSCloseTaskArrayF
ret
// -------------------------------------------------------------------------------------------------------------
OSGetPtrToDSF:
	lds r16, CURRENT_TASK
	call OSKernelGetPtrToDSF
ret

/*
	R16 - Номера на процеса
	Return:
		R16		- Размера на паметта
		ZH:ZL	- Адреса на паметта
*/
OSKernelGetPtrToDSF:
	pop XL
	pop XH
	call OSGetDisplacementOfTaskF
	adiw ZH:ZL, MALLOC_TABLE_HIGH_BYTE
	movw XH:XL, ZH:ZL
	ld ZH, X+
	ld ZL, X+
	ld r16, X+
	pop XH
	pop XL
ret
// -------------------------------------------------------------------------------------------------------------
