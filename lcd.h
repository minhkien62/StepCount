#ifndef __lcd_H__
#define __lcd_H__

#include "stm32f10x_i2c.h"
#include "string.h"


void lcd_init (void);   // initialize lcd

void lcd_cmd (char cmd);  // send command to the lcd

void lcd_data (char data);  // send data to the lcd

void lcd_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);


#endif /* __lcd_H__ */
