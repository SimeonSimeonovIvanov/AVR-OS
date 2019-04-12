/*
	 The^day^of^DooM

    Borland C++ 3.1
    (Model->Large)

    Create date:   XX.XX.2005
    Last Update:   07.02.2006
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mouse.h"

typedef unsigned char UCHAR;

#define COLOR
#define ERROR_MESSAGE

#define WINDOW_IS_OPEN 1
#define WINDOW_IS_CLOSE 0

#define SetColor(color) { Color=(UCHAR)color; };
#define GoTo(x,y) { xScr=(UCHAR)x;  yScr=(UCHAR)y; };

#define WaitForVerticalRetrace {           \
	  while((inportb(0x3DA) & 0x08));  \
	  while(!(inportb(0x3DA) & 0x08)); \
	}

typedef struct {
    char *Title;
#ifdef COLOR
    char far *CharColor;
    char border_color, window_color,  title_color;
#endif
    char far *SavePoint;
    unsigned FLAG:1;
    char X, Y, SIZE_X, SIZE_Y;
} WINDOW;

UCHAR xScr, yScr, Color;

//----------------------------------------------------------------------------
char GetChar(void);
char GetColor(void);
void PutChar(char ch);
void PutStr(char *str);
void SaveWindow(WINDOW *Window);
void DrawWindow(WINDOW *Window);
void OpenWindow(WINDOW *Window);
void CloseWindow(WINDOW *Window);
void ResetWindow(WINDOW *Window);
void SetVideoMode(UCHAR VideoMode);
void DrawWindowBorder(WINDOW *Window);
void CheckWindowPosition(WINDOW *Window);
void MoveWindow(WINDOW *Window,char xNew,char yNew);
//----------------------------------------------------------------------------

#ifdef COLOR
void DefineWindow( WINDOW *Window,char *Title,char x,char y,char size_x,
		   char size_y, char border_color, char window_color,
		   char title_color
		 );
#else
void DefineWindow( WINDOW *Window,char *Title,char x,char y,char size_x,
		   char size_y
		 );
#endif


#ifdef COLOR
void DefineWindow( WINDOW *Window,char *Title,char x,char y,char size_x,
		   char size_y, char border_color, char window_color,
		   char title_color
		 )
#else
void DefineWindow( WINDOW *Window,char *Title,char x,char y,char size_x,
		   char size_y
		 )
#endif
{
    if(size_y<2)
      size_y=2;
    if(size_x<strlen(Title)+4)
      size_x=strlen(Title)+4;

    if(size_x<=strlen(Title)) size_x=strlen(Title)+3;
    if(size_y>25) size_y=25;
    if(size_x>80) size_x=80;

    Window->SavePoint=(char far *) malloc(size_x*size_y);
#ifdef COLOR
    Window->CharColor=(char far *) malloc(size_x*size_y);
#endif
    if( !*Window->SavePoint
#ifdef COLOR
	|| !*Window->CharColor
#endif
      ) {
	  puts( "DefineWindow(char *Title,int x,int y,int size_x,int size_y,\
		 border_color,window_color,title_color);"
	      );
	  return;
	}

#ifdef COLOR
    Window->border_color=border_color;
    Window->window_color=window_color;
    Window->title_color=title_color;
#endif
    Window->Title=(char *) malloc(strlen(Title));
    strcpy(Window->Title,Title);
    Window->X=x; Window->SIZE_X=size_x;
    Window->Y=y; Window->SIZE_Y=size_y;
    CheckWindowPosition(Window);
    Window->FLAG=WINDOW_IS_CLOSE;
}

void OpenWindow(WINDOW *Window)
{
    if(Window->FLAG==WINDOW_IS_CLOSE) {
      SaveWindow(Window);
      DrawWindow(Window);
      Window->FLAG=WINDOW_IS_OPEN;
    }
}

void CloseWindow(WINDOW *Window)
{
    if( *Window->SavePoint && Window->FLAG==WINDOW_IS_OPEN
#ifdef COLOR
	&& *Window->CharColor
#endif
      ) {
	  int n=0;
	  register char i,j;

	  HideMouse
	  for(i=Window->X;i<Window->X+Window->SIZE_X;i++)
	    for(j=Window->Y;j<Window->Y+Window->SIZE_Y;j++) {
	       GoTo(i,j);
#ifdef COLOR
	       SetColor(Window->CharColor[n]);
#endif
	       PutChar(Window->SavePoint[n]);
	       n++;
	    }
	  ShowMouse
	  Window->FLAG=WINDOW_IS_CLOSE;
	}
}

void MoveWindow(WINDOW *Window,char xNew,char yNew)
{
    if( *Window->SavePoint && (xNew!=Window->X || yNew!=Window->Y)
#ifdef COLOR
	&& *Window->CharColor
#endif
      ) {
	  WaitForVerticalRetrace;
	  CloseWindow(Window);
	  Window->X=xNew;  Window->Y=yNew;
	  CheckWindowPosition(Window);
	  OpenWindow(Window);
	}
}

void ResetWindow(WINDOW *Window)
{
    if( *Window->SavePoint
#ifdef COLOR
	&& *Window->CharColor
#endif
      ) {
	  CloseWindow(Window);
	  free(Window->Title);
	  free((char *)Window->SavePoint);
	  Window->X=Window->Y=Window->SIZE_X=Window->SIZE_Y=Window->FLAG=0;
#ifdef COLOR
	  free((char *)Window->CharColor);
	  Window->border_color=Window->window_color=Window->title_color=0;
#endif
	}
#ifdef ERROR_MESSAGE
	else puts("void ResetWindow(WINDOW *Window);");
#endif
}

void CheckWindowPosition(WINDOW *Window)
{
    if(Window->X<0) Window->X=0;
    else if(Window->X+Window->SIZE_X>80) Window->X=80-Window->SIZE_X;

    if(Window->Y<0) Window->Y=0;
    else if(Window->Y+Window->SIZE_Y>25) Window->Y=25-Window->SIZE_Y;
}

void SaveWindow(WINDOW *Window)
{
    if( *Window->SavePoint
#ifdef COLOR
       && *Window->CharColor
#endif
      ) {
	  int n=0;
	  register char i,j;

	  HideMouse
	  for(i=Window->X;i<Window->X+Window->SIZE_X;i++)
	    for(j=Window->Y;j<Window->Y+Window->SIZE_Y;j++) {
	      GoTo(i,j)
	      Window->SavePoint[n]=GetChar();
#ifdef COLOR
	      Window->CharColor[n]=GetColor();
#endif
	      n++;
	    }
	  ShowMouse
	}
#ifdef ERROR_MESSAGE
	else puts("void SaveWindow(WINDOW *Window);");
#endif
}

void DrawWindow(WINDOW *Window)
{
    register UCHAR i,j;
    if( *Window->SavePoint
#ifdef COLOR
	&& *Window->CharColor
#endif
      ) {
	  HideMouse
#ifdef COLOR
	  SetColor(Window->window_color);
#endif
	  for(i=Window->X+1;i<Window->X+Window->SIZE_X-1;i++)
	     for(j=Window->Y+1;j<Window->Y+Window->SIZE_Y-1;j++) {
		GoTo(i,j)  PutChar(219);
	     }

	  DrawWindowBorder(Window);
#ifdef COLOR
	  SetColor(Window->title_color);
#endif
	  GoTo(Window->X+2,Window->Y)
	  PutStr(Window->Title);
	  ShowMouse
	}
#ifdef ERROR_MESSAGE
	else puts("void DrawWindow(WINDOW *Window);");
#endif
}

void DrawWindowBorder(WINDOW *Window)
{
    register char i,j;
#ifdef COLOR
    SetColor(Window->border_color);
#endif
    GoTo(Window->X,Window->Y);  PutChar(201);
    GoTo(Window->X+Window->SIZE_X-1,Window->Y);  PutChar(187);
    GoTo(Window->X,Window->Y+Window->SIZE_Y-1);  PutChar(200);
    GoTo(Window->X+Window->SIZE_X-1,Window->Y+Window->SIZE_Y-1); PutChar(188);

    for(i=Window->X+1;i<Window->X+Window->SIZE_X-1;i++) {
       GoTo(i,Window->Y)  PutChar(205);
       GoTo(i,Window->Y+Window->SIZE_Y-1)  PutChar(205);
    }

    for(i=Window->Y+1;i<Window->Y+Window->SIZE_Y-1;i++) {
       GoTo(Window->X,i);  PutChar(186);
       GoTo(Window->X+Window->SIZE_X-1,i)  PutChar(186);
    }
}

void SetVideoMode(UCHAR VideoMode)
{
    asm {
      mov Ah, 0x00
      mov AL, VideoMode
      int 0x10
    }
#ifndef COLOR
    Color=15;
#endif
}

void PutChar(char ch)
{
    char far *video_buffer=(char far *) 0xB8000000;
    *(video_buffer+((xScr*2)+(yScr*160)))=ch;
    *(video_buffer+(((xScr*2)+(yScr*160))+1))=Color;
}

char GetChar(void)
{
    char far *video_buffer=(char far *) 0xB8000000;
    return *(video_buffer+((xScr*2)+(yScr*160)));
}

char GetColor(void)
{
    char far *video_buffer=(char far *) 0xB8000000;
    return *(video_buffer+(((xScr*2)+(yScr*160))+1));
}

void PutStr(char *str)
{
    while(*str) {
      switch(*str) {
      case '\t': xScr+=4; str++; break;
      case '\n': xScr=0; yScr++; str++; break;
      default: GoTo(xScr++,yScr); PutChar(*str++);
      }
    }
}