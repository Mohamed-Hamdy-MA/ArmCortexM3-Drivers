/*
 * STM32F103x6_GPIO_Driver.c
 *
 *  Created on: Aug 18, 2024
 *      Author: Mohamed Hamdy
 */

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPIO_Driver.h"

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================

/**==================================================
 * @brief  Gets the base position of the corresponding 4 bits for a given pin number.
 *
 * This function returns the base position of the 4 bits that need to be configured
 * for the specified pin number in the configuration register (CRL or CRH).
 *
 * @param  PinNumber  The pin number for which the base position is required @reference GPIO_PinNumber_Define.
 *
 * @retval uint8_t    The base position of the 4 bits in the configuration register.
 *
 * @note
 *
 * @example
 * uint8_t position;
 * position = getConfigPositionInReg_CRLH(GPIO_PinNumber_0);
 * // position will be 0
 */
uint8_t getConfigPositionInReg_CRLH(uint16_t PinNumber)
{
	switch (PinNumber)
	{
	case GPIO_PinNumber_0: return 0;
	case GPIO_PinNumber_1: return 4;
	case GPIO_PinNumber_2: return 8;
	case GPIO_PinNumber_3: return 12;
	case GPIO_PinNumber_4: return 16;
	case GPIO_PinNumber_5: return 20;
	case GPIO_PinNumber_6: return 24;
	case GPIO_PinNumber_7: return 28;
	case GPIO_PinNumber_8: return 0;
	case GPIO_PinNumber_9: return 4;
	case GPIO_PinNumber_10: return 8;
	case GPIO_PinNumber_11: return 12;
	case GPIO_PinNumber_12: return 16;
	case GPIO_PinNumber_13: return 20;
	case GPIO_PinNumber_14: return 24;
	case GPIO_PinNumber_15: return 28;
	default:
		return 0;
	}
}

//==========================================================================================
//===========================APIs Supported by "MCAL GPIO DRIVER"===========================
//==========================================================================================

void MCAL_GPIO_InitPin(GPIOx_REG_t* GPIOx, GPIO_PinConfig_t* PinConfig)
{
	//Each of the general-purpose I/O ports has 16 PINs and two 32-bit configuration registers
	//(GPIOx_CRL for PIN0:PIN7, GPIOx_CRH for PIN8:PIN15)

	volatile uint32_t* ConfigReg = NULL;
	uint8_t ConfigPositionInReg_CRLH = 0;

	//Specify which register to work with (GPIOx_CRL for PIN0:PIN7, GPIOx_CRH for PIN8:PIN15).
	ConfigReg = (PinConfig->GPIO_PinNumber < GPIO_PinNumber_8)? &GPIOx->GPIOx_CRL : &GPIOx->GPIOx_CRH;

	//Specify base position to 4 configuration bits [CNFx1 CNFx0 MODEx1 MODEx0] where x is PinNumber.
	ConfigPositionInReg_CRLH = getConfigPositionInReg_CRLH(PinConfig->GPIO_PinNumber);

	//Clear these 4 configuration bits [CNFx1 CNFx0 MODEx1 MODEx0] related to PinNumber x
	*ConfigReg &= ~(0xF << ConfigPositionInReg_CRLH);

	switch (PinConfig->GPIO_PinMode)
	{
	case GPIO_PinMode_Input_ANALOG: 		// [CNFx1 CNFx0 MODEx1 MODEx0] = [0 0 0 0]
		break;

	case GPIO_PinMode_Input_FLOATING:		 // [CNFx1 CNFx0 MODEx1 MODEx0] = [0 1 0 0]
		*ConfigReg |= (0b0100 << (ConfigPositionInReg_CRLH));
		break;

	case GPIO_PinMode_Input_PULLUP:		 // [CNFx1 CNFx0 MODEx1 MODEx0] = [1 0 0 0] & [ODRx] = [1]
		*ConfigReg |= (0b1000 << (ConfigPositionInReg_CRLH));
		GPIOx->GPIOx_ODR |= PinConfig->GPIO_PinNumber;
		break;

	case GPIO_PinMode_Input_PULLDOWN:		 // [CNFx1 CNFx0 MODEx1 MODEx0] = [1 0 0 0] & [ODRx] = [0]
		*ConfigReg |= (0b1000 << (ConfigPositionInReg_CRLH));
		GPIOx->GPIOx_ODR &= ~PinConfig->GPIO_PinNumber;
		break;

	case GPIO_PinMode_Output_PUSHPULL:		 // [CNFx1 CNFx0 MODEx1 MODEx0] = [0 0 x y] x, y depend on PinSpeed
		*ConfigReg |= ((0b0000 | PinConfig->GPIO_PinSpeed) << (ConfigPositionInReg_CRLH));
		break;

	case GPIO_PinMode_Output_OPENDRAIN: 		// [CNFx1 CNFx0 MODEx1 MODEx0] = [0 1 x y] x, y depend on PinSpeed
		*ConfigReg |= ((0b0100 | PinConfig->GPIO_PinSpeed) << (ConfigPositionInReg_CRLH));
		break;

	case GPIO_PinMode_Output_ALTFUNPUSHPULL: 		// [CNFx1 CNFx0 MODEx1 MODEx0] = [1 0 x y] x, y depend on PinSpeed
		*ConfigReg |= ((0b1000 | PinConfig->GPIO_PinSpeed) << (ConfigPositionInReg_CRLH));
		break;

	case GPIO_PinMode_Output_ALTFUNOPENDRAIN: 		// [CNFx1 CNFx0 MODEx1 MODEx0] = [1 1 x y] x, y depend on PinSpeed
		*ConfigReg |= ((0b1100 | PinConfig->GPIO_PinSpeed) << (ConfigPositionInReg_CRLH));
		break;

	default:	//the I/O ports are configured in Input Floating mode by default [CNFx1 CNFx0 MODEx1 MODEx0] = [0 1 0 0].
		*ConfigReg |= (0b0100 << (ConfigPositionInReg_CRLH));
		break;
	}
}

