/*
 * LCD2x16.h
 *
 *  Created on: Sep 24, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_LCD2X16_H_
#define INC_LCD2X16_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"
#include "STM32F103x6_GPTIM_Driver.h"


//==========================================================================================
//============================User Hardware Connection Schematic============================
//==========================================================================================
#define DB0_Port            GPIOA  				// Port for Data Bit 0
#define DB0_PinNumber       GPIO_PinNumber_0  	// Pin number for Data Bit 0

#define DB1_Port            GPIOA  				// Port for Data Bit 1
#define DB1_PinNumber       GPIO_PinNumber_1  	// Pin number for Data Bit 1

#define DB2_Port            GPIOB  				// Port for Data Bit 2
#define DB2_PinNumber       GPIO_PinNumber_8  	// Pin number for Data Bit 2

#define DB3_Port            GPIOA  				// Port for Data Bit 3
#define DB3_PinNumber       GPIO_PinNumber_14  	// Pin number for Data Bit 3

#define DB4_Port            GPIOA  				// Port for Data Bit 4
#define DB4_PinNumber       GPIO_PinNumber_4  	// Pin number for Data Bit 4

#define DB5_Port            GPIOA  				// Port for Data Bit 5
#define DB5_PinNumber       GPIO_PinNumber_11  	// Pin number for Data Bit 5

#define DB6_Port            GPIOA  				// Port for Data Bit 6
#define DB6_PinNumber       GPIO_PinNumber_12  	// Pin number for Data Bit 6

#define DB7_Port            GPIOA  				// Port for Data Bit 7
#define DB7_PinNumber       GPIO_PinNumber_13  	// Pin number for Data Bit 7

#define RS_Port             GPIOA  				// Port for Register Select (RS) pin
#define RS_PinNumber        GPIO_PinNumber_8  	// Pin number for Register Select (RS) pin

#define RW_Port             GPIOB  				// Port for Read/Write (R/W) pin
#define RW_PinNumber        GPIO_PinNumber_9  	// Pin number for Read/Write (R/W) pin

#define EN_Port             GPIOA  				// Port for Enable (E) pin
#define EN_PinNumber        GPIO_PinNumber_15  	// Pin number for Enable (E) pin

//==========================================================================================
//===============================User Initialization Options================================
//==========================================================================================

// Function set: 0 0 1 DL N F 0 0
#define FunctionSet_8Bit_1Line_5x8Dots      			0x30  // DL = 1, N = 0, F = 0 -----> 0b 0011 0000 [0x30]
#define FunctionSet_8Bit_1Line_5x10Dots     			0x34  // DL = 1, N = 0, F = 1 -----> 0b 0011 0100 [0x34]
#define FunctionSet_8Bit_2Line_5x8Dots      			0x38  // DL = 1, N = 1, F = 0 -----> 0b 0011 1000 [0x38]
#define FunctionSet_8Bit_2Line_5x10Dots     			0x3C  // DL = 1, N = 1, F = 1 -----> 0b 0011 1100 [0x3C]
#define FunctionSet_4Bit_1Line_5x8Dots      			0x20  // DL = 0, N = 0, F = 0 -----> 0b 0010 0000 [0x20]
#define FunctionSet_4Bit_1Line_5x10Dots     			0x24  // DL = 0, N = 0, F = 1 -----> 0b 0010 0100 [0x24]
#define FunctionSet_4Bit_2Line_5x8Dots      			0x28  // DL = 0, N = 1, F = 0 -----> 0b 0010 1000 [0x28]
#define FunctionSet_4Bit_2Line_5x10Dots     			0x2C  // DL = 0, N = 1, F = 1 -----> 0b 0010 1100 [0x2C]

// Display on/off control: 0 0 0 0 1 D C B
#define DisplayControl_DisplayOff_CursorOff_BlinkOff    0x08  // D = 0, C = 0, B = 0 -----> 0b 0000 1000 [0x08]
#define DisplayControl_DisplayOn_CursorOff_BlinkOff     0x0C  // D = 1, C = 0, B = 0 -----> 0b 0000 1100 [0x0C]
#define DisplayControl_DisplayOn_CursorOn_BlinkOff      0x0E  // D = 1, C = 1, B = 0 -----> 0b 0000 1110 [0x0E]
#define DisplayControl_DisplayOn_CursorOn_BlinkOn       0x0F  // D = 1, C = 1, B = 1 -----> 0b 0000 1111 [0x0F]

//Entry mode set: 0 0 0 0 0 1 I/D S
#define EntryModeSet_DisplayShiftOff_CursorIncrement		0x06  	// S = 0, I/D = 1 -----> 0b 0000 0110 [0x06]
#define EntryModeSet_DisplayShiftOff_CursorDecrement		0x04  	// S = 0, I/D = 0 -----> 0b 0000 0100 [0x04]
#define EntryModeSet_DisplayShiftOn_CursorIncrement			0x07  	// S = 1, I/D = 1 -----> 0b 0000 0111 [0x07]
#define EntryModeSet_DisplayShiftOn_CursorDecrement			0x05  	// S = 1, I/D = 0 -----> 0b 0000 0101 [0x05]

//==========================================================================================
//==================================User Instructions Set===================================
//==========================================================================================

// Cursor or display shift: 0 0 0 0 0 1 S/C R/L
#define CursorOrDisplayShift_CursorMove_Left          	0x10  // S/C = 0, R/L = 0 -----> 0b 0001 0000 [0x10]
#define CursorOrDisplayShift_CursorMove_Right        	0x14  // S/C = 0, R/L = 1 -----> 0b 0001 0100 [0x14]
#define CursorOrDisplayShift_DisplayShift_Left        	0x18  // S/C = 1, R/L = 0 -----> 0b 0001 1000 [0x18]
#define CursorOrDisplayShift_DisplayShift_Right       	0x1C  // S/C = 1, R/L = 1 -----> 0b 0001 1100 [0x1C]

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================
typedef enum
{
	UnsignedInteger 	= 0U,
	SignedInteger 		= 1U,
}LCD_IntegerType_ENUM_t;

//==========================================================================================
//===========================APIs Supported by "MCAL USART DRIVER"==========================
//==========================================================================================

/**==================================================
 * @brief  Initializes the HD44780U-based LCD.
 *
 * This function sets up the LCD with specific configurations for its operation.
 * It configures the data bus pins, control pins (RS, RW, EN), and initializes
 * the LCD with user-defined settings for function set, display control,
 * and entry mode set.
 *
 * @param  FunctionSet    Configuration for number of data bus pins, number of display lines and character font size.
 *                        Must be one of the predefined 'FunctionSet' macros (e.g., FunctionSet_8Bit_2Line_5x8Dots).
 *
 * @param  DisplayControl Configuration for turning on/off the display and cursor,
 *                        setting blink functionality. Must be one of the predefined
 *                        'DisplayControl' macros (e.g., DisplayControl_DisplayOn_CursorOff_BlinkOff).
 *
 * @param  EntryModeSet   Configuration for text direction (left-to-right or right-to-left)
 *                        and specifies whether or not to shift the display. Must be one
 *                        of the predefined 'EntryModeSet' macros (e.g., EntryModeSet_DisplayShiftOff_CursorIncrement).
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Copies user initialization options to LCD_globeConfig and initializes NextDisplayPosition.
 *           - Configures all pins of GPIO connected to LCD as output.
 *           - Initializes the LCD by instructions based on the function set (8-bit or 4-bit mode).
 *           - Initializes the LCD with the user initialization options.
 *
 * @example
 * HAL_LCD_Init(FunctionSet_8Bit_2Line_5x8Dots, DisplayControl_DisplayOn_CursorOn_BlinkOff, EntryModeSet_DisplayShiftOff_CursorIncrement);
 * // Initializes the LCD with the specified configuration
 */
