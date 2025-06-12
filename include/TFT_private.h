/*
 * TFT_private.h
 *
 *  Created on: May 20, 2025
 *      Author: Maged
 */

#ifndef TFT_PRIVATE_H_
#define TFT_PRIVATE_H_

// TFT Commands
#define TFT_NOP		 0x00
#define TFT_SWRESET  0x01	// Software Reset
#define TFT_SLPOUT   0x11
#define TFT_COLMOD   0x3A
#define TFT_MADCTL   0x36
#define TFT_CASET    0x2A	// set column adress (X)
#define TFT_RASET    0x2B	// set row adress (Y)
#define TFT_RAMWR    0x2C	// write to RAM
#define TFT_INVOFF   0x20
#define TFT_INVON    0x21
#define TFT_GAMSET   0x26
#define TFT_DISPON   0x29
#define TFT_DISPOFF  0x28
#define TFT_MADCTL	 0x36	// Memory Data Access Control

#define TFT_ACTIVE		1
#define TFT_NOTACTIVE	0






#endif /* TFT_PRIVATE_H_ */
