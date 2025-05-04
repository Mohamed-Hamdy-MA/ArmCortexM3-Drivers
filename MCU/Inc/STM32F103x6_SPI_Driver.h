/*
 * STM32F103x6_SPI_Driver.h
 *
 *  Created on: Sep 6, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_SPI_DRIVER_H_
#define INC_STM32F103X6_SPI_DRIVER_H_

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
	SingleMaster_SingleSlave,
	SingleMaster_MultipleSlaves,
	MultipleMasters_SingleSlave,
	MultipleMasters_MultipleeSlaves,
}SPI_System_ENUM_t;

typedef enum
{
	SPI_PollingENABLE,
	SPI_PollingDISABLE,
	SPI_Interrupt,
}SPI_Mechanism_ENUM_t;

typedef struct
{
	uint8_t  RXNE:1;		//RXNE: Receive buffer not empty
	uint8_t  TXE:1;			//TXE: Transmit buffer empty
	uint8_t  :2;
	uint8_t  CRCERR:1;		//CRCERR: CRC error flag
	uint8_t  MODF:1;		//MODF: Mode fault
	uint8_t  OVR:1;			//OVR: Overrun flag
	uint8_t  :1;
} SPI_InterruptSource_t;

typedef struct
{
	uint8_t	SPI_DeviceMode;				// This parameter must be a value of @reference SPI_DeviceMode_Define
	uint16_t SPI_CommunicationMode;		// This parameter must be a value of @reference SPI_CommunicationMode_Define
	uint16_t SPI_DataLength;			// This parameter must be a value of @reference SPI_DataLength_Define
	uint8_t SPI_FrameFormat;			// This parameter must be a value of @reference SPI_FrameFormat_Define
	uint8_t SPI_ClockPolarity;			// This parameter must be a value of @reference SPI_ClockPolarity_Define
	uint8_t SPI_ClockPhase;		 		// This parameter must be a value of @reference SPI_ClockPhase_Define
	uint8_t SPI_ClockPreScaler; 		// This parameter must be a value of @reference SPI_ClockPreScaler_Define
	uint16_t SPI_NSS; 					// This parameter must be a value of @reference SPI_NSS_Define
	uint8_t SPI_Interrupt_EN;			// This parameter must be a value of @reference SPI_Interrupt_EN_Define
	void(*SPI_ptr_irqCallbackFUN)(SPI_InterruptSource_t);
	/*
	 * This parameter must be a pointer to interrupt callback function in case of interrupt mechanism,
	 * or NULL in case of polling mechanism.
	 *
	 * If the user need to work with the interrupt mechanism for SPIx. When an interrupt occurs,
	 * the SPIx_IRQHandler function will be called. This function calls the user callback function
	 * which supposed to handle the interrupt based on the received interrupt source flag from SPIx_IRQHandler.
	 *
	 * AS A USER FOR THIS DRIVER, YOUR CALLBACK FUNCTION IN CASE OF INTERRUPT MECHANISM USART ROCOMMENDED TO BE LIKE THIS:
	 *
	 * void userIRQCallBackFUN(SPI_InterruptSource_t flag)
		{
			if (flag.RXNE)
			{
				// Actions for RXNE flag
			}
			if (flag.TXE)
			{
				// Actions for TXE flag
			}
			if (flag.CRCERR)
			{
				// Actions for CRCERR flag
			}
			if (flag.MODF)
			{
				// Actions for MODF flag
			}
			if (flag.OVR)
			{
				// Actions for OVR flag
			}
		}
	 * */
} SPI_Config_t;

//==========================================================================================
//==============================Macros Configuration References=============================
//==========================================================================================

/*
	@reference SPI_DeviceMode_Define
 */
#define SPI_DeviceMode_SLAVE		( (uint8_t) (0 << 2) )					//CR1Bit2 --> 0: Slave configuration
#define SPI_DeviceMode_MASTER		( (uint8_t) (1 << 2) )					//CR1Bit2 --> 1: Master configuration

