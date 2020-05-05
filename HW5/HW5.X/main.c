#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "ws2812b.h"
#include <stdio.h>
// DEVCFG0
#pragma config DEBUG = ON // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_12 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
void delay(){
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000000/100){}
}

int main(){
    wsColor color[1];
    //color[4]={HSBtoRGB(30.0,1.0,0.95),HSBtoRGB(120.0,1.0,0.95),HSBtoRGB(180.0,1.0,0.95),HSBtoRGB(230.0,1.0,0.95)};
    unsigned char R=0b10000000;
    unsigned char G=0b00000000;
    unsigned char B=0b00000000;
    
   /* int i=0;
    for (i=0;i<1;i++){
    color[i].r=R;
    color[i].g=G;
    color[i].b=B;
    }
    */
    /*
    color[0].r=R;
    color[0].g=G;
    color[0].b=B;
    color[1].r=R;
    color[1].g=G;
    color[1].b=B;
    color[2].r=R;
    color[2].g=G;
    color[2].b=B;
    color[3].r=R;
    color[3].g=G;
    color[3].b=B;*/
   /* color[0]=HSBtoRGB(30.0,1,0.95);
    color[1]=HSBtoRGB(120.0,1,0.95);
    color[2]=HSBtoRGB(180.0,1,0.95);
    color[3]=HSBtoRGB(270.0,1,0.95);
    */
    int num=1;
    int i=0;
  /*  color[0].r=R;
    color[0].g=G;
    color[0].b=B;*/
    ws2812b_setup();
    while(1){
      for(i=0;i<=360;i++) {
        //  color[0]=HSBtoRGB(i,1,0.95);
          ws2812b_setColor(color,num);
        //  delay();
      }

    }
    return 0;
}
