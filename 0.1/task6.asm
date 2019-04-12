#ifndef __MOD_INC__
	.include "mod.inc"
#endif

task6:
	sbi ddra, 7
	ldi r16, 1
	call Mod
ret