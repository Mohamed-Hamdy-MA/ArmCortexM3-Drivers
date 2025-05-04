/*
 * STM32F103x6_RCC_Driver.c
 *
 *  Created on: Aug 26, 2024
 *      Author: Mohamed Hamdy
 */



//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_RCC_Driver.h"

//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================

const uint8_t ABP_Prescalar[8U] = {0, 0, 0, 0, 1, 2, 3, 4};
const uint8_t AHB_Prescalar[16U] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================


//==========================================================================================
//===========================APIs Supported by "MCAL RCC DRIVER"============================
//==========================================================================================

uint32_t MCAL_RCC_getSYSCLK(void)
{
	/*
	Bits 3:2 SWS[1:0]: System clock switch status
	Set and cleared by hardware to indicate which clock source is used as system clock.
	00: HSI oscillator used as system clock
	01: HSE oscillator used as system clock
	10: PLL used as system clock
	11: Not applicable
	 */
	switch ( (RCC->RCC_CFGR >> 2) & 0b11 )
	{
	case 0b00:
		return HSI_RC;
		break;
	case 0b01:
		return HSE;
		break;
		/*
	case 0b10:
		//TO-DO: Need to calculate PLLCLK from PLL source mux & PLL Mul.
		return PLLCLK;
		break;
		 */
	default:
		return HSI_RC;
	}
}
uint32_t MCAL_RCC_getHCLK(void)
{
	/*
	AHB_Prescalar[0b0000] = AHB_Prescalar[0]  --> (HCLK >> 0)  --> SYSCLK not divided
	AHB_Prescalar[0b0001] = AHB_Prescalar[1]  --> (HCLK >> 0)  --> SYSCLK not divided
 	AHB_Prescalar[0b0011] = AHB_Prescalar[3]  --> (HCLK >> 0)  --> SYSCLK not divided
	AHB_Prescalar[0b0100] = AHB_Prescalar[4]  --> (HCLK >> 0)  --> SYSCLK not divided
	AHB_Prescalar[0b0101] = AHB_Prescalar[5]  --> (HCLK >> 0)  --> SYSCLK not divided
	AHB_Prescalar[0b0110] = AHB_Prescalar[6]  --> (HCLK >> 0)  --> SYSCLK not divided
	AHB_Prescalar[0b0111] = AHB_Prescalar[7]  --> (HCLK >> 0)  --> SYSCLK not divided
	AHB_Prescalar[0b1000] = AHB_Prescalar[8]  --> (HCLK >> 1)  --> SYSCLK divided by 2
	AHB_Prescalar[0b1001] = AHB_Prescalar[9]  --> (HCLK >> 2)  --> SYSCLK divided by 4
	AHB_Prescalar[0b1010] = AHB_Prescalar[10] --> (HCLK >> 3)  --> SYSCLK divided by 8
	AHB_Prescalar[0b1011] = AHB_Prescalar[11] --> (HCLK >> 4)  --> SYSCLK divided by 16
	AHB_Prescalar[0b1100] = AHB_Prescalar[12] --> (HCLK >> 6)  --> SYSCLK divided by 64
	AHB_Prescalar[0b1101] = AHB_Prescalar[13] --> (HCLK >> 7)  --> SYSCLK divided by 128
	AHB_Prescalar[0b1110] = AHB_Prescalar[14] --> (HCLK >> 8)  --> SYSCLK divided by 256
	AHB_Prescalar[0b1111] = AHB_Prescalar[15] --> (HCLK >> 9)  --> SYSCLK divided by 512
	 */

	//Bits 7:4 HPRE[3:0] in (RCC_CFGR): AHB prescaler
	return ( MCAL_RCC_getSYSCLK() >> AHB_Prescalar[ ( (RCC->RCC_CFGR >> 4) & 0b1111 ) ] );
}
uint32_t MCAL_RCC_getPCLK1(void)
{
	/*
	ABP_Prescalar[0b000] = ABP_Prescalar[0] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b001] = ABP_Prescalar[1] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b010] = ABP_Prescalar[2] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b011] = ABP_Prescalar[3] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b100] = ABP_Prescalar[4] --> (HCLK >> 1) --> HCLK divided by 2
	ABP_Prescalar[0b101] = ABP_Prescalar[5] --> (HCLK >> 2) --> HCLK divided by 4
	ABP_Prescalar[0b110] = ABP_Prescalar[6] --> (HCLK >> 3) --> HCLK divided by 8
	ABP_Prescalar[0b111] = ABP_Prescalar[7] --> (HCLK >> 4) --> HCLK divided by 16
	 */

	//Bits 10:8 PPRE1[2:0] in (RCC_CFGR): APB Low-speed prescaler (APB1)
	return ( MCAL_RCC_getHCLK() >> ABP_Prescalar[ ( (RCC->RCC_CFGR >> 8) & 0b111 ) ] );
}
uint32_t MCAL_RCC_getPCLK2(void)
{
	/*
	ABP_Prescalar[0b000] = ABP_Prescalar[0] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b001] = ABP_Prescalar[1] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b010] = ABP_Prescalar[2] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b011] = ABP_Prescalar[3] --> (HCLK >> 0) --> HCLK not divided
	ABP_Prescalar[0b100] = ABP_Prescalar[4] --> (HCLK >> 1) --> HCLK divided by 2
	ABP_Prescalar[0b101] = ABP_Prescalar[5] --> (HCLK >> 2) --> HCLK divided by 4
	ABP_Prescalar[0b110] = ABP_Prescalar[6] --> (HCLK >> 3) --> HCLK divided by 8
	ABP_Prescalar[0b111] = ABP_Prescalar[7] --> (HCLK >> 4) --> HCLK divided by 16
	 */

	//Bits 13:11 PPRE2[2:0] in (RCC_CFGR): APB High-speed prescaler (APB2)
	return ( MCAL_RCC_getHCLK() >> ABP_Prescalar[ ( (RCC->RCC_CFGR >> 11) & 0b111 ) ] );
}


