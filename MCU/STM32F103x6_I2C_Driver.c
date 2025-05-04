/*
 * STM32F103x6_I2C_Driver.c
 *
 *  Created on: Sep 14, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_I2C_Driver.h"

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================
typedef enum
{
	CLEARED 	= 0U,
	SET 		= 1U,
}I2C_flagStatus_ENUM_t;

typedef enum
{
	WRITE_TransmitterMode 	= 0U,
	READ_ReceiverMode 		= 1U,
}I2C_ReadWrite_ENUM_t;

typedef enum
{
	//----------CR2----------
	ITERREN,       	// ITERREN: Error interrupt enable
	ITEVTEN,     	// ITEVTEN: Event interrupt enable
	ITBUFEN,      	// ITBUFEN: Buffer interrupt enable

	//----------SR1----------
	SB,       	// SB: Start bit sent (Master)
	ADDR,     	// ADDR: Address sent (Master) or Address matched (Slave)
	BTF,      	// BTF: Data byte transfer finished
	ADD10,   	// ADD10: 10-bit header sent (Master)
	STOPF,    	// STOPF: Stop received (Slave)
	RxNE,     	// RxNE: Receive buffer not empty
	TxE,      	// TxE: Transmit buffer empty
	BERR,     	// BERR: Bus error
	ARLO,     	// ARLO: Arbitration loss (Master)
	AF,		 	// ACKFAIL: Acknowledge failure
	OVR,      	// OVR: Overrun/Underrun
	PECERR,   	// PECERR: PEC Error in reception
	TIMEOUT,  	// TIMEOUT: Timeout or Tlow error
	SMBALERT, 	// SMBALERT: SMBus alert

	//----------SR2----------
	MSL,	// MSL: Master/slave
	BUSY,	// BUSY: Bus busy -> indicates a communication in progress on the bus.
	TRA,	// TRA: Transmitter/receiver

	//----------EV----------

	EV1,	// EV1: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	EV2,	// EV2: RxNE=1, cleared by reading DR register.
	EV3,	// EV3: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
	EV3_1,	// EV3_1: TxE=1, shift register empty, data register empty, write Data1 in DR
	EV3_2,	// EV3_2:  AF=1, AF1 cleared by writing ‘0’ in AF bit of SR1 register.
	EV4,	// EV4: STOPF=1, cleared by reading SR1 register followed by writing to the CR1 register.

	EV5,	// EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
	EV6,	// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	EV7,	// EV7: RxNE=1, cleared by reading DR register.
	EV7_1,	// EV7_1: RxNE=1, cleared by reading DR register, program ACK=0 and STOP request
	EV8,	// EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
	EV8_1,	// EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR
	EV8_2,	// EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition

}I2C_Flag_ENUM_t;

typedef enum
{
	NOT_HANDLED 	= 0U,
	HANDLED 		= 1U,
}I2C_InterruptHandler_ENUM_t;

//==========================================================================================
//=====================================Assisting Macros=====================================
//==========================================================================================
#define I2C1_INDEX		0U
#define I2C2_INDEX		1U

//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================
I2C_Config_t I2C_globeConfig[2];		//Uninitialized global array, so it will be initialized to 0 by default.

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================

/**==================================================
 * @brief  Checks the status of the specified I2C flag.
 *
 * This function checks the status of the specified I2C flag by reading the appropriate
 * register and bit. It returns whether the flag is set or cleared.
 *
 * @param  I2Cx  Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  Flag  The flag to check (defined in I2C_Flag_ENUM_t).
 *
 * @retval I2C_flagStatus_ENUM_t  The status of the flag (SET or CLEARED).
 *
 * @note   - The function performs the following steps:
 *           - Checks the specified flag in the appropriate register (CR2, SR1, SR2, or EV).
 *
 * @example
 * I2C_flagStatus_ENUM_t flagStatus;
 * flagStatus = I2C_getFLAG(I2C1, SB);
 * // Checks the status of the SB flag in I2C1
 */
