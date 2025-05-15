/*
 * EXTI_interface.h
 *
 *  Created on: Mar 19, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

typedef enum
{
    EXTI_LINE_0,
    EXTI_LINE_1,
    EXTI_LINE_2,
    EXTI_LINE_3,
    EXTI_LINE_4,
    EXTI_LINE_5,
    EXTI_LINE_6,
    EXTI_LINE_7,
    EXTI_LINE_8,
    EXTI_LINE_9,
    EXTI_LINE_10,
    EXTI_LINE_11,
    EXTI_LINE_12,
    EXTI_LINE_13,
    EXTI_LINE_14,
    EXTI_LINE_15
} EXTI_LINES;

typedef enum
{
	EXTI_FALLING,
	EXTI_RISING,
	EXTI_LOGIC_CHANGE
}TRIGGER_MODE;

typedef enum
{
	EXTI_PORTA,
	EXTI_PORTB,
	EXTI_PORTC
}EXTI_PORTS;

void EXTI_Enable_Interrupt(EXTI_LINES EXTI_LINE);
void EXTI_Disable_Interrupt(EXTI_LINES EXTI_LINE);
void EXTI_Set_Trigger_Detection(EXTI_LINES EXTI_LINE, TRIGGER_MODE TRIGGER_DETECTION_MODE);
void EXTI_Set_Interrupt_Port(EXTI_LINES EXTI_LINE, EXTI_PORTS PORT);
void EXTI_Call_Back_Function_EXTI0(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI1(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI2(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI3(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI4(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI5(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI6(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI7(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI8(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI9(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI10(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI11(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI12(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI13(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI14(void(*ptr)(void));
void EXTI_Call_Back_Function_EXTI15(void(*ptr)(void));

#endif /* EXTI_INTERFACE_H_ */
