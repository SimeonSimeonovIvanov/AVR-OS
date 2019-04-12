/*
	 The^day^of^DooM

    Borland C++ 3.1

    Create date:   XX.XX.2005
    Last Update:   01.01.2006
*/

#define UsesMouse

typedef struct {
   int x,y,button;
} MOUSE;

MOUSE mouse;

#ifdef UsesMouse
   #define Mouse {                                        \
	     int x,y,button;                              \
	     _asm mov AX,0x03;                            \
	     _asm int 0x33;                               \
	     _asm mov button,BX;                          \
	     _asm mov x,CX;                               \
	     _asm mov y,DX;                               \
	     mouse.x=x;  mouse.y=y;  mouse.button=button; \
	   };

   #define InitMouse {                                    \
	     mouse.x=mouse.y=mouse.button=-1;             \
	     _asm mov AX,0x00;                            \
	     _asm int 0x33;                               \
	   };

   #define ShowMouse {                                    \
	     _asm mov AX,0x01;                            \
	     _asm int 0x33;                               \
	   };

   #define HideMouse {                                    \
	     _asm mov AX,0x02;                            \
	     _asm int 0x33;                               \
	   };
#else
   #define Mouse;
   #define ShowMouse;
   #define HideMouse;
   #define InitMouse       mouse.x=mouse.y=mouse.button=-1;
#endif