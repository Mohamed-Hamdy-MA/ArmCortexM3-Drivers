/*
 * EEPROM_SlaveI2C.h
 *
 *  Created on: Sep 21, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_EEPROM_SLAVEI2C_H_
#define INC_EEPROM_SLAVEI2C_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_I2C_Driver.h"

//==========================================================================================
//===============================User defined generic Macros================================
//==========================================================================================
#define EEPROM_SLAVE_OWN_ADDRESS		0x2AU
#define EEPROM_MEMORY_ADDR_BYTE_COUNT	2U		// EX1: Range is 0x0 to 0xFF --> 1 Byte, EX2: Range is 0x0 to 0xFFFF --> 2 Bytes.
#define EEPROM_I2C_PORT					I2C1	// This parameter must be a value of I2C1 or I2C2.


//==========================================================================================
//===========================APIs Supported by HAL EEPROM DRIVER============================
//==========================================================================================
void HAL_EEPROM_Init(void);
void HAL_EEPROM_Write(uint16_t MemoryAddress, uint8_t* pTX_Buffer, uint32_t TX_NumberOfBytes);
void HAL_EEPROM_Read(uint16_t MemoryAddress, uint8_t* pRX_Buffer, uint32_t RX_NumberOfBytes);


#endif /* INC_EEPROM_SLAVEI2C_H_ */
