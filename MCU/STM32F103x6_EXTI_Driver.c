/*
 * STM32F103x6_EXTI_Driver.C
 *
 *  Created on: Aug 21, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_EXTI_Driver.h"

//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================

void(*Gptr_CallbackFUN[15])(void); //Array of 15 pointer to function

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================

void Enable_NVIC_EXTIx(uint8_t EXTI_LineNumber)
{
	switch (EXTI_LineNumber)
	{
	case EXTI0:
		NVIC_EXTI0_IRQ6_EN;
		break;
	case EXTI1:
		NVIC_EXTI1_IRQ7_EN;
		break;
	case EXTI2:
		NVIC_EXTI2_IRQ8_EN;
		break;
	case EXTI3:
		NVIC_EXTI3_IRQ9_EN;
		break;
	case EXTI4:
		NVIC_EXTI4_IRQ10_EN;
		break;
	case EXTI5:
	case EXTI6:
	case EXTI7:
	case EXTI8:
	case EXTI9:
		NVIC_EXTI9_5_IRQ23_EN;
		break;
	case EXTI10:
	case EXTI11:
	case EXTI12:
	case EXTI13:
	case EXTI14:
	case EXTI15:
		NVIC_EXTI15_10_IRQ40_EN;
		break;
	}
}
void Disable_NVIC_EXTIx(uint8_t EXTI_LineNumber)
{
	switch (EXTI_LineNumber)
	{
	case EXTI0:
		NVIC_EXTI0_IRQ6_DIS;
		break;
	case EXTI1:
		NVIC_EXTI1_IRQ7_DIS;
		break;
	case EXTI2:
		NVIC_EXTI2_IRQ8_DIS;
		break;
	case EXTI3:
		NVIC_EXTI3_IRQ9_DIS;
		break;
	case EXTI4:
		NVIC_EXTI4_IRQ10_DIS;
		break;
	case EXTI5:
	case EXTI6:
	case EXTI7:
	case EXTI8:
	case EXTI9:
		NVIC_EXTI9_5_IRQ23_DIS;
		break;
	case EXTI10:
	case EXTI11:
	case EXTI12:
	case EXTI13:
	case EXTI14:
	case EXTI15:
		NVIC_EXTI15_10_IRQ40_DIS;
		break;
	}
}
void MCAL_EXTI_Config(EXTI_Config_t* EXTI_Config)
{
	//==========STEP1: Configure GPIO==========

	GPIO_PinConfig_t pinConfig;
	pinConfig.GPIO_PinNumber = EXTI_Config->EXTI_Pin.GPIO_PinNumber;
	pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
	MCAL_GPIO_InitPin(EXTI_Config->EXTI_Pin.GPIO_Port, &pinConfig);

	//==========STEP2: Configure AFIO (AFIO_EXTICR register)==========

	//Specify which register to work with (AFIO_EXTICR1, AFIO_EXTICR2, AFIO_EXTICR3, AFIO_EXTICR4).
	uint8_t EXTICR_index = EXTI_Config->EXTI_Pin.EXTI_LineNumber / 4;

	//Specify base position to 4 configuration bits in AFIO_EXTICR [EXTIx] where x is LineNumber.
	uint8_t ConfigPositionInReg_EXTICRx = (EXTI_Config->EXTI_Pin.EXTI_LineNumber % 4) * 4;

	//Clear these 4 configuration bits [EXTIx] related to LineNumber x
	AFIO->AFIO_EXTICR[EXTICR_index] &= ~(0xF << ConfigPositionInReg_EXTICRx);

	//Write to select the source input (GPIO_Port) for EXTIx external interrupt.

	if (EXTI_Config->EXTI_Pin.GPIO_Port == GPIOA)			// 0000: PA[x] pin
	{
	    // No action needed for GPIOA, already 0000
	}
	else if (EXTI_Config->EXTI_Pin.GPIO_Port == GPIOB) 		// 0001: PB[x] pin
	{
	    AFIO->AFIO_EXTICR[EXTICR_index] |= (0b0001 << ConfigPositionInReg_EXTICRx);
	}
	else if (EXTI_Config->EXTI_Pin.GPIO_Port == GPIOC)		// 0010: PC[x] pin
	{
	    AFIO->AFIO_EXTICR[EXTICR_index] |= (0b0010 << ConfigPositionInReg_EXTICRx);
	}
	else if (EXTI_Config->EXTI_Pin.GPIO_Port == GPIOD)		// 0011: PD[x] pin
	{
	    AFIO->AFIO_EXTICR[EXTICR_index] |= (0b0011 << ConfigPositionInReg_EXTICRx);
	}

	//==========STEP3: Configure EXTI (EXTI_RTSR & EXTI_FTSR registers)==========

	//Clear rising and falling trigger configuration bits related to LineNumber x
	EXTI->EXTI_RTSR &= ~(1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
	EXTI->EXTI_FTSR &= ~(1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);

	//Write to select the trigger event for EXTIx external interrupt.
	switch (EXTI_Config->EXTI_Trigger)
	{
	case EXTI_Trigger_RISING:
		EXTI->EXTI_RTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
		break;
	case EXTI_Trigger_FALLING:
		EXTI->EXTI_FTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
		break;
	case EXTI_Trigger_RISING_FALLING:
		EXTI->EXTI_RTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
		EXTI->EXTI_FTSR |= (1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
		break;
	}

	//==========STEP4: Handle IRQ callback function==========

	Gptr_CallbackFUN[EXTI_Config->EXTI_Pin.EXTI_LineNumber] = EXTI_Config->ptr_CallbackFUN;

	//==========STEP5: Enable/Disable EXTI (EXTI_IMR & NVIC_ISER & NVIC_ICER registers)==========

	if (EXTI_Config->EXTI_EN_OR_DIS == EXTI_ENABLED)
	{
		//Enable EXTI_IMR bit related to LineNumber x.
		EXTI->EXTI_IMR |= (1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
		//Enable NVIC by ISER register bit related to LineNumber x.
		Enable_NVIC_EXTIx(EXTI_Config->EXTI_Pin.EXTI_LineNumber);
	}
	else
	{
		//Disable EXTI_IMR bit related to LineNumber x.
		EXTI->EXTI_IMR &= ~(1 << EXTI_Config->EXTI_Pin.EXTI_LineNumber);
		//Disable NVIC by ICER register bit related to LineNumber x.
		Disable_NVIC_EXTIx(EXTI_Config->EXTI_Pin.EXTI_LineNumber);
	}
}

//==========================================================================================
//===========================APIs Supported by "MCAL EXTI DRIVER"===========================
//==========================================================================================

void MCAL_EXTI_Init(EXTI_Config_t* EXTI_Config)
{
	MCAL_EXTI_Config(EXTI_Config);
}
void MCAL_EXTI_Update(EXTI_Config_t* EXTI_Config)
{
	MCAL_EXTI_Config(EXTI_Config);
}
void MCAL_EXTI_DeInit(void)
{
	//Reset EXTI peripheral -> Reset all EXTI registers to its reset value since no EXTI isn't connected to RCC.
	EXTI->EXTI_IMR = 0x00000000;
	EXTI->EXTI_EMR = 0x00000000;
	EXTI->EXTI_RTSR = 0x00000000;
	EXTI->EXTI_FTSR = 0x00000000;
	EXTI->EXTI_SWIER = 0x00000000;
	EXTI->EXTI_PR = 0x11111111;		//cleared by writing a ‘1’ into the bit.

	//Reset EXTI IRQ lines in NVIC.
	NVIC_EXTI0_IRQ6_DIS;
	NVIC_EXTI1_IRQ7_DIS;
	NVIC_EXTI2_IRQ8_DIS;
	NVIC_EXTI3_IRQ9_DIS;
	NVIC_EXTI4_IRQ10_DIS;
	NVIC_EXTI9_5_IRQ23_DIS;
	NVIC_EXTI15_10_IRQ40_DIS;
}

//==========================================================================================
//=========================================EXTI ISR=========================================
//==========================================================================================

void EXTI0_IRQHandler(void)
{
	//Clear pending request bit related to LineNumber 0 to avoid infinite interrupt.
	EXTI->EXTI_PR |= (1 << 0);	//bit is cleared by writing a ‘1’ into the bit.

	//Call callback function related related to LineNumber 0.
	Gptr_CallbackFUN[0]();
}
void EXTI1_IRQHandler(void)
{
	//Clear pending request bit related to LineNumber 1 to avoid infinite interrupt.
	EXTI->EXTI_PR |= (1 << 1);	//bit is cleared by writing a ‘1’ into the bit.

	//Call callback function related related to LineNumber 1.
	Gptr_CallbackFUN[1]();
}
void EXTI2_IRQHandler(void)
{
	//Clear pending request bit related to LineNumber 2 to avoid infinite interrupt.
	EXTI->EXTI_PR |= (1 << 2);	//bit is cleared by writing a ‘1’ into the bit.

	//Call callback function related related to LineNumber 2.
	Gptr_CallbackFUN[2]();
}
void EXTI3_IRQHandler(void)
{
	//Clear pending request bit related to LineNumber 3 to avoid infinite interrupt.
	EXTI->EXTI_PR |= (1 << 3);	//bit is cleared by writing a ‘1’ into the bit.

	//Call callback function related related to LineNumber 3.
	Gptr_CallbackFUN[3]();
}
void EXTI4_IRQHandler(void)
{
	//Clear pending request bit related to LineNumber 4 to avoid infinite interrupt.
	EXTI->EXTI_PR |= (1 << 4);	//bit is cleared by writing a ‘1’ into the bit.

	//Call callback function related related to LineNumber 4.
	Gptr_CallbackFUN[4]();
}
void EXTI9_5_IRQHandler(void)
{
	//STEP1: Specify which EXTI LineNumber from pending request register.
	//STEP2: Clear pending request bit related to LineNumber 5:9 to avoid infinite interrupt. cleared by writing a ‘1’.
	//STEP3: Call callback function related related to LineNumber 5:9.

	if 		( (EXTI->EXTI_PR & (1 << 5) ) != 0) { EXTI->EXTI_PR |= (1 << 5) ; Gptr_CallbackFUN[5]() ; }
	else if ( (EXTI->EXTI_PR & (1 << 6) ) != 0) { EXTI->EXTI_PR |= (1 << 6) ; Gptr_CallbackFUN[6]() ; }
	else if ( (EXTI->EXTI_PR & (1 << 7) ) != 0) { EXTI->EXTI_PR |= (1 << 7) ; Gptr_CallbackFUN[7]() ; }
	else if ( (EXTI->EXTI_PR & (1 << 8) ) != 0) { EXTI->EXTI_PR |= (1 << 8) ; Gptr_CallbackFUN[8]() ; }
	else if ( (EXTI->EXTI_PR & (1 << 9) ) != 0) { EXTI->EXTI_PR |= (1 << 9) ; Gptr_CallbackFUN[9]() ; }
}
void EXTI15_10_IRQHandler(void)
{
	// STEP1: Specify which EXTI LineNumber from pending request register.
	// STEP2: Clear pending request bit related to LineNumber 10:15 to avoid infinite interrupt. Cleared by writing a ‘1’.
	// STEP3: Call callback function related to LineNumber 10:15.

	if 		( (EXTI->EXTI_PR & (1 << 10) ) != 0) { EXTI->EXTI_PR |= (1 << 10); Gptr_CallbackFUN[10](); }
	else if ( (EXTI->EXTI_PR & (1 << 11) ) != 0) { EXTI->EXTI_PR |= (1 << 11); Gptr_CallbackFUN[11](); }
	else if ( (EXTI->EXTI_PR & (1 << 12) ) != 0) { EXTI->EXTI_PR |= (1 << 12); Gptr_CallbackFUN[12](); }
	else if ( (EXTI->EXTI_PR & (1 << 13) ) != 0) { EXTI->EXTI_PR |= (1 << 13); Gptr_CallbackFUN[13](); }
	else if ( (EXTI->EXTI_PR & (1 << 14) ) != 0) { EXTI->EXTI_PR |= (1 << 14); Gptr_CallbackFUN[14](); }
	else if ( (EXTI->EXTI_PR & (1 << 15) ) != 0) { EXTI->EXTI_PR |= (1 << 15); Gptr_CallbackFUN[15](); }
}
