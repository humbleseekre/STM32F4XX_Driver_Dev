/* =====================================================================
* Karuna Raja Reddy
* ECEN 5613 - Lab #4
* Source File: LCD device driver function definitions
* Fall 2012, Prof. McClure
* University of Colorado at Boulder
* =====================================================================*/


/* Includes ==============================*/
//#include <at89c51ed2.h>  //also includes 8052.h and 8051.h
//#include <mcs51reg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "code_lcd.h"

 
/* ==========================================
* lcd_init()
* ===========================================
* purpose: initializing lcd
* params: None
* ==========================================*/
void lcd_init()
{
	
	delay(50); // To give a delay of more than 15ms (72ms)
	
	
	value = LCD_CMD;	// unlock command
	*value = 0x30;
	
	delay(50);	//	To give a delay of more than 4.1ms (72ms)
	value = LCD_CMD;	// unlock command
	*value = 0x30;
	
	delay(50);			//	To give a delay of more than 100microSec (72ms)
	value = LCD_CMD;	// unlock command
	*value = 0x30;
	
	lcd_busy_wait();	// poll for busy flag
	value = LCD_CMD;	// Function set command
	*value = 0x38;
		
	lcd_busy_wait();	// poll for busy flag
	value = LCD_CMD;	// Turn the display OFF
	*value = 0x08;
		
	lcd_busy_wait();	// poll for busy flag
	value = LCD_CMD;	// Turn the display ON
	*value = 0x0C;
		
	lcd_busy_wait();		// poll for busy flag
	value = LCD_CMD;	// Entry mode set command
	*value = 0x06;
		
	lcd_busy_wait();	// poll for busy flag
	value = LCD_CMD;	// Clear the screen and send the cuRSLCDor home
	*value = 0x01;
	
	lcd_busy_wait();	// poll for busy flag
	value = LCD_CMD;	// Blink the cursor at home position
	*value = 0x0F;
		
	printf_tiny("-----LCD is initialized-----\n\r");
}

#if 0
/* ==========================================
* lcd_busy_wait()
* ===========================================
* purpose: polls for Busy Flag
* params: None
* ==========================================*/
void lcd_busy_wait()
{
	value = LCD_CMD_RD;
	*value = lcdDataOut;
	
	while((*value & BUSY_BYTE) == BUSY_BYTE);
}


/* ==========================================
* lcd_get_gotoAddr()
* ===========================================
* purpose: Set the cursor to the specified LCD DDRAM address
* params: None
* ==========================================*/
void lcd_get_gotoAddr()
{
	unsigned char addr[3];
	unsigned int addrInt=0;
	
	while(1)
	{
		printf_tiny("Enter the address to go to\n\r"
		"Address for the LCD lines:\n\r"
		"00.......0F\n\r"
		"40.......4F\n\r"
		"10.......1F\n\r"
		"50.......5F\n\r");

		/* accept address for jumping*/
		addr[0] = getchar();
		putchar(addr[0]);
		addr[1] = getchar();
		putchar(addr[1]);
		
		addrInt = charToInt(addr);		/*Convert ASCII to int*/

		if((addrInt < 0x60) && ((addrInt < 0x20) || (addrInt > 0x3F)))
		{
			lcdgotoaddr(addrInt);
			break;
		} 
		else
			printf_tiny("Address entered is OUT OF RANGE!\n\r"); 
	}
}


/* ==========================================
* lcdgotoaddr()
* ===========================================
* purpose: Set the cursor to the specified LCD DDRAM address
* params: char - unsigned char 
* ==========================================*/
/*void lcdgotoaddr(unsigned char addr)
{
	addr = addr | BUSY_BYTE;	//set bit 7
	lcd_busy_wait();
	value = LCD_CMD;
	*value = addr;  			//set DDRAM Address
	lcd_busy_wait();
}*/


/* ==========================================
* lcdgotoxy()
* ===========================================
* purpose: Sets the cursor to the position specified by the user
* params: row - unsigned char, column - unsigned char
* function calls - lcdgotoaddr(), lcd_busy_wait 
* ==========================================*/
void lcdgotoxy(unsigned char row, unsigned char column)
{
	unsigned char addr = 0;
	
	//row -= 48;
	
	if(row > 3) {
		printf_tiny("\n\rINVALID ROW\n\r");
		return;
	}
	if(column > 15) {
		printf_tiny("\n\rINVALID COLUMN\n\r");
		return;
	}
	switch(row)	/* Calculate the right column with the given row */
	{
		case 0:
			addr = column;					//set the address  to the 1st row
			break;
		case 1:
			addr = column + 0x40;			//set the address  to the 2nd row
			break;
		case 2:
			addr = column + 0x10;			//set the address  to the 3rd row
			break;
		case 3:
			addr = column + 0x50;			//set the address  to the 4th row
			break;
	}
	lcd_busy_wait();
	lcdgotoaddr(addr);
}


/* ==========================================
* lcdputch()
* ===========================================
* purpose: Writes the specified character to the current address on LCD
* params: cc - char
* function calls - lcdgotoaddr(), lcd_busy_wait()
* ==========================================*/
void lcdputch(char cc) 
{
	unsigned char *cursor = 0;

	lcd_busy_wait();

	cursor = LCD_CMD_RD;
	*cursor = lcdDataOut & 0x7F;	//read the address counter(DB0 to DB6)
	
	lcd_busy_wait();
	value = WR_LCD_INSTR;
	*value = cc;		//writes the character in the cursor position
	
	// Update cursor location
	if(*cursor == 0x0F)					//Move cursor to next row
		lcdgotoaddr(0x40);
	else if(*cursor == 0x4F)				//Move cursor to next row
		lcdgotoaddr(0x10);
	else if(*cursor == 0x1F)				//Move cursor to next row
		lcdgotoaddr(0x50);
	else if(*cursor == 0x58)				//Move cursor to next row
		lcdgotoaddr(0x00);
}

