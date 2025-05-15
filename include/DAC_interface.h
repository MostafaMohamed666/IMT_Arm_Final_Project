/*
 * DAC_interface.h
 *
 *  Created on: Mar 21, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef DAC_INTERFACE_H_
#define DAC_INTERFACE_H_

void DAC_init();
void DAC_Send_Sample(const u8_t *arr, u16_t sample_index);

#endif /* DAC_INTERFACE_H_ */
