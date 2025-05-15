/*
 * TFT_interface.h
 *
 *  Created on: Apr 12, 2025
 *      Author: Mostafa Mohamed
 */
#ifndef TFT_INTERFACE_H_
#define TFT_INTERFACE_H_

void TFT_init();
void TFT_Write_Data(u8_t data);
void TFT_Write_Command(u8_t cmd);
void TFT_Display(const u16_t *imgArr);



#endif /* TFT_INTERFACE_H_ */
