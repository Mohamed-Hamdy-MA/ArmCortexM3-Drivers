/*
 * STM32F103x6_SPI_Driver.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_SPI_Driver.h"

//==========================================================================================
//=====================================Assisting Macros=====================================
//==========================================================================================
#define SPI1_INDEX		0U
#define SPI2_INDEX		1U

//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================
SPI_Config_t SPI_globeConfig[2];		//Uninitialized global array, so it will be initialized to 0 by default.

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================

/**==================================================
 * @brief  Configures the GPIO pins for the specified SPI peripheral.
 *
 * This function configures the GPIO pins for the specified SPI peripheral (SPI1 or SPI2)
 * based on the provided configuration settings and system configuration.
 *
 * @param  SPIx        Pointer to the SPI peripheral (SPI1 or SPI2).
 * @param  SPI_Config  Pointer to the SPI configuration structure containing the
 *                     initialization settings.
 * @param  SPI_System  Enumeration specifying the SPI system configuration.
 *
 * @retval None
 *
 * @note   - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *         - The function performs the following steps:
 *           - Configures the NSS pin based on the provided NSS configuration.
 *           - Configures the SCK pin based on the device mode (master or slave).
 *           - Configures the MISO pin based on the device mode and communication mode.
 *           - Configures the MOSI pin based on the device mode and communication mode.
 *         - The pin configurations are based on the recommended GPIO configurations
 *           for device peripherals on STM32F10xxx RM0008 reference manual table 25. SPI.
 *
 * @example
 * SPI_Config_t spiConfig;
 * spiConfig.SPI_DeviceMode = SPI_DeviceMode_MASTER;
 * spiConfig.SPI_CommunicationMode = SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX;
 * spiConfig.SPI_NSS = SPI_NSS_SW_HIGH;
 *
 * MCAL_SPI_ConfigurePin(SPI1, &spiConfig, SingleMaster_SingleSlave);
 * // Configures the GPIO pins for SPI1 based on the specified configuration
 */
