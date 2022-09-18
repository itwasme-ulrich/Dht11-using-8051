/*
 * LCD16x2 4 bit 8051 interface
 * http://www.electronicwings.com
 */

#ifndef LCD16x2_4bit
#define LCD16x2_4bit

#include<reg51.h>

void delay(unsigned int count);
void LCD_Command (char cmnd);
void LCD_Char (char char_data);
void LCD_String (char *str);	
void LCD_String_xy (char row, char pos, char *str);
void LCD_Init (void);

#endif
