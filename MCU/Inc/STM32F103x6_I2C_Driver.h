/*
 * STM32F103x6_I2C_Driver.h
 *
 *  Created on: Sep 14, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_I2C_DRIVER_H_
#define INC_STM32F103X6_I2C_DRIVER_H_

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
	DISABLE,
	ENABLE,
}EnableDisable_ENUM_t;

typedef enum
{
	START,
	RepeatedSTART,
	WithoutSTART,		//This option can only be chosen in case of transmit or receive from ongoing bus request not to initiate bus request.
}I2C_StartCondition_ENUM_t;

typedef enum
{
	Slave_Address7Matched,
	Slave_DataRequestFromMaster,
	Slave_DataReceivedFromMaster,
	Slave_StopReceived,
	Master_StartSent,
	Master_Address7Sent,
	Master_TxE,
	Master_RxNE,
}I2C_InterruptSource_t;

typedef struct
{
	uint16_t	I2C_AddressMode;		// This parameter must be a value of @reference I2C_AddressMode_Define
	uint16_t	I2C_PrimaryAddress;		// This parameter is defined by the user (support 7 bits and 10 bits addressing modes)
	uint8_t 	I2C_DualAddressing;		// This parameter must be a value of @reference I2C_DualAddressing_Define
	uint8_t 	I2C_SecondryAddress;	// This parameter is defined by the user (support only 7 bits addressing modes)
}I2C_AddressConfig_t;

typedef struct
{
	uint8_t				I2C_DeviceMode;				// This parameter must be a value of @reference I2C_DeviceMode_Define
	uint32_t 			I2C_ClockSpeed;				// This parameter must be a value of @reference I2C_ClockSpeed_Define
	uint8_t 			I2C_ClockStretching;		// This parameter must be a value of @reference I2C_ClockStretching_Define
	I2C_AddressConfig_t I2C_OwnAddress;
	uint16_t 			I2C_Interrupt_EN;			// This parameter must be a value of @reference I2C_Interrupt_EN_Define
	void(*I2C_ptr_irqCallbackFUN)(I2C_InterruptSource_t);
	/*
	 * This parameter must be a pointer to an interrupt callback function for the interrupt mechanism,
	 * or NULL for the polling mechanism.
	 *
	 * It is recommended to use the Master device with a polling mechanism and the Slave device with an interrupt mechanism,
	 * as the Master interrupt mechanism is not yet fully implemented or tested.
	 *
	 * If you need to work with the interrupt mechanism for I2Cx:
	 * - When an interrupt occurs, the I2Cx_EV_IRQHandler or I2Cx_ER_IRQHandler function will be called.
	 * - These functions will call the user callback function, which should handle the interrupt based on the received interrupt source flag.
	 *
	 * As a user of this driver, your callback function for the interrupt mechanism should be structured like this:
	 *
	 * void userIRQCallBackFUN(I2C_InterruptSource_t flag)
	 * {
	 *     switch (flag)
	 *     {
	 *         case Slave_Address7Matched:
	 *             // Action for Slave Address 7-bit Matched
	 *             // Example: Acknowledge the address (done automatically)
	 *             break;
	 *
	 *         case Slave_DataRequestFromMaster:
	 *             // Action for Slave Data Request From Master
	 *             // Example: Send data to master
	 *             // Use: void MCAL_I2C_Slave_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte)
	 *             break;
	 *
	 *         case Slave_DataReceivedFromMaster:
	 *             // Action for Slave Data Received From Master
	 *             // Example: Read data from master
	 *             // Use: void MCAL_I2C_Slave_ReceiveData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pRX_Byte)
	 *             break;
	 *
	 *         case Slave_StopReceived:
	 *             // Action for Slave Stop Received
	 *             // Example: Handle stop condition
	 *             break;
	 *
	 *         case Master_StartSent:
	 *             // Action for Master Start Sent
	 *             // Example: Prepare to send address
	 *             // Use: MCAL_I2C_Master_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte)
	 *             break;
	 *
	 *         case Master_Address7Sent:
	 *             // Action for Master Address 7-bit Sent
	 *             // Example: Load first byte to transmit
	 *             // Use: MCAL_I2C_Master_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte)
	 *             break;
	 *
	 *         case Master_TxE:
	 *             // Action for Master Transmit Buffer Empty & Closing the transmission communication (Mandatory)
	 *             // Example: Load next byte to transmit
	 *             // Use: MCAL_I2C_Master_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte)
	 *             // Note: Generating STOP condition is not supported yet
	 *             break;
	 *
	 *         case Master_RxNE:
	 *             // Action for Master Receive Buffer Not Empty & Closing the Reception communication (Mandatory)
	 *             // Example: Read received byte
	 *             // Use: void MCAL_I2C_Master_ReceiveData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pRX_Byte)
	 *             // Note: Generating NACK and STOP conditions are not supported yet
	 *             break;
	 *
	 *         default:
	 *             // Default action if no matching case
	 *             break;
	 *     }
	 * }
	 */
} I2C_Config_t;

