/*
 * Buzzer.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "Buzzer.h"


//==========================================================================================
//===========================APIs Supported by HAL Buzzer DRIVER============================
//==========================================================================================
void HAL_Buzzer_Init(GPIOx_REG_t* Buzzer_Port, uint16_t Buzzer_PinNumber)
{
	//==========STEP1: Configure pin of GPIO connected to Buzzer==========

	GPIO_PinConfig_t Buzzer_GPIO_pinConfig;

	// Configure pin as output (push-pull)
	Buzzer_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
	Buzzer_GPIO_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
	Buzzer_GPIO_pinConfig.GPIO_PinNumber = Buzzer_PinNumber;
	MCAL_GPIO_InitPin(Buzzer_Port, &Buzzer_GPIO_pinConfig);
}
void HAL_Buzzer_ON(GPIOx_REG_t* Buzzer_Port, uint16_t Buzzer_PinNumber)
{
	MCAL_GPIO_WritePin(Buzzer_Port, Buzzer_PinNumber, GPIO_PinValue_HIGH);
}
void HAL_Buzzer_OFF(GPIOx_REG_t* Buzzer_Port, uint16_t Buzzer_PinNumber)
{
	MCAL_GPIO_WritePin(Buzzer_Port, Buzzer_PinNumber, GPIO_PinValue_LOW);
}
