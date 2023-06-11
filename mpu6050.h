#ifndef __MPU6050_H
#define __MPU6050_H
#include <stm32f10x.h>

#define MPU6050_ADDR 0xD0

void MPU6050_init(void);
uint16_t getAccelX();
uint16_t getAccelY();
uint16_t getAccelZ();
double getAccel();
#endif