void MCAL_SPI_ConfigurePin(SPIx_REG_t* SPIx, SPI_Config_t* SPI_Config, SPI_System_ENUM_t SPI_System)
{
	GPIO_PinConfig_t SPI_pinConfig;

	if (SPIx == SPI1)
	{
		/*	SPI1_NSS --> PA4
		 *	SPI1_SCK --> PA5
		 * 	SPI1_MISO --> PA6
		 * 	SPI1_MOSI --> PA7
		 * */

		//==========NSS==========

		if (SPI_Config->SPI_NSS == SPI_NSS_HW_MASTER_INPUT || SPI_Config->SPI_NSS == SPI_NSS_HW_SLAVE_INPUT)
		{
			//Configure NSS: Input floating
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_4;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
		}
		else if (SPI_Config->SPI_NSS == SPI_NSS_HW_MASTER_OUTPUT)
		{
			//Configure NSS: Alternate function push-pull
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_4;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
		}

		//==========SCK==========

		if (SPI_Config->SPI_DeviceMode == SPI_DeviceMode_MASTER)
		{
			//Configure SCK: Alternate function push-pull
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_5;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
		}
		else	//In case of SPI_DeviceMode_SLAVE
		{
			//Configure SCK: Input floating
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_5;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
		}

		//==========MISO==========

		if (SPI_Config->SPI_DeviceMode == SPI_DeviceMode_MASTER)
		{
			if (SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX || SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_RXonly)
			{
				//Configure MISO: Input floating
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_6;
				SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
				MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
			}
		}
		else	//In case of SPI_DeviceMode_SLAVE
		{
			if (SPI_Config->SPI_CommunicationMode != SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_RX)
			{
				//Configure MOSI: Alternate function push-pull OR Alternate function open drain based on (point to point) OR (multi-slave)
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_6;
				SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
				if (SPI_System == SingleMaster_SingleSlave || SPI_System == MultipleMasters_SingleSlave)
				{
					SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
				}
				else	//In case of SingleMaster_SingleSlave || MultipleMasters_SingleSlave
				{
					SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_OPENDRAIN;
				}
				MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
			}
		}

		//==========MOSI==========

		if (SPI_Config->SPI_DeviceMode == SPI_DeviceMode_MASTER)
		{
			if (SPI_Config->SPI_CommunicationMode != SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_RX)
			{
				//Configure MOSI: Alternate function push-pull
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_7;
				SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
				SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
				MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
			}
		}
		else	//In case of SPI_DeviceMode_SLAVE
		{
			if (SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX || SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_RXonly)
			{
				//Configure MOSI: Input floating
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_7;
				SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
				MCAL_GPIO_InitPin(GPIOA, &SPI_pinConfig);
			}
		}

	}



	else if (SPIx == SPI2)
	{
		/*	SPI2_NSS --> PB12
		 *	SPI2_SCK --> PB13
		 * 	SPI2_MISO --> PB14
		 * 	SPI2_MOSI --> PB15
		 * */

		//==========NSS==========

		if (SPI_Config->SPI_NSS == SPI_NSS_HW_MASTER_INPUT || SPI_Config->SPI_NSS == SPI_NSS_HW_SLAVE_INPUT)
		{
			//Configure NSS: Input floating
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_12;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
		}
		else if (SPI_Config->SPI_NSS == SPI_NSS_HW_MASTER_OUTPUT)
		{
			//Configure NSS: Alternate function push-pull
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_12;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
		}

		//==========SCK==========

		if (SPI_Config->SPI_DeviceMode == SPI_DeviceMode_MASTER)
		{
			//Configure SCK: Alternate function push-pull
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_13;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
		}
		else	//In case of SPI_DeviceMode_SLAVE
		{
			//Configure SCK: Input floating
			SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_13;
			SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
		}

		//==========MISO==========

		if (SPI_Config->SPI_DeviceMode == SPI_DeviceMode_MASTER)
		{
			if (SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX || SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_RXonly)
			{
				//Configure MOSI: Alternate function push-pull
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_14;
				SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
				MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
			}
		}
		else	//In case of SPI_DeviceMode_SLAVE
		{
			if (SPI_Config->SPI_CommunicationMode != SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_RX)
			{
				//Configure MOSI: Alternate function push-pull OR Alternate function open drain based on (point to point) OR (multi-slave)
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_14;
				SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
				if (SPI_System == SingleMaster_SingleSlave || SPI_System == MultipleMasters_SingleSlave)
				{
					SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
				}
				else	//In case of SingleMaster_SingleSlave || MultipleMasters_SingleSlave
				{
					SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_OPENDRAIN;
				}
				MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
			}
		}

		//==========MOSI==========

		if (SPI_Config->SPI_DeviceMode == SPI_DeviceMode_MASTER)
		{
			if (SPI_Config->SPI_CommunicationMode != SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_RX)
			{
				//Configure MOSI: Alternate function push-pull
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_15;
				SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
				SPI_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
				MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
			}
		}
		else	//In case of SPI_DeviceMode_SLAVE
		{
			if (SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX || SPI_Config->SPI_CommunicationMode == SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_RXonly)
			{
				//Configure MOSI: Input floating
				SPI_pinConfig.GPIO_PinNumber = GPIO_PinNumber_15;
				SPI_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
				MCAL_GPIO_InitPin(GPIOB, &SPI_pinConfig);
			}
		}

	}
}

//==========================================================================================
//============================APIs Supported by MCAL SPI DRIVER=============================
//==========================================================================================

