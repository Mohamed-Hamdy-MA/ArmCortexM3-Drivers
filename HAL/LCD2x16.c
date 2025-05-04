/*
 * LCD2x16.c
 *
 *  Created on: Sep 24, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "LCD2x16.h"

//==========================================================================================
//==============================LCD Control BitField Definition=============================
//==========================================================================================

// Function set: 0 0 1 DL N F 0 0
#define LCD_FunctionSet_DL_POSITION            ((uint8_t) 4)
#define LCD_FunctionSet_DL_MASK                ((uint8_t) 0x1 << LCD_FunctionSet_DL_POSITION)  // 0b 0001 0000
#define LCD_FunctionSet_DL                     I2C_CR1_PE_MASK  // Bit 4 DL: Data Length

#define LCD_FunctionSet_N_POSITION             ((uint8_t) 3)
#define LCD_FunctionSet_N_MASK                 ((uint8_t) 0x1 << LCD_FunctionSet_N_POSITION)   // 0b 0000 1000
#define LCD_FunctionSet_N                      I2C_CR1_PE_MASK  // Bit 3 N: Number of Lines

#define LCD_FunctionSet_F_POSITION             ((uint8_t) 2)
#define LCD_FunctionSet_F_MASK                 ((uint8_t) 0x1 << LCD_FunctionSet_F_POSITION)   // 0b 0000 0100
#define LCD_FunctionSet_F                      I2C_CR1_PE_MASK  // Bit 2 F: Font Size

// Display control: 0 0 0 0 1 D C B
#define LCD_DisplayControl_D_POSITION          ((uint8_t) 2)
#define LCD_DisplayControl_D_MASK              ((uint8_t) 0x1 << LCD_DisplayControl_D_POSITION)  // 0b 0000 0100
#define LCD_DisplayControl_D                   I2C_CR1_PE_MASK  // Bit 2 D: Display On/Off

#define LCD_DisplayControl_C_POSITION          ((uint8_t) 1)
#define LCD_DisplayControl_C_MASK              ((uint8_t) 0x1 << LCD_DisplayControl_C_POSITION)  // 0b 0000 0010
#define LCD_DisplayControl_C                   I2C_CR1_PE_MASK  // Bit 1 C: Cursor On/Off

#define LCD_DisplayControl_B_POSITION          ((uint8_t) 0)
#define LCD_DisplayControl_B_MASK              ((uint8_t) 0x1 << LCD_DisplayControl_B_POSITION)  // 0b 0000 0001
#define LCD_DisplayControl_B                   I2C_CR1_PE_MASK  // Bit 0 B: Blinking On/Off

// Entry mode set: 0 0 0 0 0 1 I/D S
#define LCD_EntryModeSet_ID_POSITION           ((uint8_t) 1)
#define LCD_EntryModeSet_ID_MASK               ((uint8_t) 0x1 << LCD_EntryModeSet_ID_POSITION)  // 0b 0000 0010
#define LCD_EntryModeSet_ID                    I2C_CR1_PE_MASK  // Bit 1 I/D: Increment/Decrement

#define LCD_EntryModeSet_S_POSITION            ((uint8_t) 0)
#define LCD_EntryModeSet_S_MASK                ((uint8_t) 0x1 << LCD_EntryModeSet_S_POSITION)   // 0b 0000 0001
#define LCD_EntryModeSet_S                     I2C_CR1_PE_MASK  // Bit 0 S: Display Shift

//==========================================================================================
//=====================================LCD Instructions=====================================
//==========================================================================================

//Clear Display: 0 0 0 0 0 0 0 1
#define ClearDisplay                  			0x01  // Clear Display (also clear DDRAM content)

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================
typedef enum
{
	OUTPUT 		= 0U,
	INPUT 		= 1U,
}LCD_GPIO_DB_ENUM_t;
typedef enum
{
	CLEARED 		= 0U,
	SET 			= 1U,
	UNDETERMINED	= 2U,
}LCD_flagStatus_ENUM_t;
typedef enum
{
	//----------Busy----------
	LCD_Busy,       			// BF = 1: Internally operating, BF = 0: Instructions acceptable

	//----------Function set----------
	LCD_DataLong,       		// DL = 1: 8 bits, DL = 0: 4 bits
	LCD_NoOfLines,     			// N = 1: 2 lines, N = 0: 1 line
	LCD_FontSize,      			// F = 1: 5 × 10 dots, F = 0: 5 × 8 dots

	//----------DisplayControl----------
	LCD_DisplayOnOff,       	// D = 1: Display On, D = 0: Display Off
	LCD_CursorOnOff,     		// C = 1; Cursor On, C = 0; Cursor Off
	LCD_BlinkingOnOff,      	// B = 1; Blinking On, B = 0; Blinking Off

	//----------EntryModeSet----------
	LCD_IncrementDecrement,		// I/D = 1: Increment, I/D = 0: Decrement
	LCD_DisplayShift,			// S = 1: Accompanies display shift

}LCD_Flag_ENUM_t;
typedef enum
{
	Write_1x8Bits 	= 0U,
	Write_2x4Bits 	= 1U,
	Write_1x4Bits	= 2U,
}LCD_WriteOnDB_ENUM_t;

//==========================================================================================
//=====================================Assisting Macros=====================================
//==========================================================================================
#define FunctionSet_INDEX				0U
#define DisplayControl_INDEX			1U
#define EntryModeSet_INDEX				2U
#define NextDisplayPosition_INDEX		3U


//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================
uint8_t LCD_globeConfig[4];		//Uninitialized global array, so it will be initialized to 0 by default.

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================

/**==================================================
 * @brief  Delays execution for a specified number of microseconds.
 *
 * This function creates a delay of approximately 1 microsecond per unit of time,
 * assuming the MCU is running at an 8 MHz clock. It uses two non-nested loops to achieve
 * the desired delay.
 *
 * @param  time  The number of microseconds to delay.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Uses two non-nested loops to create the delay.
 *           - The delay is calculated based on the assumption that the MCU executes approximately 8 instructions per microsecond.
 *
 * @example
 * uint32_t delayTime = 100;
 * LCD_delay_us(delayTime);
 * // Delays execution for approximately 100 microseconds
 */
