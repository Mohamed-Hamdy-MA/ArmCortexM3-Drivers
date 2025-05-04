/*
 * LED.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "LED.h"

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================


//==========================================================================================
//=====================================Assisting Macros=====================================
//==========================================================================================


//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================


//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================


//==========================================================================================
//===========================APIs Supported by HAL Keypad DRIVER============================
//==========================================================================================
void HAL_LED_Init(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber)
{
	//==========STEP1: Configure pin of GPIO connected to LED==========

	GPIO_PinConfig_t LED_GPIO_pinConfig;

	// Configure pin as output (push-pull)
	LED_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
	LED_GPIO_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
	LED_GPIO_pinConfig.GPIO_PinNumber = LED_PinNumber;
	MCAL_GPIO_InitPin(LED_Port, &LED_GPIO_pinConfig);
}
void HAL_LED_ON(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber)
{
	MCAL_GPIO_WritePin(LED_Port, LED_PinNumber, GPIO_PinValue_HIGH);
}
void HAL_LED_OFF(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber)
{
	MCAL_GPIO_WritePin(LED_Port, LED_PinNumber, GPIO_PinValue_LOW);
}
void HAL_LED_Toggle(GPIOx_REG_t* LED_Port, uint16_t LED_PinNumber)
{
	MCAL_GPIO_TogglePin(LED_Port, LED_PinNumber);
}
