#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"
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
            while(_CP0_GET_COUNT()<24000000/2){}
}
void setPin(unsigned char address,unsigned char regi,unsigned char value);
unsigned char readPin(unsigned char address,unsigned char regi);
int main(){
    unsigned char Write_add=0b01000000;
    unsigned char IODIRA_reg=0x00;
    unsigned char IODIRA_value=0x00;
    unsigned char IODIRB_reg=0x01;
    unsigned char IODIRB_value=0xFF;
    unsigned char OLATA_reg=0x14;
    unsigned char OLATA_value=0b10000000;
    unsigned char GPIOB_reg=0x13;
    i2c_master_setup();
    setPin(Write_add,IODIRA_reg,IODIRA_value);
    setPin(Write_add,IODIRB_reg,IODIRB_value);
  /*
    i2c_master_start();
    i2c_master_send(Write_add);
    i2c_master_send(0x01);
    i2c_master_send(0xFF);
    i2c_master_stop();
    */
    while(1){
    if(readPin(Write_add,GPIOB_reg)){
        setPin(Write_add,OLATA_reg,OLATA_value);
      //  delay();
    }
    else{
        setPin(Write_add,OLATA_reg,OLATA_value<<1);
      //  delay();
    }
    }
    //   IODIRA=0x00;
}
void setPin(unsigned char address,unsigned char regi,unsigned char value){
    
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(regi);
    i2c_master_send(value);
    i2c_master_stop();
}
unsigned char readPin(unsigned char address,unsigned char regi){
    unsigned char pushstate;
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(regi);
    i2c_master_restart();
    i2c_master_send(address|0b00000001);
    pushstate=i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return pushstate;
}