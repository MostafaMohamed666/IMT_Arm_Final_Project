/*
 * IR_interface.h
 *
 *  Created on: Mar 22, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef IR_INTERFACE_H_
#define IR_INTERFACE_H_

void IR_EXTI_ISR(void);
void IR_SYSTK_ISR(void);
extern u8_t IR_value;

#endif /* IR_INTERFACE_H_ */
