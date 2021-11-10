#include "BMI055LITE.h"
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

BMI055::BMI055()
{
    Wire.begin();
};

void BMI055::write(int addr, int reg, int data) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission(true);
};

bool BMI055::initialize() {
    
    write(BMI055_ACC_ADDR, ACC_SFRSET, 0xb6);
    delay(100);
    write(BMI055_ACC_ADDR, ACC_RANGE, 0x08); // 8g
    delay(10);
    write(BMI055_ACC_ADDR, ACC_BW, 0x0d); // 500hz
    delay(10);

    write(BMI055_GYRO_ADDR, GYRO_RANGE, 0x00);
    delay(10);
    write(BMI055_GYRO_ADDR, GYRO_BW, 0x02); // 1Khz, 0x03 is 400hz
    delay(10);
    write(BMI055_GYRO_ADDR, GYRO_INT_EN0, 0xa0); //new data int enable and auto-offset compensation
    delay(10);

    return true;
};

void BMI055::read_gyro() {
    Wire.beginTransmission(BMI055_GYRO_ADDR);
    Wire.write(GYRO_X_LSB);
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO_X_LSB, 6, true);
    
    // if (rad == true) {
    gyro.x = (int16_t)(Wire.read()|Wire.read()<<8) / 900.0;
    gyro.y = (int16_t)(Wire.read()|Wire.read()<<8) / 900.0;
    gyro.z = (int16_t)(Wire.read()|Wire.read()<<8) / 900.0; 
    // }

    // if (rad == false) {
    //     gyro.x = (int16_t)(Wire.read()|Wire.read()<<8) / 16.0;
    //     gyro.y = (int16_t)(Wire.read()|Wire.read()<<8) / 16.0;
    //     gyro.z = (int16_t)(Wire.read()|Wire.read()<<8) / 16.0; 
    // }
    
};

void BMI055::read_accel() {
    Wire.beginTransmission(BMI055_ACC_ADDR);
    Wire.write(ACC_X_LSB);
    Wire.endTransmission(false);
    Wire.requestFrom(ACC_X_LSB, 6, true);
    
    accel.y = (int16_t)(Wire.read()|Wire.read()<<8) / 100.0;
    accel.x = (int16_t)(Wire.read()|Wire.read()<<8) / 100.0;
    accel.z = (int16_t)(Wire.read()|Wire.read()<<8) / 100.0;
};
