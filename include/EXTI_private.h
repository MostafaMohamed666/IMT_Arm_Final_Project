/*
 * EXTI_private.h
 *
 *  Created on: Mar 19, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_

#define EXTI_BASE_ADDRESS   (0x40013C00)
#define SYSCFG_BASE_ADDRESS (0x40013800)

typedef struct
{
	u32_t IMR;
	u32_t EMR;
	u32_t RTSR;
	u32_t FTSR;
	u32_t SWIER;
	u32_t PR;
}EXTI_t;

typedef struct
{
	u32_t MEMRMP;
	u32_t PMC;
	u32_t EXTICR[4];
	u32_t RESERVED[2];
	u32_t CMPCR;
}SYSCFG_t;

#define EXTI ((volatile EXTI_t*)EXTI_BASE_ADDRESS)
#define SYSCFG ((volatile SYSCFG_t*)SYSCFG_BASE_ADDRESS)

#define EXTI_DIVISOR 4
#define EXTI_MASK 15


#endif /* EXTI_PRIVATE_H_ */
