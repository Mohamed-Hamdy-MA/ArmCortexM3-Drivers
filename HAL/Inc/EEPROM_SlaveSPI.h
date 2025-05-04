/*
 * EEPROM_SlaveSPI.h
 *
 *  Created on: Oct 20, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_EEPROM_SLAVESPI_H_
#define INC_EEPROM_SLAVESPI_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_SPI_Driver.h"

//==========================================================================================
//===============================User defined generic Macros================================
//==========================================================================================
#define EEPROM_25LC256_SPI_PORT						SPI1				// This parameter must be a value of SPI1 or SPI2.
#define ChipSelect_Port           					GPIOB               // Port for Row A
#define ChipSelect_PinNumber      					GPIO_PinNumber_12    // Pin number for Row A
#define EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT		2U					// EX1: Range is 0x0 to 0xFF --> 1 Byte, EX2: Range is 0x0 to 0xFFFF --> 2 Bytes.


//==========================================================================================
//===========================APIs Supported by HAL EEPROM DRIVER============================
//==========================================================================================
void HAL_EEPROM_25LC256_Init(void);
void HAL_EEPROM_25LC256_Write(uint16_t InputMemoryAddress, uint8_t* pTX_Buffer, uint16_t TX_NumberOfBytes);
void HAL_EEPROM_25LC256_Read(uint16_t InputMemoryAddress, uint8_t* pRX_Buffer, uint16_t RX_NumberOfBytes);


#endif /* INC_EEPROM_SLAVESPI_H_ */
