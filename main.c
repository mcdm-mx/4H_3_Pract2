#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, /*NOMCLR,*/ STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)
#use fast_io(e)
signed long results=0;
//#define __DEBUG_SERIAL__ //Si comentas esta linea se deshabilita el debug por serial y el PIN_C6 puede ser usado en forma digital I/O

#ifdef __DEBUG_SERIAL__
#define TX_232        PIN_C6
#use RS232(BAUD=9600, XMIT=TX_232, BITS=8,PARITY=N, STOP=1)
#use fast_io(c)
#endif
void rebound(void);
void bloob(void);
void main(void) {
   setup_oscillator(OSC_16MHZ);
   setup_adc_ports(NO_ANALOGS);
   set_tris_a(0xc0);
   set_tris_b(0xf0);
   set_tris_c(0x3f);
   set_tris_d(0xff);
   set_tris_e(0x08);
   
#ifdef __DEBUG_SERIAL__ //Deberiamos de proteger nuestras depuraciones de esta forma o usar una macro ya protegida.
   printf("Hola Mundo\n"); //Puedes usar putc o printf. Revisa la documentación de CCS para ver que mas puedes hacer.
#endif
   int selection=0;
while (1) {
   if(input(PIN_B4)==1){
      selection=1;
   } 
   if(input(PIN_B5)==1){
      selection=2;
   }
   if(input(PIN_B6)==1){
      selection=3;
   }
   if(input(PIN_B7)==1){
      selection=4;
   }    
   if(input(PIN_B4)==0 && selection==1){
      results=(long)input_c()+(long)input_d();
      selection=0;
   }
   if(input(PIN_B5)==0 && selection==2){
      results=(long)input_c()-(long)input_d();
      selection=0;
      }   
   if(input(PIN_B6)==0 && selection==3){
      results=(long)input_c()*(long)input_d();
      selection=0;
   }
   if(input(PIN_B7)==0 && selection==4){
      if(input_d()!=0){
         results=(long)input_c()/(long)input_d();
         selection=0;
      }
      else{
         rebound();
         selection=0;
         //bloob();
      }
   }
   output_a(results);
   output_b(results>>6);
   output_e(results>>10);  
}
}
   
void rebound(void){
   long Error = 0x0;
   for(int repeater = 1; repeater < 5; repeater++){
      Error = 0x1fff;
      output_a(Error);
      output_b(Error>>6);
      output_e(Error>>10);
      delay_ms(200);
      Error = 0x0;
      output_a(Error);
      output_b(Error>>6);
      output_e(Error>>10);
      delay_ms(200);
   }
   break;
}

void bloob(void){
    long Error=0x1;
    for(int repeatergo = 1; repeatergo <= 13; repeatergo++){
        Error=Error+Error;
       output_a(Error);
       output_b(Error>>6);
       output_e(Error>>10);
       delay_ms(10);
    }

    Error=0x1000;
    for(int repeaterback = 13; repeaterback >=1; repeaterback--){
        Error/=2;
       output_a(Error);
       output_b(Error>>6);
       output_e(Error>>10);
       delay_ms(10);
    }
    break;
}