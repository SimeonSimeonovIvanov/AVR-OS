/*
	 The^day^of^DooM

    Borland C++ 3.1

    Create date:   XX.XX.2005
    Last Update:   02.02.2006
*/

#ifndef __PORTH__
  #define __PORTH__
#endif

#ifndef UINT
  typedef unsigned int UINT;
#endif

#ifndef UCHAR
  typedef unsigned char UCHAR;
#endif

#define LPT_1 888

#define port_sbi(port,bit) (                                          \
	  outpb((UINT)port,(inpb((UINT)port) | X_na_N(2,(UCHAR)bit))) \
	);

// ---------------------------------------------------------------------------
char inpb(UINT port);
UCHAR X_na_N(UCHAR X, UCHAR N);
void outpb(UINT port, char value);
void port_cbi(UINT port, UCHAR bit);
unsigned port_gbi(UINT port,UCHAR bit);
// ---------------------------------------------------------------------------

UCHAR X_na_N(UCHAR X, UCHAR N)
{
    register UCHAR i, temp=X;
    for(i=1;i<N;i++,X*=temp);
    return X;
}

char inpb(UINT port)
{
    char value;
    asm {
      mov dx, port
      in al, dx
      mov value, al
    }
    return value;
}

void outpb(UINT port, char value)
{
    asm {
      mov dx, port
      mov al, value
      out dx, al
    }
}

unsigned port_gbi(UINT port,UCHAR bit)
{
    if((inpb(port) & X_na_N(2,bit))) return 1;
    return 0;
}

void port_cbi(UINT port, UCHAR bit)
{
    UCHAR ch=inpb(port);
    if((ch & X_na_N(2,bit))) {
       outpb(port,(ch^=X_na_N(2,bit)));
    }
}
