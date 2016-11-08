#ifndef I2C_COMM_H
#define I2C_COMM_H

void i2c_read_from_reg (uint8_t i2c_add, uint8_t reg, uint32_t *buffer, 
    uint8_t length);

void i2c_write (uint8_t i2c_add, uint8_t * data, uint8_t length);

void i2c_write_to_reg (int i2c_add, uint8_t reg,  
    uint8_t * data, uint8_t length);

void i2c_write_to_reg (int i2c_add, uint8_t reg,  uint8_t val);

void i2c_read (uint8_t i2c_add, uint32_t * buffer, uint8_t length);


#endif

