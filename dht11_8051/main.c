#include <reg51.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD16x2_4bit.h"

sbit DHT11 = P3^7;
int x, i, j = 0;
int n_Hum, d_Hum, n_Tem, d_Tem, sum;
// 31.4  <=> n=31, d=4

void delay_ms(t){
		for (i = 0; i < t; i++){
				for(j = 0; j < 115 ; j++){
						;
				};
		};
}

void delay_30us(){
		TMOD = 0x01;		
		TH0 = 0xFF;			
		TL0 = 0xF1;			
		TR0 = 1;				
		while(TF0 == 0);
		TR0 = 0;				
    TF0 = 0;		
}	

void Req(){
	DHT11 = 0;		 		
	delay_ms(20);
	DHT11 = 1;				
}

void Res(){
	while(DHT11==1);
	while(DHT11==0);
	while(DHT11==1);
}

int Recv_data()	{
	int q,c=0;	
	for (q=0; q<8; q++){
		while(DHT11==0);		
		delay_30us();
		if(DHT11 == 1)			
				c = (c<<1)|(0x01);	
		else								
				c = (c<<1);
		while(DHT11==1);
	}
	return c;
}

void main()
{
	unsigned char data_log[20];
	LCD_Init();						
	
while(1)
	{		
		Req();						
		Res();						
		
		n_Hum = Recv_data();		/* store first eight bit in I_RH */		
		d_Hum = Recv_data();		/* store next eight bit in D_RH */	
		n_Tem = Recv_data();	/* store next eight bit in I_Temp */
		d_Tem = Recv_data();	/* store next eight bit in D_Temp */
		sum = Recv_data();/* store next eight bit in CheckSum */
	
		if ((n_Hum + d_Hum + n_Tem + d_Tem) != sum)
		{
			LCD_String_xy(0,0,"Error");
		}
		
		else
		{
			sprintf(data_log,"Hum : %d.%d",n_Hum ,d_Hum);
			LCD_String_xy(0,0,data_log);
			LCD_Char(0xFE);
			LCD_String("%");
			sprintf(data_log,"ok!");
			LCD_String_xy(0,13,data_log);
			sprintf(data_log,"Tem : %d.%d",n_Tem,d_Tem);   
			LCD_String_xy(1,0,data_log);
			LCD_Char(0xDF);
			LCD_String("C");
			memset(data_log,0,20);
			if( x == 4 )  x = 0;
			if( x == 0 ) sprintf(data_log,".");
			if( x == 1 ) sprintf(data_log,"..");
			if( x == 2 ) sprintf(data_log,"...");
			if( x == 3 ) {
				sprintf(data_log,"   ");
			}
			LCD_String_xy(1,13,data_log);
			x++;
		}		
		delay(1000);
	}	
}