void MCAL_SPI_Init(SPIx_REG_t* SPIx, SPI_Config_t* SPI_Config, SPI_System_ENUM_t SPI_System)
{
	//==========STEP0: control registers safety==========

	//Define temporary control registers variables and initialize them with reset value for safety
	uint32_t tempSPI_CR1 = 0U;
	uint32_t tempSPI_CR2 = 0U;

	//==========STEP1: Enable SPI CLK & set SPI enable bit==========

	//Enable SPIx CLK & copy SPI_Config to SPI_globeConfig by value
	if (SPIx == SPI1)
	{
		RCC_SPI1_CLK_EN();
		SPI_globeConfig[SPI1_INDEX] = *SPI_Config;		//Copy the value not the reference
	}
	else if (SPIx == SPI2)
	{
		RCC_SPI2_CLK_EN();
		SPI_globeConfig[SPI2_INDEX] = *SPI_Config;		//Copy the value not the reference
	}

	//Set SPI enable bit
	tempSPI_CR1 |= (1 << 6);		//Bit 6 SPE: SPI enable

	//==========STEP2: Configure GPIO FOR SPIx==========

	MCAL_SPI_ConfigurePin(SPIx, SPI_Config, SPI_System);

	//==========STEP3: Configure SPIx control registers==========

	tempSPI_CR1 |= SPI_Config->SPI_DeviceMode;
	tempSPI_CR1 |= SPI_Config->SPI_CommunicationMode;
	tempSPI_CR1 |= SPI_Config->SPI_DataLength;
	tempSPI_CR1 |= SPI_Config->SPI_FrameFormat;
	tempSPI_CR1 |= SPI_Config->SPI_ClockPolarity;
	tempSPI_CR1 |= SPI_Config->SPI_ClockPhase;
	tempSPI_CR1 |= SPI_Config->SPI_ClockPreScaler;

	//==========STEP4: Configure NSS on CR1 & CR2 control registers==========

	if (SPI_Config->SPI_NSS == SPI_NSS_SW_HIGH || SPI_Config->SPI_NSS == SPI_NSS_SW_LOW)
	{
		tempSPI_CR1 |= SPI_Config->SPI_NSS;
	}
	else	//In case of SPI_NSS_HW_SLAVE_INPUT || SPI_NSS_HW_MASTER_INPUT || SPI_NSS_HW_MASTER_OUTPUT
	{
		tempSPI_CR1 &= ~(1 << 9);				//Clear CR1Bit9 SSM: Software slave management
		tempSPI_CR2 |= SPI_Config->SPI_NSS;		//Copy the configuration from @reference SPI_NSS_HW_Define to CR2Bit2 SSOE: SS output enable
	}

	//==========STEP5: Enable/disable SPIx interrupts & NVIC==========

	//Enable SPIx Interrupts
	tempSPI_CR2 |= SPI_Config->SPI_Interrupt_EN;

	//Enable SPIx NVIC IRQ
	if (SPI_Config->SPI_Interrupt_EN != SPI_Interrupt_EN_NONE)
	{
		if (SPIx == SPI1)
		{
			NVIC_SPI1_IRQ35_EN;
		}
		else if (SPIx == SPI2)
		{
			NVIC_SPI2_IRQ36_EN;
		}
	}

	//==========STEP6: Write on the real control registers==========

	SPIx->SPI_CR1 = (uint32_t)tempSPI_CR1;
	SPIx->SPI_CR2 = (uint32_t)tempSPI_CR2;
}
void MCAL_SPI_DeInit(SPIx_REG_t* SPIx)
{
	//Reset SPIx CLK & disable SPIx NVIC IRQ
	if (SPIx == SPI1)
	{
		RCC_SPI1_CLK_RST();
		NVIC_SPI1_IRQ35_DIS;
	}
	else if (SPIx == SPI2)
	{
		RCC_SPI2_CLK_RST();
		NVIC_SPI2_IRQ36_DIS;
	}
}
void MCAL_SPI_SendData(SPIx_REG_t* SPIx, uint16_t* pTX_Buffer, SPI_Mechanism_ENUM_t SPI_Mechanism)
{
	if (SPI_Mechanism == SPI_PollingENABLE)
	{
		while ( ( SPIx->SPI_SR & (1 << 1) ) == 0){
			/*
			 * Wait until bit 1 the TXE (TX Buffer EMPTY) is set which means Transmit buffer empty
			 * and it's allowed to write on SPI_DR.
			 * */
		}
	}
	//Write the data to send in the SPI_DR register (this clears the TXE bit).

	if (SPIx == SPI1)
	{
		if (SPI_globeConfig[SPI1_INDEX].SPI_DataLength == SPI_DataLength_8BITS)
		{
			SPIx->SPI_DR = (*pTX_Buffer & (uint16_t)0xFF);		//take 8 bits
		}
		else //In case of SPI_DataLength_16BITS
		{
			SPIx->SPI_DR = (*pTX_Buffer & (uint16_t)0xFFFF);	//take 16 bits
		}
	}
	else if (SPIx == SPI2)
	{
		if (SPI_globeConfig[SPI2_INDEX].SPI_DataLength == SPI_DataLength_8BITS)
		{
			SPIx->SPI_DR = (*pTX_Buffer & (uint16_t)0xFF);		//take 8 bits
		}
		else //In case of SPI_DataLength_16BITS
		{
			SPIx->SPI_DR = (*pTX_Buffer & (uint16_t)0xFFFF);	//take 16 bits
		}
	}
}
void MCAL_SPI_ReceiveData(SPIx_REG_t* SPIx, uint16_t* pRX_Buffer, SPI_Mechanism_ENUM_t SPI_Mechanism)
{
	if (SPI_Mechanism == SPI_PollingENABLE)
	{
		while ( ( SPIx->SPI_SR & (1 << 0) ) == 0){
			/*
			 * Wait until bit 0 the RXNE (RX NOT EMPTY) is set which means RX buffer not empty
			 * and it's allowed to read SPI_DR.
			 * */
		}
	}
	//clearing the RXNE bit is performed by a software read to the SPI_DR register.

	if (SPIx == SPI1)
	{
		if (SPI_globeConfig[SPI1_INDEX].SPI_DataLength == SPI_DataLength_8BITS)
		{
			*((uint16_t*)pRX_Buffer) = SPIx->SPI_DR & (uint32_t)0xFF;		//take 8 bits
		}
		else //In case of SPI_DataLength_16BITS
		{
			*((uint16_t*)pRX_Buffer) = SPIx->SPI_DR & (uint32_t)0xFFFF;		//take 16 bits
		}
	}
	else if (SPIx == SPI2)
	{
		if (SPI_globeConfig[SPI2_INDEX].SPI_DataLength == SPI_DataLength_8BITS)
		{
			*((uint16_t*)pRX_Buffer) = SPIx->SPI_DR & (uint32_t)0xFF;		//take 8 bits
		}
		else //In case of SPI_DataLength_16BITS
		{
			*((uint16_t*)pRX_Buffer) = SPIx->SPI_DR & (uint32_t)0xFFFF;		//take 16 bits
		}
	}
}
void MCAL_SPI_SendReceiveData(SPIx_REG_t* SPIx, uint16_t* pTX_Buffer, SPI_Mechanism_ENUM_t SPI_SendMechanism, uint16_t* pRX_Buffer, SPI_Mechanism_ENUM_t SPI_ReceiveMechanism)
{
	MCAL_SPI_SendData(SPIx, pTX_Buffer, SPI_SendMechanism);
	MCAL_SPI_ReceiveData(SPIx, pRX_Buffer, SPI_ReceiveMechanism);
}

