/*
 * EEPROM_SlaveI2C.c
 *
 *  Created on: Sep 21, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "EEPROM_SlaveI2C.h"

//==========================================================================================
//===========================APIs Supported by HAL EEPROM DRIVER============================
//==========================================================================================
void HAL_EEPROM_Init(void)
{
	I2C_Config_t I2CConfig;

	I2CConfig.I2C_DeviceMode = I2C_DeviceMode_MASTER;
	I2CConfig.I2C_ClockSpeed = I2C_ClockSpeed_SM_100kHz;
	I2CConfig.I2C_ClockStretching = I2C_ClockStretching_EN;
	I2CConfig.I2C_Interrupt_EN = I2C_Interrupt_EN_NONE;
	I2CConfig.I2C_ptr_irqCallbackFUN = NULL;

	MCAL_I2C_Init(EEPROM_I2C_PORT, &I2CConfig);
}
void HAL_EEPROM_Write(uint16_t MemoryAddress, uint8_t* pTX_Buffer, uint32_t TX_NumberOfBytes)
{
	/*
	 * Ex1:
	 * MemoryAddress = AAFF --> Most address byte transfered first (0xAA) then least address byte transfered second (0xFF).
	 * In case uint8_t* pTX_Buffer = &MemoryAddress, (0xFF) will transfered first then (0xAA) will transfered second
	 */

	uint8_t MemoryAddressBuffer[EEPROM_MEMORY_ADDR_BYTE_COUNT] = { (uint8_t)(MemoryAddress>>8), (uint8_t)(MemoryAddress)};

	MCAL_I2C_Master_SendData_Polling(EEPROM_I2C_PORT, EEPROM_SLAVE_OWN_ADDRESS, ENABLE, MemoryAddressBuffer, EEPROM_MEMORY_ADDR_BYTE_COUNT, START, DISABLE);
	MCAL_I2C_Master_SendData_Polling(EEPROM_I2C_PORT, EEPROM_SLAVE_OWN_ADDRESS, DISABLE, pTX_Buffer, TX_NumberOfBytes, WithoutSTART, ENABLE);
	/*
	 * Sequence:
	 * START  ACK  SlaveAddress|W  MostAddressByte  ACK  LeastAddressByte  ACK  Data0  ACK  Data1  ACK  ...  DataN  ACK  STOP
	 *
	 * First MCAL_I2C_Master_SendData call:
	 * START  ->  ACK  ->  SlaveAddress|W  ->  ACK  ->  MostAddressByte  ->  ACK  ->  LeastAddressByte  ->  ACK
	 *
	 * Second MCAL_I2C_Master_SendData call:
	 * Data0  ->  ACK  ->  Data1  ->  ACK  ->  ...  ->  DataN  ->  ACK  ->  STOP
	 */
}
void HAL_EEPROM_Read(uint16_t MemoryAddress, uint8_t* pRX_Buffer, uint32_t RX_NumberOfBytes)
{
	/*
	 * EX1:
	 * MemoryAddress = AAFF --> Most address byte transfered first (0xAA) then least address byte transfered second (0xFF).
	 * In case uint8_t* pTX_Buffer = &MemoryAddress, (0xFF) will transfered first then (0xAA) will transfered second
	 */

	uint8_t MemoryAddressBuffer[EEPROM_MEMORY_ADDR_BYTE_COUNT] = { (uint8_t)(MemoryAddress>>8), (uint8_t)(MemoryAddress)};

	MCAL_I2C_Master_SendData_Polling(EEPROM_I2C_PORT, EEPROM_SLAVE_OWN_ADDRESS, ENABLE, MemoryAddressBuffer, EEPROM_MEMORY_ADDR_BYTE_COUNT, START, DISABLE);
	MCAL_I2C_Master_ReceiveData_Polling(EEPROM_I2C_PORT, EEPROM_SLAVE_OWN_ADDRESS, ENABLE, pRX_Buffer, RX_NumberOfBytes, RepeatedSTART, ENABLE);
	/*
	 * Sequence:
	 * START  ACK  SlaveAddress|W  MostAddressByte  ACK  LeastAddressByte  ACK  RepeatedSTART  ACK  SlaveAddress|R  ACK  Data0  ACK  Data1  ACK  ...  DataN  NACK  STOP
	 *
	 * MCAL_I2C_Master_SendData call:
	 * START  ->  ACK  ->  SlaveAddress|W  ->  ACK  ->  MostAddressByte  ->  ACK  ->  LeastAddressByte  ->  ACK
	 *
	 * MCAL_I2C_Master_ReceiveData call:
	 * RepeatedSTART  ->  ACK  ->  SlaveAddress|R  ->  ACK  ->  Data0  ->  ACK  ->  Data1  ->  ACK  ->  ...  ->  DataN  ->  NACK  ->  STOP
	 */
}
