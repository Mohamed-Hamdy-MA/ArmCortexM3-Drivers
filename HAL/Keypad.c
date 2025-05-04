/*
 * Keypad.c
 *
 *  Created on: Oct 17, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "Keypad.h"

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================
typedef struct
{
	uint16_t Keypad_PinNumber;
	GPIOx_REG_t* Keypad_PinPort;
}Keypad_Pin_t;

//==========================================================================================
//=====================================Assisting Macros=====================================
//==========================================================================================


//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================
Keypad_Pin_t Keypad_Row[4] 	= {{RowA_PinNumber, RowA_Port}, {RowB_PinNumber, RowB_Port}, {RowC_PinNumber, RowC_Port}, {RowD_PinNumber, RowD_Port}};
Keypad_Pin_t Keypad_Column[4] = {{Column1_PinNumber, Column1_Port}, {Column2_PinNumber, Column2_Port}, {Column3_PinNumber, Column3_Port}, {Column4_PinNumber, Column4_Port}};

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================


//==========================================================================================
//===========================APIs Supported by HAL Keypad DRIVER============================
//==========================================================================================
void HAL_Keypad_Init(void)
{
	//==========STEP1: Configure all pins of GPIO connected to Keypad==========

	GPIO_PinConfig_t Keypad_GPIO_pinConfig;

	// Configure Rows as inputs (pull-up)
	Keypad_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;

	Keypad_GPIO_pinConfig.GPIO_PinNumber = RowA_PinNumber;
	MCAL_GPIO_InitPin(RowA_Port, &Keypad_GPIO_pinConfig);

	Keypad_GPIO_pinConfig.GPIO_PinNumber = RowB_PinNumber;
	MCAL_GPIO_InitPin(RowB_Port, &Keypad_GPIO_pinConfig);

	Keypad_GPIO_pinConfig.GPIO_PinNumber = RowC_PinNumber;
	MCAL_GPIO_InitPin(RowC_Port, &Keypad_GPIO_pinConfig);

	Keypad_GPIO_pinConfig.GPIO_PinNumber = RowD_PinNumber;
	MCAL_GPIO_InitPin(RowD_Port, &Keypad_GPIO_pinConfig);


	// Configure columns as outputs (push-pull)
	Keypad_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
	Keypad_GPIO_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;


	Keypad_GPIO_pinConfig.GPIO_PinNumber = Column1_PinNumber;
	MCAL_GPIO_InitPin(Column1_Port, &Keypad_GPIO_pinConfig);

	Keypad_GPIO_pinConfig.GPIO_PinNumber = Column2_PinNumber;
	MCAL_GPIO_InitPin(Column2_Port, &Keypad_GPIO_pinConfig);

	Keypad_GPIO_pinConfig.GPIO_PinNumber = Column3_PinNumber;
	MCAL_GPIO_InitPin(Column3_Port, &Keypad_GPIO_pinConfig);

	Keypad_GPIO_pinConfig.GPIO_PinNumber = Column4_PinNumber;
	MCAL_GPIO_InitPin(Column4_Port, &Keypad_GPIO_pinConfig);

	//Connect all columns to GND (LOW logic)
	MCAL_GPIO_WritePin(Column1_Port, Column1_PinNumber, GPIO_PinValue_LOW);
	MCAL_GPIO_WritePin(Column2_Port, Column2_PinNumber, GPIO_PinValue_LOW);
	MCAL_GPIO_WritePin(Column3_Port, Column3_PinNumber, GPIO_PinValue_LOW);
	MCAL_GPIO_WritePin(Column4_Port, Column4_PinNumber, GPIO_PinValue_LOW);
}
char HAL_Keypad_getPressedKey(void)
{
	uint8_t rowINDEX=0, columnINDEX=0;

	// Loop through each column
	for (columnINDEX = 0; columnINDEX < 4; columnINDEX++)
	{
		//	Connect all columns to GND (LOW logic)
		MCAL_GPIO_WritePin(Column1_Port, Column1_PinNumber, GPIO_PinValue_LOW);
		MCAL_GPIO_WritePin(Column2_Port, Column2_PinNumber, GPIO_PinValue_LOW);
		MCAL_GPIO_WritePin(Column3_Port, Column3_PinNumber, GPIO_PinValue_LOW);
		MCAL_GPIO_WritePin(Column4_Port, Column4_PinNumber, GPIO_PinValue_LOW);

		// Set the current column to HIGH
		MCAL_GPIO_WritePin(Keypad_Column[columnINDEX].Keypad_PinPort, Keypad_Column[columnINDEX].Keypad_PinNumber, GPIO_PinValue_HIGH);

		// Loop through each row
		for (rowINDEX = 0; rowINDEX < 4; rowINDEX++)
		{
			// Check if the current row is LOW (key pressed)
			if (MCAL_GPIO_ReadPin(Keypad_Row[rowINDEX].Keypad_PinPort, Keypad_Row[rowINDEX].Keypad_PinNumber) == GPIO_PinValue_HIGH)
			{
				// Wait until the key is released (debouncing)
				while (MCAL_GPIO_ReadPin(Keypad_Row[rowINDEX].Keypad_PinPort, Keypad_Row[rowINDEX].Keypad_PinNumber) == GPIO_PinValue_HIGH)
				{
					/* Wait until Key is released (Single Press Operation). */
				}

				// Determine the pressed key based on the row and column
				switch (columnINDEX)
				{

				case 0:
				{
					switch (rowINDEX)
					{
					case 0: return '7';
					case 1: return '4';
					case 2: return '1';
					case 3: return '?';
					}
					break;
				}

				case 1:
				{
					switch (rowINDEX)
					{
					case 0: return '8';
					case 1: return '5';
					case 2: return '2';
					case 3: return '0';
					}
					break;
				}

				case 2:
				{
					switch (rowINDEX)
					{
					case 0: return '9';
					case 1: return '6';
					case 2: return '3';
					case 3: return '=';
					}
					break;
				}

				case 3:
				{
					switch (rowINDEX)
					{
					case 0: return '/';
					case 1: return '*';
					case 2: return '-';
					case 3: return '+';
					}
					break;
				}

				}
			}
		}
	}

	// Return 'N' if no key is pressed.
	return 'N';
}

