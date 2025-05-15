/*
 * RCC_private.h
 *
 *  Created on: Mar 1, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef RCC_PRIVATE_H_
#define RCC_PRIVATE_H_

#define RCC_BASE_ADDRESS (0x40023800)

typedef struct
{
	u32_t CR;
	u32_t PLLCFGR;
	u32_t CFGR;
	u32_t CIR;
	u32_t AHB1RSTR;
	u32_t AHB2RSTR;
	u32_t RESERVED1;
	u32_t RESERVED2;
	u32_t APB1RSTR;
	u32_t APB2RSTR;
	u32_t RESERVED3;
    u32_t RESERVED4;
    u32_t AHB1ENR;
    u32_t AHB2ENR;
    u32_t RESERVED5;
    u32_t RESERVED6;
    u32_t APB1ENR;
    u32_t APB2ENR;
    u32_t RESERVED7;
    u32_t RESERVED8;
    u32_t AHB1LPENR;
    u32_t AHB2LPENR;
    u32_t RESERVED9;
    u32_t RESERVED10;
    u32_t APB1LPENR;
    u32_t APB2LPENR;
    u32_t RESERVED11;
    u32_t RESERVED12;
    u32_t BDCR;
    u32_t CSR;
    u32_t RESERVED13;
    u32_t RESERVED14;
    u32_t SSCGR;
    u32_t PLLI2SCFGR;
    u32_t RESERVED15;
    u32_t DCKCFGR;
}RCC;

#define RCC ((volatile RCC*)RCC_BASE_ADDRESS)

#define HSI 0
#define HSE 1

#define RC_OSC 0
#define MECHANICAL_OSC 1

#define HSION 0
#define HSIRDY 1
#define HSEON 16
#define HSERDY 17
#define HSEBYP 18
#define SW0 0
#define SW1 1

#endif /* RCC_PRIVATE_H_ */
