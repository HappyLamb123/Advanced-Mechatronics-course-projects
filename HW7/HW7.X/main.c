#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"
#include "ws2812b.h"
#include "font.h"   
#include "ssd1306.h"
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
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
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
    TRISAbits.TRISA4=0;

    LATAbits.LATA4=0;

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
    ssd1306_clear();
    int i;

    wsColor color[4];
    while (1){
        read1=0;
        read2=0;
        for (i=0;i<10;i++){
           read1+=ctmu_read(4,20);
            
           read2+=ctmu_read(5,20);
        }
        color[0].r=255;
        color[0].g=255;
        color[0].b=255;
        color[1].r=255;
        color[1].g=255;
        color[1].b=255;
        color[2].r=255;
        color[2].g=255;
        color[2].b=255;
        color[3].r=255;
        color[3].g=255;
        color[3].b=255;
        /*color[1]=HSBtoRGB(151,0.37,0.3);
        color[2]=HSBtoRGB(151,0.37,0.3);
        color[3]=HSBtoRGB(151,0.37,0.3);*/
//        read1=read1/10;
//        read2=read2/10;

        sprintf(message," %d",read1);
        drawMessage(1,0,message);
        sprintf(message," %d",read2);
        drawMessage(1,10,message);
        ssd1306_update();
        ssd1306_clear();
        //write if statement for LED blinking
        ws2812b_setColor(color,4);
       if (read1<5000){
            color[0]=HSBtoRGB(60,1,0.95);
            color[2]=HSBtoRGB(180,1,0.95);
            ws2812b_setColor(color,4);
            delay();
        }
       if(read2<5000){
            color[1]=HSBtoRGB(120,1,0.95);
            color[3]=HSBtoRGB(240,1,0.95);
            ws2812b_setColor(color,4);
            delay();
        }
        ws2812b_setColor(color,4);
        delay();
       /* else if(read2<9000){
            color[0]=HSBtoRGB(0,1,0.5);
            color[1]=HSBtoRGB(0,1,0.5);
            color[2]=HSBtoRGB(0,1,0.5);
            color[3]=HSBtoRGB(0,1,0.5);
            ws2812b_setColor(color,4);
            
        }*/
         
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



