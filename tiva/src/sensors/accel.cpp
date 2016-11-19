#include <Arduino.h>
#include <Energia.h>
#include <Wire.h>
#include <math.h>
#include "sensors/accel.h"
#include "Wire_Util.h"

#define NUM_AXIS 3

//from RPS demo 
void WireRequestArray(int address, uint8_t* buffer, uint8_t amount);
void WireWriteRegister(int address, uint8_t reg, uint8_t value);
void WireWriteByte(int address, uint8_t value);

static float const    SensorMaximumReading= 512.0;
static float const    SensorMaximumAccel  = 9.81 * 4.0;
static uint8_t const  SensorAccelerometer = 0x1D;

float raw_accel_val[NUM_AXIS];
float filtered_accel_val[NUM_AXIS];

void accel_init()
{
  for(int i = 0; i < NUM_AXIS; i++)
  {
    filtered_accel_val[i] = 0;
    raw_accel_val[i] = 0;
  }
  WireWriteRegister(SensorAccelerometer, 0x31, 1);
  WireWriteRegister(SensorAccelerometer, 0x2D, 1 << 3);
}

float K_LOW_PASS_GAIN = 0.01;
static float low_pass (float new_val, float last_val)
{
  return last_val * (1.0 - K_LOW_PASS_GAIN) + new_val * K_LOW_PASS_GAIN;
}


void accel_tick ()
{
  size_t const DataLength = 6;
  uint32_t data[DataLength] = { 0 };
  
  WireWriteByte(SensorAccelerometer, 0x32);
  WireRequestArray(SensorAccelerometer, data, DataLength);

  uint16_t xi = (data[1] << 8) | data[0];
  uint16_t yi = (data[3] << 8) | data[2];
  uint16_t zi = (data[5] << 8) | data[4];


  raw_accel_val[0] = *(int16_t*)(&xi) / SensorMaximumReading * SensorMaximumAccel; //X 
  raw_accel_val[1] = *(int16_t*)(&yi) / SensorMaximumReading * SensorMaximumAccel; //Y
  raw_accel_val[2] = *(int16_t*)(&zi) / SensorMaximumReading * SensorMaximumAccel; //Z

  //TODO need to add dtime to make filtering consistent
  for(int i = 0; i<NUM_AXIS; i++)
    filtered_accel_val[i] = low_pass(raw_accel_val[i], filtered_accel_val[i]);
  
  //testing
  Serial.println(accel_get_tilt());
}

double get_x_accel ()
{
  return filtered_accel_val[0];
}

double get_y_accel ()
{
  return filtered_accel_val[1];
}

double get_z_accel ()
{
  return filtered_accel_val[2];
}

double accel_get_tilt ()
{
  double x_acc = get_x_accel ();
  double y_acc = get_y_accel ();
  double z_acc = get_y_accel ();
  return atan2 (x_acc,y_acc) * 180.0 / PI;  
}



