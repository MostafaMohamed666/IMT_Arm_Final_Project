/*
 * TFT_interface.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Mostafa Mohamed
 */
#ifndef TFT_INTERFACE_H_
#define TFT_INTERFACE_H_

#include "fonts.h"

typedef enum
{
	GC0 = 0x01,
	GC1 = 0x02,
	GC2 = 0x04,
	GC3 = 0x08
}TFT_Gama;


void TFT_init();
void TFT_Write_Data(u8_t data);
void TFT_Write_Command(u8_t cmd);
void TFT_ImgDisplay(const u16_t *imgArr);
void TFT_SWReset(void);
void TFT_SetDrawingArea(u8_t x0, u8_t y0, u8_t x1, u8_t y1);
void TFT_DrawPixel(u8_t x, u8_t y, u16_t color);
void TFT_WriteChar(u8_t x, u8_t y, char ch, FontDef font, u16_t color, u16_t bgcolor);
void TFT_FillDisplay(const u16_t color);
void TFT_WriteString(u8_t x, u8_t y, const char* str, FontDef font, u16_t color, u16_t bgcolor);
void TFT_FillRectangle(u8_t x, u8_t y, u8_t w, u8_t h, u16_t color);
void TFT_DrawProgressBar(u8_t x, u8_t y, u8_t w, u8_t h, u8_t progress, u16_t fillColor, u16_t bgColor);
void TFT_DrawVerticalBar(u8_t x, u8_t y_bottom, u8_t barWidth, u8_t levelHeight, u8_t gap, u8_t level, u8_t maxLevel);
void TFT_DrawVU_Meter_Array(u8_t barCount);
void TFT_DrawPlayButton(u8_t x, u8_t y, u8_t size, u16_t color);
void TFT_DrawPauseButton(u8_t x, u8_t y, u8_t width, u8_t height, u16_t color);
void TFT_DrawNextButton(u8_t x, u8_t y, u8_t size, u16_t color);
void TFT_DrawPrevButton(u8_t x, u8_t y, u8_t size, u16_t color);
void TFT_DrawPage(u8_t PageNumber);
void TFT_IconDisplay(const u16_t *imgArr, u8_t x_start, u8_t y_start, u8_t x_end, u8_t y_end, u8_t active);
void TFT_DrawSoundLevel(u8_t x, u8_t y, u8_t level, u16_t activeColor, u16_t inactiveColor);
void TFT_PlayPauseButtonActive(u8_t Button);
void TFT_UpdateSongProgress(u8_t progress);
void TFT_UpdateVolumeProgress(u8_t progress);
void TFT_ClickNext(void);
void TFT_ClickPrev(void);
void TFT_InvertColors(u8_t active);
void TFT_UpdateUI(void);
void TFT_SetGammaCurve(TFT_Gama GCn);
void TFT_UpdateSongName (char* song_name);


// Color definitions
#define	TFT_BLACK   0x0000
#define TFT_GRAY	0x8C51
#define	TFT_BLUE    0x001F
#define TFT_BBLUE	0x8D1F
#define	TFT_RED     0xF800
#define	TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF

#define TFT_PAUSE		0
#define TFT_PLAY		1


extern u8_t G_Brightness;
extern u8_t G_Gamma;
extern u8_t G_IsInverted;
extern u8_t G_Volume;
extern u8_t G_SongProgress;
extern u8_t G_EQMode;
extern u8_t G_CurrentPage;
extern u8_t G_Selection;
extern u8_t G_PP_Button;
extern u8_t G_Connected;

#endif /* TFT_INTERFACE_H_ */
