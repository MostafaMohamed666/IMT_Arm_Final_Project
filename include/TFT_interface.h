/*
 * TFT_interface.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Mostafa Mohamed
 */
#ifndef TFT_INTERFACE_H_
#define TFT_INTERFACE_H_

#include "fonts.h"

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

// Color definitions
#define	TFT_BLACK   0x0000
#define	TFT_BLUE    0x001F
#define	TFT_RED     0xF800
#define	TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF


#endif /* TFT_INTERFACE_H_ */