I2C_flagStatus_ENUM_t I2C_getFLAG(I2Cx_REG_t* I2Cx, I2C_Flag_ENUM_t Flag)
{
	switch (Flag)
	{
	//----------CR2----------
	case ITERREN:
		return ( (I2Cx->I2C_CR2 & I2C_CR2_ITERREN_MASK) == I2C_CR2_ITERREN_MASK )? SET: CLEARED;
	case ITEVTEN:
		return ( (I2Cx->I2C_CR2 & I2C_CR2_ITEVTEN_MASK) == I2C_CR2_ITEVTEN_MASK )? SET: CLEARED;
	case ITBUFEN:
		return ( (I2Cx->I2C_CR2 & I2C_CR2_ITBUFEN_MASK) == I2C_CR2_ITBUFEN_MASK )? SET: CLEARED;

		//----------SR1----------
	case SB:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_SB_MASK) == I2C_SR1_SB_MASK )? SET: CLEARED;
	case ADDR:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_ADDR_MASK) == I2C_SR1_ADDR_MASK )? SET: CLEARED;
	case BTF:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_BTF_MASK) == I2C_SR1_BTF_MASK )? SET: CLEARED;
	case ADD10:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_ADD10_MASK) == I2C_SR1_ADD10_MASK )? SET: CLEARED;
	case STOPF:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_STOPF_MASK) == I2C_SR1_STOPF_MASK )? SET: CLEARED;
	case RxNE:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_RxNE_MASK) == I2C_SR1_RxNE_MASK )? SET: CLEARED;
	case TxE:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_TxE_MASK) == I2C_SR1_TxE_MASK )? SET: CLEARED;
	case BERR:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_BERR_MASK) == I2C_SR1_BERR_MASK )? SET: CLEARED;
	case ARLO:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_ARLO_MASK) == I2C_SR1_ARLO_MASK )? SET: CLEARED;
	case AF:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_AF_MASK) == I2C_SR1_AF_MASK )? SET: CLEARED;
	case OVR:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_OVR_MASK) == I2C_SR1_OVR_MASK )? SET: CLEARED;
	case PECERR:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_PECERR_MASK) == I2C_SR1_PECERR_MASK )? SET: CLEARED;
	case TIMEOUT:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_TIMEOUT_MASK) == I2C_SR1_TIMEOUT_MASK )? SET: CLEARED;
	case SMBALERT:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_SMBALERT_MASK) == I2C_SR1_SMBALERT_MASK )? SET: CLEARED;

		//----------SR2----------
	case MSL:
		return ( (I2Cx->I2C_SR2 & I2C_SR2_MSL_MASK) == I2C_SR2_MSL_MASK )? SET: CLEARED;
	case BUSY:
		return ( (I2Cx->I2C_SR2 & I2C_SR2_BUSY_MASK) == I2C_SR2_BUSY_MASK )? SET: CLEARED;
	case TRA:
		return ( (I2Cx->I2C_SR2 & I2C_SR2_TRA_MASK) == I2C_SR2_TRA_MASK )? SET: CLEARED;

		//----------EV----------
	case EV1:
	case EV6:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_ADDR_MASK) == I2C_SR1_ADDR_MASK )? SET: CLEARED;
	case EV2:
	case EV7:
	case EV7_1:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_RxNE_MASK) == I2C_SR1_RxNE_MASK )? SET: CLEARED;
	case EV3:
	case EV3_1:
	case EV8:
	case EV8_1:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_TxE_MASK) == I2C_SR1_TxE_MASK )? SET: CLEARED;
	case EV3_2:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_AF_MASK) == I2C_SR1_AF_MASK )? SET: CLEARED;
	case EV4:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_STOPF_MASK) == I2C_SR1_STOPF_MASK )? SET: CLEARED;
	case EV5:
		return ( (I2Cx->I2C_SR1 & I2C_SR1_SB_MASK) == I2C_SR1_SB_MASK )? SET: CLEARED;
	case EV8_2:
		return ( ((I2Cx->I2C_SR1 & I2C_SR1_TxE_MASK) == I2C_SR1_TxE_MASK) && ((I2Cx->I2C_SR1 & I2C_SR1_BTF_MASK) == I2C_SR1_BTF_MASK) )? SET: CLEARED;

	default:
		return CLEARED;
		break;
	}
}

/**==================================================
 * @brief  Generates a START condition on the I2C bus.
 *
 * This function generates a START condition on the I2C bus by setting the START bit in the I2C_CR1 register.
 * If the start condition is not a repeated start, it waits until the bus is not busy before generating the START condition.
 * If the start condition is a repeated start, it generates the START condition directly.
 *
 * @param  I2Cx            Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  StartCondition  Specifies whether to generate a repeated start condition.
 *
 * @retval None
 *
 * @note
 *
 *
 * @example
 * I2C_generateSTART(I2C1, WithoutSTART);
 * // Generates a START condition on the I2C1 bus
 */
void I2C_generateSTART(I2Cx_REG_t* I2Cx, I2C_StartCondition_ENUM_t StartCondition)
{
	if (StartCondition != RepeatedSTART)
	{
		while(I2C_getFLAG(I2Cx, BUSY) == SET)
		{
			/*
			 * Wait until Bit 1 BUSY: Bus busy is cleared which means no communication in progress on the bus
			 * and it's allowed to send START condition.
			 * */
		}
	}

	I2Cx->I2C_CR1 |= I2C_CR1_START;
	//START bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
}