void LCD_delay_us(uint32_t time)
{
	/*
	 * This function delay about 1us if time = 1, assuming that MCU work with 8 MHz clock.
	 *
	 * STM32 running at 8 MHz executes approximately 8 instructions per microsecond
	 * (because each instruction takes 125 nanoseconds). Assuming an average of 1 cycle per instruction.
	 * Therefore, we want the total number of instructions executed by the LCD_delay_us function to be 8 instructions when time = 1.
	 *
	 * 4(instructions)×time(iterations)(for the 1st loop) + 4(instructions)×time(iterations)(for the 2nd loop) = Total time=8×time×0.125μs=time×1μs.
	 *
	 */
	uint32_t i , j;
	for (i = 0; i < time; i++)
	{
	}
	for (j = 0; j < time; j++)
	{
	}
}

/**==================================================
 * @brief  Configures the GPIO pins connected to the LCD data bus (DBs pins).
 *
 * This function configures the GPIO pins connected to the LCD data bus (DB0 to DB7)
 * as either input or output based on the specified direction. It handles both 4-bit
 * and 8-bit data interfaces.
 *
 * @param  Direction  Specifies the direction for the GPIO pins (INPUT or OUTPUT).
 *                    Must be one of the predefined 'LCD_GPIO_DB_ENUM_t' values.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Configures the GPIO pins (DB7, DB6, DB5, DB4) based on the specified direction.
 *           - Checks the global configuration to determine if the interface is 8-bit.
 *           - If 8-bit, configures the additional GPIO pins (DB3, DB2, DB1, DB0) based on the specified direction.
 *
 * @example
 * LCD_GPIO_DB_ConfigurePins(INPUT);
 * // Configures the GPIO pins connected to the LCD data bus as input
 */
void LCD_GPIO_DB_ConfigurePins(LCD_GPIO_DB_ENUM_t Direction)
{
	GPIO_PinConfig_t LCD_GPIO_pinConfig;

	if (Direction == INPUT)
	{
		LCD_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
	}
	else
	{
		LCD_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
		LCD_GPIO_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
	}

	//Configure DB7, DB6, DB5 and DB4, then check for the data long bits interface to decide to configure DB3, DB2, DB1 and DB0 or not.
	LCD_GPIO_pinConfig.GPIO_PinNumber = DB7_PinNumber;
	MCAL_GPIO_InitPin(DB7_Port, &LCD_GPIO_pinConfig);

	LCD_GPIO_pinConfig.GPIO_PinNumber = DB6_PinNumber;
	MCAL_GPIO_InitPin(DB6_Port, &LCD_GPIO_pinConfig);

	LCD_GPIO_pinConfig.GPIO_PinNumber = DB5_PinNumber;
	MCAL_GPIO_InitPin(DB5_Port, &LCD_GPIO_pinConfig);

	LCD_GPIO_pinConfig.GPIO_PinNumber = DB4_PinNumber;
	MCAL_GPIO_InitPin(DB4_Port, &LCD_GPIO_pinConfig);

	if ((LCD_globeConfig[FunctionSet_INDEX] & LCD_FunctionSet_DL_MASK) == LCD_FunctionSet_DL_MASK) 		// FunctionSet_8Bit
	{
		LCD_GPIO_pinConfig.GPIO_PinNumber = DB3_PinNumber;
		MCAL_GPIO_InitPin(DB3_Port, &LCD_GPIO_pinConfig);

		LCD_GPIO_pinConfig.GPIO_PinNumber = DB2_PinNumber;
		MCAL_GPIO_InitPin(DB2_Port, &LCD_GPIO_pinConfig);

		LCD_GPIO_pinConfig.GPIO_PinNumber = DB1_PinNumber;
		MCAL_GPIO_InitPin(DB1_Port, &LCD_GPIO_pinConfig);

		LCD_GPIO_pinConfig.GPIO_PinNumber = DB0_PinNumber;
		MCAL_GPIO_InitPin(DB0_Port, &LCD_GPIO_pinConfig);
	}
}

