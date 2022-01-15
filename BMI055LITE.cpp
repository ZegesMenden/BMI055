#include "BMI055.h"
#include "Arduino.h"
#include "Wire.h"

#define ACC_CHIPID  0x00
#define ACC_X_LSB   0x02
#define ACC_X_MSB   0x03
#define ACC_Y_LSB   0x04
#define ACC_Y_MSB   0x05
#define ACC_Z_LSB   0x06
#define ACC_Z_MSB   0x07
#define ACC_RANGE   0x0f
#define ACC_BW      0x10
#define ACC_SFRSET  0x14
#define ACC_OFC_CTRL     0x36

#define GYRO_CHIPID       0x00
#define GYRO_X_LSB        0x02
#define GYRO_X_MSB        0x03
#define GYRO_Y_LSB        0x04
#define GYRO_Y_MSB        0x05
#define GYRO_Z_LSB        0x06
#define GYRO_Z_MSB        0x07
#define GYRO_INT_STATUS1  0x0a
#define GYRO_RANGE        0x0f
#define GYRO_BW           0x10
#define GYRO_SFRSET       0x14
#define GYRO_INT_EN0      0x15
#define GYRO_SOC          0x31
#define GYRO_FOC          0x32
#define GYRO_INTMAP_1     0x1b

#define BMI055_ACC_ADDR  0x30
#define BMI055_GYRO_ADDR 0xd0
/************************************/
#define ACC_0G_X      2048
#define ACC_1G_X      (2048+1024)
#define ACC_MINUS1G_X (2048-1024)
#define ACC_0G_Y      2048   
#define ACC_1G_Y      (2048+1024)
#define ACC_MINUS1G_Y (2048-1024)
#define ACC_0G_Z      2048       
#define ACC_1G_Z      (2048+1024)
#define ACC_MINUS1G_Z (2048-1024)
/****************************************/

BMI055::BMI055(){};

void BMI055::write(int addr, int reg, int data) {
    Wire.beginTransmission(addr>>1);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission(true);
};

bool BMI055::initialize() {
    
    write(BMI055_ACC_ADDR, ACC_SFRSET, 0xb6);
    delay(100);
    write(BMI055_ACC_ADDR, ACC_RANGE, 0b00001000); // 8g
    delay(10);
    write(BMI055_ACC_ADDR, ACC_BW, 0x0d); // 500hz
    delay(10);

    write(BMI055_GYRO_ADDR, GYRO_RANGE, 0x01);
    delay(10);
    write(BMI055_GYRO_ADDR, GYRO_BW, 0b00000011);
    delay(10);
    write(BMI055_GYRO_ADDR, GYRO_INT_EN0, 0xa0); //new data int enable and auto-offset compensation
    delay(10);
    write(BMI055_GYRO_ADDR, GYRO_INTMAP_1, 0b00000001);
    delay(10);
    
    return true;
};

void BMI055::read_gyro() {

    last_gyros = gyro;

    Wire.beginTransmission(BMI055_GYRO_ADDR>>1);
    Wire.write(GYRO_X_LSB);
    Wire.requestFrom(BMI055_GYRO_ADDR>>1, 6, true);

    gyro.x = (int16_t)(Wire.read()|Wire.read()<<8) * 0.03051757812; // same as x / 32768.0 * 1000.0
    gyro.y = (int16_t)(Wire.read()|Wire.read()<<8) * 0.03051757812; 
    gyro.z = (int16_t)(Wire.read()|Wire.read()<<8) * 0.03051757812; 

    gyro.x *= DEG_TO_RAD;
    gyro.y *= DEG_TO_RAD;
    gyro.z *= DEG_TO_RAD;

    Wire.endTransmission(true);
    
};

void BMI055::read_accel() {

    Wire.beginTransmission(BMI055_ACC_ADDR>>1);
    Wire.write(ACC_X_LSB);
    Wire.requestFrom(BMI055_ACC_ADDR>>1, 6, true);
    
    accel.y = (int16_t)(Wire.read()|Wire.read()<<8) * 0.00239501953;
    accel.x = (int16_t)(Wire.read()|Wire.read()<<8) * 0.00239501953;
    accel.z = (int16_t)(Wire.read()|Wire.read()<<8) * 0.00239501953;

    Wire.endTransmission(true);
};
