#include "imu.h"

void imu_setup(){
    unsigned char who = 0;//0;

    // read from IMU_WHOAMI
    who=readPin(IMU_ADDR,IMU_WHOAMI);
    if (who != 0b01101001){
        while(1){
            
        }
    }

    // init IMU_CTRL1_XL 
    setPin(IMU_ADDR,IMU_CTRL1_XL,0b10000010);
    // init IMU_CTRL2_G
    setPin(IMU_ADDR,IMU_CTRL2_G,0b10001000);
    // init IMU_CTRL3_C
    setPin(IMU_ADDR,IMU_CTRL3_C,0b00000100);
}

void imu_read(unsigned char reg, signed short * data, int len){
    unsigned char unsign_data[len*2];
    signed short a;
    int i=0;
    // read multiple from the imu, each data takes 2 reads so you need len*2 chars
    I2C_read_multiple(IMU_ADDR, reg, unsign_data, len*2);
    // turn the chars into the shorts
    for(i=0;i<len;i++){
        a=unsign_data[2*i+1];
        *(data+i)=(a<<8)|unsign_data[2*i];
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