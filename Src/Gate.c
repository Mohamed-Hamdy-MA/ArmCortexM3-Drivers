/*
 * Gate.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed Hamdy
 */

#include "Gate.h"

#define UP		Clockwise_Plus_90
#define DOWN	Rest_0


void APP_EntranceGate(void)
{
	HAL_Servo_Rotate(Servo_Entrance_Port, Servo_Entrance_PinNumber, UP);
	MCAL_TIM_Delay_s(TIM2, 5);

	while(HAL_IRSensor_getStatus(IRSensor1_Entrance_Port, IRSensor1_Entrance_PinNumber) == ObsticleFound)
	{
		HAL_Buzzer_ON(Buzzer_Entrance_Port, Buzzer_Entrance_PinNumber);
	}

	HAL_Buzzer_OFF(Buzzer_Entrance_Port, Buzzer_Entrance_PinNumber);
	HAL_Servo_Rotate(Servo_Entrance_Port, Servo_Entrance_PinNumber, DOWN);
}

void APP_ExitGate(void)
{
	HAL_Servo_Rotate(Servo_Exit_Port, Servo_Exit_PinNumber, UP);
	MCAL_TIM_Delay_s(TIM2, 5);

	while(HAL_IRSensor_getStatus(IRSensor1_Exit_Port, IRSensor1_Exit_PinNumber) == ObsticleFound)
	{
		HAL_Buzzer_ON(Buzzer_Exit_Port, Buzzer_Exit_PinNumber);
	}

	HAL_Buzzer_OFF(Buzzer_Exit_Port, Buzzer_Exit_PinNumber);
	HAL_Servo_Rotate(Servo_Exit_Port, Servo_Exit_PinNumber, DOWN);
}
