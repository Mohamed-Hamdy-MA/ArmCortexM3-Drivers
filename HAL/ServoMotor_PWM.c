/*
 * ServoMotor_PWM.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "ServoMotor_PWM.h"


//==========================================================================================
//===========================APIs Supported by "HAL SERVO DRIVER"==========================
//==========================================================================================
void HAL_Servo_Init(GPIOx_REG_t* Servo_Port, uint16_t Servo_PinNumber)
{
	//==========STEP1: Configure pin of GPIO connected to Servo==========

	GPIO_PinConfig_t Servo_GPIO_pinConfig;

	// Configure pin as output (push-pull)
	Servo_GPIO_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_PUSHPULL;
	Servo_GPIO_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
	Servo_GPIO_pinConfig.GPIO_PinNumber = Servo_PinNumber;
	MCAL_GPIO_InitPin(Servo_Port, &Servo_GPIO_pinConfig);
}
void HAL_Servo_Rotate(GPIOx_REG_t* Servo_Port, uint16_t Servo_PinNumber, Servo_Angle_ENUM_t RotationAngle)
{
	if (RotationAngle == Clockwise_Plus_90)
	{
		MCAL_GPIO_WritePin(Servo_Port, Servo_PinNumber, GPIO_PinValue_HIGH);
		MCAL_TIM_Delay_us(TIM2,	2000);
		MCAL_GPIO_WritePin(Servo_Port, Servo_PinNumber, GPIO_PinValue_LOW);

	}
	else if (RotationAngle == AntiClockwise_Minus_90)
	{
		MCAL_GPIO_WritePin(Servo_Port, Servo_PinNumber, GPIO_PinValue_HIGH);
		MCAL_TIM_Delay_us(TIM2,	1000);
		MCAL_GPIO_WritePin(Servo_Port, Servo_PinNumber, GPIO_PinValue_LOW);
	}
	else 	// In case of Rest_0
	{
		MCAL_GPIO_WritePin(Servo_Port, Servo_PinNumber, GPIO_PinValue_HIGH);
		MCAL_TIM_Delay_us(TIM2,	1500);
		MCAL_GPIO_WritePin(Servo_Port, Servo_PinNumber, GPIO_PinValue_LOW);
	}
}
