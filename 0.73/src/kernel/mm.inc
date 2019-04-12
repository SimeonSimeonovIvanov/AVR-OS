/*
		The^day^of^DooM

	Create Date:	09.08.2006
	Full Change:	10.01.2008 - Реалната дата на започване на работа по MM.
	Last Update:	17.01.2008


 SRAM_START (0x60)                                                     RAMEND (0x45F)
	 ------------------------------------------------------------------------
	| STACK |  HEAP TABLE       ~ ~HEAP_END                       HEAP_START |
	|   24  | others | pointers ~ (pointers end)      Heap                   |
	 ------------------------------------------------------------------------

	Heap Table:
	Malloc Pointers Top
	---malloc pointers---
*/

/*
	InPut: size (r25:r24)
	OutPut: ptr (zh:zl)
*/
malloc: // + (?)
	call MMOpenHeapF
	push r4
	push r5
	push r16
	push r17
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push YL
	push YH

	subi r25:r24, 1
	// current_address: ZH:ZL = ramend - newSize;
	ldi zl, low(ramend)
	ldi zh, high(ramend)
	sub zl, r24
	sbc zh, r25

	// Pointers top (pointers_top; Heap End: heap_end)
	lds r16, MM_HEAP_TABLE
	lds r17, MM_HEAP_TABLE + 1

	// pointer_table
	ldi YL, low(MM_HEAP_TABLE+2)
	ldi YH, high(MM_HEAP_TABLE+2)

  // while(current_address > heap_end) {
  isFreeAddress:
  	cp r16, zl
	cpc r17, zh
	brlo isFreeBlock

	clr zl
	clr zh
	jmp mallocRet

	isFreeBlock:
	ldi YL, low(MM_HEAP_TABLE+2)
	ldi YH, high(MM_HEAP_TABLE+2)
	// for(;pointer_table[]<pointers_top;) {
	currentBlockIsFree:
	  cp r16, yl
	  cpc r17, yh
	  breq blockIsFree
	  
	  ld r20, Y+
	  ld r21, Y+
	  ld r22, Y+
	  ld r23, Y+
	  
	  // if( current_address + newSize < pointer_table[].pointer )
	  movw r5:r4, zh:zl
	  add r4, r24
	  adc r5, r25
	  cp r4, r20
	  cpc r5, r21
	  brlo currentBlockIsFree
	  
	  // if( current_address > pointer_table[].pointer+pointer_table[].size )
	  add r22, r20
	  adc r23, r21
	  cp r22, zl
	  cpc r23, zh
	  brlo currentBlockIsFree
	  
	  sub r20, r24
	  sbc r21, r25
	  movw ZH:ZL, r21:r20
	  subi zh:zl, 1
  jmp isFreeAddress

  blockIsFree:
	st Y+, zl
	st Y+, zh
	st Y+, r24
	st Y+, r25
	
	sts MM_HEAP_TABLE, YL
	sts MM_HEAP_TABLE + 1, YH

  mallocRet:
	pop YH
	pop YL
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r17
	pop r16
	pop r5
	pop r4
	call MMCloseHeapF
ret

//	InPut: ptr (r24:r25)
free: // +
	call MMOpenHeapF
	push r16
	push r17
	push r20
	push r21
	push r22
	push r23
	push XL
	push ZH
	push YL
	push YH

	// Pointers top (heap_end)
	lds r16, MM_HEAP_TABLE
	lds r17, MM_HEAP_TABLE + 1

	ldi YL, low(MM_HEAP_TABLE+2)
	ldi YH, high(MM_HEAP_TABLE+2)

  loop:
	cp r16, yl
	cpc r17, yh
	brlo freeRet

	ld r20, y+
	ld r21, y+
	//adiw yh:yl, 2
	ld r22, y+
	ld r23, y+

	cp r24, r20
	cpc r25, r21
  brne loop

	movw XH:XL, YH:YL
	subi XH:XL, 4
  looop:
	cp r16, yl
	cpc r17, yh
	brlo freeRet00
	
	ld r20, y+
	ld r21, y+
	ld r22, y+
	ld r23, y+

	st x+, r20
	st x+, r21
	st x+, r22
	st x+, r23
  jmp looop
  	freeRet00:
	subi xh:xl, 4
	sts MM_HEAP_TABLE, XL
	sts MM_HEAP_TABLE + 1, XH

freeRet:
	pop YH
	pop YL
	pop XH
	pop XL
	pop r23
	pop r22
	pop r21
	pop r20
	pop r17
	pop r16
	call MMCloseHeapF
ret

MMCloseHeapF:
	push r16
	clr r16
	sts MM_IS_OPEN_HEAP, r16
	pop r16
ret

MMOpenHeapF:
	push r16
  MMOpenHeapWaitL:					//////////////////////////////////
	cli								//		Изчакваме докато приключи
	lds r16, MM_IS_OPEN_HEAP		// предишната операция работеща с
	tst r16							// указателите към заделена памет.
	breq MMOpenHeapEndOfWaitL
	sei
  jmp MMOpenHeapWaitL
  
  MMOpenHeapEndOfWaitL:
	ldi r16, 1
	sts MM_IS_OPEN_HEAP, r16
	pop r16
reti

mmInit: // +
	push r16
	push r17
	push zl
	push zh

	clr r16
	ldi r17, high(ramend+1)
	ldi zl, low(MM_HEAP_TABLE)
	ldi zh, high(MM_HEAP_TABLE)
  mmClearLoop:
	st Z+, r16
	cpi zl, low(ramend+1)
	cpc zh, r17
  brlo mmClearLoop

	ldi r16, low(MM_HEAP_TABLE+2)
	sts MM_HEAP_TABLE, r16
	ldi r16, high(MM_HEAP_TABLE+2)
	sts MM_HEAP_TABLE+1, r16
	
	call MMCloseHeapF

	pop zh
	pop zl
	pop r17
	pop r16
ret
