/*
 * UART_interface.h
 *
 *  Created on: Mar 21, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#define UART_BUFFER_SIZE 32

void UART1_Peripheral_init();
void UART1_Send_Data(u8_t Data);
u8_t UART1_Recieve_Data();
u8_t UART1_Available(void);
u8_t UART1_Recieve_Data_With_Timeout(void);
void UART_Call_Back_Function_USART1(void (*ptr)(void));
u8_t UART1_Receive_Line(char *buffer);

#endif /* UART_INTERFACE_H_ */