/* ==========================================
* lcdputstr()
* ===========================================
* purpose: Writes the specified null-terminated string to the LCD starting at the current LCD cursor position. 
* Automatically wraps long strings to the next LCD line after the right edge of the display screen has been reached. Must call lcdputch().
* params: cc - char
* function calls - lcdgotoaddr(), lcd_busy_wait()
* ==========================================*/
void lcdputstr(char *ss)
{
	unsigned char cursor =0;

	while(*ss)
	{
		lcd_busy_wait();
		cursor=*LCD_CMD_RD & 0x7F;

		lcd_busy_wait();
		lcdputch(*(ss++));					//prints the current character on the LCD

		// Update cursor location
		if(cursor == 0x0F)					//Move cursor to next row
			lcdgotoaddr(0x40);
		else if(cursor == 0x4F)				//Move cursor to next row
			lcdgotoaddr(0x10);
		else if(cursor == 0x1F)				//Move cursor to next row
			lcdgotoaddr(0x50);
		else if(cursor == 0x58)		//Move cursor to next row
			lcdgotoaddr(0x00);
	}
}


/* ==========================================
* lcdcreatechar()
* ===========================================
* purpose: Creates a custom character and writes it in the currnt cursor position
* params: ccode - unsigned char, row_vals - unsigned char
* function calls - lcdgotoaddr(), lcd_busy_wait()
* ==========================================*/
void lcdcreatechar(unsigned char ccode,unsigned char row_vals[])
{
	unsigned char cgAddr;
	unsigned int ccodeInt=0;
	unsigned int outval = 0;
	
	unsigned int i;
	
	/* Convert ccode to int value*/
	outval = ccode - 48;
	ccodeInt = ccodeInt << 4;
	ccodeInt = ccodeInt | outval;
	
	ccodeInt = ccodeInt << 3;
	
	TR0 = 0;
	
	for(i=0;i<8;i++)
	{
		cgAddr = ccodeInt | i;
		cgAddr = cgAddr | CG_ADDR_MASK;
				
		lcd_busy_wait();	/* Setting the CGRAM addr */
		*LCD_CMD = cgAddr;
		
		
		lcd_busy_wait();
		*WR_LCD_INSTR = row_vals[i];		/* Writing row values to the particular row in CGRAM */
		lcd_busy_wait();
	}
	
	switch(ccode)		/* To print the custom characters in their respective DDRAM addr: Print flower */
	{
		case '0': 
		lcdgotoaddr(0x00);lcd_busy_wait();
		lcdputch(0); break;
		
		case '1': 
		lcdgotoaddr(0x01);lcd_busy_wait();
		lcdputch(1); break;
		
		case '2': 
		lcdgotoaddr(0x40);lcd_busy_wait();
		lcdputch(2); break;
		
		case '3': 
		lcdgotoaddr(0x41);lcd_busy_wait();
		lcdputch(3); break;
		
		case '4': 
		lcdgotoaddr(0x10);lcd_busy_wait();
		lcdputch(4); break;
		
		case '5': 
		lcdgotoaddr(0x11);lcd_busy_wait();
		lcdputch(5); break;
		
	}
	TR0 = 1;
}

/* ==========================================
* putstr()
* ===========================================
* purpose: To ouput characters of a string to terminal emulator
* params: s - Character string
* ==========================================*/
int putstr (char *s)
{
	int i = 0;
	while (*s != '\0')
	{	/*output characters until NULL found*/
		putchar(*s++);
		i++;
	}
	
	return i+1;
}


/* ==========================================
* putchar()
* ===========================================
* purpose: To ouput a character to terminal emulator
* params: c - Character
* ==========================================*/
void putchar (char c) __critical
{
	while (!TI);	/*wait for TX ready, spin on TI*/
	SBUF = c;  		/*load serial port with transmit value&*/
	TI = 0;  		/*clear TI flag*/
}

/* ==========================================
* getchar()
* ===========================================
* purpose: To ouput a character to terminal emulator
* params: None
* ==========================================*/
char getchar ()
{
	while (!RI);    /*wait for character to be received, spin on RI*/
   	RI = 0;			/*clear RI flag*/
	return SBUF;  	/*return character from SBUF*/
}


/* ==========================================
* delay()
* ===========================================
* purpose: Introduce delay in the program
* params: time - unsigned int
* ==========================================*/
void delay(unsigned int time)
{
	int i,j;
	
	for(i=0;i<time;i++)
		for(j=0;j<time;j++);
}


/* ==========================================
* charToInt()
* ===========================================
* purpose: Convert ASCII to hexadecimal
* params: in_addr - unsigned char
* ==========================================*/
unsigned int charToInt(unsigned char *in_addr)
{
	unsigned int i=0,outval=0, hexval = 0;
	
	while(in_addr[i] != 0x00)
	{
		if((in_addr[i] >= '0') && (in_addr[i] <= '9'))
		{
			outval = in_addr[i] - 48;
		}
		else if((in_addr[i] >= 'A') && (in_addr[i] <= 'F'))
		{
			outval = in_addr[i] - 55;
		}
		else if((in_addr[i] >= 'a') && (in_addr[i] <= 'f'))
		{
			outval = in_addr[i] - 87;
		}
		else
		{
			printf("Invalid input!\n\r");
			return(-1);
		}
		hexval = hexval << 4;
		hexval = hexval | outval;
		i++;
	}

	return(hexval);
}
#endif //0