void HAL_LCD_Init(uint8_t FunctionSet, uint8_t DisplayControl, uint8_t EntryModeSet);

/**==================================================
 * @brief  Sends a character to the LCD.
 *
 * This function sends a single character to the LCD. It ensures that the LCD is not busy
 * before sending the character and adjusts the cursor and display position accordingly.
 *
 * @param  Char  The character to be sent to the LCD.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Checks the busy flag to ensure the LCD is ready.
 *           - Adjusts the cursor and display position for the next writing operation.
 *           - Writes the character to the data bus and handles display clearing if necessary.
 *
 * @example
 * char myChar = 'A';
 * HAL_LCD_SendChar(myChar);
 * // Sends the character 'A' to the LCD
 */
void HAL_LCD_SendChar(char Char);

/**==================================================
 * @brief  Sends a string to the LCD.
 *
 * This function sends a null-terminated string to the LCD. It sends each character
 * in the string one by one until the null terminator is encountered.
 *
 * @param  String  Pointer to the null-terminated string to be sent to the LCD.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Iterates through each character in the string.
 *           - Sends each character to the LCD using HAL_LCD_SendChar.
 *
 * @example
 * char myString[] = "Hello, World!";
 * HAL_LCD_SendString(myString);
 * // Sends the string "Hello, World!" to the LCD
 */
