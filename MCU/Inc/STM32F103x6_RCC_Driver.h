/*
 * STM32F103x6_RCC_Driver.h
 *
 *  Created on: Aug 26, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_RCC_DRIVER_H_
#define INC_STM32F103X6_RCC_DRIVER_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6.h"


//==========================================================================================
//======================================Generic Macros======================================
//==========================================================================================
#define HSI_RC		(uint32_t)8000000
#define HSE			(uint32_t)NULL			//Replace NULL with external HSE oscillator frequency in Hz



//==========================================================================================
//============================User type definitions (structures)============================
//==========================================================================================



//==========================================================================================
//==============================Macros Configuration References=============================
//==========================================================================================



//==========================================================================================
//===========================APIs Supported by "MCAL RCC DRIVER"============================
//==========================================================================================

/**==================================================
 * @brief  Gets the current system clock (SYSCLK) frequency.
 *
 * This function returns the frequency of the system clock (SYSCLK) based on the
 * clock source selected in the RCC_CFGR register.
 *
 * @retval uint32_t  The frequency of the system clock (SYSCLK).
 *
 * @note
 * The function checks the SWS[1:0] bits in the RCC_CFGR register to determine
 * the current clock source:
 * - 00: HSI oscillator used as system clock
 * - 01: HSE oscillator used as system clock
 * - 10: PLL used as system clock (not implemented in this function yet)
 * - 11: Not applicable
 *
 * @example
 * uint32_t sysclk;
 * sysclk = MCAL_RCC_getSYSCLK();
 * // sysclk will contain the current system clock frequency
 */
uint32_t MCAL_RCC_getSYSCLK(void);

/**==================================================
 * @brief  Gets the current AHB clock (HCLK) frequency.
 *
 * This function returns the frequency of the AHB clock (HCLK) based on the
 * system clock (SYSCLK) and the AHB prescaler value.
 *
 * @retval uint32_t  The frequency of the AHB clock (HCLK).
 *
 * @note
 * The function uses the HPRE[3:0] bits in the RCC_CFGR register to determine
 * the AHB prescaler value and calculates the HCLK frequency accordingly.
 *
 * @example
 * uint32_t hclk;
 * hclk = MCAL_RCC_getHCLK();
 * // hclk will contain the current AHB clock frequency
 */
uint32_t MCAL_RCC_getHCLK(void);

/**==================================================
 * @brief  Gets the current APB1 peripheral clock (PCLK1) frequency.
 *
 * This function returns the frequency of the APB1 peripheral clock (PCLK1) based on the
 * AHB clock (HCLK) and the APB1 prescaler value.
 *
 * @retval uint32_t  The frequency of the APB1 peripheral clock (PCLK1).
 *
 * @note
 * The function uses the PPRE1[2:0] bits in the RCC_CFGR register to determine
 * the APB1 prescaler value and calculates the PCLK1 frequency accordingly.
 *
 * @example
 * uint32_t pclk1;
 * pclk = MCAL_RCC_getPCLK1();
 * // pclk1 will contain the current APB1 peripheral clock frequency
 */
uint32_t MCAL_RCC_getPCLK1(void);

/**==================================================
 * @brief  Gets the current APB2 peripheral clock (PCLK2) frequency.
 *
 * This function returns the frequency of the APB2 peripheral clock (PCLK2) based on the
 * AHB clock (HCLK) and the APB2 prescaler value.
 *
 * @retval uint32_t  The frequency of the APB2 peripheral clock (PCLK2).
 *
 * @note
 * The function uses the PPRE2[2:0] bits in the RCC_CFGR register to determine
 * the APB2 prescaler value and calculates the PCLK2 frequency accordingly.
 *
 * @example
 * uint32_t pclk2;
 * pclk2 = MCAL_RCC_getPCLK2();
 * // pclk2 will contain the current APB2 peripheral clock frequency
 */
uint32_t MCAL_RCC_getPCLK2(void);


#endif /* INC_STM32F103X6_RCC_DRIVER_H_ */
