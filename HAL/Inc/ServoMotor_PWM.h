/*
 * ServoMotor_PWM.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef SERVOMOTOR_PWM_H_
#define SERVOMOTOR_PWM_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"
#include "STM32F103x6_GPTIM_Driver.h"


//==========================================================================================
//============================User Hardware Connection Schematic============================
//==========================================================================================

#define Servo_Entrance_Port           	GPIOA
#define Servo_Entrance_PinNumber      	GPIO_PinNumber_0

#define Servo_Exit_Port           		GPIOB
#define Servo_Exit_PinNumber      		GPIO_PinNumber_13


//==================================User type definitions===================================
//==========================================================================================
typedef enum
{
	Rest_0,
	Clockwise_Plus_90,
	AntiClockwise_Minus_90,
}Servo_Angle_ENUM_t;


//==========================================================================================
//===========================APIs Supported by "HAL SERVO DRIVER"==========================
//==========================================================================================

/**
 * @brief  Initializes the Servo motor.
 *
 * This function sets up the Servo motor with specific configurations for its operation.
 * It configures the GPIO pin connected to the Servo as an output pin.
 *
 * @param  Servo_Port      GPIO port where the Servo is connected.
 * @param  Servo_PinNumber GPIO pin number where the Servo is connected.
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
 * HAL_Servo_Init(GPIOA, GPIO_PinNumber_6);
 * // Initializes the Servo connected to GPIO port A, pin 6.
 */
void HAL_Servo_Init(GPIOx_REG_t* Servo_Port, uint16_t Servo_PinNumber);

/**
 * @brief  Rotates the Servo motor.
 *
 * This function rotates the Servo motor to the specified angle.
 *
 * @param  Servo_Port      GPIO port where the Servo is connected.
 * @param  Servo_PinNumber GPIO pin number where the Servo is connected.
 * @param  RotationAngle   Angle to rotate the Servo.
 *                        Must be one of the predefined 'Servo_Angle_ENUM_t' values.
 *                        - Clockwise_Plus_90: Rotate +90 degrees clockwise.
 *                        - AntiClockwise_Minus_90: Rotate -90 degrees counterclockwise.
 *                        - Rest_0: Rotate to 0 degrees (rest position).
 *
 * @retval None
 *
 * @example
 * HAL_Servo_Rotate(GPIOA, GPIO_PinNumber_6, Clockwise_Plus_90);
 * // Rotates the Servo connected to GPIO port A, pin 6, +90 degrees clockwise.
 */
void HAL_Servo_Rotate(GPIOx_REG_t* Servo_Port, uint16_t Servo_PinNumber, Servo_Angle_ENUM_t RotationAngle);

#endif /* SERVOMOTOR_PWM_H_ */