/*
	@reference SPI_CommunicationMode_Define

	1. SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX:
	   Full-duplex communication allows the device to send and receive data at the same time.
	   - **Master**:
	     - **MOSI (Master Out Slave In)**: Used for sending data to the Slave
	     - **MISO (Master In Slave Out)**: Used for receiving data from the Slave
	     - **Transmit Buffer**: Active (Master sends data via MOSI)
	     - **Receive Buffer**: Active (Master receives data via MISO)
	   - **Slave**:
	     - **MOSI (Master Out Slave In)**: Used for receiving data from the Master
	     - **MISO (Master In Slave Out)**: Used for sending data to the Master
	     - **Transmit Buffer**: Active (Slave sends data via MISO)
	     - **Receive Buffer**: Active (Slave receives data via MOSI)
	   Both buffers are active, and data is exchanged on separate lines (MOSI for sending, MISO for receiving) simultaneously in both Master and Slave.

	2. SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_TXonly:
	   Simplex transmit-only mode allows the device to only send data.
	   - **Master**:
	     - **MOSI**: Used for sending data to the Slave
	     - **MISO**: Not used (can be repurposed as a general-purpose I/O)
	     - **Transmit Buffer**: Active (Master sends data via MOSI)
	     - **Receive Buffer**: Active (Application must ignore reading the Rx buffer)
	   - **Slave**:
	     - **MOSI**: Not used (can be repurposed as a general-purpose I/O)
	     - **MISO**: Used for sending data to the master
	     - **Transmit Buffer**: Active (Slave sends data via MISO)
	     - **Receive Buffer**: Active (Application must ignore reading the Rx buffer)
	   In this mode, only the **Transmit Buffer** of the Master and the **Transmit Buffer** of the Slave are active, as the communication is one-way.

	3. SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_RXonly:
	   Simplex receive-only mode allows the device to only receive data.
	   - **Master**:
	     - **MISO**: Used for receiving data from the Slave (MOSI is not configured)
	     - **Transmit Buffer**: Not Active (Master does not send any data)
	     - **Receive Buffer**: Active (Master receives data via MISO)
	   - **Slave**:
	     - **MOSI**: Used for receiving data from the Master (MISO is not configured)
	     - **Transmit Buffer**: Not Active (Slave does not send any data)
	     - **Receive Buffer**: Active (Slave receives data via MOSI)
	   In this case, only the **Receive Buffer** of the Master and the **Receive Buffer** of the Slave are active, as communication is one-way but in the opposite direction compared to transmit-only mode.

	4. SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_TX:
	   Half-duplex bidirectional mode uses a single data line for sending, and it cannot receive at the same time.
	   - **Master**:
	     - **MOSI**: Used for sending data (MISO is not configured)
	     - **Transmit Buffer**: Active (Master sends data via MOSI)
	     - **Receive Buffer**: Not Active (Master does not receive any data)
	   - **Slave**:
	     - **MISO**: Used for sending data (MOSI is not configured)
	     - **Transmit Buffer**: Active (Slave sends data via MISO)
	     - **Receive Buffer**: Not Active (Slave does not receive any data)
	   In this mode, only the **Transmit Buffer** is active, and the single data line (MOSI for Master, MISO for Slave) is used for sending data only.

	5. SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_RX:
	   Half-duplex bidirectional mode uses a single data line for receiving, and it cannot send at the same time.
	   - **Master**:
	     - **MOSI**: Used for receiving data (MISO is not used)
	     - **Transmit Buffer**: Not Active (Master does not send any data)
	     - **Receive Buffer**: Active (Master receives data via MOSI)
	   - **Slave**:
	     - **MISO**: Used for receiving data (MOSI is not used)
	     - **Transmit Buffer**: Not Active (Slave does not send any data)
	     - **Receive Buffer**: Active (Slave receives data via MISO)
	   Here, only the **Receive Buffer** is active, and the single data line (MOSI for Master, MISO for Slave) is used for receiving data only.

	NOTE1: Modes 4 and 5 can be interchanged between each other by software at runtime to get the full advantage of bidirectional line.
*/
#define SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX	( (uint16_t) ( (0 << 10) | (0 << 15) ) )	//CR1Bit10 --> 0: Full duplex (Transmit and receive) & CR1Bit15 --> 0: 2-line unidirectional data mode selected
#define SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_TXonly		( (uint16_t) ( (0 << 10) | (0 << 15) ) )	//CR1Bit10 --> 0: Full duplex (Transmit and receive) & CR1Bit15 --> 0: 2-line unidirectional data mode selected
#define SPI_CommunicationMode_SIMPLEX_UNIDIRECTION_RXonly		( (uint16_t) ( (1 << 10) | (0 << 15) ) )	//CR1Bit10 --> 1: Output disabled (Receive-only mode) & CR1Bit15 --> 0: 2-line unidirectional data mode selected
#define SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_TX		( (uint16_t) ( (1 << 14) | (1 << 15) ) )	//CR1Bit14 --> 1: Output enabled (transmit-only mode) & CR1Bit15 --> 0: 1: 1-line bidirectional data mode selected
#define SPI_CommunicationMode_HALF_DUPLEX_BIDIRECTION_RX		( (uint16_t) ( (0 << 14) | (1 << 15) ) )	//CR1Bit14 --> 0: Output disabled (receive-only mode) & CR1Bit15 --> 0: 1: 1-line bidirectional data mode selected