/**==================================================
 * @brief  Retrieves the status of a specified LCD flag.
 *
 * This function checks the status of a specified flag for the HD44780U-based LCD.
 * It can check various flags such as busy state, and any control bit in LCD_globeConfig.
 *
 * @param  Flag  The flag to be checked. Must be one of the predefined flag enums
 *               (e.g., LCD_Busy, LCD_NoOfLines).
 *
 * @retval LCD_flagStatus_ENUM_t  The status of the specified flag (SET, CLEARED or UNDETERMINED).
 *
 * @note   - The function performs the following steps:
 *           - For the busy flag (LCD_Busy):
 *             - Configures all DB GPIO pins connected to LCD as input.
 *             - Enters LCD instruction read mode.
 *             - Enables data read/write and reads the busy flag from DB7.
 *             - If 4-bit interface, repeats the read operation.
 *             - Restores configuration for all DB GPIO pins connected to LCD as output.
 *           - For other flags:
 *             - Retrieves the flag status from the global configuration variable LCD_globeConfig.
 *
 * @example
 * while(LCD_getFLAG(LCD_Busy) == SET)
 * {
 * 		// Wait until LCD is not busy.
 * }
 */
LCD_flagStatus_ENUM_t LCD_getFLAG(LCD_Flag_ENUM_t Flag)
{
	LCD_flagStatus_ENUM_t flagStatus = UNDETERMINED;

	switch (Flag)
	{
	case LCD_Busy:

		//1. Configure all DB GPIO pins connected to LCD as input.
		LCD_GPIO_DB_ConfigurePins(INPUT);

		//2. Enter LCD instruction read mode.
		MCAL_GPIO_WritePin(RS_Port, RS_PinNumber, GPIO_PinValue_LOW);   // RS=0 (Instruction register)
		MCAL_GPIO_WritePin(RW_Port, RW_PinNumber, GPIO_PinValue_HIGH);  // RW=1 (Read)

		//3. Enable data read/write and read busy flag from DB7, then disable data read/write.
		MCAL_GPIO_WritePin(EN_Port, EN_PinNumber, GPIO_PinValue_HIGH);  // EN=1 (Data read/write enabled).
		flagStatus = (MCAL_GPIO_ReadPin(DB7_Port, DB7_PinNumber) == GPIO_PinValue_HIGH) ? SET : CLEARED;
		MCAL_GPIO_WritePin(EN_Port, EN_PinNumber, GPIO_PinValue_LOW);  // EN=0 (Data read/write disabled).

		//4. If 4-bits interface, two cycles are needed for the busy flag check, so repeat read operation.
		if (LCD_getFLAG(LCD_DataLong) == CLEARED) 					// FunctionSet_4Bit
		{
			MCAL_GPIO_WritePin(EN_Port, EN_PinNumber, GPIO_PinValue_HIGH);  // EN=1 (Data read/write enabled).
			MCAL_GPIO_ReadPin(DB7_Port, DB7_PinNumber);						// Return AC3 which is the 3rd bit of the address counter
			MCAL_GPIO_WritePin(EN_Port, EN_PinNumber, GPIO_PinValue_LOW);  // EN=0 (Data read/write disabled).
		}

		//5. Configuration restore for all DB GPIO pins connected to LCD as output & write mode.
		LCD_GPIO_DB_ConfigurePins(OUTPUT);
		MCAL_GPIO_WritePin(RW_Port, RW_PinNumber, GPIO_PinValue_LOW);  // RW=0 (Write)

		return flagStatus;

		//----------Function set----------
	case LCD_DataLong:
		flagStatus = (LCD_globeConfig[FunctionSet_INDEX] & LCD_FunctionSet_DL_MASK) == LCD_FunctionSet_DL_MASK ? SET : CLEARED;
		return flagStatus;

	case LCD_NoOfLines:
		flagStatus = (LCD_globeConfig[FunctionSet_INDEX] & LCD_FunctionSet_N_MASK) == LCD_FunctionSet_N_MASK ? SET : CLEARED;
		return flagStatus;

	case LCD_FontSize:
		flagStatus = (LCD_globeConfig[FunctionSet_INDEX] & LCD_FunctionSet_F_MASK) == LCD_FunctionSet_F_MASK ? SET : CLEARED;
		return flagStatus;

		//----------DisplayControl----------
	case LCD_DisplayOnOff:
		flagStatus = (LCD_globeConfig[DisplayControl_INDEX] & LCD_DisplayControl_D_MASK) == LCD_DisplayControl_D_MASK ? SET : CLEARED;
		return flagStatus;

	case LCD_CursorOnOff:
		flagStatus = (LCD_globeConfig[DisplayControl_INDEX] & LCD_DisplayControl_C_MASK) == LCD_DisplayControl_C_MASK ? SET : CLEARED;
		return flagStatus;

	case LCD_BlinkingOnOff:
		flagStatus = (LCD_globeConfig[DisplayControl_INDEX] & LCD_DisplayControl_B_MASK) == LCD_DisplayControl_B_MASK ? SET : CLEARED;
		return flagStatus;

		//----------EntryModeSet----------
	case LCD_IncrementDecrement:
		flagStatus = (LCD_globeConfig[EntryModeSet_INDEX] & LCD_EntryModeSet_ID_MASK) == LCD_EntryModeSet_ID_MASK ? SET : CLEARED;
		return flagStatus;

	case LCD_DisplayShift:
		flagStatus = (LCD_globeConfig[EntryModeSet_INDEX] & LCD_EntryModeSet_S_MASK) == LCD_EntryModeSet_S_MASK ? SET : CLEARED;
		return flagStatus;

	default:
		return UNDETERMINED;
	}
}

