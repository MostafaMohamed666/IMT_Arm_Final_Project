/*
 * BIT_MATH.h
 *
 *  Created on: Sep 20, 2024
 *      Author: Mostafa Mohamed
 */

#ifndef LIB_BIT_MATH_H_
#define LIB_BIT_MATH_H_

#define SET_BIT(var, bitNum) (var |=  (1<<bitNum))
#define CLEAR_BIT(var, bitNum) (var &= ~(1<<bitNum))
#define TOG_BIT(var, bitNum) (var ^= (1<<bitNum))
#define READ_BIT(var, bitNum) ((var>>bitNum)&1)


#endif /* LIB_BIT_MATH_H_ */