/**==================================================
 * @brief  Generates a STOP condition on the I2C bus.
 *
 * This function generates a STOP condition on the I2C bus by setting the STOP bit in the
 * I2C_CR1 register.
 *
 * @param  I2Cx  Pointer to the I2C peripheral (I2C1 or I2C2).
 *
 * @retval None
 *
 * @note
 *
 * @example
 * I2C_generateSTOP(I2C1);
 * // Generates a STOP condition on the I2C1 bus
 */
void I2C_generateSTOP(I2Cx_REG_t* I2Cx)
{
	I2Cx->I2C_CR1 |= I2C_CR1_STOP;
	//STOP bit set and cleared by software, cleared by hardware when a Stop condition is
	//detected, set by hardware when a timeout error is detected.
}

/**==================================================
 * @brief  Sends the slave address on the I2C bus.
 *
 * This function sends the slave address on the I2C bus by writing it to DR register. It handles both 7-bit and 10-bit
 * addressing modes, although 10-bit addressing is not supported yet.
 *
 * @param  I2Cx          Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  Address       The address of the slave device.
 * @param  READ_or_WRITE Specifies whether the operation is a read (master reception) or write (master transmission).
 *
 * @retval None
 *
 * @note   - The function shifts the address and adds the read/write bit in LSB for 7-bit addressing mode.
 *         - 10-bit addressing mode is not supported yet.
 *
 * @example
 * I2C_sendADDRESS(I2C1, 0x52, WRITE_TransmitterMode);
 * // Sends the address 0x52 with write mode on the I2C1 bus
 */
void I2C_sendADDRESS(I2Cx_REG_t* I2Cx, uint16_t Address, I2C_ReadWrite_ENUM_t READ_or_WRITE)
{
	uint8_t I2Cx_INDEX = (I2Cx == I2C1 )? I2C1_INDEX: I2C2_INDEX;

	if (I2C_globeConfig[I2Cx_INDEX].I2C_OwnAddress.I2C_AddressMode == I2C_AddressMode_7BITS)
	{
		/*
		The master can decide to enter Transmitter or Receiver mode depending on the LSB of the
		slave address sent.
		• In 7-bit addressing mode,
			– To enter Transmitter mode, a master sends the slave address with LSB reset.
			– To enter Receiver mode, a master sends the slave address with LSB set.
		 */
		I2Cx->I2C_DR = ( (uint8_t)(Address << 1) | (READ_or_WRITE) );
	}

	else	//In case of I2C_AddressMode_10BITS
	{
		//NOT SUPPORTED YET.
	}
}

/**==================================================
 * @brief  Configures the acknowledge condition on the I2C bus.
 *
 * This function enables or disables the acknowledge condition on the I2C bus by setting
 * or clearing the ACK bit in the I2C_CR1 register.
 *
 * @param  I2Cx                	Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  AcknowledgeCondition Specifies whether to enable or disable the acknowledge condition.
 *
 * @retval None
 *
 * @note
 *
 * @example
 * configureACKNOWLEDGE(I2C1, ENABLE);
 * // Enables the acknowledge condition on the I2C1 bus
 */
void configureACKNOWLEDGE(I2Cx_REG_t* I2Cx, EnableDisable_ENUM_t AcknowledgeCondition)
{
	if (AcknowledgeCondition == ENABLE)
	{
		I2Cx->I2C_CR1 |= I2C_CR1_ACK;
	}
	else	//In case of Disable
	{
		I2Cx->I2C_CR1 &= ~(I2C_CR1_ACK);
	}
}

/**==================================================
 * @brief  Configures the GPIO pins for the specified I2C peripheral.
 *
 * This function configures the GPIO pins for the specified I2C peripheral (I2C1 or I2C2)
 * with the appropriate setting for SCL and SDA lines.
 *
 * @param  I2Cx  Pointer to the I2C peripheral (I2C1 or I2C2).
 *
 * @retval None
 *
 * @note   - The function configures the GPIO pins as alternate function open drain with a speed of 2 MHz.
 *         - The function performs the following steps:
 *           - Configures the SCL and SDA pins for I2C1 (PB6 and PB7).
 *           - Configures the SCL and SDA pins for I2C2 (PB10 and PB11).
 *         - Configuration of SMBA pin is not supported yet.
 *
 * @example
 * MCAL_I2C_ConfigurePin(I2C1);
 * // Configures the GPIO pins for I2C1
 */
