#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"
#include "imu.h"
#include "font.h"
#include "ssd1306.h"
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
            while(_CP0_GET_COUNT()<24000000/10){}
}

void draw_letter(unsigned char num);
//void write_printf(unsigned char i);
void drawMessage(unsigned char x_begin, unsigned char y_begin, char *message);
void bar_x(signed short x);
void bar_y(signed short y);
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
    signed short IMU_receive[7];
    //initialize I2C , ssd and IMU    
    i2c_master_setup();
    setPin(Write_add,IODIRA_reg,IODIRA_value);
    setPin(Write_add,IODIRB_reg,IODIRB_value);
    ssd1306_setup();
    imu_setup();
    
    char dd[50]={"ABCDEFGHJKLMNOPQRSTUVWXYZ1234567890"};
    int state=1;
    ssd1306_clear();
    while (1){
        imu_read(IMU_OUT_TEMP_L,IMU_receive,7);
        LATAbits.LATA4=1;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<24000000/50){}
        LATAbits.LATA4=0;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<24000000/50){}
    //display a,g and t
        if (!state){
            sprintf(message,"g: %d %d %d",IMU_receive[1],IMU_receive[2],IMU_receive[3]);
            drawMessage(1,0,message);
            ssd1306_update();
            sprintf(message,"a: %d %d %d",IMU_receive[4],IMU_receive[5],IMU_receive[6]);
            drawMessage(1,10,message);
            ssd1306_update();
            sprintf(message,"t: %d",IMU_receive[0]);
            drawMessage(1,20,message);
            ssd1306_update();
        }
        else{
            bar_x(-IMU_receive[5]);
            bar_y(-IMU_receive[4]);
        }
        
        ssd1306_update();
        ssd1306_clear();
    }
    

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

                ssd1306_drawPixel(x,y,color);

            }
        }
        
        s++;
    }
    
    
}
void bar_x(signed short x){
    int num,i;
    num=x/512;
    if (num>0){
        for(i=0;i<num;i++){
            ssd1306_drawPixel(64+i,16,1);
        }
    }
    else{
        num=-num;
        for(i=0;i<num;i++){
            ssd1306_drawPixel(64-i,16,1);
        }
    }
}
void bar_y(signed short y){
    int num,i;
    num=y/2050;
    
    if (num>0){
        for(i=0;i<num;i++){
            ssd1306_drawPixel(64,16+i,1);
        }
    }
    else{
        num=-num;
        for(i=0;i<num;i++){
            ssd1306_drawPixel(64,16-i,1);
        }
    }
}



