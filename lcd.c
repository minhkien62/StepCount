#include <stm32f10x.h>
#include <stm32f10x_i2c.h>
#include "lcd.h"
#include "i2c.h"
#include "systick.h"

#define LCD_ADDRESS 0x4E  // Ð?a ch? I2C c?a màn hình LCD



void lcd_init(void) {			
	
		//initialization process
		delay(16);
		lcd_cmd(0x30);
		delay(5);
		lcd_cmd(0x30);
		delay(1);
		lcd_cmd(0x32);
		lcd_cmd(0x20);

		lcd_cmd(0x28);
		delay(1);
	
		lcd_cmd(0x08); //4-bit mode, 2 lines, 5x8 font
		delay(1);
		lcd_cmd(0x01); // Clear display	
		delay(2);
		lcd_cmd(0x06); // Entry mode set: Increment cursor, no shift
		delay(1);
		lcd_cmd(0x0C); // Display on, cursor off, blink off
}

void lcd_cmd(char cmd)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;  // G?i l?nh v?i RS = 0
		I2C_Write(0x4E,data_t[0]);
    data_t[1] = data_u | 0x08;  // G?i l?nh v?i RS = 0
		I2C_Write(0x4E,data_t[1]);
    data_t[2] = data_l | 0x0C;  // G?i l?nh v?i RS = 0
		I2C_Write(0x4E,data_t[2]);
    data_t[3] = data_l | 0x08;  // G?i l?nh v?i RS = 0
    I2C_Write(0x4E,data_t[3]);
}

void lcd_data(char data)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;  // G?i l?nh v?i RS = 0
		I2C_Write(0x4E,data_t[0]);
    data_t[1] = data_u | 0x09;  // G?i l?nh v?i RS = 0
		I2C_Write(0x4E,data_t[1]);
    data_t[2] = data_l | 0x0D;  // G?i l?nh v?i RS = 0
		I2C_Write(0x4E,data_t[2]);
    data_t[3] = data_l | 0x09;  // G?i l?nh v?i RS = 0
    I2C_Write(0x4E,data_t[3]);
}

void lcd_string(char *str)
{
    while (*str) {
        lcd_data(*str++);
    }
}

void lcd_put_cur(int row, int col)
{
    uint8_t address = 0;
    if (row == 0) {
        address = 0x80 + col;  // Ð?a ch? c?a hàng 1
    } else if (row == 1) {
        address = 0xC0 + col;  // Ð?a ch? c?a hàng 2
    }
    lcd_cmd(address);
}

void lcd_clear(void)
{
    lcd_cmd(0x01); // Xóa màn hình
    delay(2); // Ch? m?t lúc d? màn hình xóa
}