void MCAL_I2C_ConfigurePin(I2Cx_REG_t* I2Cx)
{
	GPIO_PinConfig_t I2C_pinConfig;

	if (I2Cx == I2C1)
	{
		/*	I2C1_SMBA --> PB5
		 *	I2C1_SCL --> PB6
		 * 	I2C1_SDA --> PB7
		 * */

		//==========SCL==========
		//Configure SCL:  Alternate function open drain
		I2C_pinConfig.GPIO_PinNumber = GPIO_PinNumber_6;
		I2C_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNOPENDRAIN;
		I2C_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
		MCAL_GPIO_InitPin(GPIOB, &I2C_pinConfig);

		//==========SDA==========
		//Configure SDA:  Alternate function open drain
		I2C_pinConfig.GPIO_PinNumber = GPIO_PinNumber_7;
		I2C_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNOPENDRAIN;
		I2C_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
		MCAL_GPIO_InitPin(GPIOB, &I2C_pinConfig);
	}

	else if (I2Cx == I2C2)
	{
		/*	I2C2_SMBA --> PB12
		 *	I2C2_SCL --> PB10
		 * 	I2C2_SDA --> PB11
		 * */

		//==========SCL==========
		//Configure SCL:  Alternate function open drain
		I2C_pinConfig.GPIO_PinNumber = GPIO_PinNumber_10;
		I2C_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNOPENDRAIN;
		I2C_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
		MCAL_GPIO_InitPin(GPIOB, &I2C_pinConfig);

		//==========SDA==========
		//Configure SDA:  Alternate function open drain
		I2C_pinConfig.GPIO_PinNumber = GPIO_PinNumber_11;
		I2C_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNOPENDRAIN;
		I2C_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
		MCAL_GPIO_InitPin(GPIOB, &I2C_pinConfig);
	}
}

//==========================================================================================
//============================APIs Supported by MCAL I2C DRIVER=============================
//==========================================================================================