void MCAL_GPIO_DeInit(GPIOx_REG_t* GPIOx)
{
	//volatile RCC_REG_t* RCC = RCC;
	if (GPIOx == GPIOA)
	{
		//Bit 2 IOPARST: IO port A reset. Set and cleared by software. 0: No effect 1: Reset IO port A
		RCC->RCC_APB2RSTR |= (1<<2);
		RCC->RCC_APB2RSTR &= ~(1<<2);
	}
	else if (GPIOx == GPIOB)
	{
		//Bit 3 IOPBRST: IO port B reset. Set and cleared by software. 0: No effect 1: Reset IO port B
		RCC->RCC_APB2RSTR |= (1<<3);
		RCC->RCC_APB2RSTR &= ~(1<<3);
	}
	else if (GPIOx == GPIOC)
	{
		//Bit 4 IOPCRST: IO port C reset. Set and cleared by software. 0: No effect 1: Reset IO port C
		RCC->RCC_APB2RSTR |= (1<<4);
		RCC->RCC_APB2RSTR &= ~(1<<4);
	}
	else if (GPIOx == GPIOD)
	{
		//Bit 5 IOPDRST: IO port D reset. Set and cleared by software. 0: No effect 1: Reset IO port D
		RCC->RCC_APB2RSTR |= (1<<5);
		RCC->RCC_APB2RSTR &= ~(1<<5);
	}
}

uint8_t MCAL_GPIO_ReadPin(GPIOx_REG_t* GPIOx, uint16_t PinNumber)
{
	if ((GPIOx->GPIOx_IDR & PinNumber) != GPIO_PinValue_LOW)
	{
		return GPIO_PinValue_HIGH;
	}
	else
	{
		return GPIO_PinValue_LOW;
	}
}

uint16_t MCAL_GPIO_ReadPort(GPIOx_REG_t* GPIOx)
{
	return (uint16_t)GPIOx->GPIOx_IDR;
}

void MCAL_GPIO_WritePin(GPIOx_REG_t* GPIOx, uint16_t PinNumber, uint8_t PinValue)
{
	if (PinValue != GPIO_PinValue_LOW)
	{
		GPIOx->GPIOx_BSRR = (uint32_t)PinNumber;		//Set pin
	}
	else
	{
		GPIOx->GPIOx_BRR = (uint32_t)PinNumber;		//Clear (reset) pin
	}
}

void MCAL_GPIO_WritePort(GPIOx_REG_t* GPIOx, uint16_t PortValue)
{
	GPIOx->GPIOx_ODR = (uint32_t)PortValue;
}

void MCAL_GPIO_TogglePin(GPIOx_REG_t* GPIOx, uint16_t PinNumber)
{
	GPIOx->GPIOx_ODR ^= PinNumber;		//Toggle pin
}

uint8_t MCAL_GPIO_LockPin(GPIOx_REG_t* GPIOx, uint16_t PinNumber)
{
	//GPIOx_LCKR is used to lock the configuration of the port bits when a correct write sequence
	//is applied to bit 16 (LCKK). The value of bits [15:0] is used to lock the configuration of the GPIO.

	//It can only be modified using the Lock Key Writing Sequence.
	//It can only modified once until the next reset.
	//(once you apply the sequence, the LCKK bit can not be cleared again by software)

	/*
	 * LOCK key writing sequence:
		Write 1
		Write 0
		Write 1
		Read 0
		Read 1 (this read is optional but confirms that the lock is active)

			Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
					Any error in the lock sequence will abort the lock
	 */
	uint32_t tempVar = (1<<16);

	tempVar |= PinNumber;

	GPIOx->GPIOx_LCKR = tempVar;
	GPIOx->GPIOx_LCKR = (uint32_t)PinNumber;
	GPIOx->GPIOx_LCKR = tempVar;
	tempVar = GPIOx->GPIOx_LCKR;
	tempVar = GPIOx->GPIOx_LCKR;

	tempVar = (tempVar>>16);
	if (tempVar == 1)
	{
		return GPIO_PinLock_ACTIVATED;
	}
	else
	{
		return GPIO_PinLock_APORTED;
	}
}