//==========================================================================================
//=========================================SPI ISR==========================================
//==========================================================================================
void SPI1_IRQHandler(void)
{
	SPI_InterruptSource_t SPI_InterruptSource={0};

	SPI_InterruptSource.RXNE 	= ( (SPI1->SPI_SR & (1 << 0)) >> 0 );
	SPI_InterruptSource.TXE 	= ( (SPI1->SPI_SR & (1 << 1)) >> 1 );
	SPI_InterruptSource.CRCERR 	= ( (SPI1->SPI_SR & (1 << 4)) >> 4 );
	SPI_InterruptSource.MODF 	= ( (SPI1->SPI_SR & (1 << 5)) >> 5 );
	SPI_InterruptSource.OVR 	= ( (SPI1->SPI_SR & (1 << 6)) >> 6 );

	SPI_globeConfig[SPI1_INDEX].SPI_ptr_irqCallbackFUN(SPI_InterruptSource);
}
void SPI2_IRQHandler(void)
{
	SPI_InterruptSource_t SPI_InterruptSource={0};

	SPI_InterruptSource.RXNE 	= ( (SPI2->SPI_SR & (1 << 0)) >> 0 );
	SPI_InterruptSource.TXE 	= ( (SPI2->SPI_SR & (1 << 1)) >> 1 );
	SPI_InterruptSource.CRCERR 	= ( (SPI2->SPI_SR & (1 << 4)) >> 4 );
	SPI_InterruptSource.MODF 	= ( (SPI2->SPI_SR & (1 << 5)) >> 5 );
	SPI_InterruptSource.OVR 	= ( (SPI2->SPI_SR & (1 << 6)) >> 6 );

	SPI_globeConfig[SPI2_INDEX].SPI_ptr_irqCallbackFUN(SPI_InterruptSource);
}
