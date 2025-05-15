/*
 * SYSTICK_interface.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef SYSTICK_INTERFACE_H_
#define SYSTICK_INTERFACE_H_

void SYSTICK_peripheral_init();
void SYSTICK_start_timer(u32_t LOAD_VALUE);
void SYSTICK_interrupt_enable();
void SYSTICK_interrupt_disable();
u8_t SYSTICK_read_flag();
void delay_ms(u32_t delayTime);
void delay_us(u32_t delayTime);
void SYSTICK_Call_Back_Function(void(*ptr)(void), u8_t time_in_us);
u32_t  SYSTICK_Get_Elapsed_TickSingleShot();
void SYSTICK_Call_Back_Function_SingleShot(void(*ptr)(void), u16_t time_in_us);


#endif /* SYSTICK_INTERFACE_H_ */
