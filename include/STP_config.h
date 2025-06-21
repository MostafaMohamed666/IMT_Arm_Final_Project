/*
 * STP_config.h
 *
 *  Created on: Mar 20, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef STP_CONFIG_H_
#define STP_CONFIG_H_

/*
 * Choose Port
 * 1- PORTA
 * 2- PORTB
 * 3- PORTC
 */
#define STP_SERIAL_PORT PORTA

/*
 * Choose Port
 * 1- PORTA
 * 2- PORTB
 * 3- PORTC
 */
#define STP_SHIFT_PORT PORTA

/*
 * Choose Port
 * 1- PORTA
 * 2- PORTB
 * 3- PORTC
 */
#define STP_LATCH_PORT PORTA

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Serial Pin
 */
#define STP_SERIAL_PIN PIN4

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Shift Pin
 */
#define STP_SHIFT_PIN PIN6

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Latch Pin
 */
#define STP_LATCH_PIN PIN11

/*
 * Choose Port
 * 1- PORTA
 * 2- PORTB
 * 3- PORTC
 */
#define STP_SERIAL_TWO_PORT PORTA

/*
 * Choose Port
 * 1- PORTA
 * 2- PORTB
 * 3- PORTC
 */
#define STP_SHIFT_TWO_PORT PORTB

/*
 * Choose Port
 * 1- PORTA
 * 2- PORTB
 * 3- PORTC
 */
#define STP_LATCH_TWO_PORT PORTB

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Serial Pin
 */
#define STP_SERIAL_TWO_PIN PIN12

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Shift Pin
 */
#define STP_SHIFT_TWO_PIN PIN0

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Latch Pin
 */
#define STP_LATCH_TWO_PIN PIN1

#endif /* STP_CONFIG_H_ */
