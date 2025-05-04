/*
 * EEPROM_SlaveSPI.c
 *
 *  Created on: Oct 20, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "EEPROM_SlaveSPI.h"

//==========================================================================================
//================================EEPROM 25LC256 Instructions===============================
//==========================================================================================
#define READ            0x03    // Read data from memory array beginning at selected address
#define WRITE           0x02    // Write data to memory array beginning at selected address
#define WRDI           	0x04   // Reset the write enable latch (disable write operations)
#define WREN            0x06    // Set the write enable latch (enable write operations)
#define RDSR            0x05    // Read Status Register
#define WRSR            0x01    // Write Status Register

//==========================================================================================
//===========================APIs Supported by HAL EEPROM DRIVER============================
//==========================================================================================
void HAL_EEPROM_25LC256_Init(void)
{
	SPI_Config_t SPIConfig;
	GPIO_PinConfig_t ChipSelect_GPIO_pinConfig;

	SPIConfig.SPI_DeviceMode = SPI_DeviceMode_MASTER;
	SPIConfig.SPI_CommunicationMode = SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX;
	SPIConfig.SPI_DataLength = SPI_DataLength_8BITS;
	SPIConfig.SPI_FrameFormat = SPI_FrameFormat_MSB_FIRST;
	SPIConfig.SPI_ClockPolarity = SPI_ClockPolarity_HIGH_IDLE;
	SPIConfig.SPI_ClockPhase = SPI_ClockPhase_2ND_EDGE_CAPTURE;
	SPIConfig.SPI_ClockPreScaler = SPI_ClockPreScaler_2;
	SPIConfig.SPI_NSS = SPI_NSS_SW_HIGH;
	SPIConfig.SPI_Interrupt_EN = SPI_Interrupt_EN_NONE;
	SPIConfig.SPI_ptr_irqCallbackFUN = NULL;
	MCAL_SPI_Init(EEPROM_25LC256_SPI_PORT, &SPIConfig, SingleMaster_SingleSlave);

	ChipSelect_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
	ChipSelect_GPIO_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
	ChipSelect_GPIO_pinConfig.GPIO_PinNumber = ChipSelect_PinNumber;
	MCAL_GPIO_InitPin(ChipSelect_Port, &ChipSelect_GPIO_pinConfig);
	MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);
}
void HAL_EEPROM_25LC256_Write(uint16_t InputMemoryAddress, uint8_t* pTX_BuFFer, uint16_t TX_NumberOfBytes)
{

	uint16_t CurrentPageStartAddress = (InputMemoryAddress / 64) * 64;
	uint16_t NextPageStartAddress = CurrentPageStartAddress + 64;
	uint8_t AvailableCurrentPageBytes = NextPageStartAddress - InputMemoryAddress;

	uint8_t Address_Buffer[EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT] = { (uint8_t)(InputMemoryAddress>>8), (uint8_t)(InputMemoryAddress)};
	uint16_t NumberOfBytes_INDEX_COUNTER = 0;

	uint8_t WREN_INSTRUCTION = WREN;
	uint8_t WRITE_INSTRUCTION = WRITE;

	/*************************************************************************************************************/

	// Issuing the write enable sequence (WREN instruction) to set the write enable latch.
	MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_LOW);
	MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, (uint16_t*) &WREN_INSTRUCTION, SPI_PollingENABLE);
	MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);

	/*************************************************************************************************************/
	do
	{

		if ( (TX_NumberOfBytes-NumberOfBytes_INDEX_COUNTER) <= AvailableCurrentPageBytes )		// 1 write operation on 1 page.
		{

			// Issuing a WRITE instruction
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_LOW);
			MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &WRITE_INSTRUCTION, SPI_PollingENABLE);

			// Followed by the 16-bit address, with the first MSB of the address [input address(1st iteration only) or current page start address(other iterations)].
			for (uint8_t AddressINDEX = 0; AddressINDEX < EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT; AddressINDEX++)
			{
				MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &Address_Buffer[AddressINDEX], SPI_PollingENABLE);
			}

			// The data to be written [to the end of transmit data buffer, not necessary the end of the page].
			for (; NumberOfBytes_INDEX_COUNTER < TX_NumberOfBytes; NumberOfBytes_INDEX_COUNTER++)
			{
				MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &pTX_BuFFer[NumberOfBytes_INDEX_COUNTER], SPI_PollingENABLE);
			}

			// The CS must be brought high to complete the write operation. When the write cycle is completed, the write enable latch is reset.
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);

		}
		else					// 2 write operations on 2 pages
		{

			// Issuing a WRITE instruction.
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_LOW);
			MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &WRITE_INSTRUCTION, SPI_PollingENABLE);

			// Followed by the 16-bit address, with the first MSB of the address [input address(1st iteration only) or current page start address(other iterations)].
			for (uint8_t AddressINDEX = 0; AddressINDEX < EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT; AddressINDEX++)
			{
				MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &Address_Buffer[AddressINDEX], SPI_PollingENABLE);
			}

			// The data to be written [to the end of the current page].
			for (uint8_t ByteCOUNTER = 0; ByteCOUNTER < AvailableCurrentPageBytes; NumberOfBytes_INDEX_COUNTER++, ByteCOUNTER++)/**/
			{
				MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &pTX_BuFFer[NumberOfBytes_INDEX_COUNTER], SPI_PollingENABLE);
			}

			// The CS must be brought high to complete the write operation. When the write cycle is completed, the write enable latch is reset.
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);

			/********************/

			// Adjust the address buffer to the next page start address.
			Address_Buffer[0] = (uint8_t)(NextPageStartAddress>>8);
			Address_Buffer[1] = (uint8_t)(NextPageStartAddress);

			// ReIssuing the write enable sequence (WREN instruction), since the write enable latch is reset on the 1st write operation on the previous page.
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_LOW);
			MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &WREN_INSTRUCTION, SPI_PollingENABLE);
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);

			// Issuing a WRITE instruction.
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_LOW);
			MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &WRITE_INSTRUCTION, SPI_PollingENABLE);

			// Followed by the 16-bit address, with the first MSB of the address [ the next address to the previous address].
			for (uint8_t AddressINDEX = 0; AddressINDEX < EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT; AddressINDEX++)
			{
				MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, (uint16_t*) &Address_Buffer[AddressINDEX], SPI_PollingENABLE);
			}

			// The data to be written [to the end of transmit data buffer or to the end of the page].
			for (uint8_t ByteCOUNTER = 0; (NumberOfBytes_INDEX_COUNTER < TX_NumberOfBytes) && (ByteCOUNTER < 64); NumberOfBytes_INDEX_COUNTER++, ByteCOUNTER++)/**/
			{
				MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, (uint16_t*) &pTX_BuFFer[NumberOfBytes_INDEX_COUNTER], SPI_PollingENABLE);
			}

			// The CS must be brought high to complete the write operation. When the write cycle is completed, the write enable latch is reset.
			MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);
		}

		// In case of there are more data to be written, update the current and next page start addresses to repeat the whole 1 or 2 page logic again.
		if (NumberOfBytes_INDEX_COUNTER < TX_NumberOfBytes)
		{
			CurrentPageStartAddress = NextPageStartAddress + 64;		// Current page become 3rd page
			NextPageStartAddress = CurrentPageStartAddress + 64;		// Next page becomes 4th page
			AvailableCurrentPageBytes = 64;								// 64 bytes is available on 3rd page

			// Put the 3rd page start address on the address buffer
			Address_Buffer[0] = (uint8_t)(CurrentPageStartAddress>>8);
			Address_Buffer[1] = (uint8_t)(CurrentPageStartAddress);
		}

	}

	// In case of there are more data to be written, repeat the whole 1 or 2 page logic again.
	while (NumberOfBytes_INDEX_COUNTER < TX_NumberOfBytes);

}
void HAL_EEPROM_25LC256_Read(uint16_t InputMemoryAddress, uint8_t* pRX_Buffer, uint16_t RX_NumberOfBytes)
{

	uint8_t Address_Buffer[EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT] = { (uint8_t)(InputMemoryAddress>>8), (uint8_t)(InputMemoryAddress)};

	uint8_t READ_INSTRUCTION = READ;

	/*************************************************************************************************************/

	// Issuing a READ instruction
	MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_LOW);
	MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &READ_INSTRUCTION, SPI_PollingENABLE);

	// Followed by the 16-bit address, with the first MSB of the address [input address].
	for (uint8_t AddressINDEX = 0; AddressINDEX < EEPROM_25LC256_MEMORY_ADDR_BYTE_COUNT; AddressINDEX++)
	{
		MCAL_SPI_SendData(EEPROM_25LC256_SPI_PORT, 	(uint16_t*) &Address_Buffer[AddressINDEX], SPI_PollingENABLE);
	}

	// The data to be read.
	for (uint8_t ByteCOUNTER = 0; ByteCOUNTER < RX_NumberOfBytes; ByteCOUNTER++)
	{
		MCAL_SPI_ReceiveData(EEPROM_25LC256_SPI_PORT, (uint16_t*) &pRX_Buffer[ByteCOUNTER], SPI_PollingENABLE);
	}

	MCAL_GPIO_WritePin(ChipSelect_Port, ChipSelect_PinNumber, GPIO_PinValue_HIGH);

}
