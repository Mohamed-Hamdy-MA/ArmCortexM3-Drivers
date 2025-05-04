/*
 * IRSensor.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_IRSENSOR_H_
#define INC_IRSENSOR_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"


//==========================================================================================
//============================User Hardware Connection Schematic============================
//==========================================================================================

#define IRSensor1_Entrance_Port           	GPIOA
#define IRSensor1_Entrance_PinNumber      	GPIO_PinNumber_1

//#define IRSensor2_Entrance_Port           	GPIOB
//#define IRSensor2_Entrance_PinNumber      	GPIO_PinNumber_13

#define IRSensor1_Exit_Port           		GPIOB
#define IRSensor1_Exit_PinNumber      		GPIO_PinNumber_14

//#define IRSensor2_Exit_Port           		GPIOB
//#define IRSensor2_Exit_PinNumber      		GPIO_PinNumber_13


//==================================User type definitions===================================
//==========================================================================================
typedef enum
{
	NoObsticle,
	ObsticleFound,
}IRSensor_Status_ENUM_t;


//==========================================================================================
//===========================APIs Supported by "HAL SERVO DRIVER"==========================
//==========================================================================================

/**
 * @brief  Initializes the IR Sensor.
 *
 * This function sets up the IR Sensor with specific configurations for its operation.
 * It configures the GPIO pin connected to the IR Sensor as an input pin.
 *
 * @param  IRSensor_Port      GPIO port where the IR Sensor is connected.
 * @param  IRSensor_PinNumber GPIO pin number where the IR Sensor is connected.
 *
 * @retval None
 *
 * @note
 *         - The function performs the following steps:
 *           - Configures the GPIO pin as an input with floating mode.
 *           - Calls the MCAL_GPIO_InitPin function to initialize the pin.
 *
 * @example
 * HAL_IRSensor_Init(GPIOA, GPIO_PinNumber_7);
 * // Initializes the IR Sensor connected to GPIO port A, pin 7.
 */
void HAL_IRSensor_Init(GPIOx_REG_t* IRSensor_Port, uint16_t IRSensor_PinNumber);

/**
 * @brief  Gets the status of the IR Sensor.
 *
 * This function reads the GPIO pin connected to the IR Sensor and
 * returns the status indicating whether an obstacle is detected.
 *
 * @param  IRSensor_Port      GPIO port where the IR Sensor is connected.
 * @param  IRSensor_PinNumber GPIO pin number where the IR Sensor is connected.
 *
 * @retval IRSensor_Status_ENUM_t
 *         - ObsticleFound: If an obstacle is detected.
 *         - NoObsticle: If no obstacle is detected.
 *
 * @example
 * IRSensor_Status_ENUM_t status = HAL_IRSensor_getStatus(GPIOA, GPIO_PinNumber_7);
 * if (status == ObsticleFound) {
 *     // Handle obstacle detection
 * }
 */
IRSensor_Status_ENUM_t HAL_IRSensor_getStatus(GPIOx_REG_t* IRSensor_Port, uint16_t IRSensor_PinNumber);

#endif /* INC_IRSENSOR_H_ */
