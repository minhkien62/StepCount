#include "i2c.h"

void I2C1_Init()
{
	I2C_InitTypeDef cau_hinh;

  // Enable I2C1 peripheral clock
  	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  // Configure I2C1 pins
  // ...
	GPIOB->CRL |= (GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
	GPIOB->CRL |= (GPIO_CRL_CNF7 | GPIO_CRL_MODE7);

  // Initialize I2C1 peripheral
  cau_hinh.I2C_ClockSpeed = 100000;  // Set I2C clock speed (in Hz)
  cau_hinh.I2C_Mode = I2C_Mode_I2C;  // Set I2C mode
  cau_hinh.I2C_DutyCycle = I2C_DutyCycle_2;  // Set I2C duty cycle
  cau_hinh.I2C_OwnAddress1 = 0x00;  // Set own address (not used in master mode)
  cau_hinh.I2C_Ack = I2C_Ack_Enable;  // Enable I2C acknowledge
  cau_hinh.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // Set acknowledged address mode

  I2C_Init(I2C1, &cau_hinh);  // Initialize I2C1 with the specified parameters

  I2C_Cmd(I2C1, ENABLE);  // Enable I2C1 peripheral
	
}

void I2C_Write(uint8_t addr, uint8_t data) {

    I2C_GenerateSTART(I2C1, ENABLE);

    // G?i d?a ch? slave và ch? ACK
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, addr, 0);//de ghi 

    // G?i d? li?u
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

        I2C_SendData(I2C1, data);

    // K?t thúc truy?n d? li?u b?ng STOP condition
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_WriteFromRegister(int8_t addr, uint8_t reg, uint8_t data)
{

  // Generate START condition
  I2C_GenerateSTART(I2C1, ENABLE);

  // Wait until the START condition is sent
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  // Send the device address and specify write mode
  I2C_Send7bitAddress(I2C1, addr, 0);

  // Wait until the device is ready to receive data
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  // Send the register address
  I2C_SendData(I2C1, reg);

  // Wait until the byte is transmitted
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  // Send data to the register
    I2C_SendData(I2C1, data);

    // Wait until the byte is transmitted
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//	 while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  // Generate STOP condition
  I2C_GenerateSTOP(I2C1, ENABLE);

}

void I2C_ReadFromRegister(int8_t addr, uint8_t reg, uint8_t data) {
    // Kh?i t?o và c?u hình I2C
    
    // G?i d?a ch? thi?t b?
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // G?i d?a ch? thanh ghi
    I2C_SendData(I2C1, reg);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // G?i l?i START d? d?c d? li?u
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // Ð?c d? li?u t? thanh ghi
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData(I2C1);

    // G?i STOP d? k?t thúc truy?n nh?n
    I2C_GenerateSTOP(I2C1, ENABLE);
}
