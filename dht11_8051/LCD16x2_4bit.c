/*  
   LCD16x2 4 bit 8051 interface
   http://www.electronicwings.com
*/


#include<reg51.h>
#include"LCD16x2_4bit.h"

sfr LCD_Port=0x90;				 			/* P1 port as data port */
sbit rs=P1^0;        					 	/* Register select pin */
sbit rw=P1^1;        						/* Read/Write pin */
sbit en=P1^2;        						/* Enable pin */

/* Function to provide delay Approx 1ms with 11.0592 Mhz crystal*/
void delay(unsigned int count)    		
{
	int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<112;j++);
}

void LCD_Command (char cmnd)   	/* LCD16x2 command funtion */
{
	LCD_Port =(LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	rs=0;	 										/* command reg. */
	rw=0;											/* Write operation */
	en=1; 
	delay(1);
	en=0;
	delay(2);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	en=1; 											/* enable pulse */
	delay(1);
	en=0;
	delay(5);
}

void LCD_Char (char char_data)	/* LCD data write function */
{
	LCD_Port =(LCD_Port & 0x0F) | (char_data & 0xF0); /* sending upper nibble */    
	rs=1;												/*Data reg.*/
	rw=0;												/*Write operation*/
	en=1;   				
	delay(1);
	en=0;
	delay(2);

	LCD_Port = (LCD_Port & 0x0F) | (char_data << 4);  /* sending lower nibble */
	en=1; 											/* enable pulse */
	delay(1);
	en=0;
	delay(5);
}

void LCD_String (char *str)			/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)				/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);					/* Call LCD data write */
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD function */
{
	if (row == 0)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);								/* Call LCD string function */
}

void LCD_Init (void)							/* LCD Initialize function */
{
	delay(20);					/* LCD Power ON Initialization time >15ms */
	
	LCD_Command (0x02);				/* 4bit mode */
	LCD_Command (0x28);				/* Initialization of 16X2 LCD in 4bit mode */
	LCD_Command (0x0C);				/* Display ON Cursor OFF */
	LCD_Command (0x06);				/* Auto Increment cursor */
	LCD_Command (0x01);				/* clear display */
	delay(3);
	LCD_Command (0x80);				/* cursor at home position */
}