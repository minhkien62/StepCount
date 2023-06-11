#ifndef __I2C1_H
#define __I2C1_H
#include <stm32f10x.h>
#include <stm32f10x_i2c.h>

void I2C1_Init(void);
void I2C_Write(uint8_t addr, uint8_t data);
void I2C_WriteFromRegister(int8_t addr, uint8_t reg, uint8_t data);
void I2C_ReadFromRegister(int8_t addr, uint8_t reg, uint8_t data);

#endif
