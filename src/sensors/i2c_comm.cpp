#include <Energia.h>
#include <Wire.h>

static TwoWire boosted_wire(0); 

void i2c_write (uint8_t i2c_add, byte data[], uint8_t length)
{
  boosted_wire.beginTransmission (i2c_add);  
  for (int i = 0; i < length; i++)
  {
    boosted_wire.write (data[i]);
  }
  boosted_wire.endTransmission ();
}

void i2c_write_to_reg (uint8_t i2c_add, uint8_t reg,  byte data[], uint8_t length)
{
  boosted_wire.beginTransmission (i2c_add);  
  boosted_wire.write (reg);
  for (int i = 0; i < length; i++)
  {
    boosted_wire.write (data[i]);
  }
  boosted_wire.endTransmission ();
}

void i2c_read (uint8_t i2c_add, byte buffer[], uint8_t length)
{
  boosted_wire.beginTransmission (i2c_add);  
  for (int i = 0; i < length; i++)
  {
    boosted_wire.read (data[i]);
  }
  boosted_wire.endTransmission ();
}

void i2c_read_from_reg (uint8_t i2c_add, uint8_t reg, byte buffer[], uint8_t length)
{
  boosted_wire.requestFrom (i2c_add, length);  
  for(int i = 0; i<length; i++){
  while(!boosted_wire.availible)
  {
    delay(20); //don't hog the cpu... doesn't really matter if we have one thread though
  }
    buffer[i] = boosted_wire.read();
  }
}



