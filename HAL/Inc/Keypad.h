/*
 * Keypad.h
 *
 *  Created on: Oct 17, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"


//==========================================================================================
//============================User Hardware Connection Schematic============================
//==========================================================================================

// Row A to Row D
#define RowA_Port           GPIOB               // Port for Row A
#define RowA_PinNumber      GPIO_PinNumber_0    // Pin number for Row A

#define RowB_Port           GPIOB               // Port for Row B
#define RowB_PinNumber      GPIO_PinNumber_1    // Pin number for Row B

#define RowC_Port           GPIOB               // Port for Row C
#define RowC_PinNumber      GPIO_PinNumber_2    // Pin number for Row C

#define RowD_Port           GPIOB               // Port for Row D
#define RowD_PinNumber      GPIO_PinNumber_3    // Pin number for Row D

// Column 1 to Column 4
#define Column1_Port        GPIOB               // Port for Column 1
#define Column1_PinNumber   GPIO_PinNumber_4    // Pin number for Column 1

#define Column2_Port        GPIOB               // Port for Column 2
#define Column2_PinNumber   GPIO_PinNumber_5    // Pin number for Column 2

#define Column3_Port        GPIOB               // Port for Column 3
#define Column3_PinNumber   GPIO_PinNumber_6    // Pin number for Column 3

#define Column4_Port        GPIOB               // Port for Column 4
#define Column4_PinNumber   GPIO_PinNumber_7    // Pin number for Column 4

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================


//==========================================================================================
//===========================APIs Supported by "MCAL USART DRIVER"==========================
//==========================================================================================

/**==================================================
 * @brief  Initializes the keypad GPIO pins.
 *
 * This function configures the GPIO pins connected to a keypad. It sets the row pins as inputs
 * floating and the column pins as outputs with push-pull configuration. It also
 * initializes the columns to a low logic level.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Configures the row pins as input floating.
 *           - Configures the column pins as output push-pull with a speed of 2 MHz.
 *           - Sets all column pins to a low logic level.
 *
 * @example
 * HAL_Keypad_Init();
 * // Initializes the GPIO pins connected to the keypad
 */
void HAL_Keypad_Init(void);

/**==================================================
 * @brief  Gets the pressed key from the keypad.
 *
 * This function scans the keypad (by polling mechanism) to detect which key is pressed. It sets each column
 * to high one by one and checks the rows to determine the pressed key. It handles
 * debouncing by waiting until the key is released before returning the key value.
 *
 * @retval char     The character representing the pressed key. Returns 'N' if no key is pressed.
 *
 * @note
 *         - The function performs the following steps:
 *           - Sets all columns to low.
 *           - Sets each column to high one by one and checks each row.
 *           - If a key press is detected, waits until the key is released (debouncing).
 *           - Determines the pressed key based on the row and column indices.
 *
 * @example
 * char key = HAL_Keypad_getPressedKey();
 * // key will contain the character of the pressed key or 'N' if no key is pressed
 */
char HAL_Keypad_getPressedKey(void);

#endif /* INC_KEYPAD_H_ */
