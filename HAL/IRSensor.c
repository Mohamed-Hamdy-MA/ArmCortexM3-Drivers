/*
 * IRSensor.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "IRSensor.h"


//==========================================================================================
//===========================APIs Supported by "HAL IR Sensor DRIVER"==========================
//==========================================================================================
void HAL_IRSensor_Init(GPIOx_REG_t* IRSensor_Port, uint16_t IRSensor_PinNumber)
{
	//==========STEP1: Configure pin of GPIO connected to IR Sensor==========

	GPIO_PinConfig_t IRSensor_GPIO_pinConfig;

	// Configure pin as input (Floating)
	IRSensor_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
	IRSensor_GPIO_pinConfig.GPIO_PinNumber = IRSensor_PinNumber;
	MCAL_GPIO_InitPin(IRSensor_Port, &IRSensor_GPIO_pinConfig);
}
IRSensor_Status_ENUM_t HAL_IRSensor_getStatus(GPIOx_REG_t* IRSensor_Port, uint16_t IRSensor_PinNumber)
{
	if (MCAL_GPIO_ReadPin(IRSensor_Port, IRSensor_PinNumber) == GPIO_PinValue_HIGH)
	{
		return ObsticleFound;

	}
	else 	// In case of MCAL_GPIO_ReadPin(IRSensor_Port, IRSensor_PinNumber) == GPIO_PinValue_LOW
	{
		return NoObsticle;
	}
}
