/*
 * TIM2_private.h
 *
 *  Created on: Apr 21, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef TIM2_PRIVATE_H_
#define TIM2_PRIVATE_H_

#define TIM2_BASE_ADDRESS (0x40000000)

typedef struct
{
	u32_t CR1;
	u32_t CR2;
	u32_t SMCR;
	u32_t DIER;
	u32_t SR;
	u32_t EGR;
    u32_t CCMR1;
    u32_t CCMR2;
    u32_t CCER;
    u32_t CNT;
    u32_t PSC;
    u32_t ARR;
    u32_t RESERVED1;
    u32_t CCR1;
    u32_t CCR2;
    u32_t CCR3;
    u32_t CCR4;
	u32_t RESERVED2;
    u32_t DCR;
    u32_t DMAR;
    u32_t OR;
}TIM2_t;

#define TIM2 ((volatile TIM2_t*)TIM2_BASE_ADDRESS)

#define ARPE 7
#define CMS 5
#define DIR 4
#define CEN 0
#define URS 2
#define UIE 0
#define UIF 0
#define UG 0
#define OPM 3

#define TIM2_UPCOUNT   0
#define TIM2_DOWNCOUNT 1

#endif /* TIM2_PRIVATE_H_ */
