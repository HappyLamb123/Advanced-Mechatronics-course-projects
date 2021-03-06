#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "../i2c_master_noint.h"
#include "../font.h"
#include "../ssd1306.h"
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
            while(_CP0_GET_COUNT()<24000000/50){}
}
void setPin(unsigned char address,unsigned char regi,unsigned char value);
void draw_letter(unsigned char num);
//void write_printf(unsigned char i);
void drawMessage(unsigned char x_begin, unsigned char y_begin, char *message);
unsigned char readPin(unsigned char address,unsigned char regi);
int main(){
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=0;
    unsigned char Write_add=0b01000000;
    unsigned char IODIRA_reg=0x00;
    unsigned char IODIRA_value=0x00;
    unsigned char IODIRB_reg=0x01;
    unsigned char IODIRB_value=0xFF;
    unsigned char OLATA_reg=0x14;
    unsigned char OLATA_value=0b10000000;
    unsigned char GPIOB_reg=0x13;
    char message[50];
    char message2[50];
    int count1,count2;
    
    //initialize I2C
    i2c_master_setup();
    setPin(Write_add,IODIRA_reg,IODIRA_value);
    setPin(Write_add,IODIRB_reg,IODIRB_value);
    unsigned char x; 
    unsigned char y; 
    unsigned char color;
    ssd1306_setup();
  //  draw_letter('A'-0x20);
    
    
    char dd[50]={"ABCDEFGHJKLMNOPQRSTUVWXYZ1234567890"};

    drawMessage(1,0,dd);
  //  drawMessage(0,16,message);

//    i=1/_CP0_GET_COUNT();
//    sprintf(message2,'The fps = %d',i);
//    drawMessage(1,24,message2);
    int i=1;
    while(1){
        _CP0_SET_COUNT(0);
        
        
        sprintf(message,"i = %d",i);
        drawMessage(0,16,message);
        ssd1306_update();
        count1=_CP0_GET_COUNT();
        count2=24000000/count1;
        sprintf(message,"fps = %d",count2);
        drawMessage(0,24,message);  
        ssd1306_update();
//        x=0;
//        y=0;
//        color=0;
//        LATAbits.LATA4=1;
//        ssd1306_drawPixel(x,y,color);
//        ssd1306_update();
//        delay();
//        color=1;
//        ssd1306_drawPixel(x,y,color);
//        ssd1306_update();
//        LATAbits.LATA4=0;
//        delay();
        i++;
//        
    }
    

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
void draw_letter(unsigned char num){
    _CP0_SET_COUNT(0);
    unsigned char x; 
    unsigned char y; 
    unsigned char color;
    int i=0,j=0;
    
    for (j=0; j<5; j++){
        for (i=0; i<8;i++){
            x=j;
            y=i;
            color=((ASCII[num][j])>>i)&0b1;
//            if((==1)
//            {
//                color=1;
//            }
//            else{
//                color=0;
//            }
            ssd1306_drawPixel(x,y,color);
            
        }
    }
    ssd1306_update();
}
void drawMessage(unsigned char x_begin,unsigned char y_begin,char *message){
    
    int i=0,j=0,s=0;
    unsigned char x,y;
    unsigned char color;
    while(message[s]!='\0'){
        if (x_begin+5>125){
            x_begin=0;
            y_begin+=8;
        }
        else if (s!=0){
        x_begin+=5;
        }
        for (j=0; j<5; j++){
            for (i=0; i<8;i++){
                x=j+x_begin;
                y=i+y_begin;
                color=((ASCII[(message[s]-0x20)][j])>>i)&0b1;
    //            if((==1)
    //            {
    //                color=1;
    //            }
    //            else{
    //                color=0;
    //            }
                ssd1306_drawPixel(x,y,color);

            }
        }
        
        s++;
    }
    
    
}

