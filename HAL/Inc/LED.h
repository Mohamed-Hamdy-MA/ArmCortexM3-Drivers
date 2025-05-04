/*
 * LED.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_LED_H_
#define INC_LED_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"
#include "STM32F103x6_GPTIM_Driver.h"


//==========================================================================================
//============================User Hardware Connection Schematic============================
//==========================================================================================

#define LedGreenEntrance_Port           GPIOB
#define LedGreenEntrance_PinNumber      GPIO_PinNumber_10

#define LedRedEntrance_Port           	GPIOB
#define LedRedEntrance_PinNumber      	GPIO_PinNumber_11

//#define LedGreenExit_Port           	GPIOB
//#define LedGreenExit_PinNumber      	GPIO_PinNumber_13

//#define LedRedExit_Port          	 	GPIOB
//#define LedRedExit_PinNumber      	GPIO_PinNumber_14


//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================


//==========================================================================================
//===========================APIs Supported by "MCAL USART DRIVER"==========================
//==========================================================================================

/**
 * @brief  Initializes the LED.
 *
 * This function sets up the LED with specific configurations for its operation.
 * It configures the GPIO pin connected to the LED as an output pin.
 *
 * @param  LED_Port      GPIO port where the LED is connected.
 * @param  LED_PinNumber GPIO pin number where the LED is connected.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Configures the GPIO pin as an output with push-pull mode.
 *           - Sets the pin speed to 2MHz.
 *           - Calls the MCAL_GPIO_InitPin function to initialize the pin.
 *
 * @example
 * HAL_LED_Init(GPIOA, GPIO_PinNumber_5);
 * // Initializes the LED connected to GPIO port A, pin 5.
 */
void HAL_LED_Init(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber);

/**
 * @brief  Turns on the LED.
 *
 * This function sets the GPIO pin connected to the LED to HIGH state,
 * turning the LED on.
 *
 * @param  LED_Port      GPIO port where the LED is connected.
 * @param  LED_PinNumber GPIO pin number where the LED is connected.
 *
 * @retval None
 *
 * @example
 * HAL_LED_ON(GPIOA, GPIO_PinNumber_5);
 * // Turns on the LED connected to GPIO port A, pin 5.
 */
void HAL_LED_ON(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber);

/**
 * @brief  Turns off the LED.
 *
 * This function sets the GPIO pin connected to the LED to LOW state,
 * turning the LED off.
 *
 * @param  LED_Port      GPIO port where the LED is connected.
 * @param  LED_PinNumber GPIO pin number where the LED is connected.
 *
 * @retval None
 *
 * @example
 * HAL_LED_OFF(GPIOA, GPIO_PinNumber_5);
 * // Turns off the LED connected to GPIO port A, pin 5.
 */
void HAL_LED_OFF(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber);

/**
 * @brief  Toggles the state of the LED.
 *
 * This function toggles the GPIO pin connected to the LED,
 * changing its state from HIGH to LOW or from LOW to HIGH.
 *
 * @param  LED_Port      GPIO port where the LED is connected.
 * @param  LED_PinNumber GPIO pin number where the LED is connected.
 *
 * @retval None
 *
 * @example
 * HAL_LED_Toggle(GPIOA, GPIO_PinNumber_5);
 * // Toggles the state of the LED connected to GPIO port A, pin 5.
 */
void HAL_LED_Toggle(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber);

#endif /* INC_LED_H_ */
