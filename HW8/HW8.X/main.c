#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"
#include "ws2812b.h"
#include "font.h"   
#include "ssd1306.h"
#include "rtcc.h"
#include <stdio.h>
// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF// disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = ON // disable secondary oscillator
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
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
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

void draw_letter(unsigned char num);
//void write_printf(unsigned char i);
void drawMessage(unsigned char x_begin, unsigned char y_begin, char *message);
void setPin(unsigned char address,unsigned char regi,unsigned char value);
unsigned char readPin(unsigned char address,unsigned char regi);
int main(){
     __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
//    TRISAbits.TRISA4=0;
//
//    LATAbits.LATA4=0;

    __builtin_enable_interrupts();
    
    //set up
    i2c_master_setup();
    adc_setup();
    ssd1306_setup();
    ctmu_setup();
    ws2812b_setup();
    
    
    
    unsigned char Write_add=0b01000000;
    unsigned char IODIRA_reg=0x00;
    unsigned char IODIRA_value=0x00;
    unsigned char IODIRB_reg=0x01;
    unsigned char IODIRB_value=0xFF;
    unsigned char OLATA_reg=0x14;
    unsigned char OLATA_value=0b10000000;
    unsigned char GPIOB_reg=0x13;
    unsigned long TIME,DATE;
    int hrs[2];
    int mins[2];
    int secs[2];
    rtccTime current_time;
    char day[20];
    char message[50];
    char message2[50];
    int count1,count2;
    //initialize I2C , ssd and adc and ctmu    
//    AD1PCFGbits.PCFG11=0;
//    AD1PCFGbits.PCFG12=0;
    setPin(Write_add,IODIRA_reg,IODIRA_value);
    setPin(Write_add,IODIRB_reg,IODIRB_value);
    int read1,read2,calculate;
    char dd[50]={"ABCDEFGHJKLMNOPQRSTUVWXYZ1234567890"};
    TIME=0x12461000;
    DATE=0x20060606;
    rtcc_setup(TIME,DATE);
    
    int timeindex=12;
    int dateindex=25;
    int counter=0;
    while (1){
        current_time=readRTCC();
        dayOfTheWeek(current_time.wk,day);
        sprintf(message,"%s","Hi!");
        drawMessage(4,0,message);
        sprintf(message,"%d",counter);
        drawMessage(50,0,message);
        sprintf(message,"%d",current_time.hr10);
        drawMessage(4,timeindex,message);
        sprintf(message,"%d",current_time.hr01);
        drawMessage(10,timeindex,message);
        sprintf(message,"%c",':');
        drawMessage(16,timeindex,message);
        sprintf(message,"%d",current_time.min10);
        drawMessage(22,timeindex,message);
        sprintf(message,"%d",current_time.min01);
        drawMessage(28,timeindex,message);
        sprintf(message,"%c",':');
        drawMessage(34,timeindex,message);
        sprintf(message,"%d",current_time.sec10);
        drawMessage(40,timeindex,message);
        sprintf(message,"%d",current_time.sec01);
        drawMessage(46,timeindex,message);
        sprintf(message,"%s",day);
        drawMessage(4,dateindex,message);
        sprintf(message,"%c",',');
        drawMessage(45,dateindex,message);
        sprintf(message,"%d",current_time.mn10);
        drawMessage(54,dateindex,message);
        sprintf(message,"%d",current_time.mn01);
        drawMessage(60,dateindex,message);
        sprintf(message,"%c",'/');
        drawMessage(66,dateindex,message);
        sprintf(message,"%d",current_time.dy10);
        drawMessage(72,dateindex,message);
        sprintf(message,"%d",current_time.dy01);
        drawMessage(78,dateindex,message);
        sprintf(message,"%c",'/');
        drawMessage(84,dateindex,message);
        sprintf(message,"%d",20);
        drawMessage(90,dateindex,message);
        sprintf(message,"%d",current_time.yr10);
        drawMessage(100,dateindex,message);
        sprintf(message,"%d",current_time.yr01);
        drawMessage(106,dateindex,message);
        
//        sprintf(message," %d",current_time.yr01);
//        drawMessage(14,dateindex,message);
//        sprintf(message," %d",current_time.mn10);
//        drawMessage(24,dateindex,message);
//        sprintf(message," %d",current_time.mn01);
//        drawMessage(34,dateindex,message);
//          
        
////        sprintf(message," %d",current_time.yr10);
//        drawMessage(4,8,message);
        ssd1306_update();
        ssd1306_clear();
        delay();
        counter++;
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



