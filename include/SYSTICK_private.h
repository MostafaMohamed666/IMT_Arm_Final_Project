/*
 * SYSTICK_private.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef SYSTICK_PRIVATE_H_
#define SYSTICK_PRIVATE_H_

#define SYSTICK_BASE_ADDRESS (0xE000E010)

typedef struct
{
	u32_t CTRL;
	u32_t LOAD;
	u32_t VAL;
	u32_t CALIB;
}SYSTICK_t;

#define STK ((volatile SYSTICK_t*)SYSTICK_BASE_ADDRESS)

#define COUNT_FLAG 16
#define CLK_SOURCE 2
#define TICK_INT 1
#define STK_ENABLE 0

#define STK_AHB 1
#define STK_AHB_DIV_8 0

#endif /* SYSTICK_PRIVATE_H_ */
