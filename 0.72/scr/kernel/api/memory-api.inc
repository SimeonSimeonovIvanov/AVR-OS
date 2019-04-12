/*
	      The^day^of^DooM

	Create date:   26.01.2007
	Last Update:   01.02.2007
*/

.macro OSFree
	call OSFreeF
.endmacro

.macro OSMallocC
	ldi r16, @0
	call OSMallocF
.endmacro
