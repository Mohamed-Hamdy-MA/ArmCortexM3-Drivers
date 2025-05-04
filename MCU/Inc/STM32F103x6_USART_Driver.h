/*
 * STM32F103x6_USART_Driver.h
 *
 *  Created on: Aug 26, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_USART_DRIVER_H_
#define INC_STM32F103X6_USART_DRIVER_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6.h"
#include "STM32F103x6_GPIO_Driver.h"
#include "STM32F103x6_RCC_Driver.h"


//==========================================================================================
//======================================Generic Macros======================================
//==========================================================================================


//==========================================================================================
//==================================User type definitions===================================
//==========================================================================================
typedef enum
{
	USART_Polling,
	USART_Interrupt,
}USART_Mechanism_ENUM_t;
typedef struct
{
	uint8_t	USART_TxRxMode;				// This parameter must be a value of @reference USART_TxRxMode_Define
	uint16_t USART_WordLength;			// This parameter must be a value of @reference USART_WordLength_Define
	uint16_t USART_Parity;				// This parameter must be a value of @reference USART_Parity_Define
	uint16_t USART_StopBits;				// This parameter must be a value of @reference USART_StopBits_Define
	uint32_t USART_BaudRate;		 	// This parameter must be a value of @reference USART_BaudRate_Define
	uint16_t USART_FlowControl; 			// This parameter must be a value of @reference USART_FlowControl_Define
	uint16_t USART_Interrupt_EN;			// This parameter must be a value of @reference USART_Interrupt_EN_Define
	void(*USART_ptr_irqCallbackFUN)();
	/*
	 * This parameter must be a pointer to interrupt callback function in case of interrupt mechanism,
	 * or NULL in case of polling mechanism.
	 *
	 * If the user need to work with the interrupt mechanism for USARTx. When an interrupt occurs,
	 * the USARTx_IRQHandler function will be called. This function calls the user callback function
	 * which supposed to handle the interrupt.
	 *
	 * */
} USART_Config_t;

//==========================================================================================
//==============================Macros Configuration References=============================
//==========================================================================================

/*
	@reference USART_TxRxMode_Define
 */
#define USART_TxRxMode_RXonly		( (uint8_t) (1 << 2) )					//CR1Bit2 --> 1: Receiver is enabled and begins searching for a start bit
#define USART_TxRxMode_TXonly		( (uint8_t) (1 << 3) )					//CR1Bit3 --> 1: Transmitter is enabled
#define USART_TxRxMode_TXandRX		( (uint8_t) ( (1 << 2) | (1 << 3) ) )	//CR1Bit2 --> 1: Receiver is enabled & CR1Bit3 --> 1: Transmitter is enabled

/*
	@reference USART_WordLength_Define
	NOTE1: When the parity control is enabled, the computed parity is inserted at
		   the MSB position of your data word (9th bit if 9BITS; 8th bit if 8BITS),
		   this means that the parity bit is a part of data word.

		   So if the user need to send 9 bits and word length is configured to be 9 bits,
		   then the parity MUST be disabled to avoid losing of MSB bit.

		   Same consideration if the user need to send 8 bit and word length is configured to be 8 bits,
		   then the parity MUST be disabled to avoid losing of MSB bit OR configure word length to be 9 bits and keeps the parity.

 */
#define USART_WordLength_8BITS		( (uint16_t) (0 << 12) )		//CR1Bit12 --> 0: 1 Start bit, 8 Data bits, n Stop bit
#define USART_WordLength_9BITS		( (uint16_t) (1 << 12) )		//CR1Bit12 --> 1: 1 Start bit, 9 Data bits, n Stop bit

/*
	@reference USART_Parity_Define
 */
#define USART_Parity_NONE			( (uint16_t) (0 << 10) )			//CR1Bit10 --> 0: Parity control disabled
#define USART_Parity_EVEN			( (uint16_t) (1 << 10) | (0 << 9) )	//CR1Bit10 --> 1: Parity control enabled & CR1Bit9 --> 0: Even parity
#define USART_Parity_ODD			( (uint16_t) (1 << 10) | (1 << 9) )	//CR1Bit10 --> 1: Parity control enabled & CR1Bit9 --> 1: Odd parity

/*
	@reference USART_StopBits_Define
 */
#define USART_StopBits_1BIT		( (uint16_t) (0b00 << 12) )		//CR2Bit12,13 --> 00: 1 Stop bit
#define USART_StopBits_0_5BIT	( (uint16_t) (0b01 << 12) )		//CR2Bit12,13 --> 01: 0.5 Stop bit
#define USART_StopBits_2BITS	( (uint16_t) (0b00 << 12) )		//CR2Bit12,13 --> 10: 2 Stop bits
#define USART_StopBits_1_5BIT	( (uint16_t) (0b01 << 12) )		//CR2Bit12,13 --> 01: 1.5 Stop bit

