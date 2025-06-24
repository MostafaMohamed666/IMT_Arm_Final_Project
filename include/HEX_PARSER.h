/*
 * HEX_PARSER.h
 *
 *  Created on: May 2, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef HEX_PARSER_H_
#define HEX_PARSER_H_

extern u8_t GLB_Flag;

void HEX_PARSER_Parse_Record(const char* INTEL_HEX_RECORD);
void HEX_PARSER_Parse_Data(const char* INTEL_HEX_RECORD);
u8_t HEX_PARSER_CHecksum(const char* INTEL_HEX_RECORD);
u8_t HEX_PARSER_HighAdress_CHecksum(const char* INTEL_HEX_RECORD);

#endif /* HEX_PARSER_H_ */
