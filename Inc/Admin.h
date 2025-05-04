/*
 * Admin.h
 *
 *  Created on: Oct 24, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef ADMIN_H_
#define ADMIN_H_

#include <string.h>
#include "LCD2x16.h"
#include "Keypad.h"

void HAL_MemorySIM_Init();
void HAL_SentToMemorySIM(uint16_t InputMemoryAddress, uint8_t* pTX_BuFFer, uint16_t TX_NumberOfBytes);
void HAL_ReadFromMemorySIM(uint16_t InputMemoryAddress, uint8_t* pRX_Buffer, uint16_t RX_NumberOfBytes);
void HAL_DeleteFromMemorySIM(uint16_t InputMemoryAddress, uint16_t NumberOfBytes);

uint8_t InputKey_OutputLCD(char* ReturnString);

uint16_t APP_Search_RFID(char* arrRFID_Required);

void APP_Add_RFID(void);
void APP_Edit_RFID(void);
void APP_Delete_RFID(void);


#endif /* ADMIN_H_ */