/**==================================================
 * @brief  Generates an enable pulse to latch data into the LCD.
 *
 * This function generates a high-to-low pulse on the enable pin (EN) to latch
 * data into the LCD. It ensures the correct timing for the enable pulse.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Sets the enable pin high to enable data read/write.
 *           - Delays for the enable pulse width (high level).
 *           - Sets the enable pin low to disable data read/write.
 *           - Delays to complete the enable cycle time.
 *
 * @example
 * LCD_EN_Kick();
 * // Generates an enable pulse to latch data into the LCD
 */
void LCD_EN_Kick(void)
{
	MCAL_GPIO_WritePin(EN_Port, EN_PinNumber, GPIO_PinValue_HIGH);  // EN=1 (Data read/write enabled).

	//Delay Enable pulse width (high level) PWEH 450 ns.
	MCAL_TIM_Delay_us(TIM2, 1);

	MCAL_GPIO_WritePin(EN_Port, EN_PinNumber, GPIO_PinValue_LOW);  // EN=0 (Data read/write disabled).

	//Delay (low level) to complete Enable cycle time tcycE 1000 ns.
	MCAL_TIM_Delay_us(TIM2, 1);
}

/**==================================================
 * @brief  Writes data to the LCD data bus.
 *
 * This function writes a byte of data to the LCD data bus. It handles both 4-bit
 * and 8-bit data interfaces, ensuring the correct sequence of operations.
 *
 * @param  Data  The data byte to be written to the LCD.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Checks the data length flag to determine if the interface is 8-bit or 4-bit.
 *           - For 8-bit interface:
 *             - Writes the entire byte to the data bus in a single operation (DB7..DB0).
 *             - Triggers the enable pin to latch the data.
 *           - For 4-bit interface:
 *             - Writes the higher nibble (4 bits) to the data bus (DB7..DB4).
 *             - Triggers the enable pin to latch the data.
 *             - If the operation is Write_2x4Bits, it writes the lower nibble (4 bits) to the data bus (DB7..DB4).
 *             - Triggers the enable pin to latch the data.
 *
 * @example
 * uint8_t data = 0x41;	//ASCII A character
 * LCD_WriteOnDB(data, Write_1x8Bits);
 * // Writes the data 0x41 to the LCD data bus to display 'A' character
 */
