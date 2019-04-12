#ifndef __MOD_INC__
	.include "mod.inc"
#endif

task7:
	sbi ddra, 7
	ldi r16, 2
	call Mod
ret