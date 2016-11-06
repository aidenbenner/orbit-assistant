#ifndef I2C_COMM_H
#define I2C_COMM_H

void i2c_write (uint8_t i2c_add, byte[] data, uint8_t length);
void i2c_write_to_reg (uint8_t i2c_add, uint8_t reg,  byte[] data, uint8_t length);
void i2c_read (uint8_t i2c_add, byte[] buffer, uint8_t length);
void i2c_read_from_reg (uint8_t i2c_add, uint8_t reg, byte[] buffer, uint8_t length);

#endif