/*
	@reference SPI_DataLength_Define
 */
#define SPI_DataLength_8BITS		( (uint16_t) (0 << 11) )	//CR1Bit11 --> 0: 8-bit data frame format is selected for transmission/reception
#define SPI_DataLength_16BITS		( (uint16_t) (1 << 11) )	//CR1Bit11 --> 16-bit data frame format is selected for transmission/reception

/*
	@reference SPI_FrameFormat_Define
 */
#define SPI_FrameFormat_MSB_FIRST		( (uint8_t) (0 << 7) )	//CR1Bit7 --> 0: MSB transmitted first
#define SPI_FrameFormat_LSB_FIRST		( (uint8_t) (1 << 7) )	//CR1Bit7 --> 1: LSB transmitted first

/*
	@reference SPI_ClockPolarity_Define
 */
#define SPI_ClockPolarity_LOW_IDLE		( (uint8_t) (0 << 1) )	//CR1Bit1 --> 0: CK to 0 when idle
#define SPI_ClockPolarity_HIGH_IDLE		( (uint8_t) (1 << 1) )	//CR1Bit1 --> 1: CK to 1 when idle

/*
	@reference SPI_ClockPhase_Define
 */
#define SPI_ClockPhase_1ST_EDGE_CAPTURE		( (uint8_t) (0 << 0) )	//CR1Bit0 --> 0: The first clock transition is the first data capture edge
#define SPI_ClockPhase_2ND_EDGE_CAPTURE		( (uint8_t) (1 << 0) )	//CR1Bit0 --> 1: The second clock transition is the first data capture edge

/*
	@reference SPI_ClockPreScaler_Define
 */
#define SPI_ClockPreScaler_2    ( (uint8_t) (0b000 << 3) ) 	 	// CR1Bit3,4,5 --> 000: fPCLK/2
#define SPI_ClockPreScaler_4    ( (uint8_t) (0b001 << 3) ) 	 	// CR1Bit3,4,5 --> 001: fPCLK/4
#define SPI_ClockPreScaler_8    ( (uint8_t) (0b010 << 3) ) 	 	// CR1Bit3,4,5 --> 010: fPCLK/8
#define SPI_ClockPreScaler_16   ( (uint8_t) (0b011 << 3) ) 	 	// CR1Bit3,4,5 --> 011: fPCLK/16
#define SPI_ClockPreScaler_32   ( (uint8_t) (0b100 << 3) ) 	 	// CR1Bit3,4,5 --> 100: fPCLK/32
#define SPI_ClockPreScaler_64   ( (uint8_t) (0b101 << 3) )  	// CR1Bit3,4,5 --> 101: fPCLK/64
#define SPI_ClockPreScaler_128  ( (uint8_t) (0b110 << 3) )  	// CR1Bit3,4,5 --> 110: fPCLK/128
#define SPI_ClockPreScaler_256  ( (uint8_t) (0b111 << 3) )  	// CR1Bit3,4,5 --> 111: fPCLK/256

