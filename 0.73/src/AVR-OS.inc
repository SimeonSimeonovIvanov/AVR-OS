/*
		The^day^of^DooM

	Create date:	16.04.2006 \ Last UpDate For 0.72
	Last Update:	26.01.2007 /
	Full Change:	15.01.2008 - And Last Create 0.73
	Last Update:	17.01.2008
	Last Update:	01.02.2009
*/

.include "src/kernel/mm.inc"
.include "src/kernel/irq.inc"
.include "src/kernel/rtc.inc"
.include "src/kernel/task.inc"
.include "src/kernel/timer.inc"
.include "src/kernel/message.inc"

#ifdef USART
.include "src/drivers/usart/usart.inc"
#endif

#ifdef DISPLAY
	.include "src/drivers/display/display.inc"
	AVR_OS_MESSAGE:
		.DB "AVR-OS 0.73",0
#endif
