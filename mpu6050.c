#include "MPU6050.h"
#include "i2c.h"
#include <math.h>


// Kh?i t?o MPU6050
void MPU6050_init(void)
{
    uint8_t check;
    uint8_t Data;

    // Ki?m tra device ID WHO_AM_I
    I2C_ReadFromRegister(MPU6050_ADDR, 0x75, check);

    if (check == 0x68)
    {
        // Ghi vào thanh ghi PWR_MGMT_1 d? b? ng? MPU6050
        Data = 0;
        I2C_WriteFromRegister(MPU6050_ADDR, 0x6B, Data);

        // Thi?t l?p DATA RATE là 1KHz b?ng cách ghi vào thanh ghi SMPLRT_DIV
        Data = 0x07;
         I2C_WriteFromRegister(MPU6050_ADDR, 0x19, Data);

        // Thi?t l?p c?u hình gia t?c trong thanh ghi ACCEL_CONFIG
        Data = 0x00;
         I2C_WriteFromRegister(MPU6050_ADDR, 0x1C, Data);
    }
}

// Ð?c d? li?u gia t?c t? MPU6050
uint16_t getAccelX()
{
    uint8_t data[2];
		I2C_ReadFromRegister(MPU6050_ADDR, 0x3B, data[0]);
		I2C_ReadFromRegister(MPU6050_ADDR, 0x3C, data[1]);
    uint16_t accelX = (int16_t)(data[0] << 8 | data[1]);
		return accelX;
}

uint16_t getAccelY()
{
    uint8_t data[2];
		I2C_ReadFromRegister(MPU6050_ADDR, 0x3D, data[0]);
		I2C_ReadFromRegister(MPU6050_ADDR, 0x3E, data[1]);
    uint16_t accelY = (int16_t)(data[0] << 8 | data[1]);
		return accelY;
}

uint16_t getAccelZ()
{
    uint8_t data[2];
		I2C_ReadFromRegister(MPU6050_ADDR, 0x3F, data[0]);
		I2C_ReadFromRegister(MPU6050_ADDR, 0x40, data[1]);
    uint16_t accelZ = (int16_t)(data[0] << 8 | data[1]);
		return accelZ;
}

double getAccel(){
	double accel = getAccelX()*getAccelX()*1.0 + getAccelY() * getAccelY () * 1.0 + getAccelZ() * getAccelZ() *1.0;
	accel = sqrt(accel);
	return accel;
}