/*
	@reference SPI_NSS_Define

	// NSS Pin Management Options:

	1. **Single Master, Single Slave**:
  [RECOMMENDED]- **Option 1**: SPI_NSS_SW_HIGH (Master), SPI_NSS_SW_LOW (Slave).
  [RECOMMENDED]- **Option 2**: SPI_NSS_SW_HIGH (Master), SPI_NSS_HW_SLAVE_INPUT driven by general-purpose output pin from the master (Slave).
			   - **Option 3**: SPI_NSS_HW_MASTER_OUTPUT (Master), SPI_NSS_HW_SLAVE_INPUT driven by NSS of the master (Slave).
			   - **Option 4**: SPI_NSS_HW_MASTER_OUTPUT (Master), SPI_NSS_HW_SLAVE_INPUT driven general-purpose output pin from the master (Slave).

	2. **Single Master, Multiple Slaves**:
			   - **Option 1**: SPI_NSS_SW_HIGH (Master), SPI_NSS_SW_LOW (Slave).
  [RECOMMENDED]- **Option 2**: SPI_NSS_SW_HIGH (Master), SPI_NSS_HW_SLAVE_INPUT driven by general-purpose output pin from the master (Slave).
			   - **Option 3**: SPI_NSS_HW_MASTER_OUTPUT (Master), SPI_NSS_HW_SLAVE_INPUT driven by NSS of the master (Slave).
			   - **Option 4**: SPI_NSS_HW_MASTER_OUTPUT (Master), SPI_NSS_HW_SLAVE_INPUT driven general-purpose output pin from the master (Slave).

	3. **Multiple Masters, Single Slave**:
			   - **Option 1**: SPI_NSS_HW_MASTER_INPUT (Master), SPI_NSS_SW_LOW (Slave).
  [RECOMMENDED]- **Option 2**: SPI_NSS_HW_MASTER_INPUT (Master), SPI_NSS_HW_SLAVE_INPUT driven by general-purpose output pin from the master (Slave).

  	3. **Multiple Masters, Multiple Slaves**:
			   - **Option 1**: SPI_NSS_HW_MASTER_INPUT (Master), SPI_NSS_SW_LOW (Slave).
  [RECOMMENDED]- **Option 2**: SPI_NSS_HW_MASTER_INPUT (Master), SPI_NSS_HW_SLAVE_INPUT driven by general-purpose output pin from the master (Slave).
*/
#define SPI_NSS_SW_HIGH				( (uint16_t) ( (1 << 8) | (1 << 9) ) )	//CR1Bit8 --> 1: The value of this bit is forced onto the NSS pin & CR1Bit9 --> 1: Software slave management enabled
#define SPI_NSS_SW_LOW				( (uint16_t) ( (0 << 8) | (1 << 9) ) )	//CR1Bit8 --> 0: The value of this bit is forced onto the NSS pin & CR1Bit9 --> 1: Software slave management enabled
#define SPI_NSS_HW_SLAVE_INPUT		( (uint16_t) (0 << 2) )		//CR2Bit2 --> 0: SS output is disabled in slave mode & CR1Bit9 --> 0: Software slave management disabled
#define SPI_NSS_HW_MASTER_INPUT		( (uint16_t) (0 << 2) )		//CR2Bit2 --> 0: SS output is disabled in master mode, cell can work in multimaster configuration & CR1Bit9 --> 0: Software slave management disabled
#define SPI_NSS_HW_MASTER_OUTPUT	( (uint16_t) (1 << 2) )		//CR2Bit2 --> 1: SS output is enabled in master mode, cell cannot work in multimaster configuration & CR1Bit9 --> 0: Software slave management disabled

/*
	@reference SPI_Interrupt_EN_Define
	NOTE1: Multiple types of interrupts can be achieved by OR operation.
		   The interrupts share the same IRQ source however identifying which one is requesting is done through flags.
		   EX: SPI_Config->SPI_Interrupt_EN |= (SPI_Interrupt_EN_TXE | SPI_Interrupt_EN_RXNE)
 */