/*
	@reference USART_BaudRate_Define
 */
#define USART_BaudRate_2400bps		( (uint32_t)2400 )
#define USART_BaudRate_2400bps     	( (uint32_t)2400 )
#define USART_BaudRate_9600bps     	( (uint32_t)9600 )
#define USART_BaudRate_19200bps    	( (uint32_t)19200 )
#define USART_BaudRate_57600bps    	( (uint32_t)57600 )
#define USART_BaudRate_115200bps   	( (uint32_t)115200 )
#define USART_BaudRate_230400bps   	( (uint32_t)230400 )
#define USART_BaudRate_460800bps   	( (uint32_t)460800 )
#define USART_BaudRate_921600bps   	( (uint32_t)921600 )
#define USART_BaudRate_2250000bps  	( (uint32_t)2250000 )
#define USART_BaudRate_4500000bps  	( (uint32_t)4500000 )

/*
	@reference USART_FlowControl_Define
 */
#define USART_FlowControl_NONE		( (uint16_t) (0 << 8) | (0 << 9) )	//CR3Bit8 --> 0: RTS flow control disabled & CR3Bit9 --> 0: CTS flow control disabled
#define USART_FlowControl_RTSonly	( (uint16_t) (1 << 8) )				//CR3Bit8 --> 1: RTS flow control enabled
#define USART_FlowControl_CTSonly	( (uint16_t) (1 << 9) )				//CR3Bit9 --> 1: CTS flow control enabled
#define USART_FlowControl_CTSandRTS	( (uint16_t) (1 << 8) | (1 << 9) )	//CR3Bit8 --> 1: RTS flow control enabled & CR3Bit9 --> 1: CTS flow control enabled

/*
	@reference USART_Interrupt_EN_Define
	NOTE1: Multiple types of interrupts can be achieved by OR operation.
		   The interrupts share the same IRQ source however identifying which one is requesting is done through flags.
		   EX: USART_Config->USART_Interrupt_EN |= (USART_Interrupt_EN_TXE | USART_Interrupt_EN_RXNE)
 */
#define USART_Interrupt_EN_NONE	( (uint16_t) (0b0000 << 5) )	//CR1Bit5,6,7,8 --> 0: Interrupt is inhibited
#define USART_Interrupt_EN_TXE	( (uint16_t) (1 << 7) )			//CR1Bit7 TX EMPTY interrupt enable --> 1: A USART interrupt is generated whenever TXE=1 in the USART_SR register
#define USART_Interrupt_EN_TC	( (uint16_t) (1 << 6) )			//CR1Bit6 TRANSMISSION COMPLETE interrupt enable --> 1: A USART interrupt is generated whenever TC=1 in the USART_SR register
#define USART_Interrupt_EN_RXNE	( (uint16_t) (1 << 5) )			//CR1Bit5 RX NOT EMPTY interrupt enable --> 1: A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
#define USART_Interrupt_EN_PE	( (uint16_t) (1 << 8) )			//CR1Bit8 PARITY ERROR interrupt enable --> 1: A USART interrupt is generated whenever PE=1 in the USART_SR register

//==========================================================================================
//===========================APIs Supported by "MCAL USART DRIVER"==========================
//==========================================================================================

/**==================================================
 * @brief  Initializes the specified USART peripheral with the given configuration.
 *
 * This function initializes the specified USART peripheral (USART1, USART2, or USART3)
 * with the provided configuration settings. It enables the USART clock, configures the
 * GPIO pins, sets the control registers, configures the baud rate, and enables/disables
 * interrupts and NVIC as needed.
 *
 * @param  USARTx       Pointer to the USART peripheral (USART1, USART2, or USART3).
 * @param  USART_Config Pointer to the USART configuration structure containing the
 *                      initialization settings.
 *
 * @retval None
 *
 * @note	- Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *          - Ensure that the GPIO driver is included (STM32F103x6_GPIO_Driver.h) and compiled
 *            (STM32F103x6_GPIO_Driver.c, STM32F103x6_GPIO_Driver.o or STM32F103x6_GPIO_Driver.a).
 *          - The function performs the following steps:
 *          	- Enables the USART clock and sets the USART enable bit.
 *          	- Configures the GPIO pins for the specified USART.
 *		             - Configures the TX pin as alternate function push-pull if TX is enabled.
 *		             - Configures the RX pin as input floating if RX is enabled.
 *		             - Configures the CTS pin as input floating if CTS is enabled.
 *		             - Configures the RTS pin as alternate function push-pull if RTS is enabled.
 *            	- Configures the USART control registers (CR1, CR2, CR3) based on the provided settings.
 *            	- Configures the baud rate register (USART_BRR) based on the provided baud rate.
 *           	- Enables or disables USART interrupts and NVIC based on the provided settings.
 *
 * @example
 * USART_Config_t usartConfig;
 * usartConfig.USART_TxRxMode = USART_TxRxMode_TXandRX;
 * usartConfig.USART_WordLength = USART_WordLength_8BITS;
 * usartConfig.USART_Parity = USART_Parity_NONE;
 * usartConfig.USART_StopBits = USART_StopBits_1BIT;
 * usartConfig.USART_BaudRate = USART_BaudRate_9600;
 * usartConfig.USART_FlowControl = USART_FlowControl_NONE;
 * usartConfig.USART_Interrupt_EN = USART_Interrupt_EN_RXNE;
 * usartConfig.USART_ptr_irqCallbackFUN = &userIRQCallBackFUN;
 *
 * MCAL_USART_Init(USART1, &usartConfig);
 * // Initializes USART1 with the specified configuration
 */
