/*
	      The^day^of^DooM

	Create date:   10.05.2006
	Last Update:   22.09.2006
*/

// ---------------------------------------------------------
#define		UNKNOWN_DISPLEY_TYPE	0
#define					  JM162A	1
#define					  JM162B	2

#define 			DISPLAY_TYPE	JM162A
// ---------------------------------------------------------
#if DISPLAY_TYPE == UNKNOWN_DISPLEY_TYPE
	#define DISPLEY_X_SIZE	0
	#define DISPLEY_Y_SIZE	0
#else
	#if DISPLAY_TYPE == JM162A
		.include "src/drivers/display/jm162a/jm162a-api.inc"
	#endif

	#if DISPLAY_TYPE == JM162B
	#endif
#endif
// ---------------------------------------------------------
.macro OSDisplayPutStr
	ldi ZH, high(2*@0)
	ldi ZL, low(2*@0)
	call OSDisplayPutStrF
.endmacro
// -----------------------------------------
.macro OSDisplayGoToXY
	ldi r16, @0
	sts OSDisplayX, r16
	ldi r16, @1
	sts OSDisplayY, r16
.endmacro
// -----------------------------------------
.macro ClearDisplay
	DisplayClear
.endmacro

.macro DisplayClear
	OSDisplayClear
.endmacro

.macro OSDisplayClear
#if DISPLAY_TYPE == JM162A
	JM162AClear
#endif
.endmacro
// -----------------------------------------
//  Освобождава дисплея за използване
// от другите процеси.
.macro OSCloseDisplay
	call OSCloseDisplayF
.endmacro
// -----------------------------------------
//  Изчаква докато се освободи дислея и
// след това го резервира за своя процес.
.macro OSOpenDisplay
	call OSOpenDisplayF
.endmacro
// -----------------------------------------
.macro OSDisplayInit
	call OSDisplayInitF
.endmacro
// -----------------------------------------