//==========================================================================================
//==============================Macros Configuration References=============================
//==========================================================================================

/*
	@reference I2C_DeviceMode_Define
	NOTE1: It is recommended to use the master device with a polling mechanism and the Slave device with an interrupt mechanism,
	       as the Master interrupt mechanism is not yet fully implemented or tested.
 */
#define I2C_DeviceMode_SLAVE		( (uint8_t) 0 )				//No configuration In CR1 or CR2
#define I2C_DeviceMode_MASTER		( (uint8_t) 1 )				//No configuration In CR1 or CR2

/*
	@reference I2C_ClockSpeed_Define
	NOTE1: Fast mode IS NOT SUPPORTED YET in this driver.
 */
#define I2C_ClockSpeed_SM_50kHz			50000UL			//Standard mode I2C with 50 kHz Speed
#define I2C_ClockSpeed_SM_100kHz		100000UL		//Standard mode I2C with 100 kHz Speed
#define I2C_ClockSpeed_FM_200kHz		200000UL		//Fast mode I2C with 200 kHz Speed
#define I2C_ClockSpeed_FM_400kHz		400000UL		//Fast mode I2C with 400 kHz Speed

/*
	@reference I2C_DeviceMode_Define
 */
#define I2C_ClockStretching_EN		( (uint8_t) 0 )					//CR1Bit7 --> 0: Clock stretching enabled
#define I2C_ClockStretching_DIS		( (uint8_t) I2C_CR1_NOSTRETCH )	//CR1Bit7 --> 1: Clock stretching disabled

/*
	@reference I2C_AddressMode_Define
	NOTE1: 10-Bit address mode IS NOT SUPPORTED YET in this driver.
 */
#define I2C_AddressMode_7BITS		( (uint16_t) 0 )					//OAR1Bit15 --> 0: 7-bit slave address (10-bit address not acknowledged)
#define I2C_AddressMode_10BITS		( (uint16_t) I2C_OAR1_ADD_MODE )	//OAR1Bit15 --> 1: 10-bit slave address (7-bit address not acknowledged)

/*
	@reference I2C_DualAddressing_Define
	NOTE1: Dual addressing is only recognized in 7-bit addressing mode, so must configure
		   I2C_AddressMode to be 7 bits mode to enable Dual addressing.
 */
#define I2C_DualAddressing_DIS		( (uint8_t) 0 )					//OAR2Bit0 --> 0: Only OAR1 is recognized in 7-bit addressing mode
#define I2C_DualAddressing_EN		( (uint8_t) I2C_OAR2_ENDUAL )	//OAR2Bit0 --> 1: Both OAR1 and OAR2 are recognized in 7-bit addressing mode

