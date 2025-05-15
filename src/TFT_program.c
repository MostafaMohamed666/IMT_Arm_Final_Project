/*
 * TFT_program.c
 *
 *  Created on: Apr 12, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "SYSTICK_interface.h"
#include "SPI_interface.h"
#include "TFT_interface.h"
#include "TFT_config.h"

void TFT_init()
{
    GPIO_Set_Mode(RST_PORT, RST_PIN, OUTPUT);
    GPIO_Set_Mode(A0_PORT, A0_PIN, OUTPUT);

    /*__________RESET SEQUENCE OF TFT DISPLAY_________*/
    GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, HIGH);
    delay_us(100);
    GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, LOW);
    delay_us(1);
    GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, HIGH);
    delay_us(100);
    GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, LOW);
    delay_us(100);
    GPIO_Set_Atomic_Pin_Value(RST_PORT, RST_PIN, HIGH);
    delay_us(120);

    //SLEEP OUT MODE within sequence
    TFT_Write_Command(0x11);
    delay_ms(10);

    //COLOR MODE within sequence
    TFT_Write_Command(0x3A);
    TFT_Write_Data(0x05); //RGB565 color code standard

    //DISPLAY on
    TFT_Write_Command(0x29);
}
void TFT_Write_Data(u8_t data)
{
    GPIO_Set_Atomic_Pin_Value(A0_PORT, A0_PIN, HIGH);
    SPI_Transmit(data);
}
void TFT_Write_Command(u8_t cmd)
{
	GPIO_Set_Atomic_Pin_Value(A0_PORT, A0_PIN, LOW);
	SPI_Transmit(cmd);
}
void TFT_Display(const u16_t *imgArr)
{
	u16_t i = 0;
	u8_t HIGH, LOW;

    //SET X coordinate
	TFT_Write_Command(0x2A);
	// start at -> 0,0
	TFT_Write_Data(0);
	TFT_Write_Data(0);
	// end at -> 127
	TFT_Write_Data(0);
	TFT_Write_Data(127);

    //SET Y coordinate
	TFT_Write_Command(0x2B);
	// start at -> 0,0
	TFT_Write_Data(0);
	TFT_Write_Data(0);
	// end at -> 127
	TFT_Write_Data(0);
	TFT_Write_Data(159);

	// WRITE on screen
	TFT_Write_Command(0x2C);
	for(i=0; i<20480; i++){
		HIGH = (u8_t)(imgArr[i]>>8);
		LOW = (u8_t)(imgArr[i]);
		TFT_Write_Data(HIGH);
		TFT_Write_Data(LOW);
	}
}