void MCAL_USART_Init(USARTx_REG_t* USARTx, USART_Config_t* USART_Config);

/**==================================================
 * @brief  Deinitializes the specified USART peripheral.
 *
 * This function deinitializes the specified USART peripheral (USART1, USART2, or USART3)
 * by resetting the USART clock and disabling the NVIC IRQ for the USART.
 *
 * @param  USARTx  Pointer to the USART peripheral (USART1, USART2, or USART3).
 *
 * @retval None
 *
 * @note    - Ensure that the USART peripheral clocks are enabled before calling this function.
 *          - The function performs the following steps:
 *            - Resets the USART clock.
 *            - Disables the NVIC IRQ for the specified USART.
 *
 * @example
 * MCAL_USART_DeInit(USART1);
 * // Deinitializes USART1 by resetting its clock and disabling its NVIC IRQ
 */
void MCAL_USART_DeInit(USARTx_REG_t* USARTx);

/**==================================================
 * @brief  Sends data through the specified USART peripheral.
 *
 * This function sends data through the specified USART peripheral (USART1, USART2, or USART3)
 * using either polling or interrupt mechanism based on the provided configuration.
 *
 * @param  USARTx          Pointer to the USART peripheral (USART1, USART2, or USART3).
 * @param  pTX_Buffer      Pointer to the data buffer to be transmitted.
 * @param  USART_Mechanism Specifies the mechanism to be used for transmission (USART_Polling or USART_Interrupt).
 *
 * @retval None
 *
 * @note    - Ensure that the USART peripheral is initialized before calling this function.
 *          - The function performs the following steps:
 *            - If polling mechanism is selected, waits until the TXE (Transmit Data Register Empty) flag is set.
 *            - Writes the data to the USART_DR register for transmission.
 *            - Handles different word lengths (8 bits or 9 bits) based on the configuration.
 *
 * @example
 * uint16_t data = 0x123;
 * MCAL_USART_SendData(USART1, &data, USART_Polling);
 * // Sends the data 0x123 through USART1 using polling mechanism
 */
void MCAL_USART_SendData(USARTx_REG_t* USARTx, uint16_t* pTX_Buffer, USART_Mechanism_ENUM_t USART_Mechanism);

/**==================================================
 * @brief  Receives data from the specified USART peripheral.
 *
 * This function receives data from the specified USART peripheral (USART1, USART2, or USART3)
 * using either polling or interrupt mechanism based on the provided configuration.
 *
 * @param  USARTx          Pointer to the USART peripheral (USART1, USART2, or USART3).
 * @param  pRX_Buffer      Pointer to the buffer where the received data will be stored.
 * @param  USART_Mechanism Specifies the mechanism to be used for reception (USART_Polling or USART_Interrupt).
 *
 * @retval None
 *
 * @note    - Ensure that the USART peripheral is initialized before calling this function.
 *          - The function performs the following steps:
 *            - If polling mechanism is selected, waits until the RXNE (Receive Data Register Not Empty) flag is set.
 *            - Reads the data from the USART_DR register and stores it in the provided buffer.
 *            - Handles different word lengths (8 bits or 9 bits) and parity settings based on the configuration.
 *
 * @example
 * uint16_t data;
 * MCAL_USART_ReceiveData(USART1, &data, USART_Polling);
 * // Receives data from USART1 using polling mechanism and stores it in the variable 'data'
 */
void MCAL_USART_ReceiveData(USARTx_REG_t* USARTx, uint16_t* pRX_Buffer, USART_Mechanism_ENUM_t USART_Mechanism);

#endif /* INC_STM32F103X6_USART_DRIVER_H_ */