#define SPI_Interrupt_EN_NONE	( (uint8_t) (0b000 << 5) )	//CR2Bit5,6,7 --> 0: Interrupt is inhibited
#define SPI_Interrupt_EN_TXE	( (uint8_t) (1 << 7) )		//CR2Bit7 TX buffer empty interrupt enable --> 1: A SPI interrupt is generated whenever TXE=1 in the SPI_SR register
#define SPI_Interrupt_EN_RXNE	( (uint8_t) (1 << 6) )		//CR2Bit6 RX buffer not empty interrupt enable --> 1: A SPI interrupt is generated whenever RXNE=1 in the SPI_SR register
#define SPI_Interrupt_EN_ERR	( (uint8_t) (1 << 5) )		//CR2Bit5 : Error interrupt enable --> 1: A SPI interrupt is generated whenever (CRCERR, OVR, MODF in SPI mode and UDR, OVR in I2S mode) in the SPI_SR register


//==========================================================================================
//===========================APIs Supported by "MCAL SPI DRIVER"============================
//==========================================================================================

/**==================================================
 * @brief  Initializes the specified SPI peripheral with the given configuration.
 *
 * This function initializes the specified SPI peripheral (SPI1 or SPI2)
 * with the provided configuration settings. It enables the SPI clock, configures the
 * GPIO pins, sets the control registers, and enables/disables interrupts and NVIC as needed.
 *
 * @param  SPIx        Pointer to the SPI peripheral (SPI1 or SPI2).
 * @param  SPI_Config  Pointer to the SPI configuration structure containing the
 *                     initialization settings.
 * @param  SPI_System  Enumeration specifying the SPI system configuration.
 *
 * @retval None
 *
 * @note   - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *         - Ensure that the GPIO driver is included and compiled.
 *         - The function performs the following steps:
 *           - Enables the SPI clock and sets the SPI enable bit.
 *           - Configures the GPIO pins for the specified SPI based on recommended GPIO configurations
 *             for device peripherals on STM32F10xxx RM0008 reference manual table 25. SPI.
 *             		-All input pins are configured as floating inputs, allowing the user to apply their preferred external circuitry.
 *           - Configures the SPI control registers (CR1, CR2) based on the provided settings.
 *           - Configures the NSS (Slave Select) settings in CR1 and CR2 control registers.
 *           - Enables or disables SPI interrupts and NVIC based on the provided settings.
 *
 * @example
 * SPI_Config_t spiConfig;
 * spiConfig.SPI_DeviceMode = SPI_DeviceMode_MASTER;
 * spiConfig.SPI_CommunicationMode = SPI_CommunicationMode_FULL_DUPLEX_UNIDIRECTION_TXandRX;
 * spiConfig.SPI_DataLength = SPI_DataLength_8BITS;
 * spiConfig.SPI_FrameFormat = SPI_FrameFormat_MSB_FIRST;
 * spiConfig.SPI_ClockPolarity = SPI_ClockPolarity_LOW_IDLE;
 * spiConfig.SPI_ClockPhase = SPI_ClockPhase_2ND_EDGE_CAPTURE;
 * spiConfig.SPI_ClockPreScaler = SPI_ClockPreScaler_16;
 * spiConfig.SPI_NSS = SPI_NSS_SW_HIGH;
 * spiConfig.SPI_Interrupt_EN = SPI_Interrupt_EN_RXNE;
 * spiConfig.SPI_ptr_irqCallbackFUN = &userIRQCallBackFUN;
 *
 * MCAL_SPI_Init(SPI1, &spiConfig, SingleMaster_SingleSlave);
 * // Initializes SPI1 with the specified configuration
 */
void MCAL_SPI_Init(SPIx_REG_t* SPIx, SPI_Config_t* SPI_Config, SPI_System_ENUM_t SPI_System);

/**==================================================
 * @brief  Deinitializes the specified SPI peripheral.
 *
 * This function deinitializes the specified SPI peripheral (SPI1 or SPI2)
 * by resetting the SPI clock and disabling the NVIC IRQ for the SPI.
 *
 * @param  SPIx  Pointer to the SPI peripheral (SPI1 or SPI2).
 *
 * @retval None
 *
 * @note   - Ensure that no SPI communication is ongoing before calling this function.
 *         - The function performs the following steps:
 *           - Resets the clock for the specified SPI peripheral.
 *           - Disables the NVIC IRQ for the specified SPI peripheral.
 *
 * @example
 * MCAL_SPI_DeInit(SPI1);
 * // Deinitializes SPI1
 */
void MCAL_SPI_DeInit(SPIx_REG_t* SPIx);

