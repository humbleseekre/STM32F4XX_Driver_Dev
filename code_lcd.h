/* =====================================================================
* Karuna Raja Reddy
* ECEN 5613 - Lab #4
* Fall 2012, Prof. McClure
* Header File: LCD device driver functions
* University of Colorado at Boulder
* =====================================================================*/

#ifndef CODE_LCD_H_
#define CODE_LCD_H_

#endif /*CODE_LCD_H_*/

/* Macros =================================*/
sfr at 0x90 lcdDataOut;
#define LCD_CMD ((volatile xdata unsigned char*)0xF000)		/* RS = 0, R/W = 0*/
#define LCD_CMD_RD ((volatile xdata unsigned char*)0xF400)	/* RS = 0, R/W = 1*/
#define WR_LCD_INSTR ((volatile xdata unsigned char*)0xF800)	/* RS = 1, R/W = 0*/
#define RD_LCD_INSTR ((volatile xdata unsigned char*)0xFC00)	/* RS = 1, R/W = 1*/
#define BUSY_BYTE 0X80 
#define CG_ADDR_MASK 0X40

/* Global variable =========================*/
unsigned char* value;

/* Function Declarations ==============================*/
int putstr (char *s);
void putchar (char c);
char getchar ();

void lcd_init();
void lcd_busy_wait();
void delay(unsigned int);

void lcd_get_gotoAddr();
void lcdgotoaddr(unsigned char addr);
void lcdgotoxy(unsigned char row, unsigned char column);

void lcdputch(char cc);
void lcdputstr(char *ss);
void lcdcreatechar(unsigned char ccode,unsigned char row_vals[]);

unsigned int charToInt(unsigned char *ss);




