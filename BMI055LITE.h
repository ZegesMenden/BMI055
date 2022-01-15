#ifndef BMI055_h
#define BMI055_h

#include "Arduino.h"
#include "Wire.h"

class BMI055
{
  public:

    BMI055(); // object initializer 

    bool initialize(); // sensor startup and various other functions - you need to run this line of code for the sensor to work

    void read_gyro(); // run this to read gyroscope data. gyroscopic data will be accessible in bno.gyro. x / y / z
    void read_accel(); // run this to read accelerometer data. accelerometer data will be accessible in bno.accel. x / y / z

    typedef struct {
      float x, y, z;
    } v3;

    v3 gyro;
    v3 accel;

  private:
    void write(int addr, int reg, int data);
};

#endif