/**==================================================
 * @brief  Sends data through the specified SPI peripheral.
 *
 * This function sends data through the specified SPI peripheral (SPI1 or SPI2)
 * using the provided transmission mechanism (polling enable, polling disable or interrupt).
 *
 * @param  SPIx           Pointer to the SPI peripheral (SPI1 or SPI2).
 * @param  pTX_Buffer     Pointer to the data buffer to be transmitted.
 * @param  SPI_Mechanism  Enumeration specifying the transmission mechanism (polling enable, polling disable or interrupt).
 *
 * @retval None
 *
 * @note   - Ensure that the SPI peripheral is initialized before calling this function.
 *         - The function performs the following steps:
 *           - If polling is enabled, waits until the TX buffer is empty.
 *           - Writes the data to the SPI data register (SPI_DR).
 *
 * @example
 * uint16_t data = 0x1234;
 * MCAL_SPI_SendData(SPI1, &data, SPI_PollingENABLE);
 * // Sends data through SPI1 with polling mechanism
 */
void MCAL_SPI_SendData(SPIx_REG_t* SPIx, uint16_t* pTX_Buffer, SPI_Mechanism_ENUM_t SPI_Mechanism);

/**==================================================
 * @brief  Receives data from the specified SPI peripheral.
 *
 * This function receives data from the specified SPI peripheral (SPI1 or SPI2)
 * using the provided reception mechanism (polling enable, polling disable or interrupt).
 *
 * @param  SPIx           Pointer to the SPI peripheral (SPI1 or SPI2).
 * @param  pRX_Buffer     Pointer to the data buffer to store the received data.
 * @param  SPI_Mechanism  Enumeration specifying the reception mechanism (polling enable, polling disable or interrupt).
 *
 * @retval None
 *
 * @note   - Ensure that the SPI peripheral is initialized before calling this function.
 *         - The function performs the following steps:
 *           - If polling is enabled, waits until the RX buffer is not empty.
 *           - Reads the data from the SPI data register (SPI_DR).
 *
 * @example
 * uint16_t data;
 * MCAL_SPI_ReceiveData(SPI1, &data, SPI_Interrupt);
 * // Receives data from SPI1 using interrupt mechanism
 */
void MCAL_SPI_ReceiveData(SPIx_REG_t* SPIx, uint16_t* pRX_Buffer, SPI_Mechanism_ENUM_t SPI_Mechanism);

/**==================================================
 * @brief  Sends and receives data through the specified SPI peripheral.
 *
 * This function sends and receives data through the specified SPI peripheral (SPI1 or SPI2)
 * using the provided separate transmission and reception mechanisms (polling enable, polling disable or interrupt).
 *
 * @param  SPIx                Pointer to the SPI peripheral (SPI1 or SPI2).
 * @param  pTX_Buffer          Pointer to the data buffer to be transmitted.
 * @param  SPI_SendMechanism   Enumeration specifying the transmission mechanism (polling enable, polling disable or interrupt).
 * @param  pRX_Buffer          Pointer to the data buffer to store the received data.
 * @param  SPI_ReceiveMechanism Enumeration specifying the reception mechanism (polling enable, polling disable or interrupt).
 *
 * @retval None
 *
 * @note   - Ensure that the SPI peripheral is properly initialized (Full-duplex Communication) before calling this function.
 *         - The function performs the following steps:
 *           - Sends data through the SPI peripheral.
 *           - Receives data from the SPI peripheral.
 *
 * @example
 * uint16_t txData = 0x1234;
 * uint16_t rxData;
 * MCAL_SPI_SendReceiveData(SPI1, &txData, SPI_PollingENABLE, &rxData, SPI_Interrupt);
 * // Sends and receives data through SPI1 using polling mechanism on transmission and interrupt mechanism on reception
 */
void MCAL_SPI_SendReceiveData(SPIx_REG_t* SPIx, uint16_t* pTX_Buffer, SPI_Mechanism_ENUM_t SPI_SendMechanism, uint16_t* pRX_Buffer, SPI_Mechanism_ENUM_t SPI_ReceiveMechanism);

#endif /* INC_STM32F103X6_SPI_DRIVER_H_ */