void LCD_WriteOnDB(uint8_t Data, LCD_WriteOnDB_ENUM_t Operation )
{
	if (LCD_getFLAG(LCD_DataLong) == SET && Operation == Write_1x8Bits) 			// FunctionSet_8Bit -----> 1 write operation on 8 bits.
	{
		MCAL_GPIO_WritePin(DB7_Port, DB7_PinNumber, ((Data & 0x80) >> 7));  // Bit7 mask then shift 7 right = LSB 0 (LOW) or 1 (HIGH).
		MCAL_GPIO_WritePin(DB6_Port, DB6_PinNumber, ((Data & 0x40) >> 6));  // Bit6 mask then shift 6 right = LSB 0 (LOW) or 1 (HIGH).
		MCAL_GPIO_WritePin(DB5_Port, DB5_PinNumber, ((Data & 0x20) >> 5));  // Bit5 mask then shift 5 right = LSB 0 (LOW) or 1 (HIGH)
		MCAL_GPIO_WritePin(DB4_Port, DB4_PinNumber, ((Data & 0x10) >> 4));  // Bit4 mask then shift 4 right = LSB 0 (LOW) or 1 (HIGH)
		MCAL_GPIO_WritePin(DB3_Port, DB3_PinNumber, ((Data & 0x08) >> 3));  // Bit3 mask then shift 3 right = LSB 0 (LOW) or 1 (HIGH)
		MCAL_GPIO_WritePin(DB2_Port, DB2_PinNumber, ((Data & 0x04) >> 2));  // Bit2 mask then shift 2 right = LSB 0 (LOW) or 1 (HIGH)
		MCAL_GPIO_WritePin(DB1_Port, DB1_PinNumber, ((Data & 0x02) >> 1));  // Bit1 mask then shift 1 right = LSB 0 (LOW) or 1 (HIGH)
		MCAL_GPIO_WritePin(DB0_Port, DB0_PinNumber, (Data & 0x01));  	   	// Bit0 mask = LSB 0 (LOW) or 1 (HIGH)

		LCD_EN_Kick();
	}
	else								 			// FunctionSet_4Bit
	{
		MCAL_GPIO_WritePin(DB7_Port, DB7_PinNumber, ((Data & 0x80) >> 7));  // Bit7 mask then shift 7 right = LSB 0 (LOW) or 1 (HIGH).
		MCAL_GPIO_WritePin(DB6_Port, DB6_PinNumber, ((Data & 0x40) >> 6));  // Bit6 mask then shift 6 right = LSB 0 (LOW) or 1 (HIGH).
		MCAL_GPIO_WritePin(DB5_Port, DB5_PinNumber, ((Data & 0x20) >> 5));  // Bit5 mask then shift 5 right = LSB 0 (LOW) or 1 (HIGH)
		MCAL_GPIO_WritePin(DB4_Port, DB4_PinNumber, ((Data & 0x10) >> 4));  // Bit4 mask then shift 4 right = LSB 0 (LOW) or 1 (HIGH)

		LCD_EN_Kick();

		if (Operation == Write_2x4Bits)
		{
			MCAL_GPIO_WritePin(DB7_Port, DB7_PinNumber, ((Data & 0x08) >> 3));  // Bit3 mask then shift 3 right = LSB 0 (LOW) or 1 (HIGH)
			MCAL_GPIO_WritePin(DB6_Port, DB6_PinNumber, ((Data & 0x04) >> 2));  // Bit2 mask then shift 2 right = LSB 0 (LOW) or 1 (HIGH)
			MCAL_GPIO_WritePin(DB5_Port, DB5_PinNumber, ((Data & 0x02) >> 1));  // Bit1 mask then shift 1 right = LSB 0 (LOW) or 1 (HIGH)
			MCAL_GPIO_WritePin(DB4_Port, DB4_PinNumber, (Data & 0x01));  	   	// Bit0 mask = LSB 0 (LOW) or 1 (HIGH)

			LCD_EN_Kick();
		}

	}
}

/**==================================================
 * @brief  Adjusts the next display position on the LCD.
 *
 * This function updates the next display position on the LCD. It handles the transition
 * between lines and clears the display when necessary.
 *
 * @retval None
 *
 * @note   - The function performs the following steps:
 *           - Increments the next display position if it is not at the end of a line.
 *           - If the next display position is at the end of the first line:
 *             - Checks if the LCD has two lines.
 *             - Moves the cursor to the beginning of the second line if the LCD has two lines.
 *             - Clears the display if the LCD has only one line.
 *           - If the next display position is at the end of the second line, clears the display.
 *
 * @example
 * LCD_AdjustNextDisplayPosition();
 * // Adjusts the next display position on the LCD
 */
void LCD_AdjustNextDisplayPosition(void)
{
	if (LCD_globeConfig[NextDisplayPosition_INDEX] != 17 && LCD_globeConfig[NextDisplayPosition_INDEX] != 33)
	{
		LCD_globeConfig[NextDisplayPosition_INDEX]++;
	}

	else if (LCD_globeConfig[NextDisplayPosition_INDEX] == 17)
	{
		if (LCD_getFLAG(LCD_NoOfLines) == 1) 			//N = 1: 2 lines
		{
			HAL_LCD_MoveCursor(2, 1);
			LCD_globeConfig[NextDisplayPosition_INDEX]++;
		}
		else 											//N = 0: 1 line
		{
			HAL_LCD_ClearDisplay();
		}
	}

	else if (LCD_globeConfig[NextDisplayPosition_INDEX] == 33)
	{
		HAL_LCD_ClearDisplay();
	}
}

