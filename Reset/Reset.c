#include "conio.h"
#include "port.h"
#include "ctype.h"
#include "window.h"

#define __ATMEGA16__

#ifdef __ATMEGA16__
  #define LED_BIT 7
#else
  #define LED_BIT 0
#endif

void main(void)
{
    char reset_control=10|128, led_control=10|128;

    clrscr();
    outpb(LPT_1, 0);
    delay(11);
    port_sbi(LPT_1, 3);
    port_sbi(LPT_1, LED_BIT);

    while(1) {
       while(kbhit()) {
	  switch(toupper(getch())) {
	  case  27: return;
	  case 'R': port_cbi(LPT_1, 3); reset_control=4|128; break;
	  case 'O': port_sbi(LPT_1, LED_BIT); led_control=10|128; break;
	  case 'F': port_cbi(LPT_1, LED_BIT); led_control=4|128; break;
	  case 'P': port_sbi(LPT_1, LED_BIT); port_cbi(LPT_1, LED_BIT); break;
	  default: if(port_gbi(LPT_1,LED_BIT)) {
		      led_control=4|128;
		      port_cbi(LPT_1,LED_BIT);
		   }
		   else {
		      led_control=10|128;
		      port_sbi(LPT_1,LED_BIT);
		   }
	  }
       }

       GoTo(0,0);
       SetColor(10);
       PutStr("\n\t\t\tThe^day^of^DooM\n\n");

       SetColor(reset_control|128);
       PutStr("\tR");
       SetColor(15);
       PutStr("eset\n\n");
       if(reset_control==4|128) {
	 delay(333);
	 port_sbi(LPT_1, 3)
	 reset_control=10|128;
       };


       SetColor(15);
       PutStr("\tINT0 is ");
       SetColor(led_control);
       PutStr("ON");
       SetColor(15);
       PutStr(" (press SPACE to change)");
    }
}
