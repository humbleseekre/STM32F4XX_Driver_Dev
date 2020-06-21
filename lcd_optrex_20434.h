#ifndef LCD_OPTREX_20434_H
#define LCD_OPTREX_20434_H

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <stdint.h>

/*
0x4002 2000 - 0x4002 23FF GPIOI
0x4002 1C00 - 0x4002 1FFF GPIOH
0x4002 1800 - 0x4002 1BFF GPIOG
0x4002 1400 - 0x4002 17FF GPIOF
0x4002 1000 - 0x4002 13FF GPIOE
0x4002 0C00 - 0x4002 0FFF GPIOD
0x4002 0800 - 0x4002 0BFF GPIOC
0x4002 0400 - 0x4002 07FF GPIOB
0x4002 0000 - 0x4002 03FF GPIOA
*/
void LCD1602_RS(bool state);
void LCD1602_RW(bool state);
void LCD1602_write(uint8_t byte);
void LCD1602_writeCommand(uint8_t command);
void lcd_init();

void testInfiteLoop();
//void HAL_Lcd_BusyWait();

#endif /*LCD_OPTREX_20434_H*/