/**==================================================
 * @brief  Initializes the LCD by instructions in 4-bit mode.
 *
 * This function initializes an HD44780U-based LCD to operate in 4-bit mode.
 * It sends the necessary instructions to the LCD to set it up for 4-bit communication.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Waits for the required power-up delay.
 *           - Sends the initialization instructions to the LCD.
 *           - Waits for the LCD to be ready before proceeding.
 *
 * @example
 * LCD_4Bits_InstructionsInitialization();
 * // Initializes the LCD to operate in 4-bit mode
 */
void LCD_4Bits_InstructionsInitialization(void)
{
	MCAL_TIM_Delay_ms(TIM2, 50);			//Wait for more than 40 ms after VCC rises to 2.7 V
	LCD_WriteOnDB(0x30, Write_1x4Bits);

	MCAL_TIM_Delay_ms(TIM2, 5);				//Wait for more than 4.1 ms
	LCD_WriteOnDB(0x30, Write_1x4Bits);

	MCAL_TIM_Delay_us(TIM2, 150);			//Wait for more than 100 µs
	LCD_WriteOnDB(0x30, Write_1x4Bits);

	while(LCD_getFLAG(LCD_Busy) == SET)
	{
		/*
		 * Wait until LCD is not busy.
		 * */
	}
	LCD_WriteOnDB(0x20, Write_1x4Bits);
}

/**==================================================
 * @brief  Initializes the LCD by instructions in 8-bit mode.
 *
 * This function initializes an HD44780U-based LCD to operate in 8-bit mode.
 * It sends the necessary instructions to the LCD to set it up for 8-bit communication.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Waits for the required power-up delay.
 *           - Sends the initialization instructions to the LCD.
 *           - Waits for the LCD to be ready before proceeding.
 *
 * @example
 * LCD_8Bits_InstructionsInitialization();
 * // Initializes the LCD to operate in 8-bit mode
 */
void LCD_8Bits_InstructionsInitialization(void)
{
	MCAL_TIM_Delay_ms(TIM2, 50);			//Wait for more than 40 ms after VCC rises to 2.7 V
	LCD_WriteOnDB(0x30, Write_1x8Bits);

	MCAL_TIM_Delay_ms(TIM2, 5);				//Wait for more than 4.1 ms
	LCD_WriteOnDB(0x30, Write_1x8Bits);

	MCAL_TIM_Delay_us(TIM2, 150);			//Wait for more than 100 µs
	LCD_WriteOnDB(0x30, Write_1x8Bits);

	while(LCD_getFLAG(LCD_Busy) == SET)
	{
		/*
		 * Wait until LCD is not busy.
		 * */
	}
}

/**==================================================
 * @brief  Calculates the power of a base raised to an exponent.
 *
 * This function computes the result of raising a base number to a specified exponent.
 * It handles both positive and negative integer exponents. For negative exponents,
 * it returns the reciprocal of the base raised to the absolute value of the exponent.
 *
 * @param  base     The base number to be raised.
 * @param  exponent The exponent to which the base is raised. Can be positive or negative.
 *
 * @retval int      The result of base raised to the power of exponent. For negative exponents,
 *                  the function returns the reciprocal of the result as an integer.
 *
 * @note
 *         - The function performs the following steps:
 *           - Converts the exponent to a positive value if it is negative.
 *           - Iteratively multiplies the result by the base for the number of times specified by the positive exponent.
 *           - If the original exponent was negative, returns the reciprocal of the result.
 *
 * @example
 * int result = power(2, 3);
 * // result will be 8 (2^3)
 *
 * int result = power(2, -3);
 * // result will be 0 (1/8 as an integer)
 */
int power(int base, int exponent)
{
	int result = 1;
	int positiveExponent = (exponent < 0) ? -exponent : exponent;

	for (int i = 0; i < positiveExponent; i++)
	{
		result *= base;
	}

	if (exponent < 0)
	{
		return 1 / result; // Handle negative exponents
	}

	return result;
}


