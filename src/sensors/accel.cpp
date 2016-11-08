#include <Arduino.h>
#include <Energia.h>
#include <Wire.h>
#include <math.h>
#include "sensors/i2c_comm.h"
#include "sensors/accel.h"

#define NUM_AXIS 3

const static int ACCEL_I2C_ADR = 0x1D;
const static int ACCEL_DATA_REG = 0x32;
//0x3A = write, 0x3B = read
//max poll is 800Hz for 400Khz i2c and 200Hz fo 100Khz i2c
//operating above = more noise and missed samples 

const double K_LOW_PASS = 0.2;
double low_pass_filter (double last, double new_val)
{
  return K_LOW_PASS * last + (1.0 - K_LOW_PASS) * new_val;  
}

static double accel_last[NUM_AXIS];
static double accel_curr[NUM_AXIS];

void accel_init ()
{
  //hangs
  for (int i = 0; i<NUM_AXIS; i++)
  {
    accel_last[i] = 0;
    accel_curr[i] = 0;
  }
  Serial.println("hit");
  i2c_write_to_reg(ACCEL_DATA_REG, 0x31, 1);
  i2c_write_to_reg(ACCEL_DATA_REG, 0x2D, 1 << 3);
}

void accel_tick ()
{
  uint8_t arr_size = 6;
  uint32_t sensor_data[arr_size]; 
 
  Serial.println("trying read");
  i2c_read_from_reg (ACCEL_I2C_ADR, 
      ACCEL_DATA_REG,
      sensor_data,
      arr_size); 

  Serial.println(sensor_data[0]);
}

double get_x_accel ()
{
  return 0;
}

double get_y_accel ()
{
  return 0;
}

double get_z_accel ()
{
  return 0;
}

double accel_get_tilt ()
{
  double x_acc = get_x_accel ();
  double y_acc = get_y_accel ();
  return atan2 (x_acc,y_acc);
}


