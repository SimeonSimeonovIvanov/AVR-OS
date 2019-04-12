/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   27.06.2006
*/

.include "scr/kernel/irq.inc"
.include "scr/kernel/task.inc"
.include "scr/kernel/timer.inc"
.include "scr/kernel/message.inc"

#ifdef USART
.include "scr/drivers/usart/usart.inc"
#endif

#ifdef DISPLAY
	.include "scr/drivers/display/display.inc"

		INT0_Str:
			.DB "INT0",0
		INT1_Str:
			.DB "INT1",0
		AVR_OS_MESSAGE:	
			.DB "AVR-OS 0.71",0
		AVR_OS_THIS_IS_TASK_X:
			.DB "Run task", 0
#endif
