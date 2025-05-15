/*
 * GPIO_private.h
 *
 *  Created on: Mar 3, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#define GPIOA_BASE_ADDRESS (0x40020000)
#define GPIOB_BASE_ADDRESS (0x40020400)
#define GPIOC_BASE_ADDRESS (0x40020800)

typedef struct
{
	u32_t MODER;
	u32_t OTYPER;
	u32_t OSPEEDER;
	u32_t PUPDR;
	u32_t IDR;
	u32_t ODR;
	u32_t BSRR;
	u32_t LCKR;
	u32_t AFRL;
	u32_t AFRH;
}GPIO_t;

#define GPIOA ((volatile GPIO_t*)GPIOA_BASE_ADDRESS)
#define GPIOB ((volatile GPIO_t*)GPIOB_BASE_ADDRESS)
#define GPIOC ((volatile GPIO_t*)GPIOC_BASE_ADDRESS)

#define GPIO_MASK 3
#define ALT_FUNC_MASK 15


#endif /* GPIO_PRIVATE_H_ */
