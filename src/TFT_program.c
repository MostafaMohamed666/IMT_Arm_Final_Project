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
#include "TFT_WifiIcon.h"

u8_t G_Brightness = 0;
u8_t G_Gamma = 0;
u8_t G_IsInverted = 0;
u8_t G_Volume = 0;
u8_t G_SongProgress = 0;
u8_t G_EQMode = 0;
u8_t G_CurrentPage = 0;
u8_t G_Selection = 0;
u8_t G_CurrentSong = 0;
u8_t G_PP_Button = TFT_PLAY;
u8_t G_Connected = 0;

const char* EQMode[] = {
		"NORMAL",
		"POP",
		"ROCK",
		"JAZZ",
		"CLASSIC",
		"BASS"
};

const char* Songs[] = {
		"song1",
		"song2",
		"song3",
		"song4",
		"song5"
};


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
			if (i < maxLevel * 0.5)
				color = TFT_GREEN; // Green
			else if (i < maxLevel * 0.8)
				color = TFT_YELLOW; // Yellow
			else
				color = TFT_RED; // Red
		} else {
			color = TFT_BLACK; // Off
		}

		TFT_FillRectangle(x, barY, barWidth, levelHeight, color);
	}
}

void TFT_DrawVU_Meter_Array(u8_t barCount)
{
	const u8_t maxLevel = 10;
	const u8_t levelHeight = 4;
	const u8_t gap = 0;
	const u8_t barWidth = 4;
	const u8_t spacing = 2;
	const u8_t y_bottom = 70;
	for (u8_t i = 0; i < barCount; i++) {
		u8_t x = 35 + i * (barWidth + spacing); // left offset + bar spacing
		TFT_DrawVerticalBar(x, y_bottom, barWidth, levelHeight, gap, rand() % 10, maxLevel);
		delay_ms(30);
	}
}

void TFT_DrawPlayButton(u8_t x, u8_t y, u8_t size, u16_t color)
{
	for (u8_t i = 0; i < size; i++) {
		for (u8_t j = 0; j <= i; j++) {
			TFT_DrawPixel(x + size - 1 - i, y - i / 2 + j, color);
		}
	}
}

void TFT_DrawPauseButton(u8_t x, u8_t y, u8_t width, u8_t height, u16_t color)
{
	for (u8_t i = 0; i < height; i++) {
		for (u8_t j = 0; j < width; j++) {
			TFT_DrawPixel(x + j, y - height/2 + i, color);              // Left bar
			TFT_DrawPixel(x + j + width + 2, y - height/2 + i, color);  // Right bar
		}
	}
}


void TFT_DrawPrevButton(u8_t x, u8_t y, u8_t size, u16_t color)
{
	for (u8_t i = 0; i < size; i++) {
		for (u8_t j = 0; j <= i; j++) {
			TFT_DrawPixel(x + i, y - i/2 + j, color);             // First triangle
			TFT_DrawPixel(x + i + size + 1, y - i/2 + j, color);  // Second triangle
		}
	}

	//	for (u8_t i = 0; i < size; i++) {
	//		TFT_DrawPixel(x + 2 * size + 3, y - size/2 + i, color); // Vertical bar
	//	}
}

void TFT_DrawNextButton(u8_t x, u8_t y, u8_t size, u16_t color)
{
	for (u8_t i = 0; i < size; i++) {
		for (u8_t j = 0; j <= i; j++) {
			TFT_DrawPixel(x - i, y - i/2 + j, color);             // Right triangle
			TFT_DrawPixel(x - i - size - 1, y - i/2 + j, color);  // Left triangle
		}
	}

	//	for (u8_t i = 0; i < size; i++) {
	//		TFT_DrawPixel(x - 2 * size - 3, y - size/2 + i, color); // Vertical bar
	//	}
}

