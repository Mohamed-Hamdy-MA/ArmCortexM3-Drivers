/*
 * STM32F103x6_USART_Driver.c
 *
 *  Created on: Aug 26, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_USART_Driver.h"

//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================
USART_Config_t USART_globeConfig[3];		//Uninitialized global array, so it will be initialized to 0 by default.

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================

/**==================================================
 * @brief  Calculates the USARTDIV value for the specified USART peripheral.
 *
 * This function calculates the USARTDIV value required to configure the baud rate
 * for the specified USART peripheral based on the peripheral clock and desired baud rate.
 *
 * @param  USART_PCLK     The peripheral clock frequency for the USART.
 * @param  USART_BaudRate The desired baud rate for the USART.
 *
 * @retval uint16_t       The calculated USARTDIV value to be written to the USART_BRR register.
 *
 * @note    - Ensure that the USART peripheral clock frequency and desired baud rate are correctly specified.
 *          - The function performs the following steps:
 *            - Calculates the USARTDIV value.
 *            - Splits the USARTDIV value into mantissa and fraction parts.
 *            - Combines the mantissa and fraction parts to form the final USART_BRR value.
 *
 * @example
 * uint16_t usart_brr = CalculateUSARTDIV(36000000, 9600);
 * // Calculates the USARTDIV value for a peripheral clock of 36 MHz and a baud rate of 9600
 */
uint16_t CalculateUSARTDIV(uint32_t USART_PCLK ,uint32_t USART_BaudRate)
{
	uint32_t USARTDIV = (uint32_t)( USART_PCLK / (16*USART_BaudRate) );
	uint32_t USARTDIV_MUL100 = (uint32_t)( (25*USART_PCLK) / (4*USART_BaudRate) );	//(100*USART_PCLK) / (16*USART_BaudRate) --> 100/16 = 25/4
	uint32_t DIV_Mantissa = USARTDIV;
	uint32_t DIV_Mantissa_MUL100 = USARTDIV*100;
	uint32_t DIV_Fraction = (uint32_t)( 16*(USARTDIV_MUL100 - DIV_Mantissa_MUL100)/100 + 0.5 ); // +0.5 to get the nearest real number

	uint16_t USART_BRR = 0x0000;
	USART_BRR |= DIV_Fraction;
	USART_BRR |= (DIV_Mantissa << 4);

	/*
	 * Example1: Assume USART_PCLK = 36000000 & USART_BaudRate = 9600
	 * USARTDIV = 36000000 / (16*9600) = (uint32_t)234.375									--> 234
	 * USARTDIV_MUL100 = ( 36000000 / (16*9600) )*100 = (uint32_t)23437.5					--> 23437
	 * DIV_Mantissa = 234;																	--> 234   = 0b 1110 1010
	 * DIV_Mantissa_MUL100 = 234*100 = 23400												--> 23400
	 * DIV_Fraction = ((23437 - 23400)/100)+0.5 = (37/100)+0.5 = 3.7+0.5 = (uint32_t)4.2	--> 4     = 0b 0100
	 * USART_BRR = 0b 0000 1110 1010 0100 = 0x0EA4
	 * */

	return USART_BRR;
}

/**==================================================
 * @brief  Configures the GPIO pins for the specified USART peripheral.
 *
 * This function configures the GPIO pins for the specified USART peripheral (USART1, USART2, or USART3)
 * based on the provided configuration settings.
 *
 * @param  USARTx         Pointer to the USART peripheral (USART1, USART2, or USART3).
 * @param  USART_Config   Pointer to the USART configuration structure containing the
 *                        initialization settings.
 *
 * @retval None
 *
 * @note    - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *          - The function performs the following steps:
 *            - Configures the TX pin as alternate function push-pull if TX is enabled.
 *            - Configures the RX pin as input floating if RX is enabled.
 *            - Configures the CTS pin as input floating if CTS is enabled.
 *            - Configures the RTS pin as alternate function push-pull if RTS is enabled.
 *
 * @example
 * USART_Config_t usartConfig;
 * usartConfig.USART_TxRxMode = USART_TxRxMode_TXandRX;
 * usartConfig.USART_FlowControl = USART_FlowControl_NONEe;
 * MCAL_USART_ConfigurePin(USART1, &usartConfig);
 * // Configures the GPIO pins for USART1 based on the specified configuration
 */