void HAL_LCD_SendString(char* String);

/**==================================================
 * @brief  Clears the LCD display.
 *
 * This function sends the clear display instruction to the LCD and waits until the
 * operation is complete. It also resets the next display position to the beginning.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Sends the clear display instruction to the LCD.
 *           - Waits for the clear display operation to complete by checking the busy flag.
 *           - Resets the next display position to the beginning.
 *
 * @example
 * HAL_LCD_ClearDisplay();
 * // Clears the LCD display and resets the next display position
 */
void HAL_LCD_ClearDisplay();

/**==================================================
 * @brief  Sends an instruction to the HD44780U-based LCD.
 *
 * This function sends a specific instruction to the LCD. It ensures that the LCD
 * is not busy before sending the instruction and updates the global configuration
 * if the instruction is related to function set, display control, or entry mode set.
 *
 * @param  Instruction  The instruction to be sent to the LCD. Must be one of the predefined
 *                      user instruction set macros (e.g., CursorOrDisplayShift_DisplayShift_Left).
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Checks the busy flag to ensure the LCD is ready.
 *           - Writes the instruction to the data bus.
 *           - Updates the global configuration if the instruction is FunctionSet, DisplayControl, or EntryModeSet.
 *
 *
 * @example
 * HAL_LCD_SendInstruction(CursorOrDisplayShift_DisplayShift_Left);
 * // Sends the specified instruction to the LCD
 */
void HAL_LCD_SendInstruction(uint8_t Instruction);

/**==================================================
 * @brief  Moves the cursor to a specified position on the LCD.
 *
 * This function sets the cursor position on an HD44780U-based LCD
 * by calculating the appropriate address based on the given row and column.
 * It then sends the corresponding instruction to the LCD to move the cursor.
 *
 * @param  Row    The row number where the cursor should be moved (1 to 4).
 * @param  Column The column number where the cursor should be moved (1 to 16 or 20, depending on the LCD).
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Calculates the address based on the specified row and column.
 *           - Constructs the instruction to set the DDRAM address.
 *           - Sends the instruction to the LCD to move the cursor.
 *
 * @example
 * HAL_LCD_MoveCursor(2, 5);
 * // Moves the cursor to the 5th column of the 2nd row on the LCD
 */
void HAL_LCD_MoveCursor(uint8_t Row, uint8_t Column);

/**==================================================
 * @brief  Sends an integer to the LCD.
 *
 * This function converts an integer to a string and sends it to the LCD.
 * It supports both signed and unsigned integers based on the specified type.
 *
 * @param  IntegerNumber The integer number to be sent to the LCD.
 * @param  IntegerType   The type of the integer (SignedInteger or UnsignedInteger).
 *                       Must be one of the predefined 'LCD_IntegerType_ENUM_t' values.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Converts the integer to a string based on the specified type.
 *           - Sends the resulting string to the LCD.
 *
 * @example
 * HAL_LCD_SendInteger(12345, UnsignedInteger);
 * // Sends the unsigned integer 12345 to the LCD
 */
void HAL_LCD_SendInteger(int IntegerNumber, LCD_IntegerType_ENUM_t IntegerType);

/**==================================================
 * @brief  Sends a floating-point number to the LCD.
 *
 * This function converts a floating-point number to a string with a specified
 * number of fractional digits and sends it to the LCD.
 *
 * @param  FloatNumber The floating-point number to be sent to the LCD.
 * @param  Fractions   The number of digits to display after the decimal point.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Determines the sign of the number.
 *           - Separates the number into its integer and fractional parts.
 *           - Converts the number to a string with the specified number of fractional digits.
 *           - Sends the resulting string to the LCD.
 *
 * @example
 * HAL_LCD_SendFloat(-123.456, 2);
 * // Sends the floating-point number -123.46 to the LCD (rounded to 2 decimal places)
 */
void HAL_LCD_SendFloat(float FloatNumber, uint8_t Fractions);


#endif /* INC_LCD2X16_H_ */
