/*
 * NVIC_private.h
 *
 *  Created on: Mar 10, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef NVIC_PRIVATE_H_
#define NVIC_PRIVATE_H_

#define NVIC_BASE_ADDRESS (0xE000E100)

typedef struct
{
    u32_t ISER[8];
    u32_t RESERVED0[24];
    u32_t ICER[8];
    u32_t RESERVED1[24];
    u32_t ISPR[8];
    u32_t RESERVED2[24];
    u32_t ICPR[8];
    u32_t RESERVED3[24];
    u32_t IABR[8];
    u32_t RESERVED4[56];
    u8_t  IPR[240];
    u32_t RESERVED5[580];
}NVIC_t;

#define NVIC ((volatile NVIC_t*)NVIC_BASE_ADDRESS)
#define NVIC_DIVISOR 32
#define SCB_AIRCR *((volatile u32_t*)0xE000ED0C)
#define VECTKEY 0x05FA0000




#endif /* NVIC_PRIVATE_H_ */
