/*
 * STM32F103x6_GPIO_Driver.h
 *
 *  Created on: Aug 18, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_GPIO_DRIVER_H_
#define INC_STM32F103X6_GPIO_DRIVER_H_


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6.h"

//==========================================================================================
//============================User type definitions (structures)============================
//==========================================================================================
typedef struct
{
	uint16_t GPIO_PinNumber; 		//This parameter must be a value of @reference GPIO_PinNumber_Define
	uint8_t GPIO_PinMode; 			//This parameter must be a value of @reference GPIO_PinMode_Define
	uint8_t GPIO_PinSpeed; 			//This parameter must be a value of @reference GPIO_PinSpeed_Define
}GPIO_PinConfig_t;

//==========================================================================================
//==============================Macros Configuration References=============================
//==========================================================================================

/*
	@reference GPIO_PinNumber_Define
	NOTE1: GPIO_PinNumber_x is equivalent to (1 << x)
*/
#define GPIO_PinNumber_0   (uint16_t)0x0001  // Pin 0 Selected 0000 0000 0000 0001
#define GPIO_PinNumber_1   (uint16_t)0x0002  // Pin 1 Selected 0000 0000 0000 0010
#define GPIO_PinNumber_2   (uint16_t)0x0004  // Pin 2 Selected 0000 0000 0000 0100
#define GPIO_PinNumber_3   (uint16_t)0x0008  // Pin 3 Selected 0000 0000 0000 1000
#define GPIO_PinNumber_4   (uint16_t)0x0010  // Pin 4 Selected 0000 0000 0001 0000
#define GPIO_PinNumber_5   (uint16_t)0x0020  // Pin 5 Selected 0000 0000 0010 0000
#define GPIO_PinNumber_6   (uint16_t)0x0040  // Pin 6 Selected 0000 0000 0100 0000
#define GPIO_PinNumber_7   (uint16_t)0x0080  // Pin 7 Selected 0000 0000 1000 0000
#define GPIO_PinNumber_8   (uint16_t)0x0100  // Pin 8 Selected 0000 0001 0000 0000
#define GPIO_PinNumber_9   (uint16_t)0x0200  // Pin 9 Selected 0000 0010 0000 0000
#define GPIO_PinNumber_10  (uint16_t)0x0400  // Pin 10 Selected 0000 0100 0000 0000
#define GPIO_PinNumber_11  (uint16_t)0x0800  // Pin 11 Selected 0000 1000 0000 0000
#define GPIO_PinNumber_12  (uint16_t)0x1000  // Pin 12 Selected 0001 0000 0000 0000
#define GPIO_PinNumber_13  (uint16_t)0x2000  // Pin 13 Selected 0010 0000 0000 0000
#define GPIO_PinNumber_14  (uint16_t)0x4000  // Pin 14 Selected 0100 0000 0000 0000
#define GPIO_PinNumber_15  (uint16_t)0x8000  // Pin 15 Selected 1000 0000 0000 0000

/*
	@reference GPIO_PinMode_Define

	NOTE1: During and just after reset, the alternate functions are not active and the I/O ports are
			configured in Input Floating mode by default (CNFx[1:0]=01b, MODEx[1:0]=00b).

	NOTE2: For alternate function inputs, the port must be configured in Input mode
			(floating, pull-up or pull-down) and the input pin must be driven externally.
*/
#define GPIO_PinMode_Input_ANALOG   			(uint8_t)0x00  // Analog mode
#define GPIO_PinMode_Input_FLOATING 			(uint8_t)0x01  // Floating input (reset state)
#define GPIO_PinMode_Input_PULLUP   			(uint8_t)0x02  // Input with pull-up
#define GPIO_PinMode_Input_PULLDOWN				(uint8_t)0x03  // Input with pull-down

#define GPIO_PinMode_Output_PUSHPULL			(uint8_t)0x04  // General purpose output push-pull
#define GPIO_PinMode_Output_OPENDRAIN			(uint8_t)0x05  // General purpose output Open-drain
#define GPIO_PinMode_Output_ALTFUNPUSHPULL		(uint8_t)0x06  // Alternate function output push-pull
#define GPIO_PinMode_Output_ALTFUNOPENDRAIN		(uint8_t)0x07  // Alternate function output Open-drain

/*
	@reference GPIO_PinSpeed_Define

	NOTE1: The pin speed MUST be configured if the pin mode is output
			(push-pull, open-drain or Alternate function output).

	NOTE2: The Input Data register (GPIOx_IDR) captures the data present on the I/O pin at every
			APB2 clock cycle.
*/
#define GPIO_PinSpeed_10MHz   		(uint8_t)0x01  // Output mode, max speed 10 MHz.
#define GPIO_PinSpeed_2MHz 			(uint8_t)0x02  // Output mode, max speed 2 MHz.
#define GPIO_PinSpeed_50MHz  		(uint8_t)0x03  // Output mode, max speed 50 MHz.

/*
	@reference GPIO_PinValue_Define
*/
#define GPIO_PinValue_LOW   	0b0
#define GPIO_PinValue_HIGH 	    0b1

/*
	@reference GPIO_PinLock_Define
*/
#define GPIO_PinLock_ACTIVATED   	0b1
#define GPIO_PinLock_APORTED 	  	0b0

//==========================================================================================
//===========================APIs Supported by "MCAL GPIO DRIVER"===========================
//==========================================================================================

