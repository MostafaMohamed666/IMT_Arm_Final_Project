/*
 * TFT_program.c
 *
 *  Created on: Apr 12, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "SYSTICK_interface.h"
#include "SPI_interface.h"
#include "TFT_interface.h"
#include "TFT_config.h"
#include "TFT_private.h"
#include "fonts.h"
#include <stdio.h>
#include <stdlib.h>


void TFT_init()
{
	GPIO_Set_Mode(RST_PORT, RST_PIN, OUTPUT);

	GPIO_Set_Mode(A0_PORT, A0_PIN, OUTPUT);

	/*__________RESET SEQUENCE OF TFT DISPLAY_________*/
	GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, HIGH);
	delay_us(100);
	GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, LOW);
	delay_us(1);
	GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, HIGH);
	delay_us(100);
	GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, LOW);
	delay_us(100);
	GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, HIGH);
	delay_us(120);

	//SLEEP OUT MODE within sequence
	TFT_Write_Command(0x11);
	delay_ms(10);

	//COLOR MODE within sequence
	TFT_Write_Command(0x3A);
	TFT_Write_Data(0x05); //RGB565 color code standard

	// Inverse Color mode ON
	//	TFT_Write_Command(0x21);

	// Inverse Color mode ON
	//	TFT_InvertColors(TFT_ACTIVE);

	// Row and Column exchange MY=1 , MX=1, MV=0 ( start from top left )
	TFT_Write_Command(0x36);
	TFT_Write_Data(0b11010110);		// MV = 0

	// Select BGR color filter
	//    TFT_Write_Command(TFT_MADCTL);
	//    TFT_Write_Data(0x08);

	//DISPLAY on
	TFT_Write_Command(0x29);
}
void TFT_Write_Data(u8_t data)
{
	GPIO_Set_Atomic_Pin_Value(A0_PORT, A0_PIN, HIGH);
	SPI_Transmit(data);
}
void TFT_Write_Command(u8_t cmd)
{
	GPIO_Set_Atomic_Pin_Value(A0_PORT, A0_PIN, LOW);
	SPI_Transmit(cmd);
}
void TFT_ImgDisplay(const u16_t *imgArr)
{
	u16_t i = 0;
	u8_t HIGH, LOW;

	//SET X coordinate
	TFT_Write_Command(0x2A);
	// start at -> 0,0
	TFT_Write_Data(0);
	TFT_Write_Data(0);
	// end at -> 127
	TFT_Write_Data(0);
	TFT_Write_Data(127);

	//SET Y coordinate
	TFT_Write_Command(0x2B);
	// start at -> 0,0
	TFT_Write_Data(0);
	TFT_Write_Data(0);
	// end at -> 127
	TFT_Write_Data(0);
	TFT_Write_Data(159);

	// WRITE on screen
	TFT_Write_Command(0x2C);
	for(i=0; i<20480; i++){
		HIGH = (u8_t)(imgArr[i]>>8);
		LOW = (u8_t)(imgArr[i]);
		TFT_Write_Data(HIGH);
		TFT_Write_Data(LOW);
	}
}

void TFT_SWReset(void)
{
	TFT_Write_Command(TFT_SWRESET);
	delay_ms(150);
}

void TFT_SetDrawingArea(u8_t x0, u8_t y0, u8_t x1, u8_t y1)
{
	TFT_Write_Command(TFT_CASET);	// column address

	// Start at x0
	TFT_Write_Data(0);
	TFT_Write_Data(x0);

	//End at x1
	TFT_Write_Data(0);
	TFT_Write_Data(x1);

	TFT_Write_Command(TFT_RASET);	// Row Adress Command
	// Start at y0
	TFT_Write_Data(0);
	TFT_Write_Data(y0);

	// End at y1
	TFT_Write_Data(0);
	TFT_Write_Data(y1);

	TFT_Write_Command(TFT_RAMWR);
}

