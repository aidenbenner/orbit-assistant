#include <wire.h>
#include <math.h>
#include "i2c_comm.ino"

#define NUM_AXIS 3

const static uint8_t ACCEL_I2C_ADR = 0x1D;
const static uint8_t ACCEL_DATA_REG = 0x32;

const double K_LOW_PASS = 0.2;
void low_pass_filter(double last, double new_val)
{
  return K_LOW_PASS * last + (1.0 - K_LOW_PASS) * new_val;  
}


static double accel_last[NUM_AXIS];
static double accel_curr[NUM_AXIS];

void accel_init(){
  for (int i = 0; i<NUM_AXIS; i++)
  {
    accel_last[i] = 0;
    accel_curr[i] = 0;
  }
  //ROLLEN MAGIC !?!? I think this sets the accel to stream mode? 
  i2c_write_to_reg(ACCEL_DATA_REG, 0x31, 1);
  i2c_write_to_reg(ACCEL_DATA_REG, 0x2D, 1 << 3);
}

static accel_tick ()
{
  uint8_t arr_size = 6;
  byte sensor_data[arr_size]; 
  i2c_read_from_reg (ACCEL_I2C_ADR, ACCEL_DATA_REG,sensor_data,arr_size);
  
}

static double get_x_accel ()
{

}

static double get_y_accel ()
{

}

static double get_z_accel ()
{

}

double accel_get_tilt ()
{
  double x_acc = get_x_accel ();
  double y_acc = get_y_accel ();
  return atan2 (x_acc,y_acc);
}



