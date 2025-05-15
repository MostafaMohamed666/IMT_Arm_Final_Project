/*
 * GPIO_interface.h
 *
 *  Created on: Mar 3, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_


/*PINS 13,14,15 in GPIOA
 * and pins 3,4,5 in GPIOB
 * are used for configurations
 */

typedef enum
{
	PORTA,
	PORTB,
	PORTC
}PORT_t;

typedef enum
{
	PIN0,
	PIN1,
	PIN2,
	PIN3,
	PIN4,
	PIN5,
	PIN6,
	PIN7,
	PIN8,
	PIN9,
	PIN10,
	PIN11,
	PIN12,
	PIN13,
	PIN14,
	PIN15
}PIN_t;

typedef enum
{
	INPUT,
	OUTPUT,
	ALT_FUNCTION,
	ANALOG
}PIN_MODE_t;

typedef enum
{
	PUSH_PULL,
	OPEN_DRAIN
}OUTPUT_TYPE_t;

typedef enum
{
	LOW,
	HIGH
}OUTPUT_VALUE_t;

typedef enum
{
	NO_PULL,
	PULL_UP,
	PULL_DOWN
}INPUT_TYPE_t;

typedef enum
{
	AF0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15
}ALTERNATE_FUNCTION_t;

/*___________________________GENERAL API_____________________________________*/
void GPIO_Set_Mode(PORT_t Port, PIN_t Pin, PIN_MODE_t Mode);

/*___________________________OUTPUT API______________________________________*/
void GPIO_Set_Output_Type(PORT_t Port, PIN_t Pin, OUTPUT_TYPE_t outputType);
void GPIO_Set_Pin_Value(PORT_t Port, PIN_t Pin, OUTPUT_VALUE_t Value);
void GPIO_Set_Atomic_Pin_Value(PORT_t Port, PIN_t Pin, OUTPUT_VALUE_t Value);

/*___________________________INPUT API_______________________________________*/
void GPIO_Set_Input_Type(PORT_t Port, PIN_t Pin, INPUT_TYPE_t inputType);
u8_t GPIO_Get_Pin_Value(PORT_t Port, PIN_t Pin);

/*_______________________ALTERNATIVE FUNCTION API____________________________*/
void GPIO_Set_ALT_FUNC_Mode(PORT_t Port, PIN_t Pin, ALTERNATE_FUNCTION_t ALT_FUNC);


#endif /* GPIO_INTERFACE_H_ */