void TFT_DrawPixel(u8_t x, u8_t y, u16_t color)
{
	if((x >= TFT_WIDTH) || (y >= TFT_HEIGHT))
	{
		return;
	}

	u8_t HIGH, LOW;

	TFT_SetDrawingArea(x, y, x+1, y+1);

	HIGH = color>>8;
	LOW = color;

	TFT_Write_Data(HIGH);
	TFT_Write_Data(LOW & 0xFF);

}

void TFT_WriteChar(u8_t x, u8_t y, char ch, FontDef font, u16_t color, u16_t bgcolor)
{
	u8_t HIGH, LOW;
	u32_t i, b, j;

	TFT_SetDrawingArea(x, y, x+font.width-1, y+font.height-1);
	for(i = 0; i < font.height; i++)
	{
		b = font.data[(ch - 32) * font.height + i];
		for(j = 0; j < font.width; j++)
		{
			if((b << j) & 0x8000)
			{
				HIGH = color>>8;
				LOW = color;

				TFT_Write_Data(HIGH);
				TFT_Write_Data(LOW & 0xFF);

			}
			else
			{
				HIGH = bgcolor>>8;
				LOW = bgcolor;

				TFT_Write_Data(HIGH);
				TFT_Write_Data(LOW & 0xFF);
			}
		}
	}
}

void TFT_FillDisplay(const u16_t color)
{
	u16_t i = 0;
	u8_t HIGH, LOW;

	//SET X coordinate
	TFT_Write_Command(0x2A);
	// start at -> 0,0
	TFT_Write_Data(0);
	TFT_Write_Data(0);
	// end at -> 127
	TFT_Write_Data(0);
	TFT_Write_Data(127);

	//SET Y coordinate
	TFT_Write_Command(0x2B);
	// start at -> 0,0
	TFT_Write_Data(0);
	TFT_Write_Data(0);
	// end at -> 127
	TFT_Write_Data(0);
	TFT_Write_Data(159);

	// WRITE on screen
	TFT_Write_Command(0x2C);
	for(i=0; i<20480; i++){
		HIGH = color>>8;
		LOW = color;
		TFT_Write_Data(HIGH);
		TFT_Write_Data(LOW);
	}
}

void TFT_WriteString(u8_t x, u8_t y, const char* str, FontDef font, u16_t color, u16_t bgcolor)
{
	while(*str != '\0')
	{
		if(x + font.width >= TFT_WIDTH)
		{
			x = 0;
			y += font.height;

			if(y + font.height >= TFT_HEIGHT)
			{
				break;
			}

			if(*str == ' ')
			{
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}

		}

		TFT_WriteChar(x, y, *str, font, color, bgcolor);
		x += font.width;
		str++;
	}
}
void TFT_FillRectangle(u8_t x, u8_t y, u8_t w, u8_t h, u16_t color)
{
	u8_t HIGH = color >> 8;
	u8_t LOW = color & 0xFF;

	if ((x >= TFT_WIDTH) || (y >= TFT_HEIGHT))
		return;

	if ((x + w - 1) >= TFT_WIDTH)
		w = TFT_WIDTH - x;

	if ((y + h - 1) >= TFT_HEIGHT)
		h = TFT_HEIGHT - y;

	TFT_SetDrawingArea(x, y, x + w - 1, y + h - 1);

	for (u16_t i = 0; i < h; i++)
	{
		for (u16_t j = 0; j < w; j++)
		{
			TFT_Write_Data(HIGH);
			TFT_Write_Data(LOW);
		}
	}
}

void TFT_Updating_Page(u8_t Num)
{
	TFT_FillDisplay(TFT_BLACK);
	switch (Num) {
	case 0:
		TFT_WriteString(2,80,"Updated    successfuly", Font_11x18 ,TFT_WHITE,TFT_BLACK);
		break;
	case 1:
		TFT_WriteString(2,80,"Updating...", Font_11x18 ,TFT_WHITE,TFT_BLACK);
		break;
	case 2:
		TFT_WriteString(2,80,"Error while      receiving", Font_7x10 ,TFT_WHITE,TFT_BLACK);
		break;
	case 3:
		TFT_WriteString(2,80,"Faild to connect to the server", Font_7x10 ,TFT_WHITE,TFT_BLACK);
		break;
	default:
		break;
	}

}


