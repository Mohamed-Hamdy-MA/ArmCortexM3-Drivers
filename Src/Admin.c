/*
 * Admin.c
 *
 *  Created on: Oct 24, 2024
 *      Author: Mohamed Hamdy
 */

#include "Admin.h"

uint16_t TotalNumberOfClients = 0;


uint8_t MemorySIM[64];
void HAL_MemorySIM_Init()
{
	for (int ArrINDEX = 0; ArrINDEX < 64; ArrINDEX++)
	{
		MemorySIM[ArrINDEX] = 0xFF;
	}
}
void HAL_SentToMemorySIM(uint16_t InputMemoryAddress, uint8_t* pTX_BuFFer, uint16_t TX_NumberOfBytes)
{
	for (uint16_t WriteINDEX = 0; WriteINDEX < TX_NumberOfBytes; WriteINDEX++)
	{
		MemorySIM[InputMemoryAddress + WriteINDEX] = pTX_BuFFer[WriteINDEX];
	}
}
void HAL_ReadFromMemorySIM(uint16_t InputMemoryAddress, uint8_t* pRX_Buffer, uint16_t RX_NumberOfBytes)
{
	for (uint16_t ReadINDEX = 0; ReadINDEX < RX_NumberOfBytes; ReadINDEX++)
	{
		pRX_Buffer[ReadINDEX] = MemorySIM[InputMemoryAddress + ReadINDEX];
	}
}
void HAL_DeleteFromMemorySIM(uint16_t InputMemoryAddress, uint16_t NumberOfBytes)
{
	for (uint16_t WriteINDEX = 0; WriteINDEX < NumberOfBytes; WriteINDEX++)
	{
		MemorySIM[InputMemoryAddress + WriteINDEX] = 0xFF;
	}
}

uint8_t InputKey_OutputLCD(char* ReturnString)
{
	uint8_t NoOfCharactersWrittenWithoutNULL = 0;
	uint8_t ReturnStringINDEX = 0;
	char KeyPressed = 'N';

	while(KeyPressed != '=' )
	{
		switch (KeyPressed)
		{
		case 'N':
			break;
		default:
			HAL_LCD_SendChar(KeyPressed);
			ReturnString[ReturnStringINDEX] = KeyPressed;
			ReturnStringINDEX++;
			break;
		}

		KeyPressed = HAL_Keypad_getPressedKey();
	}

	ReturnString[ReturnStringINDEX] = 0;	//Add the NULL termination.
	return NoOfCharactersWrittenWithoutNULL = ReturnStringINDEX;
}

uint16_t APP_Search_RFID(char* arrRFID_Required)
{
	char arrRFID_FetchedFromMemory[7];

	for (uint16_t SearchINDEX = 1; SearchINDEX <= TotalNumberOfClients; SearchINDEX++)
	{
		HAL_ReadFromMemorySIM((SearchINDEX*8)-8, (uint8_t*) arrRFID_FetchedFromMemory, 7);
		if (strcmp(arrRFID_Required, arrRFID_FetchedFromMemory) == 0)
		{
			return SearchINDEX;
		}
	}
	return 0;
}

void APP_Add_RFID(void)
{
	static uint8_t MemorySIM_AddFRID_WriteAddress = 0;
	char arrRFID_New[7];	// 6 Bytes RFID key + 1 Byte NULL termination

	HAL_LCD_ClearDisplay();
	HAL_LCD_SendString("Enter the RFID");
	HAL_LCD_MoveCursor(2, 1);
	HAL_LCD_SendString("Number: ");

	uint8_t NoOfCharactersWrittenWithoutNULL = InputKey_OutputLCD(arrRFID_New);

	if (APP_Search_RFID(arrRFID_New) != 0) 		// RFID Matched
	{
		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("Found! RFID must be unique");
		HAL_LCD_ClearDisplay();
	}
	else
	{
		HAL_SentToMemorySIM(MemorySIM_AddFRID_WriteAddress, (uint8_t*) arrRFID_New, NoOfCharactersWrittenWithoutNULL + 1);
		MemorySIM_AddFRID_WriteAddress = MemorySIM_AddFRID_WriteAddress + 8;
		TotalNumberOfClients++;

		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("*****Added*****");
	}
}
void APP_Edit_RFID(void)
{
	char arrRFID_Old[7];	// 6 Bytes RFID key + 1 Byte NULL termination
	char arrRFID_New[7];	// 6 Bytes RFID key + 1 Byte NULL termination

	HAL_LCD_ClearDisplay();
	HAL_LCD_SendString("Enter old RFID");
	HAL_LCD_MoveCursor(2, 1);
	HAL_LCD_SendString("Number: ");

	uint8_t NoOfCharactersWrittenWithoutNULL = InputKey_OutputLCD(arrRFID_Old);
	uint16_t SearchINDEX = APP_Search_RFID(arrRFID_Old);

	if ( SearchINDEX != 0) 		// RFID Matched
	{
		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("*****Found*****");
		HAL_LCD_ClearDisplay();

		HAL_LCD_SendString("Enter new RFID");
		HAL_LCD_MoveCursor(2, 1);
		HAL_LCD_SendString("Number: ");

		NoOfCharactersWrittenWithoutNULL = InputKey_OutputLCD(arrRFID_New);

		if (APP_Search_RFID(arrRFID_New) != 0) 		// New RFID Matched
		{
			HAL_LCD_ClearDisplay();
			HAL_LCD_SendString("Found! RFID must be unique");
			HAL_LCD_ClearDisplay();
		}
		else
		{

			HAL_SentToMemorySIM((SearchINDEX*8)-8, (uint8_t*) arrRFID_New, NoOfCharactersWrittenWithoutNULL + 1);
		}
	}
	else
	{
		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("***Not Found***");
		HAL_LCD_ClearDisplay();
	}
}
void APP_Delete_RFID(void)
{
	char arrRFID_Old[7];	// 6 Bytes RFID key + 1 Byte NULL termination

	HAL_LCD_ClearDisplay();
	HAL_LCD_SendString("Enter old RFID");
	HAL_LCD_MoveCursor(2, 1);
	HAL_LCD_SendString("Number: ");

	uint8_t NoOfCharactersWrittenWithoutNULL = InputKey_OutputLCD(arrRFID_Old);
	uint16_t SearchINDEX = APP_Search_RFID(arrRFID_Old);

	if ( SearchINDEX != 0) 		// RFID Matched
	{
		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("*****Found*****");


		HAL_DeleteFromMemorySIM((SearchINDEX*8)-8, NoOfCharactersWrittenWithoutNULL + 1);
		TotalNumberOfClients--;

		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("****Deleted****");

	}
	else
	{
		HAL_LCD_ClearDisplay();
		HAL_LCD_SendString("***Not Found***");
	}
}