void MCAL_I2C_Init(I2Cx_REG_t* I2Cx, I2C_Config_t* I2C_Config)
{
	uint8_t APB1_Freq_MHz = 8U;

	//==========STEP0: control registers safety==========

	//Define temporary control registers variables and initialize them with reset value for safety
	uint16_t tempI2C_CR1 = 0U;
	uint16_t tempI2C_CR2 = 0U;
	uint16_t tempI2C_OAR1 = 0U;
	uint16_t tempI2C_OAR2 = 0U;
	uint16_t tempI2C_CCR = 0U;
	uint16_t tempI2C_TRISE = 0U;

	//==========STEP1: Enable I2C CLK & Copy I2C_Config to I2C_globeConfig==========

	//Enable I2Cx CLK & copy I2C_Config to I2C_globeConfig by value
	if (I2Cx == I2C1)
	{
		RCC_I2C1_CLK_EN();
		I2C_globeConfig[I2C1_INDEX] = *I2C_Config;		//Copy the value not the reference
	}
	else if (I2Cx == I2C2)
	{
		RCC_I2C2_CLK_EN();
		I2C_globeConfig[I2C2_INDEX] = *I2C_Config;		//Copy the value not the reference
	}

	//==========STEP2: Configure GPIO For I2Cx==========

	MCAL_I2C_ConfigurePin(I2Cx);

	//==========STEP3: Configure I2Cx control register CR1==========

	tempI2C_CR1 |= I2C_Config->I2C_ClockStretching;

	//==========STEP4: Program the peripheral input clock For I2Cx==========

	APB1_Freq_MHz = MCAL_RCC_getPCLK1()/1000000;
	tempI2C_CR2 |= APB1_Freq_MHz << I2C_CR2_FREQ_POSITION;

	//==========STEP5: Configure the clock control registers For I2Cx==========

	if (I2C_Config->I2C_ClockSpeed == I2C_ClockSpeed_SM_50kHz || I2C_Config->I2C_ClockSpeed == I2C_ClockSpeed_SM_100kHz)
	{
		tempI2C_CCR &= ~(I2C_CCR_FS);

		//Thigh = CCR * TPCLK1
		//Tlow = CCR * TPCLK1
		//Ti2c = 2 * CCR * TPCLK1
		//CCR = (FreqPCLK1 / Freqi2c) / 2

		tempI2C_CCR |= ( ((MCAL_RCC_getPCLK1() / I2C_Config->I2C_ClockSpeed) >> 1) << I2C_CCR_CCR_POSITION );
	}
	else	//In case of FM
	{
		//NOT SUPPORTED YET.
	}

	//==========STEP6: Configure the rise time register For I2Cx==========

	//	The purpose is to keep a stable SCL frequency whatever the SCL rising edge duration.
	//	These bits must be programmed with the maximum SCL rise time given in the I2C bus
	//	specification, incremented by 1.
	//	For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns.
	//	If, in the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and TPCLK1 = 125 ns
	//	therefore the TRISE[5:0] bits must be programmed with 09h.
	//	(1000 ns / 125 ns = 8 + 1)

	tempI2C_TRISE |= ( (APB1_Freq_MHz + 1) << I2C_TRISE_TRISE_POSITION );

	//==========STEP7: Enable Acknowledge & Configure I2Cx OAR1 and OAR2 registers in case of Slave Device==========

	if (I2C_Config->I2C_DeviceMode == I2C_DeviceMode_SLAVE)
	{
		//Enable Acknowledge
		configureACKNOWLEDGE(I2Cx, ENABLE);

		//Configure I2Cx OAR1 and OAR2 registers

		//OAR1 Bit 14 Reserved: Should always be kept at 1 by software.
		tempI2C_OAR1 |= 1 << 14;

		tempI2C_OAR1 |= I2C_Config->I2C_OwnAddress.I2C_AddressMode;
		tempI2C_OAR2 |= I2C_Config->I2C_OwnAddress.I2C_DualAddressing;

		if (I2C_Config->I2C_OwnAddress.I2C_AddressMode == I2C_AddressMode_7BITS)
		{
			tempI2C_OAR1 |= I2C_Config->I2C_OwnAddress.I2C_PrimaryAddress << I2C_OAR1_ADD_1_7_POSITION;

			if (I2C_Config->I2C_OwnAddress.I2C_DualAddressing == I2C_DualAddressing_EN)
			{
				tempI2C_OAR2 |= I2C_Config->I2C_OwnAddress.I2C_SecondryAddress << I2C_OAR2_ADD2_1_7_POSITION;
			}
		}

		else	//In case of I2C_AddressMode_10BITS
		{
			//NOT SUPPORTED YET.
		}
	}

	//==========STEP8: Enable/disable I2Cx interrupts & NVIC==========

	//Enable I2Cx Interrupts
	tempI2C_CR2 |= I2C_Config->I2C_Interrupt_EN;

	//Enable I2Cx NVIC IRQs
	if (I2C_Config->I2C_Interrupt_EN != I2C_Interrupt_EN_NONE)
	{
		if (I2Cx == I2C1)
		{
			switch (I2C_Config->I2C_Interrupt_EN)
			{
			case I2C_Interrupt_EN_ITEVTEN:
			case I2C_Interrupt_EN_ITEVTEN_and_ITBUFEN:	//Handles ITEVTEN | ITEVTEN_and_ITBUFEN too
				NVIC_I2C1_EV_IRQ31_EN;
				break;
			case I2C_Interrupt_EN_ITERREN:
				NVIC_I2C1_ER_IRQ32_EN;
				break;
			default:	//In case of ITEVTEN | ITERREN or ITEVTEN_and_ITBUFEN | ITERREN
				NVIC_I2C1_EV_IRQ31_EN;
				NVIC_I2C1_ER_IRQ32_EN;
				break;
			}
		}
		else if (I2Cx == I2C2)
		{
			switch (I2C_Config->I2C_Interrupt_EN)
			{
			case I2C_Interrupt_EN_ITEVTEN:
			case I2C_Interrupt_EN_ITEVTEN_and_ITBUFEN:	//Handles ITEVTEN | ITEVTEN_and_ITBUFEN too
				NVIC_I2C2_EV_IRQ33_EN;
				break;
			case I2C_Interrupt_EN_ITERREN:
				NVIC_I2C2_ER_IRQ34_EN;
				break;
			default:	//In case of ITEVTEN | ITERREN or ITEVTEN_and_ITBUFEN | ITERREN
				NVIC_I2C2_EV_IRQ33_EN;
				NVIC_I2C2_ER_IRQ34_EN;
				break;
			}
		}
	}

	//==========STEP9: Write on the real control registers==========

	I2Cx->I2C_CR1 = (uint32_t)tempI2C_CR1;
	I2Cx->I2C_CR2 = (uint32_t)tempI2C_CR2;
	I2Cx->I2C_OAR1 = (uint32_t)tempI2C_OAR1;
	I2Cx->I2C_OAR2 = (uint32_t)tempI2C_OAR2;
	I2Cx->I2C_CCR = (uint32_t)tempI2C_CCR;
	I2Cx->I2C_TRISE = (uint32_t)tempI2C_TRISE;

	//==========STEP10: enable peripheral bit==========

	//Set I2Cx enable bit
	I2Cx->I2C_CR1 |= I2C_CR1_PE;
}
void MCAL_I2C_DeInit(I2Cx_REG_t* I2Cx)
{
	//Reset I2Cx CLK & disable I2Cx NVIC IRQs
	if (I2Cx == I2C1)
	{
		RCC_I2C1_CLK_RST();
		NVIC_I2C1_EV_IRQ31_DIS;
		NVIC_I2C1_ER_IRQ32_DIS;

	}
	else if (I2Cx == I2C2)
	{
		RCC_I2C2_CLK_RST();
		NVIC_I2C2_EV_IRQ33_DIS;
		NVIC_I2C2_ER_IRQ34_DIS;
	}
}
void MCAL_I2C_Master_SendData_Polling(I2Cx_REG_t* I2Cx, uint16_t SlaveAddress, EnableDisable_ENUM_t SlaveAddressTransferCondition, uint8_t* pTX_Buffer, uint32_t TX_NumberOfBytes, I2C_StartCondition_ENUM_t StartCondition, EnableDisable_ENUM_t StopCondition)
{
	//0. Ensure that number of bytes is 1 at least (more than 0).
	if( (uint32_t)TX_NumberOfBytes > 0 )
	{
		if (StartCondition != WithoutSTART)
		{
			//1. Send START condition.
			I2C_generateSTART(I2Cx, StartCondition);

			//2. Wait for EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
			while(I2C_getFLAG(I2Cx, EV5) == CLEARED)
			{
				/*
				 * Wait until EV5 is set.
				 * */
			}
		}

		if (SlaveAddressTransferCondition != DISABLE)
		{
			//3. Send slave address.
			I2C_sendADDRESS(I2Cx, SlaveAddress, WRITE_TransmitterMode);

			//4. Wait for EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
			while(I2C_getFLAG(I2Cx, EV6) == CLEARED)
			{
				/*
				 * Wait until EV6 is set.
				 * */
			}
		}

		//5. Wait for EV8_1: TxE=1, shift register empty, data register empty, then write Data0 in DR.
		while(I2C_getFLAG(I2Cx, EV8_1) == CLEARED)
		{
			/*
			 * Wait until EV8_1 is set.
			 * */
		}
		I2Cx->I2C_DR = pTX_Buffer[0];

		//6. Wait for EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
		for (int Data = 1; Data < TX_NumberOfBytes; Data++)
		{
			while(I2C_getFLAG(I2Cx, EV8) == CLEARED)
			{
				/*
				 * Wait until EV8 is set.
				 * */
			}
			I2Cx->I2C_DR = pTX_Buffer[Data];
		}

		//7. Wait for EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition.
		while(I2C_getFLAG(I2Cx, EV8_2) == CLEARED)
		{
			/*
			 * Wait until EV8_2 is set.
			 * */
		}
		if (StopCondition == ENABLE)
		{
			I2C_generateSTOP(I2Cx);
		}
	}

	else	//In case 0 number of bytes or any other negative value.
	{
		I2C_generateSTOP(I2Cx);
	}
}
void MCAL_I2C_Master_ReceiveData_Polling(I2Cx_REG_t* I2Cx, uint16_t SlaveAddress, EnableDisable_ENUM_t SlaveAddressTransferCondition, uint8_t* pRX_Buffer, uint32_t RX_NumberOfBytes, I2C_StartCondition_ENUM_t StartCondition, EnableDisable_ENUM_t StopCondition)
{
	//0. Ensure that number of bytes is 1 at least (more than 0).
	if( (uint32_t)RX_NumberOfBytes > 0 )
	{
		if (StartCondition != WithoutSTART)
		{
			//1. Send START condition.
			I2C_generateSTART(I2Cx, StartCondition);

			//2. Wait for EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
			while(I2C_getFLAG(I2Cx, EV5) == CLEARED)
			{
				/*
				 * Wait until EV5 is set.
				 * */
			}
		}

		if (SlaveAddressTransferCondition != DISABLE)
		{
			//3. Send slave address.
			I2C_sendADDRESS(I2Cx, SlaveAddress, READ_ReceiverMode);

			//4. Wait for EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
			while(I2C_getFLAG(I2Cx, EV6) == CLEARED)
			{
				/*
				 * Wait until EV6 is set.
				 * */
			}
		}

		//5. Wait for EV7: RxNE=1, cleared by reading DR register.
		/*
		 * NOTE: In case a single byte has to be received, the Acknowledge disable and the Stop
		 * condition generation are made just after EV6 (in EV6_1, just after ADDR is cleared).
		 */
		for (uint32_t Data = 0; (int32_t)Data < (int32_t)(RX_NumberOfBytes-1); Data++)
		{
			//Enable Acknowledge
			configureACKNOWLEDGE(I2Cx, ENABLE);

			while(I2C_getFLAG(I2Cx, EV7) == CLEARED)
			{
				/*
				 * Wait until EV7 is set.
				 * */
			}
			pRX_Buffer[Data] = (uint8_t) I2Cx->I2C_DR;
		}

		//6. EV6_1 (single byte reception) or EV7_1 (multiple bytes reception): Disable Acknowledge, Read last byte RxNE event and Stop condition generation.

		//Disable Acknowledge
		configureACKNOWLEDGE(I2Cx, DISABLE);

		//Program STOP before reading last byte.
		if (StopCondition == ENABLE)
		{
			I2C_generateSTOP(I2Cx);
		}

		while(I2C_getFLAG(I2Cx, EV7_1) == CLEARED)
		{
			/*
			 * Wait until EV7 is set.
			 * */
		}
		pRX_Buffer[RX_NumberOfBytes-1] = (uint8_t) I2Cx->I2C_DR;

	}

	else	//In case 0 number of bytes or any other negative value.
	{
		I2C_generateSTOP(I2Cx);
	}
}
void MCAL_I2C_Slave_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte)
{
	I2Cx->I2C_DR = *pTX_Byte;
}
void MCAL_I2C_Slave_ReceiveData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pRX_Byte)
{
	*pRX_Byte = (uint8_t) I2Cx->I2C_DR;
}

