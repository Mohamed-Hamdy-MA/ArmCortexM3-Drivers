/*
 * Buzzer.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"


//==========================================================================================
//============================User Hardware Connection Schematic============================
//==========================================================================================
#define Buzzer_Entrance_Port           	GPIOA
#define Buzzer_Entrance_PinNumber      	GPIO_PinNumber_14

#define Buzzer_Exit_Port           		GPIOB
#define Buzzer_Exit_PinNumber      		GPIO_PinNumber_15

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================


//==========================================================================================
//===========================APIs Supported by "MCAL USART DRIVER"==========================
//==========================================================================================

/**
 * @brief  Initializes the Buzzer.
 *
 * This function sets up the Buzzer with specific configurations for its operation.
 * It configures the GPIO pin connected to the Buzzer as an output pin.
 *
 * @param  Buzzer_Port      GPIO port where the Buzzer is connected.
 * @param  Buzzer_PinNumber GPIO pin number where the Buzzer is connected.
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
 * HAL_Buzzer_Init(GPIOA, GPIO_PinNumber_5);
 * // Initializes the Buzzer connected to GPIO port A, pin 5.
 */
void HAL_Buzzer_Init(GPIOx_REG_t* Buzzer_Port, uint16_t Buzzer_PinNumber);

/**
 * @brief  Turns on the Buzzer.
 *
 * This function sets the GPIO pin connected to the Buzzer to HIGH state,
 * turning the Buzzer on.
 *
 * @param  Buzzer_Port      GPIO port where the Buzzer is connected.
 * @param  Buzzer_PinNumber GPIO pin number where the Buzzer is connected.
 *
 * @retval None
 *
 * @example
 * HAL_Buzzer_ON(GPIOA, GPIO_PinNumber_5);
 * // Turns on the Buzzer connected to GPIO port A, pin 5.
 */
void HAL_Buzzer_ON(GPIOx_REG_t* Buzzer_Port, uint16_t Buzzer_PinNumber);

/**
 * @brief  Turns off the Buzzer.
 *
 * This function sets the GPIO pin connected to the Buzzer to LOW state,
 * turning the Buzzer off.
 *
 * @param  Buzzer_Port      GPIO port where the Buzzer is connected.
 * @param  Buzzer_PinNumber GPIO pin number where the Buzzer is connected.
 *
 * @retval None
 *
 * @example
 * HAL_Buzzer_OFF(GPIOA, GPIO_PinNumber_5);
 * // Turns off the Buzzer connected to GPIO port A, pin 5.
 */
void HAL_Buzzer_OFF(GPIOx_REG_t* Buzzer_Port, uint16_t Buzzer_PinNumber);

#endif /* INC_BUZZER_H_ */
