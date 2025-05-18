/*
 * TIM2_config.h
 *
 *  Created on: Apr 21, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef TIM2_CONFIG_H_
#define TIM2_CONFIG_H_


/*
 * Choose Timer Direction
 * 1- TIM2_UPCOUNT
 * 2- TIM2_DOWNCOUNT
 */

#define TIMER_DIRECTION TIM2_UPCOUNT

/*
 * SET prescalar value from 1 -> 65536
 * Calculation -> fosc/[PRESCALAR]-1
 */
#define PRESCALAR 15

#endif /* TIM2_CONFIG_H_ */