void MCAL_I2C_Master_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte)
{
	I2Cx->I2C_DR = *pTX_Byte;
}
void MCAL_I2C_Master_ReceiveData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pRX_Byte)
{
	*pRX_Byte = (uint8_t) I2Cx->I2C_DR;
}
//==========================================================================================
//=========================================I2C ISR==========================================
//==========================================================================================

/**==================================================
 * @brief  Handles I2C1 event interrupt requests.
 *
 * This function handles the I2C1 event interrupt requests. It checks various I2C flags
 * and calls the appropriate user callback function based on the flag status.
 *
 * @retval I2C_InterruptHandler_ENUM_t  The status of the interrupt handling (HANDLED or NOT_HANDLED).
 *
 * @note   - The function checks the status of various I2C flags and calls the user callback function.
 *         - The function performs the following steps:
 *           - Checks for slave mode events (address matched, data request, data received, stop received).
 *           - Checks for master mode events (start sent, address sent, TxE, RxNE).
 */
I2C_InterruptHandler_ENUM_t I2C1_EV_IRQHandler(void)
{
	/*
	 * This EV_IRQHandler cannot be called unless ITEVFEN in CR2 register is set,
	 * however ITBUFEN is not necessary to be set to call this this EV_IRQHandler.
	 * So, ITBUFEN bit must be checked to determine it is set or cleared in interrupts based on TxE and RxNE.
	 */

	//----------Slave----------
	if (I2C_getFLAG(I2C1, MSL)==CLEARED && I2C_getFLAG(I2C1, BUSY)==SET)
	{
		// 7-bit address Matched
		// EV1: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		if (I2C_getFLAG(I2C1, EV1)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Slave_Address7Matched);
			return HANDLED;
		}

		// Data request from master
		// EV3: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
		// EV3_1: TxE=1, shift register empty, data register empty, write Data1 in DR
		if (I2C_getFLAG(I2C1, EV3)==SET && I2C_getFLAG(I2C1, TRA)==SET && I2C_getFLAG(I2C1, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Slave_DataRequestFromMaster);
			return HANDLED;
		}

		// Data received from master
		// EV2: RxNE=1, cleared by reading DR register.
		if (I2C_getFLAG(I2C1, EV2)==SET && I2C_getFLAG(I2C1, TRA)==CLEARED && I2C_getFLAG(I2C1, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Slave_DataReceivedFromMaster);
			return HANDLED;
		}

		// Stop received
		// EV4: STOPF=1, cleared by reading SR1 register followed by writing to the CR1 register.
		if (I2C_getFLAG(I2C1, EV4)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Slave_StopReceived);
			return HANDLED;
		}
	}

	//----------Master----------
	if (I2C_getFLAG(I2C1, MSL)==SET && I2C_getFLAG(I2C1, BUSY)==SET)
	{
		// Start bit sent (Master)
		// EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
		if (I2C_getFLAG(I2C1, EV5)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Master_StartSent);
			return HANDLED;
		}

		// 7-bit address Sent
		// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		if (I2C_getFLAG(I2C1, EV6)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Master_Address7Sent);
			return HANDLED;
		}

		// Data register empty TxE=1 (Transmission)
		// EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
		// EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR
		if (I2C_getFLAG(I2C1, EV8)==SET && I2C_getFLAG(I2C1, TRA)==SET && I2C_getFLAG(I2C1, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Master_TxE);
			return HANDLED;
		}

		// Data register empty RxNE=1 (Reception)
		// EV7: RxNE=1, cleared by reading DR register
		// EV7_1: RxNE=1, cleared by reading DR register, program ACK=0 and STOP request
		if (I2C_getFLAG(I2C1, RxNE)==SET && I2C_getFLAG(I2C1, TRA)==CLEARED && I2C_getFLAG(I2C1, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C1_INDEX].I2C_ptr_irqCallbackFUN(Master_RxNE);
			return HANDLED;
		}

	}
	return NOT_HANDLED;
}
void I2C1_ER_IRQHandler(void)
{

}

