/*
	      The^day^of^DooM

	Create date:   16.04.2006
	Last Update:   26.01.2007
*/

.include "scr/kernel/irq.inc"
.include "scr/kernel/task.inc"
.include "scr/kernel/timer.inc"
.include "scr/kernel/memory.inc"
.include "scr/kernel/message.inc"

#ifdef USART
.include "scr/drivers/usart/usart.inc"
#endif

#ifdef DISPLAY
	.include "scr/drivers/display/display.inc"
	AVR_OS_MESSAGE:	
		.DB "AVR-OS 0.72",0
#endif
