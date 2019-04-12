/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   12.05.2006
*/

.include "AVR-OS-IRQ.inc"
.include "AVR-OS-TASK.inc"
.include "AVR-OS-TIMER.inc"
.include "AVR-OS-MESSAGE.inc"

#ifdef LCD
	.include "C:\MyINC\LCD\JM162A\JM162A.inc"

	#ifdef LCD_DEBUG
		INT0_Str:
			.DB "INT0",0
		INT1_Str:
			.DB "INT1",0
		AVR_OS_MESSAGE:	
			.DB "AVR-OS 0.5",0
		AVR_OS_THIS_IS_TASK_X:
			.DB "Run task", 0
	#endif // LCD_DEBUG
#endif // LCD
