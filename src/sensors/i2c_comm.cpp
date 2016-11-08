#include <Energia.h>
#include <Wire.h>
#include <sensors/i2c_comm.h>

static TwoWire boosted_wire(0); 

void i2c_write (uint8_t i2c_add, uint8_t *data, uint8_t length)
{
  boosted_wire.beginTransmission (i2c_add);  
  for (int i = 0; i < length; i++)
  {
    boosted_wire.write (data[i]);
  }
  boosted_wire.endTransmission ();
}

void i2c_write_to_reg (int i2c_add, uint8_t reg,  uint8_t *data, 
    uint8_t length)
{
  Serial.println(" writing 1");
  boosted_wire.beginTransmission (i2c_add);  
  boosted_wire.write (reg);
  Serial.println(" writing ");
  for (int i = 0; i < length; i++)
  {
    boosted_wire.write (data[i]);
    Serial.println("writing ");
    Serial.println(i);
  }
  Serial.println(" ending ");
  boosted_wire.endTransmission ();
  Serial.println(" done ");
}


void i2c_write_to_reg (int i2c_add, uint8_t reg,  uint8_t val){
  i2c_write_to_reg(i2c_add,reg,&val,1);
}


void i2c_read (int i2c_add, uint8_t *buffer, uint8_t length)
{
  boosted_wire.beginTransmission (i2c_add);  
  for (int i = 0; i < length; i++)
  {
    buffer[i] = boosted_wire.read ();
  }
  boosted_wire.endTransmission ();
}

void i2c_read_from_reg (int i2c_add, uint8_t reg, uint32_t *buffer,
    uint8_t length)
{
  boosted_wire.requestFrom (i2c_add, length);  
  for(int i = 0; i<length; i++){
    while(!boosted_wire.available())
    {
      delay(20); //don't hog the cpu... 
    }
    buffer[i] = boosted_wire.read();
  }
}



