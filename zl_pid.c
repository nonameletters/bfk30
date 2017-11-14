/*
 * Encode/decode PID hex bytes
 * gcc -std=gnu99
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char**argv)
{ int i;
  int a,b,c;
  double f;
  if(argc==9+1) {
   for(i=0; i < 3; ++i) {
    a = strtol(argv[1+i*3],0,16);
    b = strtol(argv[2+i*3],0,16);
    c = strtol(argv[3+i*3],0,16);
    a = a | b << 8;
    if( c & 0x80 ) a = -a;
    if( c & 0x40 ) c |= ~0x3F;
    printf("% 11.3f", ldexp(a,c));
   }
  } else if(argc==3+1) {
    for(i=0;i<3;++i) {
      sscanf(argv[1+i], "%lf",&f);
      f = frexp(f,&c);
      //printf("%f %d\n", f,c);
      a = f * 65536;
      b = 0;
      if( a < 0 ) { a = -a; b = 0x80; }
      c -= 16;
      //printf("\n%x %x\n", a, b);
      //printf("\n%d %d\n", a, c);
      printf(" %02x %02x %02x", a&0xff, (a>>8)&0xff, (c&0x7F) | b);
    }
  } else {
    printf("Usage: 3 float numbers or 9 hex bytes");
  }
  printf("\n");
  return 0;
}

