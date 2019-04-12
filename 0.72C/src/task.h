/*
	      The^day^of^DooM

	Create date:   26.04.2007
	Last Update:   26.04.2007
*/

//TCCR0=(1<<CS01)|(1<<CS00);
//timer_enable_int(_BV(TOIE2) | _BV(TOIE0));

struct TaskInfo {
	char state;
	char stack[100];
	unsigned char regs[32];
};

void SIG_OVERFLOW0()
{
	asm("clin");

	asm("push r0n");

	asm("in r0, 0x3fn");
	asm("push r0n");

	asm("push r1n");
	asm("push r2n");
	asm("push r3n");
	asm("push r4n");
	asm("push r5n");
	asm("push r6n");
	asm("push r7n");
	asm("push r8n");
	asm("push r9n");
	asm("push r10n");
	asm("push r11n");
	asm("push r12n");
	asm("push r13n");
	asm("push r14n");
	asm("push r15n");
	asm("push r16n");
	asm("push r17n");
	asm("push r18n");
	asm("push r19n");
	asm("push r20n");
	asm("push r21n");
	asm("push r22n");
	asm("push r23n");
	asm("push r24n");
	asm("push r25n");
	asm("push r26n");
	asm("push r27n");
	asm("push r28n");
	asm("push r29n");
	asm("push r30n");
	asm("push r31n");


	asm("pop r31n");
	asm("pop r30n");
	asm("pop r29n");
	asm("pop r28n");
	asm("pop r27n");
	asm("pop r26n");
	asm("pop r25n");
	asm("pop r24n");
	asm("pop r23n");
	asm("pop r22n");
	asm("pop r21n");
	asm("pop r20n");
	asm("pop r19n");
	asm("pop r18n");
	asm("pop r17n");
	asm("pop r16n");
	asm("pop r15n");
	asm("pop r14n");
	asm("pop r13n");
	asm("pop r12n");
	asm("pop r11n");
	asm("pop r10n");
	asm("pop r9n");
	asm("pop r8n");
	asm("pop r7n");
	asm("pop r6n");
	asm("pop r5n");
	asm("pop r4n");
	asm("pop r3n");
	asm("pop r2n");
	asm("pop r1n");
	
	asm("pop r0n");
	asm("out 0x3f, r0n");	
	
	asm("pop r0n");
	
	asm("retin");
}
