/*
 * LEDMATRIX_config.h
 *
 *  Created on: Mar 20, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef LEDMATRIX_CONFIG_H_
#define LEDMATRIX_CONFIG_H_


/*
 * Choose Row PORT
 * 1- PORTA
 * 2- PORTB
 */
#define ROW_PORT PORTA

/*
 * Choose Column Port
 * 1- PORTA
 * 2- PORTB
 */
#define COL_PORT PORTB

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Row Pins
 */
#define R0 PIN0
#define R1 PIN1
#define R2 PIN2
#define R3 PIN3
#define R4 PIN4
#define R5 PIN5
#define R6 PIN6
#define R7 PIN7

//[PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15]
/*
 * Choose Column Pins
 */
#define C0 PIN6
#define C1 PIN7
#define C2 PIN8
#define C3 PIN9
#define C4 PIN10
#define C5 PIN12
#define C6 PIN13
#define C7 PIN14
#endif /* LEDMATRIX_CONFIG_H_ */