void TFT_DrawPage(u8_t PageNumber)
{
	TFT_FillDisplay(TFT_BLACK);

	switch (PageNumber) {
	case 1:
		G_CurrentPage = 1;
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);
		TFT_WriteString(14, 5,"Main Menu", Font_11x18, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(55, 150,"1/4", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);

		break;
	case 2:
		G_CurrentPage = 2;
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);
		TFT_WriteString(35, 5,"Player", Font_11x18, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(20, 80," Song Name ", Font_7x10, TFT_MAGENTA, TFT_BLACK);
		TFT_DrawPrevButton(25, 100, 10, TFT_WHITE);
		TFT_DrawPlayButton(55, 100, 10, TFT_WHITE);
		TFT_DrawPauseButton(70, 100, 3, 10, TFT_WHITE);
		TFT_DrawNextButton(105, 100, 10, TFT_WHITE);
		TFT_DrawProgressBar(14, 115,100, 5, 0,TFT_WHITE,TFT_GRAY);
		TFT_DrawSoundLevel(60,130,G_Volume/3, TFT_WHITE, TFT_GRAY);
		TFT_WriteString(5,135,"Volume", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(55, 150,"2/4", Font_7x10, TFT_WHITE, TFT_BLACK);

		//		while(PageNumber == 2)
		//		{
		TFT_DrawVU_Meter_Array(10);
		//		}

		break;
	case 3:
		G_CurrentPage = 3;
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);
		TFT_WriteString(25, 5,"Updates", Font_11x18, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(55, 150,"3/4", Font_7x10, TFT_WHITE, TFT_BLACK);

		break;
	case 4:
		G_CurrentPage = 4;
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);
		TFT_WriteString(25, 5,"Settings", Font_11x18, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(5, 37, "2.Gamma:", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(80, 73, "-", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(120, 73, "+", Font_7x10, TFT_WHITE, TFT_BLACK);
		TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

		TFT_WriteString(55, 150,"4/4", Font_7x10, TFT_WHITE, TFT_BLACK);

		char str[10];

		snprintf(str, sizeof(str), "%u", G_Brightness);
		TFT_WriteString(100, 25, str, Font_7x10, TFT_YELLOW, TFT_BLACK);

		snprintf(str, sizeof(str), "%u", G_Gamma);
		TFT_WriteString(100, 37, str, Font_7x10, TFT_YELLOW, TFT_BLACK);

		TFT_WriteString(100, 49, G_IsInverted ? "ON " : "OFF", Font_7x10, TFT_YELLOW, TFT_BLACK);

		TFT_WriteString(75, 61, EQMode[G_EQMode], Font_7x10, TFT_YELLOW, TFT_BLACK);

		snprintf(str, sizeof(str), "%u", G_Volume);
		TFT_WriteString(100, 73, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
		
		TFT_WriteString(100, 85, G_Connected ? "ON " : "OFF", Font_7x10, TFT_YELLOW, TFT_BLACK);


		break;
	default:
		break;
	}
}

void TFT_IconDisplay(const u16_t *imgArr, u8_t x_start, u8_t y_start, u8_t x_end, u8_t y_end, u8_t active)
{
	u16_t i = 0;
	u8_t HIGH, LOW;
	u16_t IconSize = (x_end - x_start) * (y_end * y_start);
	//SET X coordinate
	TFT_Write_Command(0x2A);
	// start at -> x_start
	TFT_Write_Data(0);
	TFT_Write_Data(x_start);
	// end at -> x_end
	TFT_Write_Data(0);
	TFT_Write_Data(x_end);

	//SET Y coordinate
	TFT_Write_Command(0x2B);
	// start at -> y_start
	TFT_Write_Data(0);
	TFT_Write_Data(y_start);
	// end at -> y_end
	TFT_Write_Data(0);
	TFT_Write_Data(y_end);

	// WRITE on screen
	TFT_Write_Command(0x2C);
	for(i=0; i<IconSize; i++){
		HIGH = (u8_t)(imgArr[i]>>8);
		LOW = (u8_t)(imgArr[i]);
		TFT_Write_Data(HIGH);
		TFT_Write_Data(LOW);
	}
	if(active == 0)
	{
		for(u8_t j = 0; j < x_end-x_start; j++)
		{
			TFT_DrawPixel(x_start+j,y_start+j, TFT_RED);
			TFT_DrawPixel(x_end-j,y_start+j, TFT_RED);
		}
	}
}

void TFT_DrawSoundLevel(u8_t x, u8_t y, u8_t level, u16_t activeColor, u16_t inactiveColor)
{
	if (level > 10) level = 10;

	u8_t barWidth = 3;
	u8_t spacing = 2;
	u8_t barHeightStep = 1; // Height increases per level

	for (u8_t i = 0; i < 10; i++)
	{
		u8_t barHeight = (i + 1) * barHeightStep;
		u8_t barX = x + i * (barWidth + spacing);
		u8_t barY = y + (10 * barHeightStep - barHeight); // align bottom

		u16_t color = (i < level) ? activeColor : inactiveColor;

		TFT_FillRectangle(barX, barY, barWidth, barHeight, color);
	}
}

void TFT_PlayPauseButtonActive(u8_t Button)
{
	if(Button == TFT_PLAY)
	{
		TFT_DrawPauseButton(70, 100, 3, 10, TFT_WHITE);
		TFT_DrawPlayButton(55, 100, 10, TFT_GREEN);
	}
	if(Button == TFT_PAUSE)
	{
		TFT_DrawPlayButton(55, 100, 10, TFT_WHITE);
		TFT_DrawPauseButton(70, 100, 3, 10, TFT_YELLOW);
	}
}

void TFT_UpdateSongProgress(u8_t progress)
{
	TFT_DrawProgressBar(14, 115,100, 5, progress,TFT_WHITE,TFT_GRAY);

}
void TFT_UpdateVolumeLevel(u8_t level)
{

	TFT_DrawSoundLevel(60,130,level, TFT_WHITE, TFT_GRAY);
}

void TFT_ClickNext(void)
{
	TFT_DrawNextButton(105, 100, 10, TFT_RED);
	delay_ms(200);
	TFT_DrawNextButton(105, 100, 10, TFT_WHITE);

}

void TFT_ClickPrev(void)
{
	TFT_DrawPrevButton(25, 100, 10, TFT_RED);
	delay_ms(200);
	TFT_DrawPrevButton(25, 100, 10, TFT_WHITE);
}

void TFT_InvertColors(u8_t active)
{
	if(active == 1)
	{
		TFT_Write_Command(TFT_INVON);
		G_IsInverted = 1;
	}
	if(active == 0)
	{
		TFT_Write_Command(TFT_INVOFF);
		G_IsInverted = 0;
	}
}

void TFT_SetGammaCurve(TFT_Gama GCn)
{
	TFT_Write_Command(TFT_GAMSET);
	TFT_Write_Data(GCn);
}

void TFT_UpdateSongName (char* song_name)
{
	TFT_FillRectangle(0,80,TFT_WIDTH,10,TFT_BLACK);
	TFT_WriteString(20, 80,song_name, Font_7x10, TFT_MAGENTA, TFT_BLACK);
}

void TFT_UpdateUI(void)
{
	char str[10];

	switch (G_CurrentPage) {
	case 1:
		if(G_Selection == 1)
		{
			TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);
		}

		if(G_Selection == 2)
		{
			TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);
		}

		if(G_Selection == 3)
		{
			TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BBLUE);
		}
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);
		break;
	case 2:
		TFT_UpdateSongProgress(G_SongProgress);
		TFT_UpdateVolumeLevel(G_Volume/3);
		TFT_UpdateSongName(Songs[G_CurrentSong]);
		TFT_PlayPauseButtonActive(G_PP_Button);
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);

		break;
	case 3:
		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);

		break;
	case 4:
		if(G_Selection == 1)
		{
			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

			snprintf(str, sizeof(str), "%u", G_Brightness);
			TFT_WriteString(100, 25, str, Font_7x10, TFT_YELLOW, TFT_BLACK);

		}
		if(G_Selection == 2)
		{
			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

			snprintf(str, sizeof(str), "%u", G_Gamma);
			TFT_WriteString(100, 37, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
		}
		if(G_Selection == 3)
		{
			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

			TFT_WriteString(100, 49, G_IsInverted ? "ON " : "OFF", Font_7x10, TFT_YELLOW, TFT_BLACK);

		}
		if(G_Selection == 4)
		{
			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

			TFT_WriteString(75, 61, EQMode[G_EQMode], Font_7x10, TFT_YELLOW, TFT_BLACK);

		}
		if(G_Selection == 5)
		{
			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BBLUE);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

			snprintf(str, sizeof(str), "%u", G_Volume);
			TFT_WriteString(100, 73, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
		}
		if(G_Selection == 6)
		{
			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, TFT_WHITE, TFT_BLACK);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BBLUE);

			TFT_WriteString(100, 85, G_Connected ? "ON " : "OFF", Font_7x10, TFT_YELLOW, TFT_BLACK);

		}
		TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, TFT_WHITE, TFT_BLACK);

		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);

		break;
	default:
		break;
	}
}
