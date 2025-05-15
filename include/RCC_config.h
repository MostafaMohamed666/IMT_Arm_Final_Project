/*
 * RCC_config.h
 *
 *  Created on: Mar 1, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef RCC_CONFIG_H_
#define RCC_CONFIG_H_

/*
 * Choose CLock
 * 1 - HSE
 * 2 - HSI
 */

#define SYSCLK HSI

/*
 * Choose external clock type for HSE
 * 1 - RC circuit [RC_OSC]
 * 2 - Crystal or Ceramic oscillator [MECHANICAL_OSC]
 */

#define HSE_OSC MECHANICAL_OSC


#endif /* RCC_CONFIG_H_ */