/*
	@reference I2C_Interrupt_EN_Define
	NOTE1:
			 * Table 189. I2C Interrupt Requests
			 *
			 * | Interrupt Event                               | Event Flag | Enable Control Bit       |
			 * |-----------------------------------------------|------------|--------------------------|
			 * | Start bit sent (Master)                       | SB         | ITEVFEN                  |
			 * | Address sent (Master) or Address matched (Slave) | ADDR   	| ITEVFEN                  |
			 * | 10-bit header sent (Master)                   | ADD10      | ITEVFEN                  |
			 * | Stop received (Slave)                         | STOPF      | ITEVFEN                  |
			 * | Data byte transfer finished                   | BTF        | ITEVFEN                  |
			 * | Receive buffer not empty                      | RxNE       | ITEVFEN and ITBUFEN      |
			 * | Transmit buffer empty                         | TxE        | ITEVFEN and ITBUFEN      |
			 * |-----------------------------------------------|------------|--------------------------|
			 * | Bus error                                     | BERR       | ITERR                    |
			 * | Arbitration loss (Master)                     | ARLO       | ITERR                    |
			 * | Acknowledge failure                           | AF         | ITERR                    |
			 * | Overrun/Underrun                              | OVR        | ITERR                    |
			 * | PEC error                                     | PECERR     | ITERR                    |
			 * | Timeout/Tlow error                            | TIMEOUT    | ITERR                    |
			 * | SMBus Alert                                   | SMBALERT   | ITERR                    |
			 *
			 * These tables provide a quick reference for understanding the I2C interrupt events,
			 * their associated event flags, and the enable control bits required to handle them.
			 *
			 *  SB, ADDR, ADD10, STOPF, BTF, RxNE and TxE are logically OR-ed on the same interrupt
			 *  channel.
			 *  BERR, ARLO, AF, OVR, PECERR, TIMEOUT and SMBALERT are logically OR-ed on the same
			 *  interrupt channel.

	NOTE2: Multiple types of interrupts can be achieved by OR operation.
		   The interrupts share the same IRQ source however identifying which one is requesting is done through flags.
		   EX: I2C_Config->I2C_Interrupt_EN |= (ITEVTEN_and_ITBUFEN | I2C_Interrupt_EN_ITERREN)

	NOTE3: ITERREN IS NOT SUPPORTED YET in this driver.
 */
#define I2C_Interrupt_EN_NONE					( (uint16_t) 0 )										//CR2Bit8,9,10 --> 0: Interrupt is inhibited
#define I2C_Interrupt_EN_ITEVTEN				( (uint16_t) I2C_CR2_ITEVTEN )							//CR2Bit9 TX buffer empty interrupt enable --> 1: Event interrupt enabled
#define I2C_Interrupt_EN_ITEVTEN_and_ITBUFEN	( (uint16_t) (I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN) )		//CR2Bit9,10  Buffer interrupt enable -->  1: TxE = 1 or RxNE = 1 generates Event Interrupt (whatever the state of DMAEN)
#define I2C_Interrupt_EN_ITERREN				( (uint16_t) I2C_CR2_ITERREN )							//CR2Bit8 : Error interrupt enable -->  1: Error interrupt enabled


//==========================================================================================
//===========================APIs Supported by "MCAL I2C DRIVER"============================
//==========================================================================================

/**==================================================
 * @brief  Initializes the specified I2C peripheral with the given configuration.
 *
 * This function initializes the specified I2C peripheral (I2C1 or I2C2)
 * with the provided configuration settings. It enables the I2C clock, configures the
 * GPIO pins, sets the control registers, and enables/disables interrupts and NVIC as needed.
 *
 * @param  I2Cx        Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  I2C_Config  Pointer to the I2C configuration structure containing the
 *                     initialization settings.
 *
 * @retval None
 *
 * @note   - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *         - Ensure that the GPIO driver is included and compiled.
 *         - The function performs the following steps:
 *           - Initializes temporary control registers with reset values for safety.
 *           - Enables the I2C clock and copies the I2C_Config structure to a global configuration.
 *           - Configures the GPIO pins for I2C functionality.
 *           - Configures the I2C control register CR1.
 *           - Programs the peripheral input clock for I2C.
 *           - Configures the clock control registers for I2C.
 *           - Configures the rise time register for I2C.
 *           - Enables acknowledge and configures the OAR1 and OAR2 registers for slave devices.
 *           - Enables or disables I2C interrupts and NVIC based on the provided settings.
 *           - Writes the configured values to the actual I2C control registers.
 *           - Enables the I2C peripheral.
 *         - Fast mode is not supported yet.
 *         - It is highly recommended to use master device with polling and slave device with interrupt.
 *
 * @example
 * I2C_Config_t myI2CConfig;
 * myI2CConfig.I2C_DeviceMode = I2C_DeviceMode_MASTER;
 * myI2CConfig.I2C_ClockSpeed = I2C_ClockSpeed_SM_100kHz;
 * myI2CConfig.I2C_ClockStretching = I2C_ClockStretching_EN;
 * myI2CConfig.I2C_OwnAddress.I2C_AddressMode = I2C_AddressMode_7BITS;
 * myI2CConfig.I2C_OwnAddress.I2C_PrimaryAddress = 0x52;
 * myI2CConfig.I2C_OwnAddress.I2C_DualAddressing = I2C_DualAddressing_DIS;
 * myI2CConfig.I2C_Interrupt_EN = I2C_Interrupt_EN_NONE;
 * myI2CConfig.I2C_ptr_irqCallbackFUN = NULL;
 *
 * MCAL_I2C_Init(I2C1, &myI2CConfig);
 * // Initializes I2C1 with the specified configuration
 */