void MCAL_USART_ConfigurePin(USARTx_REG_t* USARTx, USART_Config_t* USART_Config)
{
	GPIO_PinConfig_t USART_pinConfig;

	if (USARTx == USART1)
	{
		/*	USART1_CK --> PA8
		 *	USART1_TX --> PA9
		 * 	USART1_RX --> PA10
		 * 	USART1_CTS --> PA11
		 * 	USART1_RTS --> PA12
		 * */

		if (USART_Config->USART_TxRxMode != USART_TxRxMode_RXonly)				//Configure TX if TXonly or TXandRX
		{
			//Configure TX: Alternate function push-pull
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_9;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			USART_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
		if (USART_Config->USART_TxRxMode != USART_TxRxMode_TXonly)				//Configure RX if RXonly or TXandRX
		{
			//Configure RX: Input floating
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_10;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
		if (USART_Config->USART_FlowControl == USART_FlowControl_CTSonly || USART_Config->USART_FlowControl == USART_FlowControl_CTSandRTS)	//Configure CTS if CTSonly or CTSandRTS
		{
			//Configure CTS: Input floating
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_11;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
		if (USART_Config->USART_FlowControl == USART_FlowControl_CTSonly || USART_Config->USART_FlowControl == USART_FlowControl_CTSandRTS)	//Configure RTS if RTSonly or CTSandRTS
		{
			//Configure RTS: Alternate function push-pull
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_12;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
	}

	else if (USARTx == USART2)
	{
		/*	USART2_CK --> PA4
		 *	USART2_TX --> PA2
		 * 	USART2_RX --> PA3
		 * 	USART2_CTS --> PA0
		 * 	USART2_RTS --> PA1
		 * */

		if (USART_Config->USART_TxRxMode != USART_TxRxMode_RXonly)				//Configure TX if TXonly or TXandRX
		{
			//Configure TX: Alternate function push-pull
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_2;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			USART_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
		if (USART_Config->USART_TxRxMode != USART_TxRxMode_TXonly)				//Configure RX if RXonly or TXandRX
		{
			//Configure RX: Input floating
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_3;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
		if (USART_Config->USART_FlowControl == USART_FlowControl_CTSonly || USART_Config->USART_FlowControl == USART_FlowControl_CTSandRTS)	//Configure CTS if CTSonly or CTSandRTS
		{
			//Configure CTS: Input floating
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_0;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
		if (USART_Config->USART_FlowControl == USART_FlowControl_CTSonly || USART_Config->USART_FlowControl == USART_FlowControl_CTSandRTS)	//Configure RTS if RTSonly or CTSandRTS
		{
			//Configure RTS: Alternate function push-pull
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_1;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			USART_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOA, &USART_pinConfig);
		}
	}

	else if (USARTx == USART3)
	{
		/*	USART3_CK --> PB12
		 *	USART3_TX --> PB10
		 * 	USART3_RX --> PB11
		 * 	USART3_CTS --> PB13
		 * 	USART3_RTS --> PB14
		 * */

		if (USART_Config->USART_TxRxMode != USART_TxRxMode_RXonly)				//Configure TX if TXonly or TXandRX
		{
			//Configure TX: Alternate function push-pull
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_10;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Output_ALTFUNPUSHPULL;
			USART_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOB, &USART_pinConfig);
		}
		if (USART_Config->USART_TxRxMode != USART_TxRxMode_TXonly)				//Configure RX if RXonly or TXandRX
		{
			//Configure RX: Input floating
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_11;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOB, &USART_pinConfig);
		}
		if (USART_Config->USART_FlowControl == USART_FlowControl_CTSonly || USART_Config->USART_FlowControl == USART_FlowControl_CTSandRTS)	//Configure CTS if CTSonly or CTSandRTS
		{
			//Configure CTS: Input floating
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_13;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			MCAL_GPIO_InitPin(GPIOB, &USART_pinConfig);
		}
		if (USART_Config->USART_FlowControl == USART_FlowControl_CTSonly || USART_Config->USART_FlowControl == USART_FlowControl_CTSandRTS)	//Configure RTS if RTSonly or CTSandRTS
		{
			//Configure RTS: Alternate function push-pull
			USART_pinConfig.GPIO_PinNumber = GPIO_PinNumber_14;
			USART_pinConfig.GPIO_PinMode = GPIO_PinMode_Input_FLOATING;
			USART_pinConfig.GPIO_PinSpeed = GPIO_PinSpeed_2MHz;
			MCAL_GPIO_InitPin(GPIOB, &USART_pinConfig);
		}
	}
}

//==========================================================================================
//============================APIs Supported by MCAL USART DRIVER===========================
//==========================================================================================

void MCAL_USART_Init(USARTx_REG_t* USARTx, USART_Config_t* USART_Config)
{
	uint32_t USARTx_PCLK;

	//==========STEP1: Enable USART CLK & set USART enable bit==========

	//Enable USARTx CLK & get PCLK & copy USART_Config to USART_globeConfig by value
	if (USARTx == USART1)
	{
		RCC_USART1_CLK_EN();
		USARTx_PCLK = MCAL_RCC_getPCLK2();
		USART_globeConfig[0] = *USART_Config;		//Copy the value not the reference
	}
	else if (USARTx == USART2)
	{
		RCC_USART2_CLK_EN();
		USARTx_PCLK = MCAL_RCC_getPCLK1();
		USART_globeConfig[1] = *USART_Config;		//Copy the value not the reference
	}
	else if (USARTx == USART3)
	{
		RCC_USART3_CLK_EN();
		USARTx_PCLK = MCAL_RCC_getPCLK1();
		USART_globeConfig[2] = *USART_Config;		//Copy the value not the reference
	}
	//Set USART enable bit
	USARTx->USART_CR1 |= (1 << 13);		//Bit 13 UE: USART enable

	//==========STEP2: Configure GPIO FOR USARTx==========

	MCAL_USART_ConfigurePin(USARTx, USART_Config);

	//==========STEP3: Configure USARTx control registers==========

	USARTx->USART_CR1 |= USART_Config->USART_TxRxMode;
	USARTx->USART_CR1 |= USART_Config->USART_WordLength;
	USARTx->USART_CR1 |= USART_Config->USART_Parity;
	USARTx->USART_CR2 |= USART_Config->USART_StopBits;
	USARTx->USART_CR3 |= USART_Config->USART_FlowControl;

	//==========STEP4: Configure Mantissa & Fraction in Baud rate register (USART_BRR)==========

	USARTx->USART_BRR = (uint32_t)CalculateUSARTDIV(USARTx_PCLK, USART_Config->USART_BaudRate);

	//==========STEP5: Enable/disable USARTx interrupts & NVIC==========

	//Enable USARTx Interrupts
	USARTx->USART_CR1 |= USART_Config->USART_Interrupt_EN;

	//Enable USARTx NVIC IRQ
	if (USART_Config->USART_Interrupt_EN != USART_Interrupt_EN_NONE)
	{
		if (USARTx == USART1)
		{
			NVIC_USART1_IRQ37_EN;
		}
		else if (USARTx == USART2)
		{
			NVIC_USART2_IRQ38_EN;
		}
		else if (USARTx == USART3)
		{
			NVIC_USART3_IRQ39_EN;
		}
	}
}
void MCAL_USART_DeInit(USARTx_REG_t* USARTx)
{
	//Reset USARTx CLK & disable USARTx NVIC IRQ
	if (USARTx == USART1)
	{
		RCC_USART1_CLK_RST();
		NVIC_USART1_IRQ37_DIS;
	}
	else if (USARTx == USART2)
	{
		RCC_USART2_CLK_RST();
		NVIC_USART2_IRQ38_DIS;
	}
	else if (USARTx == USART3)
	{
		RCC_USART3_CLK_RST();
		NVIC_USART3_IRQ39_DIS;
	}
}
void MCAL_USART_SendData(USARTx_REG_t* USARTx, uint16_t* pTX_Buffer, USART_Mechanism_ENUM_t USART_Mechanism)
{
	if (USART_Mechanism == USART_Polling)
	{
		while ( ( USARTx->USART_SR & (1 << 7) ) == 0){
			/*
			 * Wait until bit 7 the TXE (TX EMPTY) is set which means old Data is transferred from TDR register
			 * to the transmit shift register and the data transmission has started.
			 * */
		}
	}
	//Write the data to send in the USART_DR register (this clears the TXE bit).

	if (USARTx == USART1)
	{
		if (USART_globeConfig[0].USART_WordLength == USART_WordLength_9BITS)
		{
			USARTx->USART_DR = (*pTX_Buffer & (uint16_t)0x01FF);	//take 9 bits (9 data or 8 data + 1 parity)
		}
		else //(USART_globeConfig[0].USART_WordLength == USART_WordLength_8BITS)
		{
			USARTx->USART_DR = (*pTX_Buffer & (uint8_t)0xFF);		//take 8 bits (8 data or 7 data + 1 parity)
		}
	}
	else if (USARTx == USART2)
	{
		if (USART_globeConfig[1].USART_WordLength == USART_WordLength_9BITS)
		{
			USARTx->USART_DR = (*pTX_Buffer & (uint16_t)0x01FF);	//take 9 bits (9 data or 8 data + 1 parity)
		}
		else //(USART_globeConfig[1].USART_WordLength == USART_WordLength_8BITS)
		{
			USARTx->USART_DR = (*pTX_Buffer & (uint8_t)0xFF);		//take 8 bits (8 data or 7 data + 1 parity)
		}
	}
	else if (USARTx == USART3)
	{
		if (USART_globeConfig[2].USART_WordLength == USART_WordLength_9BITS)
		{
			USARTx->USART_DR = (*pTX_Buffer & (uint16_t)0x01FF);	//take 9 bits (9 data or 8 data + 1 parity)
		}
		else //(USART_globeConfig[2].USART_WordLength == USART_WordLength_8BITS)
		{
			USARTx->USART_DR = (*pTX_Buffer & (uint8_t)0xFF);		//take 8 bits (8 data or 7 data + 1 parity)
		}
	}
}
void MCAL_USART_ReceiveData(USARTx_REG_t* USARTx, uint16_t* pRX_Buffer, USART_Mechanism_ENUM_t USART_Mechanism)
{
	if (USART_Mechanism == USART_Polling)
	{
		while ( ( USARTx->USART_SR & (1 << 5) ) == 0){
			/*
			 * Wait until bit 5 the RXNE (RX NOT EMPTY) is set which means the content of the shift register
			 * is transferred to the RDR and the data has been received and can be read.
			 * */
		}
	}
	//clearing the RXNE bit is performed by a software read to the USART_DR register.

	if (USARTx == USART1)
	{
		if (USART_globeConfig[0].USART_WordLength == USART_WordLength_9BITS)
		{
			if (USART_globeConfig[0].USART_Parity == USART_Parity_NONE)
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint16_t)0x01FF;		//(9 data + 0 parity) take 9 bits
			}
			else
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0xFF;		//(8 data + 1 parity) take 8 bits
			}
		}
		else //(USART_globeConfig[0].USART_WordLength == USART_WordLength_8BITS)
		{
			if (USART_globeConfig[0].USART_Parity == USART_Parity_NONE)
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0xFF;		//(8 data + 0 parity) take 8 bits
			}
			else
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0x7F;		//(7 data + 1 parity) take 7 bits
			}
		}
	}
	else if (USARTx == USART2)
	{
		if (USART_globeConfig[1].USART_WordLength == USART_WordLength_9BITS)
		{
			if (USART_globeConfig[1].USART_Parity == USART_Parity_NONE)
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint16_t)0x01FF;		//(9 data + 0 parity) take 9 bits
			}
			else
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0xFF;		//(8 data + 1 parity) take 8 bits
			}
		}
		else //(USART_globeConfig[1].USART_WordLength == USART_WordLength_8BITS)
		{
			if (USART_globeConfig[1].USART_Parity == USART_Parity_NONE)
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0xFF;		//(8 data + 0 parity) take 8 bits
			}
			else
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0x7F;		//(7 data + 1 parity) take 7 bits
			}
		}
	}
	else if (USARTx == USART3)
	{
		if (USART_globeConfig[2].USART_WordLength == USART_WordLength_9BITS)
		{
			if (USART_globeConfig[2].USART_Parity == USART_Parity_NONE)
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint16_t)0x01FF;		//(9 data + 0 parity) take 9 bits
			}
			else
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0xFF;		//(8 data + 1 parity) take 8 bits
			}
		}
		else //(USART_globeConfig[2].USART_WordLength == USART_WordLength_8BITS)
		{
			if (USART_globeConfig[2].USART_Parity == USART_Parity_NONE)
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0xFF;		//(8 data + 0 parity) take 8 bits
			}
			else
			{
				*((uint16_t*)pRX_Buffer) = USARTx->USART_DR & (uint8_t)0x7F;		//(7 data + 1 parity) take 7 bits
			}
		}
	}
}

//==========================================================================================
//=========================================USART ISR========================================
//==========================================================================================
void USART1_IRQHandler(void)
{
	USART_globeConfig[0].USART_ptr_irqCallbackFUN();
}
void USART2_IRQHandler(void)
{
	USART_globeConfig[1].USART_ptr_irqCallbackFUN();
}
void USART3_IRQHandler(void)
{
	USART_globeConfig[2].USART_ptr_irqCallbackFUN();
}
