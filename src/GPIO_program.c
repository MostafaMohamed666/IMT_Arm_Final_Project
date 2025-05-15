/*
 * GPIO_program.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "GPIO_private.h"

/*___________________________GENERAL API_____________________________________*/
void GPIO_Set_Mode(PORT_t Port, PIN_t Pin, PIN_MODE_t Mode){
	switch(Port){
	case PORTA: GPIOA->MODER &= ~(GPIO_MASK<<(Pin*2));
	            GPIOA->MODER |= (Mode<<(Pin*2));
	            break;
	case PORTB: GPIOB->MODER &= ~(GPIO_MASK<<(Pin*2));
                GPIOB->MODER |= (Mode<<(Pin*2));
                break;
	case PORTC: GPIOC->MODER &= ~(GPIO_MASK<<(Pin*2));
                GPIOC->MODER |= (Mode<<(Pin*2));
                break;
	}
}

/*___________________________OUTPUT API______________________________________*/
void GPIO_Set_Output_Type(PORT_t Port, PIN_t Pin, OUTPUT_TYPE_t outputType){
	switch(Port){
	case PORTA: GPIOA->OTYPER &= ~(1<<(Pin));
	            GPIOA->OTYPER |= (outputType<<(Pin));
	            break;
	case PORTB: GPIOB->OTYPER &= ~(1<<(Pin));
                GPIOB->OTYPER |= (outputType<<(Pin));
                break;
	case PORTC: GPIOC->OTYPER &= ~(1<<(Pin));
                GPIOC->OTYPER |= (outputType<<(Pin));
                break;
	}
}
void GPIO_Set_Pin_Value(PORT_t Port, PIN_t Pin, OUTPUT_VALUE_t Value){
	switch(Port){
	case PORTA: GPIOA->ODR &= ~(1<<Pin);
                GPIOA->ODR |= (Value<<Pin);
	            break;
	case PORTB: GPIOB->ODR &= ~(1<<Pin);
                GPIOB->ODR |= (Value<<Pin);
                break;
	case PORTC: GPIOC->ODR &= ~(1<<Pin);
                GPIOC->ODR |= (Value<<Pin);
                break;
	}
}
void GPIO_Set_Atomic_Pin_Value(PORT_t Port, PIN_t Pin, OUTPUT_VALUE_t Value){
    switch(Value){
    case HIGH: 	switch(Port){
	            case PORTA: GPIOA->BSRR = (1<<(Pin));
	                        break;
	            case PORTB: GPIOB->BSRR = (1<<(Pin));
	                        break;
	            case PORTC: GPIOC->BSRR = (1<<(Pin));
	                        break;
                }
                break;
    case LOW: 	switch(Port){
                case PORTA: GPIOA->BSRR = (1<<(Pin+16));
                            break;
                case PORTB: GPIOB->BSRR = (1<<(Pin+16));
                            break;
                case PORTC: GPIOC->BSRR = (1<<(Pin+16));
                            break;
                }
                break;
    }
}

/*___________________________INPUT API_______________________________________*/
void GPIO_Set_Input_Type(PORT_t Port, PIN_t Pin, INPUT_TYPE_t inputType){
	switch(Port){
	case PORTA: GPIOA->PUPDR &= ~(GPIO_MASK<<(Pin*2));
	            GPIOA->PUPDR |= (inputType<<(Pin*2));
	            break;
	case PORTB: GPIOB->PUPDR &= ~(GPIO_MASK<<(Pin*2));
                GPIOB->PUPDR |= (inputType<<(Pin*2));
                break;
	case PORTC: GPIOC->PUPDR &= ~(GPIO_MASK<<(Pin*2));
                GPIOC->PUPDR |= (inputType<<(Pin*2));
                break;
	}
}
u8_t GPIO_Get_Pin_Value(PORT_t Port, PIN_t Pin){
	u8_t pinValue = 0;
	switch(Port){
	case PORTA: pinValue = READ_BIT(GPIOA->IDR, Pin);
	            break;
	case PORTB: pinValue = READ_BIT(GPIOB->IDR, Pin);
                break;
	case PORTC: pinValue = READ_BIT(GPIOC->IDR, Pin);
                break;
	}
	return pinValue;
}

/*_______________________ALTERNATIVE FUNCTION API____________________________*/
void GPIO_Set_ALT_FUNC_Mode(PORT_t Port, PIN_t Pin, ALTERNATE_FUNCTION_t ALT_FUNC){
    if(Pin<=7){
    	switch(Port){
    	case PORTA: GPIOA->AFRL &= ~(ALT_FUNC_MASK<<(Pin*4));
    	            GPIOA->AFRL |= (ALT_FUNC<<(Pin*4));
    	            break;
    	case PORTB: GPIOB->AFRL &= ~(ALT_FUNC_MASK<<(Pin*4));
                    GPIOB->AFRL |= (ALT_FUNC<<(Pin*4));
                    break;
    	case PORTC: GPIOC->AFRL &= ~(ALT_FUNC_MASK<<(Pin*4));
                    GPIOC->AFRL |= (ALT_FUNC<<(Pin*4));
                    break;
    	}
    } else if((Pin>7) && (Pin<=15)){
    	switch(Port){
    	case PORTA: GPIOA->AFRH &= ~(ALT_FUNC_MASK<<((Pin-8)*4));
    	            GPIOA->AFRH |= (ALT_FUNC<<((Pin-8)*4));
    	            break;
    	case PORTB: GPIOB->AFRH &= ~(ALT_FUNC_MASK<<((Pin-8)*4));
                    GPIOB->AFRH |= (ALT_FUNC<<((Pin-8)*4));
                    break;
    	case PORTC: GPIOC->AFRH &= ~(ALT_FUNC_MASK<<((Pin-8)*4));
                    GPIOC->AFRH |= (ALT_FUNC<<((Pin-8)*4));
                    break;
    	}
    }
}

