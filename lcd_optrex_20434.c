
//***** Header files *****//
#include "lcd_optrex_20434.h"

/*-----------------------LDC PIN OUT BOARD CONNECTION----------------------------------*/
/* PE1  -> EN 
/* PE0  -> RS
/* PD12 -> RW
/* --------
/* PB12 -> D0
/* PB13 -> D1
/* PB14 -> D2
/* PB15 -> D3
/* PD08 -> D4
/* PD09 -> D5
/* PD10 -> D6 
/* PD11 -> D7 
*/

//***** Variables *****//
static uint8_t mode_8_4_I2C = 1;

//2) RS control
void LCD1602_RS(bool state)
{
	if(state)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
    }
	else 
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
    }
}

void LCD1602_RW(bool state)
{
	if(state)
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    }
	else 
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    }
}

//3) Write Parallel interface
void LCD1602_write(uint8_t byte)
{
	//testInfiteLoop();
	uint8_t LSB_nibble = byte&0xF, MSB_nibble = (byte>>4)&0xF;
	 
	if(mode_8_4_I2C == 1)		//8bits mode
	{
		//write data to output pins
		//LSB data
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)(LSB_nibble&0x1));
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)(LSB_nibble>>1&0x1));
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)(LSB_nibble>>2&0x1));
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState)(LSB_nibble>>3&0x1));
		//MSB data
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, (GPIO_PinState)(MSB_nibble&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, (GPIO_PinState)(MSB_nibble>>1&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, (GPIO_PinState)(MSB_nibble>>2&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, (GPIO_PinState)(MSB_nibble>>3&0x1));
		//Write the Enable pulse
		//LCD1602_EnablePulse();
	}
	else if(mode_8_4_I2C == 2)	//4 bits mode
	{
		//write data to output pins
		//MSB data
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, (GPIO_PinState)(MSB_nibble&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, (GPIO_PinState)(MSB_nibble>>1&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, (GPIO_PinState)(MSB_nibble>>2&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, (GPIO_PinState)(MSB_nibble>>3&0x1));
		//Write the Enable pulse
		//LCD1602_EnablePulse();
		
		//LSB data
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, (GPIO_PinState)(LSB_nibble&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, (GPIO_PinState)(LSB_nibble>>1&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, (GPIO_PinState)(LSB_nibble>>2&0x1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, (GPIO_PinState)(LSB_nibble>>3&0x1));
		//Write the Enable pulse
		//LCD1602_EnablePulse();
	}
}

void LCD1602_writeCommand(uint8_t command)
{
	//Set RS to 0
	LCD1602_RS(false);
	//set R/W to 0
	LCD1602_RW(false);
	//Call low level write parallel function
	LCD1602_write(command);
}

void lcd_init()
{
	//Initialise LCD
	//1. Wait at least 15ms
	HAL_Delay(30);
	
	//step 2: Unlock Command
    // RS=0, R/W=0, DB7-DB0= 30H

	LCD1602_writeCommand(0x30);

    //Step 3: Wait for more than 4.1 ms
    HAL_Delay(10);

    //step 4: Unlock Command
    // RS=0, R/W=0, DB7-DB0= 30H
	LCD1602_writeCommand(0x30);

    //Step 5: Wait for more than 1 ms
    HAL_Delay(2);

    //step 6: Poll for busy BF = 0
     //HAL_Lcd_BusyWait();

     //step 7: Function set command
     LCD1602_writeCommand(0x38);

    //step 7: Poll for busy BF = 0
    HAL_Delay(1);

    //step 8: Function set command
    LCD1602_writeCommand(0x8);

    //step 9: Poll for busy BF = 0
    HAL_Delay(1);

    //step 10: Function set command
    LCD1602_writeCommand(0xC);

    //step 11: Poll for busy BF = 0
    HAL_Delay(1);

    //step 12: Function set command
    LCD1602_writeCommand(0x6);

    //step 13: Poll for busy BF = 0
    HAL_Delay(1);

    //step 12: Function set command
    LCD1602_writeCommand(0x1);
}

void testInfiteLoop()
{
    while(1)
    {
    }
}
