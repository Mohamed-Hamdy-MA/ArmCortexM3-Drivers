/*
 * STM32F103x6_GPTIM_Driver.c
 *
 *  Created on: Oct 13, 2024
 *      Author: Mohamed Hamdy
 */


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6_GPTIM_Driver.h"

//==========================================================================================
//================================Assisting type definitions================================
//==========================================================================================

//==========================================================================================
//=====================================Assisting Macros=====================================
//==========================================================================================

//==========================================================================================
//================================Assisting global variables================================
//==========================================================================================


//==========================================================================================
//====================================Assisting function====================================
//==========================================================================================


//==========================================================================================
//============================APIs Supported by MCAL TIM DRIVER=============================
//==========================================================================================

void MCAL_TIM_Init(TIMx_REG_t* TIMx, uint16_t MaximumCount, uint32_t TickTime_us)
{
	uint8_t APB1_Freq_MHz = (MCAL_RCC_getPCLK1()/1000000);

	//Reset TIMx CLK to ReInitilize it, then enable TIMx CLK to write new configuration.
	if (TIMx == TIM2)
	{
		RCC_TIM2_CLK_RST();
		RCC_TIM2_CLK_EN();
	}
	else if (TIMx == TIM3)
	{
		RCC_TIM3_CLK_RST();
		RCC_TIM3_CLK_EN();
	}
	else if (TIMx == TIM4)
	{
		RCC_TIM4_CLK_RST();
		RCC_TIM4_CLK_EN();
	}

	/*
	 * Prescaler = TIMx_PSC + 1
	 * NoOfTicks = TIMx_ARR + 1
	 *
	 * Tick Time(us) = 1/(InputFreq(MHz)/Prescaler) = Prescaler/InputFreq(MHz) = (TIMx_PSC+1)/InputFreq(MHz)
	 * TIMx_PSC = (Tick Time(us)*InputFreq(MHz))-1			(--->Eq1)
	 *
	 * Delay Time(us) = Tick Time(us)*NoOfTicks
	 * Tick Time(us) = Delay Time(us)/NoOfTicks
	 * Tick Time(us) = Delay Time(us)/(TIMx_ARR+1)			(--->Eq2)
	 *
	 * -----> Prescaler = ( Delay Time(us)*InputFreq(MHz) / NoOfTicks )					(General Formula)
	 * -----> TIMx_PSC = ( Delay Time(us)*InputFreq(MHz) / (TIMx_ARR + 1) ) - 1			(STM32 Formula)
	 *
	 * If Tick Time(us) = 1 us, then TIMx_PSC = (InputFreq(MHz))-1
	 * This lead to maximum delay Time of 1 us * 2^16 = 1 us * 65536 = 65536 us = 65.536 ms
	 *
	 * Assume InputFreq(MHz) = 8 MHz, then TIMx_PSC = 8-1 = 7
	 *
	 */

	//TIMx_SMCR Bits 2:0 SMS: Slave mode selection
	//000: Slave mode disabled - if CEN = 1 then the prescaler is clocked directly by the internal clock.
	TIMx->TIMx_SMCR &= ~(0x0007);			// ==Reset value

	//TIMx_CR1 Bit 4 DIR: Direction
	//0: Counter used as upcounter.
	TIMx->TIMx_CR1 &= ~(1<<4);				// ==Reset value

	TIMx->TIMx_ARR = MaximumCount;
	TIMx->TIMx_PSC = ( TickTime_us * APB1_Freq_MHz ) - 1;

	TIMx->TIMx_CNT = 0x0000;

	//TIMx_CR1 Bit 0 CEN: Counter enable
	//1: Counter enabled
	TIMx->TIMx_CR1 |= (1<<0);
}
void MCAL_TIM_DeInit(TIMx_REG_t* TIMx)
{
	//Reset TIMx CLK & disable TIMx NVIC IRQs
	if (TIMx == TIM2)
	{
		RCC_TIM2_CLK_RST();
		NVIC_TIM2_IRQ28_DIS;
	}
	else if (TIMx == TIM3)
	{
		RCC_TIM3_CLK_RST();
		NVIC_TIM3_IRQ29_DIS;
	}
	else if (TIMx == TIM3)
	{
		RCC_TIM4_CLK_RST();
		NVIC_TIM4_IRQ30_DIS;
	}
}
void MCAL_TIM_Delay_us(TIMx_REG_t* TIMx, uint32_t DelayTime_us)
{
	TIMx->TIMx_CNT = 0x0000;
	while(TIMx->TIMx_CNT < DelayTime_us)
	{
		/*
		 * Wait until the content of the counter TIMx_CNT has matched DelayTime_us.
		 * */
	}
}
void MCAL_TIM_Delay_ms(TIMx_REG_t* TIMx, uint32_t DelayTime_ms)
{
	uint32_t i=0;
	while(i<DelayTime_ms)
	{
		MCAL_TIM_Delay_us(TIMx, 1000);
		i++;
	}
}
void MCAL_TIM_Delay_s(TIMx_REG_t* TIMx, uint32_t DelayTime_s)
{
	uint32_t i=0;
	while(i<DelayTime_s)
	{
		MCAL_TIM_Delay_ms(TIMx, 1000);
		i++;
	}
}

