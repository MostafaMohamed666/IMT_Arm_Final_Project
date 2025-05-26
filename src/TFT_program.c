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
	TFT_Write_Command(0x21);

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
	TFT_Write_Command(TFT_CASET);	// column adress

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

void TFT_DrawProgressBar(u8_t x, u8_t y, u8_t w, u8_t h, u8_t progress, u16_t fillColor, u16_t bgColor)
{
    if (progress > 100) progress = 100;

    // Calculate width of the filled part
    u16_t filledWidth = (w * progress) / 100;

    // Draw filled part
    TFT_FillRectangle(x, y, filledWidth, h, fillColor);

    // Draw remaining part (unfilled background)
    if (filledWidth < w) {
        TFT_FillRectangle(x + filledWidth, y, w - filledWidth, h, bgColor);
    }
}

void TFT_DrawProgressBar(u8_t x, u8_t y, u8_t w, u8_t h, u8_t progress, u16_t fillColor, u16_t bgColor)
{
    if (progress > 100) progress = 100;

    // Calculate width of the filled part
    u16_t filledWidth = (w * progress) / 100;

    // Draw filled part
    TFT_FillRectangle(x, y, filledWidth, h, fillColor);

    // Draw remaining part (unfilled background)
    if (filledWidth < w) {
        TFT_FillRectangle(x + filledWidth, y, w - filledWidth, h, bgColor);
    }
}

void TFT_DrawVerticalBar(u8_t x, u8_t y_bottom, u8_t barWidth, u8_t levelHeight, u8_t gap, u8_t level, u8_t maxLevel)
{
    if (level > maxLevel) level = maxLevel;

    for (u8_t i = 0; i < maxLevel; i++) {
        u8_t barY = y_bottom - (i + 1) * (levelHeight + gap);
        u16_t color;

        if (i < level) {
            if (i < maxLevel * 0.6)
                color = 0x07E0; // Green
            else if (i < maxLevel * 0.85)
                color = 0xFFE0; // Yellow
            else
                color = 0xF800; // Red
        } else {
            color = 0x0000; // Off
        }

        TFT_FillRectangle(x, barY, barWidth, levelHeight, color);
    }
}

void TFT_DrawVU_Meter_Array(u8_t *levels, u8_t barCount)
{
    const u8_t maxLevel = 10;
    const u8_t levelHeight = 8;
    const u8_t gap = 2;
    const u8_t barWidth = 6;
    const u8_t spacing = 4;
    const u8_t y_bottom = 127;
    for (u8_t i = 0; i < barCount; i++) {
        u8_t x = 10 + i * (barWidth + spacing); // left offset + bar spacing
        TFT_DrawVerticalBar(x, y_bottom, barWidth, levelHeight, gap, levels[i], maxLevel);
    }
}

void TFT_DrawPlayButton(u8_t x, u8_t y, u8_t size, u16_t color)
{
    for (u8_t i = 0; i < size; i++) {
        for (u8_t j = 0; j <= i; j++) {
            TFT_DrawPixel(x + i, y - i/2 + j, color);
        }
    }
}

void TFT_DrawPauseButton(u8_t x, u8_t y, u8_t width, u8_t height, u16_t color)
{
    for (u8_t i = 0; i < height; i++) {
        for (u8_t j = 0; j < width; j++) {
            TFT_DrawPixel(x + j, y + i, color); // Left bar
            TFT_DrawPixel(x + j + width + 2, y + i, color); // Right bar (gap = 2px)
        }
    }
}

void TFT_DrawNextButton(u8_t x, u8_t y, u8_t size, u16_t color)
{
    for (u8_t i = 0; i < size; i++) {
        for (u8_t j = 0; j <= i; j++) {
            TFT_DrawPixel(x + i, y - i/2 + j, color);             // First triangle
            TFT_DrawPixel(x + i + size + 1, y - i/2 + j, color);  // Second triangle
        }
    }

    for (u8_t i = 0; i < size; i++) {
        TFT_DrawPixel(x + 2 * size + 3, y - size/2 + i, color); // Vertical bar
    }
}

void TFT_DrawPrevButton(u8_t x, u8_t y, u8_t size, u16_t color)
{
    for (u8_t i = 0; i < size; i++) {
        for (u8_t j = 0; j <= i; j++) {
            TFT_DrawPixel(x - i, y - i/2 + j, color);             // Right triangle
            TFT_DrawPixel(x - i - size - 1, y - i/2 + j, color);  // Left triangle
        }
    }

    for (u8_t i = 0; i < size; i++) {
        TFT_DrawPixel(x - 2 * size - 3, y - size/2 + i, color); // Vertical bar
    }
}