/**==================================================
 * @brief  Handles I2C2 event interrupt requests.
 *
 * This function handles the I2C2 event interrupt requests. It checks various I2C flags
 * and calls the appropriate user callback function based on the flag status.
 *
 * @retval I2C_InterruptHandler_ENUM_t  The status of the interrupt handling (HANDLED or NOT_HANDLED).
 *
 * @note   - The function checks the status of various I2C flags and calls the user callback function.
 *         - The function performs the following steps:
 *           - Checks for slave mode events (address matched, data request, data received, stop received).
 *           - Checks for master mode events (start sent, address sent, TxE, RxNE).
 */
I2C_InterruptHandler_ENUM_t I2C2_EV_IRQHandler(void)
{
	/*
	 * This EV_IRQHandler cannot be called unless ITEVFEN in CR2 register is set,
	 * however ITBUFEN is not necessary to be set to call this this EV_IRQHandler.
	 * So, ITBUFEN bit must be checked to determine it is set or cleared in interrupts based on TxE and RxNE.
	 */

	//----------Slave----------
	if (I2C_getFLAG(I2C2, MSL)==CLEARED && I2C_getFLAG(I2C2, BUSY)==SET)
	{
		// 7-bit address Matched
		// EV1: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		if (I2C_getFLAG(I2C2, EV1)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Slave_Address7Matched);
			return HANDLED;
		}

		// Data request from master
		// EV3: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
		// EV3_1: TxE=1, shift register empty, data register empty, write Data1 in DR
		if (I2C_getFLAG(I2C2, EV3)==SET && I2C_getFLAG(I2C2, TRA)==SET && I2C_getFLAG(I2C2, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Slave_DataRequestFromMaster);
			return HANDLED;
		}

		// Data received from master
		// EV2: RxNE=1, cleared by reading DR register.
		if (I2C_getFLAG(I2C2, EV2)==SET && I2C_getFLAG(I2C2, TRA)==CLEARED && I2C_getFLAG(I2C2, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Slave_DataReceivedFromMaster);
			return HANDLED;
		}

		// Stop received
		// EV4: STOPF=1, cleared by reading SR1 register followed by writing to the CR1 register.
		if (I2C_getFLAG(I2C2, EV4)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Slave_StopReceived);
			return HANDLED;
		}
	}

	//----------Master----------
	if (I2C_getFLAG(I2C2, MSL)==SET && I2C_getFLAG(I2C2, BUSY)==SET)
	{
		// Start bit sent (Master)
		// EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
		if (I2C_getFLAG(I2C2, EV5)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Master_StartSent);
			return HANDLED;
		}

		// 7-bit address Sent
		// EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		if (I2C_getFLAG(I2C2, EV6)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Master_Address7Sent);
			return HANDLED;
		}

		// Data register empty TxE=1 (Transmission)
		// EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register.
		// EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR
		if (I2C_getFLAG(I2C2, EV8)==SET && I2C_getFLAG(I2C2, TRA)==SET && I2C_getFLAG(I2C2, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Master_TxE);
			return HANDLED;
		}

		// Data register empty RxNE=1 (Reception)
		// EV7: RxNE=1, cleared by reading DR register
		// EV7_1: RxNE=1, cleared by reading DR register, program ACK=0 and STOP request
		if (I2C_getFLAG(I2C2, RxNE)==SET && I2C_getFLAG(I2C2, TRA)==CLEARED && I2C_getFLAG(I2C2, ITBUFEN)==SET)
		{
			I2C_globeConfig[I2C2_INDEX].I2C_ptr_irqCallbackFUN(Master_RxNE);
			return HANDLED;
		}

	}
	return NOT_HANDLED;
}
void I2C2_ER_IRQHandler(void)
{

}