void MCAL_I2C_Init(I2Cx_REG_t* I2Cx, I2C_Config_t* I2C_Config);

/**==================================================
 * @brief  Deinitializes the specified I2C peripheral.
 *
 * This function deinitializes the specified I2C peripheral (I2C1 or I2C2)
 * by resetting the I2C clock and disabling the NVIC IRQs associated with the I2C peripheral.
 *
 * @param  I2Cx  Pointer to the I2C peripheral (I2C1 or I2C2).
 *
 * @retval None
 *
 * @note   - Ensure that no I2C operations are ongoing before calling this function.
 *         - The function performs the following steps:
 *           - Resets the I2C clock.
 *           - Disables the NVIC IRQs for the specified I2C peripheral.
 *
 * @example
 * MCAL_I2C_DeInit(I2C1);
 * // Deinitializes I2C1, resetting its clock and disabling its NVIC IRQs
 */
void MCAL_I2C_DeInit(I2Cx_REG_t* I2Cx);

/**==================================================
 * @brief  Sends data from the master to the slave using polling.
 *
 * This function sends data from the master to the slave device using the I2C protocol
 * with a polling mechanism. It handles the start condition, address transmission, data
 * transmission, and stop condition based on the provided parameters.
 *
 * @param  I2Cx                          Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  SlaveAddress                  The address of the slave device.
 * @param  SlaveAddressTransferCondition Enable or disable the transfer of the slave address (Disable or Enable).
 * @param  pTX_Buffer                    Pointer to the data buffer to be transmitted.
 * @param  TX_NumberOfBytes              Number of bytes to be transmitted.
 * @param  StartCondition                Specifies whether to generate a start condition (START, RepeatedSTART or WithoutSTART).
 * @param  StopCondition                 Specifies whether to generate a stop condition (Disable or Enable).
 *
 * @retval None
 *
 * @note   - Ensure that the number of bytes to be transmitted is greater than 0.
 *         - The function performs the following steps:
 *           - Sends the start condition if specified.
 *           - Waits for the EV5 flag (start condition generated).
 *           - Sends the slave address if specified.
 *           - Waits for the EV6 flag (address sent and acknowledged).
 *           - Waits for the EV8_1 flag (data register empty) and sends the first byte.
 *           - Waits for the EV8 flag (data register empty) and sends the remaining bytes.
 *           - Waits for the EV8_2 flag (byte transfer finished) and generates the stop condition if specified.
 *           - Generates a stop condition if the number of bytes is 0 or negative.
 *
 * @example
 * uint8_t data[] = {0x01, 0x02, 0x03};
 * MCAL_I2C_Master_SendData_Polling(I2C1, 0x52, ENABLE, data, 3, START, ENABLE);
 * // Sends 3 bytes of data to the slave device with address 0x52 using I2C1
 */
void MCAL_I2C_Master_SendData_Polling(I2Cx_REG_t* I2Cx, uint16_t SlaveAddress, EnableDisable_ENUM_t SlaveAddressTransferCondition, uint8_t* pTX_Buffer, uint32_t TX_NumberOfBytes, I2C_StartCondition_ENUM_t StartCondition, EnableDisable_ENUM_t StopCondition);