//==========================================================================================
//===========================APIs Supported by HAL EEPROM DRIVER============================
//==========================================================================================
void HAL_LCD_Init(uint8_t FunctionSet, uint8_t DisplayControl, uint8_t EntryModeSet)
{
	/*
	 * If the electrical characteristics conditions listed under the table Power Supply Conditions Using
	 * Internal Reset Circuit are not met, the internal reset circuit will not operate normally and will fail to
	 * initialize the HD44780U. For such a case, initialization must be performed by the MPU as
	 * explained in the section, Initializing by Instruction.
	 *
	 *
	 * An internal reset circuit automatically initializes the HD44780U when the power is turned on. The
	 * following instructions are executed during the initialization. The busy flag (BF) is kept in the busy state
	 * until the initialization ends (BF = 1). The busy state lasts for 10 ms after VCC rises to 4.5 V.
	 *
	 * Power Supply Conditions Using Internal Reset Circuit
	 * Item                        Symbol       Min     Typ     Max     Unit
	 * Power supply rise time      t_rCC        0.1     —       10      ms
	 * Power supply off time       t_OFF        1       —       —       ms
	 *
	 */

	//==========STEP1: Copy user initialization options to LCD_globeConfig & Initialize NextDisplayPosition==========
	LCD_globeConfig[FunctionSet_INDEX] 		= FunctionSet;
	LCD_globeConfig[DisplayControl_INDEX] 	= DisplayControl;
	LCD_globeConfig[EntryModeSet_INDEX] 	= EntryModeSet;
	LCD_globeConfig[NextDisplayPosition_INDEX] 	= 1U;

	//==========STEP2: Configure all pins of GPIO connected to LCD as output==========

	LCD_GPIO_DB_ConfigurePins(OUTPUT);

	GPIO_PinConfig_t LCD_GPIO_RS_RW_EN_pinConfig;

	LCD_GPIO_RS_RW_EN_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
	LCD_GPIO_RS_RW_EN_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;

	LCD_GPIO_RS_RW_EN_pinConfig.GPIO_PinNumber = RS_PinNumber;
	MCAL_GPIO_InitPin(RS_Port, &LCD_GPIO_RS_RW_EN_pinConfig);

	LCD_GPIO_RS_RW_EN_pinConfig.GPIO_PinNumber = RW_PinNumber;
	MCAL_GPIO_InitPin(RW_Port, &LCD_GPIO_RS_RW_EN_pinConfig);

	LCD_GPIO_RS_RW_EN_pinConfig.GPIO_PinNumber = EN_PinNumber;
	MCAL_GPIO_InitPin(EN_Port, &LCD_GPIO_RS_RW_EN_pinConfig);
	//By default RS=0 (Instruction register), RW=0 (Write), EN=0 (Data read/write disabled), DB (Output).

	//==========STEP3: Initializing the LCD by Instructions==========

	if (LCD_getFLAG(LCD_DataLong) == SET) 			// FunctionSet_8Bit
	{
		LCD_8Bits_InstructionsInitialization();
	}
	else 											// FunctionSet_4Bit
	{
		LCD_4Bits_InstructionsInitialization();
	}

	//==========STEP4: Initializing the LCD with the user initialization options==========

	//Busy flag is checked in HAL_LCD_SendInstruction function.
	HAL_LCD_SendInstruction(FunctionSet);
	HAL_LCD_SendInstruction(DisplayControl);
	HAL_LCD_SendInstruction(EntryModeSet);
}
void HAL_LCD_SendChar(char Char)
{
	//==========STEP1: Check Busy Flag==========
	while(LCD_getFLAG(LCD_Busy) == SET)
	{
		/*
		 * Wait until LCD is not busy.
		 * */
	}
	//In this stage RS=0 (Instruction register), RW=0 (Write), EN=0 (Data read/write disabled), DB (Output).

	//==========STEP2: Adjust cursor & display position for next writing operation (Character)==========

	LCD_AdjustNextDisplayPosition();

	//==========STEP3: Write (Character) & clearing display handling==========
	if (LCD_globeConfig[NextDisplayPosition_INDEX] == 1) 					//Screen Cleared
	{
		//Call sending char sequence from the beginning for this character.
		//New HAL_LCD_SendCharsend() sends the character then return to old HAL_LCD_SendCharsend() at the end of it.
		HAL_LCD_SendChar(Char);
	}
	else
	{
		//Write (Character) each bit to corresponding DB in case of 8 bit or 4 bits interface.
		MCAL_GPIO_WritePin(RS_Port, RS_PinNumber, GPIO_PinValue_HIGH);   	// RS=1 (Data register)

		if (LCD_getFLAG(LCD_DataLong) == SET) 			// FunctionSet_8Bit -----> 1 write operation on 8 bits.
		{
			LCD_WriteOnDB(Char, Write_1x8Bits);
		}
		else 											// FunctionSet_4Bit -----> 2 write operation on 4 bits.
		{
			LCD_WriteOnDB(Char, Write_2x4Bits);
		}
	}
}
void HAL_LCD_SendString(char* String)
{
	while(*String != 0)			//Stops at the NULL termination of the array of characters (string.
	{
		HAL_LCD_SendChar(*String);
		String++;
	}
}
void HAL_LCD_ClearDisplay()
{
	HAL_LCD_SendInstruction(ClearDisplay);

	//Wait for whole clear display execution Time (till the end of clear display operation)
	while(LCD_getFLAG(LCD_Busy) == SET)
	{
		/*
		 * Wait until LCD is not busy.
		 * */
	}

	LCD_globeConfig[NextDisplayPosition_INDEX] = 1U;
}
void HAL_LCD_SendInstruction(uint8_t Instruction)
{
	/*
	 * Be sure the HD44780U LCD is not in the busy state (BF = 0) before sending an instruction from the
	 * MPU to the HD44780U LCD. If an instruction is sent without checking the busy flag, the time between
	 * the first instruction and next instruction will take much longer than the instruction time itself
	 */

	//==========STEP1: Check Busy Flag==========
	while(LCD_getFLAG(LCD_Busy) == SET)
	{
		/*
		 * Wait until LCD is not busy.
		 * */
	}
	//In this stage RS=0 (Instruction register), RW=0 (Write), EN=0 (Data read/write disabled), DB (Output).

	//==========STEP2: Write (Instruction) each bit to corresponding DB in case of 8 bit or 4 bits interface==========
	if (LCD_getFLAG(LCD_DataLong) == SET) 			// FunctionSet_8Bit -----> 1 write operation on 8 bits.
	{
		LCD_WriteOnDB(Instruction, Write_1x8Bits);
	}
	else 											// FunctionSet_4Bit -----> 2 write operation on 4 bits.
	{
		LCD_WriteOnDB(Instruction, Write_2x4Bits);
	}

	//==========STEP3: Update the global configuration if instruction is FunctionSet or DisplayControl or EntryModeSet==========
	switch (Instruction)
	{
	case FunctionSet_8Bit_1Line_5x8Dots:
	case FunctionSet_8Bit_1Line_5x10Dots:
	case FunctionSet_8Bit_2Line_5x8Dots:
	case FunctionSet_8Bit_2Line_5x10Dots:
	case FunctionSet_4Bit_1Line_5x8Dots:
	case FunctionSet_4Bit_1Line_5x10Dots:
	case FunctionSet_4Bit_2Line_5x8Dots:
	case FunctionSet_4Bit_2Line_5x10Dots:
		LCD_globeConfig[FunctionSet_INDEX] = Instruction;
		break;

	case DisplayControl_DisplayOff_CursorOff_BlinkOff:
	case DisplayControl_DisplayOn_CursorOff_BlinkOff:
	case DisplayControl_DisplayOn_CursorOn_BlinkOff:
	case DisplayControl_DisplayOn_CursorOn_BlinkOn:
		LCD_globeConfig[DisplayControl_INDEX] = Instruction;
		break;

	case EntryModeSet_DisplayShiftOff_CursorIncrement:
	case EntryModeSet_DisplayShiftOff_CursorDecrement:
	case EntryModeSet_DisplayShiftOn_CursorIncrement:
	case EntryModeSet_DisplayShiftOn_CursorDecrement:
		LCD_globeConfig[EntryModeSet_INDEX] = Instruction;
		break;

	default:
		break;
	}
}
void HAL_LCD_MoveCursor(uint8_t Row, uint8_t Column)
{
	uint8_t Address = 0x00;
	uint8_t Instruction = 0x00;

	switch (Row)
	{
	case 1:
		Address = 0x00 + (Column - 1);
		break;
	case 2:
		Address = 0x40 + (Column - 1);
		break;
	case 3:
		Address = 0x14 + (Column - 1);
		break;
	case 4:
		Address = 0x54 + (Column - 1);
		break;
	default:
		break;
	}

	Instruction = 0x80 + Address;

	HAL_LCD_SendInstruction(Instruction);

	LCD_globeConfig[NextDisplayPosition_INDEX] = (Row-1)*16 + Column;
}
void HAL_LCD_SendInteger(int IntegerNumber, LCD_IntegerType_ENUM_t IntegerType)
{
	char String[12];

	switch (IntegerType)
	{
	case UnsignedInteger:
		snprintf(String, 12,"%u", IntegerNumber);
		break;
	default:
		snprintf(String, 12,"%d", IntegerNumber);
		break;
	}

	HAL_LCD_SendString(String);
}
void HAL_LCD_SendFloat(float FloatNumber, uint8_t Fractions)
{
	char String[12];

	char* Sign = (FloatNumber < 0) ? "-" : "";
	float PositiveFloatNumber = (FloatNumber < 0) ? -FloatNumber : FloatNumber;

	int DecimalNumber = (int) PositiveFloatNumber;

	int FractionNumber = (int) ( (PositiveFloatNumber - DecimalNumber)*(power(10,Fractions)) + 0.5 );

	snprintf(String, 12,"%s%d.%d", Sign, DecimalNumber, FractionNumber);

	HAL_LCD_SendString(String);
}