/**==================================================
 * @brief  Initializes a GPIO pin.
 *
 * This function configures the mode, speed, pull-up/pull-down resistors,
 * and other settings for a specific GPIO pin.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 * @param  PinConfig  Pointer to a GPIO_PinConfig_t structure that contains
 * 					  the configuration information for the specified GPIO pin.
 *
 * @retval None
 *
 * @note   Ensure that the GPIO peripheral clock is enabled before calling this function.
 *
 * @example
 * GPIO_PinConfig_t pinConfig;
 * pinConfig.GPIO_PinNumber = GPIO_PinNumber_0;
 * pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
 * pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_10MHz;
 * MCAL_GPIO_InitPin(GPIOA, &pinConfig);
 */
void MCAL_GPIO_InitPin(GPIOx_REG_t* GPIOx, GPIO_PinConfig_t* PinConfig);

/**==================================================
 * @brief  Deinitializes the GPIO peripheral.
 *
 * This function resets the configuration of all GPIO pins of a specified port.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 *
 * @retval None
 *
 * @note   Ensure that the GPIO peripheral clock is enabled before calling this function.
 *
 * @example
 * MCAL_GPIO_DeInit(GPIOA);
 */
void MCAL_GPIO_DeInit(GPIOx_REG_t* GPIOx);

/**==================================================
 * @brief  Reads the state of a GPIO pin.
 *
 * This function reads the input data of a specified GPIO pin.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 * @param  PinNumber  The pin number to be read @reference GPIO_PinNumber_Define.
 *
 * @retval uint8_t    The input state of the pin should be a value of @reference GPIO_PinValue_Define (0 or 1).
 *
 * @note   Ensure that the GPIO pin is configured as an input before calling this function.
 *
 * @example
 * uint8_t pinState = MCAL_GPIO_ReadPin(GPIOA, GPIO_PinNumber_0);
 */
uint8_t MCAL_GPIO_ReadPin(GPIOx_REG_t* GPIOx, uint16_t PinNumber);

/**==================================================
 * @brief  Reads the state of all GPIO pins of a port.
 *
 * This function reads the input data of all GPIO pins of a specified port.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 *
 * @retval uint16_t   The input state of all pins in the port.
 *
 * @note   Ensure that the GPIO pins are configured as inputs before calling this function.
 *
 * @example
 * uint16_t portState = MCAL_GPIO_ReadPort(GPIOA);
 */
uint16_t MCAL_GPIO_ReadPort(GPIOx_REG_t* GPIOx);

/**==================================================
 * @brief  Writes a value to a GPIO pin.
 *
 * This function sets or clears the output data of a specified GPIO pin.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 * @param  PinNumber  The pin number to be written @reference GPIO_PinNumber_Define.
 * @param  PinValue   The value to be written to the pin @reference GPIO_PinValue_Define (0 or 1).
 *
 * @retval None
 *
 * @note   Ensure that the GPIO pin is configured as an output before calling this function.
 *
 * @example
 * MCAL_GPIO_WritePin(GPIOA, GPIO_PinNumber_0, 1);
 */
void MCAL_GPIO_WritePin(GPIOx_REG_t* GPIOx, uint16_t PinNumber, uint8_t PinValue);

/**==================================================
 * @brief  Writes a value to all GPIO pins of a port.
 *
 * This function sets or clears the output data of all GPIO pins of a specified port.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 * @param  PortValue  The value to be written to the port.
 *
 * @retval None
 *
 * @note   Ensure that the GPIO pins are configured as outputs before calling this function.
 *
 * @example
 * MCAL_GPIO_WritePort(GPIOA, 0xFFFF);
 */
void MCAL_GPIO_WritePort(GPIOx_REG_t* GPIOx, uint16_t PortValue);

/**==================================================
 * @brief  Toggles the state of a GPIO pin.
 *
 * This function toggles the output data of a specified GPIO pin.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 * @param  PinNumber  The pin number to be toggled @reference GPIO_PinNumber_Define.
 *
 * @retval None
 *
 * @note   Ensure that the GPIO pin is configured as an output before calling this function.
 *
 * @example
 * MCAL_GPIO_TogglePin(GPIOA, GPIO_PinNumber_0);
 */
void MCAL_GPIO_TogglePin(GPIOx_REG_t* GPIOx, uint16_t PinNumber);

/**==================================================
 * @brief  Locks the configuration of a GPIO pin.
 *
 * This function locks the configuration of a specified GPIO pin, preventing any further changes.
 *
 * @param  GPIOx      Pointer to the GPIO peripheral (e.g., GPIOA, GPIOB).
 * @param  PinNumber  The pins to be locked in the port @reference GPIO_PinNumber_Define.
 * 					  You can apply OR between pins if you need to lock more than one pin.
 *
 * @retval uint8_t    The lock status should be a value of @reference GPIO_PinLock_Define (0 if unsuccessful, 1 if successful).
 *
 * @note   -Ensure that the GPIO pin is configured correctly before locking it.
 * 		   -This function should be called once and after Initialization of all required pins in the port,
 * 		    when you call it the PinNumber must have all the pins to be locked,
 * 		    since calling it again doesn't make any change until the next reset.
 *
 * @example
 * uint8_t lockStatus = MCAL_GPIO_LockPins(GPIOA, GPIO_PinNumber_1 | GPIO_PinNumber_13);
 * 						//PA.1 1 & PA.13 Selected 0010 0000 0000 0010
 */
uint8_t MCAL_GPIO_LockPin(GPIOx_REG_t* GPIOx, uint16_t PinNumber);


#endif /* INC_STM32F103X6_GPIO_DRIVER_H_*/