/**==================================================
 * @brief  Receives data from the slave to the master using polling.
 *
 * This function receives data from the slave device to the master using the I2C protocol
 * with a polling mechanism. It handles the start condition, address transmission, data
 * reception, and stop condition based on the provided parameters.
 *
 * @param  I2Cx                          Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  SlaveAddress                  The address of the slave device.
 * @param  SlaveAddressTransferCondition Enable or disable the transfer of the slave address (Disable or Enable).
 * @param  pRX_Buffer                    Pointer to the data buffer to store the received data.
 * @param  RX_NumberOfBytes              Number of bytes to be received.
 * @param  StartCondition                Specifies whether to generate a start condition (START, RepeatedSTART or WithoutSTART).
 * @param  StopCondition                 Specifies whether to generate a stop condition (Disable or Enable).
 *
 * @retval None
 *
 * @note   - Ensure that the number of bytes to be received is greater than 0.
 *         - The function performs the following steps:
 *           - Sends the start condition if specified.
 *           - Waits for the EV5 flag (start condition generated).
 *           - Sends the slave address if specified.
 *           - Waits for the EV6 flag (address sent and acknowledged).
 *           - Waits for the EV7 flag (data register not empty) and reads the received bytes.
 *           - For single byte reception, disables acknowledge and generates the stop condition after EV6.
 *           - For multiple bytes reception, reads all bytes except the last one, then disables acknowledge and generates the stop condition before reading the last byte.
 *           - Generates a stop condition if the number of bytes is 0 or negative.
 *
 * @example
 * uint8_t data[3];
 * MCAL_I2C_Master_ReceiveData_Polling(I2C1, 0x52, ENABLE, data, 3, RepeatedSTART, ENABLE);
 * // Receives 3 bytes of data from the slave device with address 0x52 using I2C1
 */
void MCAL_I2C_Master_ReceiveData_Polling(I2Cx_REG_t* I2Cx, uint16_t SlaveAddress, EnableDisable_ENUM_t SlaveAddressTransferCondition, uint8_t* pRX_Buffer, uint32_t RX_NumberOfBytes, I2C_StartCondition_ENUM_t StartCondition, EnableDisable_ENUM_t StopCondition);

/**==================================================
 * @brief  Sends data from the slave to the master using interrupt.
 *
 * This function sends a single byte of data from the slave to the master using the I2C
 * protocol with an interrupt mechanism.
 *
 * @param  I2Cx      Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  pTX_Byte  Pointer to the data byte to be transmitted.
 *
 * @retval None
 *
 * @note   - The function writes the data byte to the I2C data register (I2C_DR).
 *
 * @example
 * uint8_t data = 0x01;
 * MCAL_I2C_Slave_SendData_Interrupt(I2C1, &data);
 * // Sends a single byte of data from the slave to the master using I2C1
 */
void MCAL_I2C_Slave_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte);

/**==================================================
 * @brief  Receives data from the master to the slave using interrupt.
 *
 * This function receives a single byte of data from the master to the slave using the I2C
 * protocol with an interrupt mechanism.
 *
 * @param  I2Cx      Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  pRX_Byte  Pointer to the data byte to store the received data.
 *
 * @retval None
 *
 * @note   - The function reads the data byte from the I2C data register (I2C_DR).
 *
 * @example
 * uint8_t data;
 * MCAL_I2C_Slave_ReceiveData_Interrupt(I2C1, &data);
 * // Receives a single byte of data from the master to the slave using I2C1
 */
void MCAL_I2C_Slave_ReceiveData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pRX_Byte);



/**==================================================
 * @brief  Sends data from the master to the slave using interrupt.
 *
 * This function sends a single byte of data from the master to the slave using the I2C
 * protocol with an interrupt mechanism.
 *
 * @param  I2Cx      Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  pTX_Byte  Pointer to the data byte to be transmitted.
 *
 * @retval None
 *
 * @note   - The function writes the data byte to the I2C data register (I2C_DR).
 *
 * @example
 * uint8_t data = 0x01;
 * MCAL_I2C_Master_SendData_Interrupt(I2C1, &data);
 * // Sends a single byte of data from the master to the slave using I2C1
 */
void MCAL_I2C_Master_SendData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pTX_Byte);

/**==================================================
 * @brief  Receives data from the slave to the master using interrupt.
 *
 * This function receives a single byte of data from the slave to the master using the I2C
 * protocol with an interrupt mechanism.
 *
 * @param  I2Cx      Pointer to the I2C peripheral (I2C1 or I2C2).
 * @param  pRX_Byte  Pointer to the data byte to store the received data.
 *
 * @retval None
 *
 * @note   - The function reads the data byte from the I2C data register (I2C_DR).
 *
 * @example
 * uint8_t data;
 * MCAL_I2C_Master_ReceiveData_Interrupt(I2C1, &data);
 * // Receives a single byte of data from the slave to the master using I2C1
 */
void MCAL_I2C_Master_ReceiveData_Interrupt(I2Cx_REG_t* I2Cx, uint8_t* pRX_Byte);

#endif /* INC_STM32F103X6_I2C_DRIVER_H_ */
