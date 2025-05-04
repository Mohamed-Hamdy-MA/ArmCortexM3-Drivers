/*
 * STM32F103x6.h
 *
 *  Created on: Aug 18, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_H_
#define INC_STM32F103X6_H_


//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


//==========================================================================================
//================================Base addresses for Memories===============================
//==========================================================================================
#define FLASH_BASE				0x08000000UL
#define SYSTEM_MEMORY_BASE		0x1FFFF000UL
#define SRAM_BASE				0x20000000UL


//==========================================================================================
//============================Base addresses for BUS Peripherals============================
//==========================================================================================

//-------InternalCortexM------
#define M3_INTERNAL_PERIPHERALS	0xE0000000UL
#define NVIC_BASE				0xE000E100UL

//-------------AHB------------
//#define FSMC_BASE       0xA0000000
//#define USB_OTG_FS_BASE 0x50000000
//#define ETHERNET_BASE   0x40028000
//#define CRC_BASE        0x40023000
//#define FLASH_IF_BASE   0x40022000
#define RCC_BASE        0x40021000UL
//#define DMA2_BASE       0x40020400
//#define DMA1_BASE       0x40020000
//#define SDIO_BASE       0x40018000

//-------------APB2-------------
//#define TIM11_BASE     0x40015400
//#define TIM10_BASE     0x40015000
//#define TIM9_BASE      0x40014C00
//#define ADC3_BASE      0x40013C00
#define USART1_BASE    0x40013800UL
//#define TIM8_BASE      0x40013400
#define SPI1_BASE      0x40013000UL
//#define TIM1_BASE      0x40012C00
//#define ADC2_BASE      0x40012800
//#define ADC1_BASE      0x40012400
//#define GPIOG_BASE     0x40012000		//NOT Included in STM32F103x6
//#define GPIOF_BASE     0x40011C00		//NOT Included in STM32F103x6
//#define GPIOE_BASE     0x40011800		//NOT Included in STM32F103x6
#define GPIOD_BASE     0x40011400UL
#define GPIOC_BASE     0x40011000UL
#define GPIOB_BASE     0x40010C00UL
#define GPIOA_BASE     0x40010800UL
#define EXTI_BASE      0x40010400UL
#define AFIO_BASE      0x40010000UL

//-------------APB1-------------
//#define DAC_BASE       0x40007400
//#define PWR_BASE       0x40007000
//#define BKP_BASE       0x40006C00
//#define bxCAN1_BASE    0x40006400
//#define bxCAN2_BASE    0x40006800
//#define CAN_SRAM_BASE  0x40006000
//#define USB_FS_BASE    0x40005C00
#define I2C2_BASE      0x40005800UL
#define I2C1_BASE      0x40005400UL
//#define UART5_BASE     0x40005000		//NOT Included in STM32F103x6
//#define UART4_BASE     0x40004C00		//NOT Included in STM32F103x6
#define USART3_BASE    0x40004800UL
#define USART2_BASE    0x40004400UL
//#define SPI3_I2S_BASE  0x40003C00		//NOT Included in STM32F103x6
#define SPI2_I2S_BASE  0x40003800UL
//#define IWDG_BASE      0x40003000
//#define WWDG_BASE      0x40002C00
//#define RTC_BASE       0x40002800
//#define TIM14_BASE     0x40002000
//#define TIM13_BASE     0x40001C00
//#define TIM12_BASE     0x40001800
//#define TIM7_BASE      0x40001400
//#define TIM6_BASE      0x40001000
//#define TIM5_BASE      0x40000C00UL		//NOT Included in STM32F103x6
#define TIM4_BASE      0x40000800UL
#define TIM3_BASE      0x40000400UL
#define TIM2_BASE      0x40000000UL

//==========================================================================================
//===================================Peripheral registers===================================
//==========================================================================================

//-------------NVIC-------------
#define NVIC_ISER0      (*(volatile uint32_t*)(NVIC_BASE + 0x00))  // Interrupt Set-Enable Register 0
#define NVIC_ISER1      (*(volatile uint32_t*)(NVIC_BASE + 0x04))  // Interrupt Set-Enable Register 1
#define NVIC_ISER2      (*(volatile uint32_t*)(NVIC_BASE + 0x08))  // Interrupt Set-Enable Register 2

#define NVIC_ICER0      (*(volatile uint32_t*)(NVIC_BASE + 0x80))  // Interrupt Clear-Enable Register 0
#define NVIC_ICER1      (*(volatile uint32_t*)(NVIC_BASE + 0x84))  // Interrupt Clear-Enable Register 1
#define NVIC_ICER2      (*(volatile uint32_t*)(NVIC_BASE + 0x88))  // Interrupt Clear-Enable Register 2

//-------------RCC-------------
typedef struct {
	volatile uint32_t RCC_CR;         // Clock control register (Offset: 0x00)
	volatile uint32_t RCC_CFGR;       // Clock configuration register (Offset: 0x04)
	volatile uint32_t RCC_CIR;        // Clock interrupt register (Offset: 0x08)
	volatile uint32_t RCC_APB2RSTR;   // APB2 peripheral reset register (Offset: 0x0C)
	volatile uint32_t RCC_APB1RSTR;   // APB1 peripheral reset register (Offset: 0x10)
	volatile uint32_t RCC_AHBENR;     // AHB peripheral clock enable register (Offset: 0x14)
	volatile uint32_t RCC_APB2ENR;    // APB2 peripheral clock enable register (Offset: 0x18)
	volatile uint32_t RCC_APB1ENR;    // APB1 peripheral clock enable register (Offset: 0x1C)
	volatile uint32_t RCC_BDCR;       // Backup domain control register (Offset: 0x20)
	volatile uint32_t RCC_CSR;        // Control/status register (Offset: 0x24)
} RCC_REG_t;

//-------------GPIO-------------
typedef struct {
	volatile uint32_t GPIOx_CRL;   // Port configuration register low (Offset: 0x00)
	volatile uint32_t GPIOx_CRH;   // Port configuration register high (Offset: 0x04)
	volatile uint32_t GPIOx_IDR;   // Port input data register (Offset: 0x08)
	volatile uint32_t GPIOx_ODR;   // Port output data register (Offset: 0x0C)
	volatile uint32_t GPIOx_BSRR;  // Port bit set/reset register (Offset: 0x10)
	volatile uint32_t GPIOx_BRR;   // Port bit reset register (Offset: 0x14)
	volatile uint32_t GPIOx_LCKR;  // Port configuration lock register (Offset: 0x18)
} GPIOx_REG_t;

//-------------EXTI-------------
typedef struct {
	volatile uint32_t EXTI_IMR;      // Interrupt mask register (Offset: 0x00)
	volatile uint32_t EXTI_EMR;      // Event mask register (Offset: 0x04)
	volatile uint32_t EXTI_RTSR;     // Rising trigger selection register (Offset: 0x08)
	volatile uint32_t EXTI_FTSR;     // Falling trigger selection register (Offset: 0x0C)
	volatile uint32_t EXTI_SWIER;    // Software interrupt event register (Offset: 0x10)
	volatile uint32_t EXTI_PR;       // Pending register (Offset: 0x14)
} EXTI_REG_t;

//-------------AFIO-------------
typedef struct {
	volatile uint32_t AFIO_EVCR;      // Event control register (Offset: 0x00)
	volatile uint32_t AFIO_MAPR;      // Remap and debug I/O configuration register 1 (Offset: 0x04)
	volatile uint32_t AFIO_EXTICR[4]; // External interrupt configuration register 1,2,3,4 (Offset: 0x08,0x0C,0x10,0x14)
	volatile uint32_t RESERVED0;	  //												(Offset: 0x18)
	volatile uint32_t AFIO_MAPR2;     // Remap and debug I/O configuration register 2 (Offset: 0x1C)
} AFIO_REG_t;

//-------------USART-------------
typedef struct {
	volatile uint32_t USART_SR;  	 // Status register (Offset: 0x00)
	volatile uint32_t USART_DR;  	 // Data register (Offset: 0x04)
	volatile uint32_t USART_BRR; 	 // Baud rate register (Offset: 0x08)
	volatile uint32_t USART_CR1; 	// Control register 1 (Offset: 0x0C)
	volatile uint32_t USART_CR2;  	// Control register 2 (Offset: 0x10)
	volatile uint32_t USART_CR3;  	// Control register 3 (Offset: 0x14)
	volatile uint32_t USART_GTPR;	 // Guard time and prescaler register (Offset: 0x18)
} USARTx_REG_t;

//-------------SPI-------------
typedef struct {
	volatile uint32_t SPI_CR1;    	// Control Register 1 (Offset: 0x00)
	volatile uint32_t SPI_CR2;    	// Control Register 2 (Offset: 0x04)
	volatile uint32_t SPI_SR;     	// Status Register (Offset: 0x08)
	volatile uint32_t SPI_DR;     	// Data Register (Offset: 0x0C)
	volatile uint32_t SPI_CRCPR;  	// CRC Polynomial Register (Offset: 0x10)
	volatile uint32_t SPI_RXCRCR; 	// RX CRC Register (Offset: 0x14)
	volatile uint32_t SPI_TXCRCR; 	// TX CRC Register (Offset: 0x18)
	volatile uint32_t SPI_I2SCFGR;	// I2S Configuration Register (Offset: 0x1C)
	volatile uint32_t SPI_I2SPR;  	// I2S Prescaler Register (Offset: 0x20)
} SPIx_REG_t;

//-------------I2C-------------
typedef struct {
	volatile uint32_t I2C_CR1;      // Control Register 1 (Offset: 0x00)
	volatile uint32_t I2C_CR2;      // Control Register 2 (Offset: 0x04)
	volatile uint32_t I2C_OAR1;     // Own Address Register 1 (Offset: 0x08)
	volatile uint32_t I2C_OAR2;     // Own Address Register 2 (Offset: 0x0C)
	volatile uint32_t I2C_DR;       // Data Register (Offset: 0x10)
	volatile uint32_t I2C_SR1;      // Status Register 1 (Offset: 0x14)
	volatile uint32_t I2C_SR2;      // Status Register 2 (Offset: 0x18)
	volatile uint32_t I2C_CCR;      // Clock Control Register (Offset: 0x1C)
	volatile uint32_t I2C_TRISE;    // TRISE Register (Offset: 0x20)
} I2Cx_REG_t;

//-------------TIMx-------------
typedef struct {
    volatile uint32_t TIMx_CR1;      // Control Register 1 (Offset: 0x00)
    volatile uint32_t TIMx_CR2;      // Control Register 2 (Offset: 0x04)
    volatile uint32_t TIMx_SMCR;     // Slave Mode Control Register (Offset: 0x08)
    volatile uint32_t TIMx_DIER;     // DMA/Interrupt Enable Register (Offset: 0x0C)
    volatile uint32_t TIMx_SR;       // Status Register (Offset: 0x10)
    volatile uint32_t TIMx_EGR;      // Event Generation Register (Offset: 0x14)
    volatile uint32_t TIMx_CCMR1;    // Capture/Compare Mode Register 1 (Offset: 0x18)
    volatile uint32_t TIMx_CCMR2;    // Capture/Compare Mode Register 2 (Offset: 0x1C)
    volatile uint32_t TIMx_CCER;     // Capture/Compare Enable Register (Offset: 0x20)
    volatile uint32_t TIMx_CNT;      // Counter (Offset: 0x24)
    volatile uint32_t TIMx_PSC;      // Prescaler (Offset: 0x28)
    volatile uint32_t TIMx_ARR;      // Auto-Reload Register (Offset: 0x2C)
    volatile uint32_t TIMx_CCR1;     // Capture/Compare Register 1 (Offset: 0x34)
    volatile uint32_t TIMx_CCR2;     // Capture/Compare Register 2 (Offset: 0x38)
    volatile uint32_t TIMx_CCR3;     // Capture/Compare Register 3 (Offset: 0x3C)
    volatile uint32_t TIMx_CCR4;     // Capture/Compare Register 4 (Offset: 0x40)
    volatile uint32_t TIMx_BDTR;     // break and dead-time register (Offset: 0x44)		//Found only in ACTIM (Advanced-Control Timers)
    volatile uint32_t TIMx_DCR;      // DMA Control Register (Offset: 0x48)
    volatile uint32_t TIMx_DMAR;     // DMA Address for Full Transfer (Offset: 0x4C)
} TIMx_REG_t;

//==========================================================================================
//===================================Peripheral instants====================================
//==========================================================================================
#define RCC			((RCC_REG_t*)RCC_BASE)

#define GPIOA		((GPIOx_REG_t*)GPIOA_BASE)
#define GPIOB		((GPIOx_REG_t*)GPIOB_BASE)
#define GPIOC		((GPIOx_REG_t*)GPIOC_BASE)
#define GPIOD		((GPIOx_REG_t*)GPIOD_BASE)
#define EXTI		((EXTI_REG_t*)EXTI_BASE)
#define AFIO		((AFIO_REG_t*)AFIO_BASE)

#define USART1  	((USARTx_REG_t*)USART1_BASE)
#define USART2  	((USARTx_REG_t*)USART2_BASE)
#define USART3   	((USARTx_REG_t*)USART3_BASE)

#define SPI1  		((SPIx_REG_t*)SPI1_BASE)
#define SPI2   		((SPIx_REG_t*)SPI2_I2S_BASE)

#define I2C1  		((I2Cx_REG_t*)I2C1_BASE)
#define I2C2   		((I2Cx_REG_t*)I2C2_BASE)

#define TIM2   		((TIMx_REG_t*)TIM2_BASE)
#define TIM3   		((TIMx_REG_t*)TIM3_BASE)
#define TIM4   		((TIMx_REG_t*)TIM4_BASE)

//==========================================================================================
//==================================NVIC IRQ number Macros==================================
//==========================================================================================

//-------------EXTI-------------
#define EXTI0_iqrNUMBER      (uint8_t)6   // EXTI0 interrupt (IRQ6)
#define EXTI1_iqrNUMBER      (uint8_t)7   // EXTI1 interrupt (IRQ7)
#define EXTI2_iqrNUMBER      (uint8_t)8   // EXTI2 interrupt (IRQ8)
#define EXTI3_iqrNUMBER      (uint8_t)9   // EXTI3 interrupt (IRQ9)
#define EXTI4_iqrNUMBER      (uint8_t)10  // EXTI4 interrupt (IRQ10)
#define EXTI5_iqrNUMBER      (uint8_t)23  // EXTI5 interrupt (IRQ23)
#define EXTI6_iqrNUMBER      (uint8_t)23  // EXTI6 interrupt (IRQ23)
#define EXTI7_iqrNUMBER      (uint8_t)23  // EXTI7 interrupt (IRQ23)
#define EXTI8_iqrNUMBER      (uint8_t)23  // EXTI8 interrupt (IRQ23)
#define EXTI9_iqrNUMBER      (uint8_t)23  // EXTI9 interrupt (IRQ23)
#define EXTI10_iqrNUMBER     (uint8_t)40  // EXTI10 interrupt (IRQ40)
#define EXTI11_iqrNUMBER     (uint8_t)40  // EXTI11 interrupt (IRQ40)
#define EXTI12_iqrNUMBER     (uint8_t)40  // EXTI12 interrupt (IRQ40)
#define EXTI13_iqrNUMBER     (uint8_t)40  // EXTI13 interrupt (IRQ40)
#define EXTI14_iqrNUMBER     (uint8_t)40  // EXTI14 interrupt (IRQ40)
#define EXTI15_iqrNUMBER     (uint8_t)40  // EXTI15 interrupt (IRQ40)

//-------------USART-------------
#define USART1_iqrNUMBER      (uint8_t)37   // USART1 global interrupt (IRQ37)
#define USART2_iqrNUMBER      (uint8_t)38   // USART2 global interrupt (IRQ38)
#define USART3_iqrNUMBER      (uint8_t)39   // USART3 global interrupt (IRQ39)

//-------------SPI-------------
#define SPI1_iqrNUMBER      (uint8_t)35   // SPI1 global interrupt (IRQ35)
#define SPI2_iqrNUMBER      (uint8_t)36   // SPI2 global interrupt (IRQ36)

//-------------I2C-------------
#define I2C1_EV_iqrNUMBER      (uint8_t)31   // I2C1 event interrupt (IRQ31)
#define I2C1_ER_iqrNUMBER      (uint8_t)32   // I2C1 error interrupt (IRQ32)
#define I2C2_EV_iqrNUMBER      (uint8_t)33   // I2C2 event interrupt (IRQ33)
#define I2C2_ER_iqrNUMBER      (uint8_t)34   // I2C2 error interrupt (IRQ34)

//-------------GPTIM-------------
#define TIM2_iqrNUMBER      (uint8_t)28   // TIM2 global interrupt (IRQ28)
#define TIM3_iqrNUMBER      (uint8_t)29   // TIM3 global interrupt (IRQ29)
#define TIM4_iqrNUMBER      (uint8_t)30   // TIM4 global interrupt (IRQ30)

//==========================================================================================
//==============================NVIC IRQ enable/disable Macros==============================
//==========================================================================================

//-------------EXTI-------------
#define NVIC_EXTI0_IRQ6_EN    		 NVIC_ISER0 |= (1 << EXTI0_iqrNUMBER) 					// Enable EXTI0 interrupt (IRQ6)
#define NVIC_EXTI1_IRQ7_EN   		 NVIC_ISER0 |= (1 << EXTI1_iqrNUMBER) 		 			// Enable EXTI1 interrupt (IRQ7)
#define NVIC_EXTI2_IRQ8_EN    		 NVIC_ISER0 |= (1 << EXTI2_iqrNUMBER) 					// Enable EXTI2 interrupt (IRQ8)
#define NVIC_EXTI3_IRQ9_EN    		 NVIC_ISER0 |= (1 << EXTI3_iqrNUMBER) 		 			// Enable EXTI3 interrupt (IRQ9)
#define NVIC_EXTI4_IRQ10_EN  		 NVIC_ISER0 |= (1 << EXTI4_iqrNUMBER) 		 			// Enable EXTI4 interrupt (IRQ10)
#define NVIC_EXTI9_5_IRQ23_EN		 NVIC_ISER0 |= (1 << EXTI5_iqrNUMBER) 		 			// Enable EXTI5 to EXTI9 interrupts (IRQ23)
#define NVIC_EXTI15_10_IRQ40_EN 	 NVIC_ISER1 |= (1 << (EXTI10_iqrNUMBER-(uint8_t)32) ) 	// Enable EXTI10 to EXTI15 interrupts (IRQ40)

#define NVIC_EXTI0_IRQ6_DIS    		 NVIC_ICER0 |= (1 << EXTI0_iqrNUMBER)  		 			// Disable EXTI0 interrupt (IRQ6)
#define NVIC_EXTI1_IRQ7_DIS   		 NVIC_ICER0 |= (1 << EXTI1_iqrNUMBER) 		 			// Disable EXTI1 interrupt (IRQ7)
#define NVIC_EXTI2_IRQ8_DIS    		 NVIC_ICER0 |= (1 << EXTI2_iqrNUMBER)  		 			// Disable EXTI2 interrupt (IRQ8)
#define NVIC_EXTI3_IRQ9_DIS   		 NVIC_ICER0 |= (1 << EXTI3_iqrNUMBER)  		 			// Disable EXTI3 interrupt (IRQ9)
#define NVIC_EXTI4_IRQ10_DIS  		 NVIC_ICER0 |= (1 << EXTI4_iqrNUMBER) 					// Disable EXTI4 interrupt (IRQ10)
#define NVIC_EXTI9_5_IRQ23_DIS		 NVIC_ICER0 |= (1 << EXTI5_iqrNUMBER) 		 			// Disable EXTI5 to EXTI9 interrupts (IRQ23)
#define NVIC_EXTI15_10_IRQ40_DIS 	 NVIC_ICER1 |= (1 << (EXTI10_iqrNUMBER-(uint8_t)32) ) 	// Disable EXTI10 to EXTI15 interrupts (IRQ40)

//-------------USART-------------
#define NVIC_USART1_IRQ37_EN   NVIC_ISER1 |= (1 << (USART1_iqrNUMBER-(uint8_t)32) ) 		// Enable USART1 interrupt (IRQ37)
#define NVIC_USART2_IRQ38_EN   NVIC_ISER1 |= (1 << (USART2_iqrNUMBER-(uint8_t)32) ) 		// Enable USART2 interrupt (IRQ38)
#define NVIC_USART3_IRQ39_EN   NVIC_ISER1 |= (1 << (USART3_iqrNUMBER-(uint8_t)32) ) 		// Enable USART3 interrupt (IRQ39)

#define NVIC_USART1_IRQ37_DIS   NVIC_ICER1 |= (1 << (USART1_iqrNUMBER-(uint8_t)32) ) 		// Disable USART1 interrupt (IRQ37)
#define NVIC_USART2_IRQ38_DIS   NVIC_ICER1 |= (1 << (USART2_iqrNUMBER-(uint8_t)32) ) 		// Disable USART2 interrupt (IRQ38)
#define NVIC_USART3_IRQ39_DIS   NVIC_ICER1 |= (1 << (USART3_iqrNUMBER-(uint8_t)32) ) 		// Disable USART3 interrupt (IRQ39)

//-------------SPI-------------
#define NVIC_SPI1_IRQ35_EN   NVIC_ISER1 |= (1 << (SPI1_iqrNUMBER-(uint8_t)32) ) 		// Enable SPI1 interrupt (IRQ35)
#define NVIC_SPI2_IRQ36_EN   NVIC_ISER1 |= (1 << (SPI2_iqrNUMBER-(uint8_t)32) ) 		// Enable SPI2 interrupt (IRQ36)

#define NVIC_SPI1_IRQ35_DIS   NVIC_ICER1 |= (1 << (SPI1_iqrNUMBER-(uint8_t)32) ) 		// Disable SPI1 interrupt (IRQ35)
#define NVIC_SPI2_IRQ36_DIS   NVIC_ICER1 |= (1 << (SPI2_iqrNUMBER-(uint8_t)32) ) 		// Disable SPI2 interrupt (IRQ36)

//-------------I2C-------------
#define NVIC_I2C1_EV_IRQ31_EN   NVIC_ISER0 |= (1 << (I2C1_EV_iqrNUMBER))     			// Enable I2C1 event interrupt (IRQ31)
#define NVIC_I2C1_ER_IRQ32_EN   NVIC_ISER1 |= (1 << (I2C1_ER_iqrNUMBER-(uint8_t)32) )   // Enable I2C1 error interrupt (IRQ32)
#define NVIC_I2C2_EV_IRQ33_EN   NVIC_ISER1 |= (1 << (I2C2_EV_iqrNUMBER-(uint8_t)32) )   // Enable I2C2 event interrupt (IRQ33)
#define NVIC_I2C2_ER_IRQ34_EN   NVIC_ISER1 |= (1 << (I2C2_ER_iqrNUMBER-(uint8_t)32) )   // Enable I2C2 error interrupt (IRQ34)

#define NVIC_I2C1_EV_IRQ31_DIS  NVIC_ICER0 |= (1 << (I2C1_EV_iqrNUMBER))				// Disable I2C1 event interrupt (IRQ31)
#define NVIC_I2C1_ER_IRQ32_DIS  NVIC_ICER1 |= (1 << (I2C1_ER_iqrNUMBER-(uint8_t)32) )   // Disable I2C1 error interrupt (IRQ32)
#define NVIC_I2C2_EV_IRQ33_DIS  NVIC_ICER1 |= (1 << (I2C2_EV_iqrNUMBER-(uint8_t)32) )   // Disable I2C2 event interrupt (IRQ33)
#define NVIC_I2C2_ER_IRQ34_DIS  NVIC_ICER1 |= (1 << (I2C2_ER_iqrNUMBER-(uint8_t)32) )   // Disable I2C2 error interrupt (IRQ34)

//-------------GPTIM-------------
#define NVIC_TIM2_IRQ28_EN    		 NVIC_ISER0 |= (1 << TIM2_iqrNUMBER) 		// Enable TIM2 global interrupt (IRQ28)
#define NVIC_TIM3_IRQ29_EN   		 NVIC_ISER0 |= (1 << TIM3_iqrNUMBER) 		// Enable TIM3 global interrupt (IRQ29)
#define NVIC_TIM4_IRQ30_EN    		 NVIC_ISER0 |= (1 << TIM4_iqrNUMBER) 		// Enable TIM4 global interrupt (IRQ30)

#define NVIC_TIM2_IRQ28_DIS    		 NVIC_ICER0 |= (1 << TIM2_iqrNUMBER) 		// Disable TIM2 global interrupt (IRQ28)
#define NVIC_TIM3_IRQ29_DIS   		 NVIC_ICER0 |= (1 << TIM3_iqrNUMBER) 		// Disable TIM3 global interrupt (IRQ29)
#define NVIC_TIM4_IRQ30_DIS    		 NVIC_ICER0 |= (1 << TIM4_iqrNUMBER) 		// Disable TIM4 global interrupt (IRQ30)

//==========================================================================================
//=================================Clock enable/reset Macros================================
//==========================================================================================

//-------------APB2-------------
#define RCC_AFIO_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 0) 	//Bit 0 AFIOEN: Alternate function IO clock enable
#define RCC_GPIOA_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 2) 	//Bit 2 IOPAEN: IO port A clock enable
#define RCC_GPIOB_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 3) 	//Bit 3 IOPBEN: IO port B clock enable
#define RCC_GPIOC_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 4) 	//Bit 4 IOPCEN: IO port C clock enable
#define RCC_GPIOD_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 5) 	//Bit 5 IOPDEN: IO port D clock enable
#define RCC_USART1_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 14) 	//Bit 14 USART1EN: USART1 clock enable
#define RCC_SPI1_CLK_EN()		RCC->RCC_APB2ENR |= (1 << 12) 	//Bit 12 SPI1EN: SPI1 clock enable

#define RCC_AFIO_CLK_RST()	 	RCC->RCC_APB2RSTR |= (1 << 0);  RCC->RCC_APB2RSTR &= ~(1 << 0);	 // Bit 0 AFIORST: Alternate function IO clock reset
#define RCC_GPIOA_CLK_RST() 	RCC->RCC_APB2RSTR |= (1 << 2);  RCC->RCC_APB2RSTR &= ~(1 << 2);  // Bit 2 IOPARST: IO port A clock reset
#define RCC_GPIOB_CLK_RST() 	RCC->RCC_APB2RSTR |= (1 << 3);  RCC->RCC_APB2RSTR &= ~(1 << 3);  // Bit 3 IOPBRST: IO port B clock reset
#define RCC_GPIOC_CLK_RST()  	RCC->RCC_APB2RSTR |= (1 << 4);  RCC->RCC_APB2RSTR &= ~(1 << 4);  // Bit 4 IOPCRST: IO port C clock reset
#define RCC_GPIOD_CLK_RST()  	RCC->RCC_APB2RSTR |= (1 << 5);  RCC->RCC_APB2RSTR &= ~(1 << 5);  // Bit 5 IOPDRST: IO port D clock reset
#define RCC_USART1_CLK_RST() 	RCC->RCC_APB2RSTR |= (1 << 14); RCC->RCC_APB2RSTR &= ~(1 << 14); // Bit 14 USART1RST: USART1 clock reset
#define RCC_SPI1_CLK_RST() 		RCC->RCC_APB2RSTR |= (1 << 12); RCC->RCC_APB2RSTR &= ~(1 << 12); // Bit 12 SPI1RST: SPI1 clock reset


//-------------APB1-------------
#define RCC_USART2_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 17) 	//Bit 17 USART2EN: USART2 clock enable
#define RCC_USART3_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 18) 	//Bit 18 USART3EN: USART3 clock enable
#define RCC_SPI2_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 14) 	//Bit 14 SPI2EN: SPI2 clock enable
#define RCC_I2C1_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 21) 	//Bit 21 I2C1EN: I2C1 clock enable
#define RCC_I2C2_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 22) 	//Bit 22 I2C2EN: I2C2 clock enable
#define RCC_TIM2_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 0) 	//Bit 0 TIM2EN: TIM2 clock enable
#define RCC_TIM3_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 1) 	//Bit 1 TIM3EN: TIM3 clock enable
#define RCC_TIM4_CLK_EN()		RCC->RCC_APB1ENR |= (1 << 2) 	//Bit 2 TIM4EN: TIM4 clock enable



#define RCC_USART2_CLK_RST() 	RCC->RCC_APB1RSTR |= (1 << 17); RCC->RCC_APB1RSTR &= ~(1 << 17); // Bit 17 USART2RST: USART2 clock reset
#define RCC_USART3_CLK_RST() 	RCC->RCC_APB1RSTR |= (1 << 18); RCC->RCC_APB1RSTR &= ~(1 << 18); // Bit 18 USART3RST: USART3 clock reset
#define RCC_SPI2_CLK_RST() 		RCC->RCC_APB1RSTR |= (1 << 14); RCC->RCC_APB1RSTR &= ~(1 << 14); // Bit 14 SPI2RST: SPI2 clock reset
#define RCC_I2C1_CLK_RST()		RCC->RCC_APB1RSTR |= (1 << 21); RCC->RCC_APB1RSTR &= ~(1 << 21); // Bit 21 I2C1RST: I2C1 clock reset
#define RCC_I2C2_CLK_RST()		RCC->RCC_APB1RSTR |= (1 << 22); RCC->RCC_APB1RSTR &= ~(1 << 22); // Bit 22 I2C2RST: I2C2 clock reset
#define RCC_TIM2_CLK_RST()		RCC->RCC_APB1RSTR |= (1 << 0); 	RCC->RCC_APB1RSTR &= ~(1 << 0);  // Bit 0 TIM2RST: TIM2 clock reset
#define RCC_TIM3_CLK_RST()		RCC->RCC_APB1RSTR |= (1 << 1); 	RCC->RCC_APB1RSTR &= ~(1 << 1);  // Bit 1 TIM3RST: TIM3 clock reset
#define RCC_TIM4_CLK_RST()		RCC->RCC_APB1RSTR |= (1 << 2); 	RCC->RCC_APB1RSTR &= ~(1 << 2);  // Bit 2 TIM4RST: TIM4 clock reset


//==========================================================================================
//==============================Registers BitField Definition===============================
//==========================================================================================

//-------------I2C-------------

//I2C_CR1 register
#define I2C_CR1_PE_POSITION    		   (0U)
#define I2C_CR1_PE_MASK        		   (0x1UL << I2C_CR1_PE_POSITION)          //0b 0000 0000 0000 0001
#define I2C_CR1_PE              	   I2C_CR1_PE_MASK                         //Bit 0 PE: Peripheral Enable

#define I2C_CR1_SMBUS_POSITION  	   (1U)
#define I2C_CR1_SMBUS_MASK      	   (0x1UL << I2C_CR1_SMBUS_POSITION)       //0b 0000 0000 0000 0010
#define I2C_CR1_SMBUS           	   I2C_CR1_SMBUS_MASK                      //Bit 1 SMBUS: SMBus Mode

#define I2C_CR1_SMBTYPE_POSITION	   (3U)
#define I2C_CR1_SMBTYPE_MASK    	   (0x1UL << I2C_CR1_SMBTYPE_POSITION)     //0b 0000 0000 0000 1000
#define I2C_CR1_SMBTYPE         	   I2C_CR1_SMBTYPE_MASK                    //Bit 3 SMBTYPE: SMBus Type

#define I2C_CR1_ENARP_POSITION  	   (4U)
#define I2C_CR1_ENARP_MASK      	   (0x1UL << I2C_CR1_ENARP_POSITION)       //0b 0000 0000 0001 0000
#define I2C_CR1_ENARP           	   I2C_CR1_ENARP_MASK                      //Bit 4 ENARP: ARP Enable

#define I2C_CR1_ENPEC_POSITION  	   (5U)
#define I2C_CR1_ENPEC_MASK      	   (0x1UL << I2C_CR1_ENPEC_POSITION)       //0b 0000 0000 0010 0000
#define I2C_CR1_ENPEC           	   I2C_CR1_ENPEC_MASK                      //Bit 5 ENPEC: PEC Enable

#define I2C_CR1_ENGC_POSITION   	   (6U)
#define I2C_CR1_ENGC_MASK       	   (0x1UL << I2C_CR1_ENGC_POSITION)        //0b 0000 0000 0100 0000
#define I2C_CR1_ENGC            	   I2C_CR1_ENGC_MASK                       //Bit 6 ENGC: General Call Enable

#define I2C_CR1_NOSTRETCH_POSITION     (7U)
#define I2C_CR1_NOSTRETCH_MASK         (0x1UL << I2C_CR1_NOSTRETCH_POSITION)   //0b 0000 0000 1000 0000
#define I2C_CR1_NOSTRETCH              I2C_CR1_NOSTRETCH_MASK                  //Bit 7 NOSTRETCH: Clock Stretching Disable (Slave mode)

#define I2C_CR1_START_POSITION         (8U)
#define I2C_CR1_START_MASK             (0x1UL << I2C_CR1_START_POSITION)       //0b 0000 0001 0000 0000
#define I2C_CR1_START                  I2C_CR1_START_MASK                      //Bit 8 START: Start Generation

#define I2C_CR1_STOP_POSITION          (9U)
#define I2C_CR1_STOP_MASK              (0x1UL << I2C_CR1_STOP_POSITION)        //0b 0000 0010 0000 0000
#define I2C_CR1_STOP                   I2C_CR1_STOP_MASK                       //Bit 9 STOP: Stop Generation

#define I2C_CR1_ACK_POSITION           (10U)
#define I2C_CR1_ACK_MASK               (0x1UL << I2C_CR1_ACK_POSITION)         //0b 0000 0100 0000 0000
#define I2C_CR1_ACK                    I2C_CR1_ACK_MASK                        //Bit 10 ACK: Acknowledge Enable

#define I2C_CR1_POS_POSITION           (11U)
#define I2C_CR1_POS_MASK               (0x1UL << I2C_CR1_POS_POSITION)         //0b 0000 1000 0000 0000
#define I2C_CR1_POS                    I2C_CR1_POS_MASK                        //Bit 11 POS: Acknowledge/PEC Position (for data reception)

#define I2C_CR1_PEC_POSITION           (12U)
#define I2C_CR1_PEC_MASK               (0x1UL << I2C_CR1_PEC_POSITION)         //0b 0001 0000 0000 0000
#define I2C_CR1_PEC                    I2C_CR1_PEC_MASK                        //Bit 12 PEC: Packet Error Checking

#define I2C_CR1_ALERT_POSITION         (13U)
#define I2C_CR1_ALERT_MASK             (0x1UL << I2C_CR1_ALERT_POSITION)       //0b 0010 0000 0000 0000
#define I2C_CR1_ALERT                  I2C_CR1_ALERT_MASK                      //Bit 13 ALERT: SMBus Alert

#define I2C_CR1_SWRST_POSITION         (15U)
#define I2C_CR1_SWRST_MASK             (0x1UL << I2C_CR1_SWRST_POSITION)       //0b 1000 0000 0000 0000
#define I2C_CR1_SWRST                  I2C_CR1_SWRST_MASK                      //Bit 15 SWRST: Software Reset

//I2C_CR2 register
#define I2C_CR2_FREQ_POSITION          (0U)
#define I2C_CR2_FREQ_MASK              (0x3FUL << I2C_CR2_FREQ_POSITION)       //0b 0000 0000 0011 1111
#define I2C_CR2_FREQ                   I2C_CR2_FREQ_MASK                       //Bit 0-5 FREQ: Peripheral Clock Frequency

#define I2C_CR2_ITERREN_POSITION       (8U)
#define I2C_CR2_ITERREN_MASK           (0x1UL << I2C_CR2_ITERREN_POSITION)     //0b 0000 0001 0000 0000
#define I2C_CR2_ITERREN                I2C_CR2_ITERREN_MASK                    //Bit 8 ITERREN: Error Interrupt Enable

#define I2C_CR2_ITEVTEN_POSITION       (9U)
#define I2C_CR2_ITEVTEN_MASK           (0x1UL << I2C_CR2_ITEVTEN_POSITION)     //0b 0000 0010 0000 0000
#define I2C_CR2_ITEVTEN                I2C_CR2_ITEVTEN_MASK                    //Bit 9 ITEVTEN: Event Interrupt Enable

#define I2C_CR2_ITBUFEN_POSITION       (10U)
#define I2C_CR2_ITBUFEN_MASK           (0x1UL << I2C_CR2_ITBUFEN_POSITION)     //0b 0000 0100 0000 0000
#define I2C_CR2_ITBUFEN                I2C_CR2_ITBUFEN_MASK                    //Bit 10 ITBUFEN: Buffer Interrupt Enable

#define I2C_CR2_DMAEN_POSITION         (11U)
#define I2C_CR2_DMAEN_MASK             (0x1UL << I2C_CR2_DMAEN_POSITION)       //0b 0000 1000 0000 0000
#define I2C_CR2_DMAEN                  I2C_CR2_DMAEN_MASK                      //Bit 11 DMAEN: DMA Requests Enable

#define I2C_CR2_LAST_POSITION          (12U)
#define I2C_CR2_LAST_MASK              (0x1UL << I2C_CR2_LAST_POSITION)        //0b 0001 0000 0000 0000
#define I2C_CR2_LAST                   I2C_CR2_LAST_MASK                       //Bit 12 LAST: DMA Last Transfer

//I2C_OAR1 register
#define I2C_OAR1_ADD0_POSITION         (0U)
#define I2C_OAR1_ADD0_MASK             (0x1UL << I2C_OAR1_ADD0_POSITION)       //0b 0000 0000 0000 0001
#define I2C_OAR1_ADD0                  I2C_OAR1_ADD0_MASK                      //Bit 0 ADD0: Interface address

#define I2C_OAR1_ADD_1_7_POSITION      (1U)
#define I2C_OAR1_ADD_1_7_MASK          (0x7FUL << I2C_OAR1_ADD_1_7_POSITION)   //0b 0000 0000 1111 1110
#define I2C_OAR1_ADD_1_7               I2C_OAR1_ADD_1_7_MASK                   //Bits 7:1 ADD[7:1]: Interface address

#define I2C_OAR1_ADD_8_9_POSITION      (8U)
#define I2C_OAR1_ADD_8_9_MASK          (0x3UL << I2C_OAR1_ADD_8_9_POSITION)   //0b 0000 0011 0000 0000
#define I2C_OAR1_ADD_8_9               I2C_OAR1_ADD_8_9_MASK                   //Bits 9:8 ADD[9:8]: Interface address

#define I2C_OAR1_ADD_MODE_POSITION     (15U)
#define I2C_OAR1_ADD_MODE_MASK         (0x1UL << I2C_OAR1_ADD_MODE_POSITION)   //0b 1000 0000 0000 0000
#define I2C_OAR1_ADD_MODE              I2C_OAR1_ADD_MODE_MASK                  //Bit 15 ADDMODE Addressing mode (slave mode)

//I2C_OAR2 register
#define I2C_OAR2_ENDUAL_POSITION       (0U)
#define I2C_OAR2_ENDUAL_MASK           (0x1UL << I2C_OAR2_ENDUAL_POSITION)     //0b 0000 0000 0000 0001
#define I2C_OAR2_ENDUAL                I2C_OAR2_ENDUAL_MASK                    //Bit 0 ENDUAL: Dual addressing mode enable

#define I2C_OAR2_ADD2_1_7_POSITION         (1U)
#define I2C_OAR2_ADD2_1_7_MASK         (0x7FUL << I2C_OAR2_ADD2_1_7_POSITION)  //0b 0000 0000 1111 1110
#define I2C_OAR2_ADD2_1_7              I2C_OAR2_ADD2_1_7_MASK                  //Bits 7:1 ADD2[7:1]: Interface address

//I2C_SR1 register
#define I2C_SR1_SB_POSITION            (0U)
#define I2C_SR1_SB_MASK                (0x1UL << I2C_SR1_SB_POSITION)          //0b 0000 0000 0000 0001
#define I2C_SR1_SB                     I2C_SR1_SB_MASK                         //Bit 0 SB: Start Bit (Master mode)

#define I2C_SR1_ADDR_POSITION          (1U)
#define I2C_SR1_ADDR_MASK              (0x1UL << I2C_SR1_ADDR_POSITION)        //0b 0000 0000 0000 0010
#define I2C_SR1_ADDR                   I2C_SR1_ADDR_MASK                       //Bit 1 ADDR: Address sent (master mode)/matched (slave mode)

#define I2C_SR1_BTF_POSITION           (2U)
#define I2C_SR1_BTF_MASK               (0x1UL << I2C_SR1_BTF_POSITION)         //0b 0000 0000 0000 0100
#define I2C_SR1_BTF                    I2C_SR1_BTF_MASK                        //Bit 2 BTF: Byte Transfer Finished

#define I2C_SR1_ADD10_POSITION         (3U)
#define I2C_SR1_ADD10_MASK             (0x1UL << I2C_SR1_ADD10_POSITION)       //0b 0000 0000 0000 1000
#define I2C_SR1_ADD10                  I2C_SR1_ADD10_MASK                      //Bit 3 ADD10: 10-bit header sent (Master mode)

#define I2C_SR1_STOPF_POSITION         (4U)
#define I2C_SR1_STOPF_MASK             (0x1UL << I2C_SR1_STOPF_POSITION)       //0b 0000 0000 0001 0000
#define I2C_SR1_STOPF                  I2C_SR1_STOPF_MASK                      //Bit 4 STOPF: Stop detection (Slave mode)

#define I2C_SR1_RxNE_POSITION          (6U)
#define I2C_SR1_RxNE_MASK              (0x1UL << I2C_SR1_RxNE_POSITION)        //0b 0000 0000 0100 0000
#define I2C_SR1_RxNE                   I2C_SR1_RxNE_MASK                       //Bit 6 RxNE: Data Register not Empty (receivers)

#define I2C_SR1_TxE_POSITION           (7U)
#define I2C_SR1_TxE_MASK               (0x1UL << I2C_SR1_TxE_POSITION)         //0b 0000 0000 1000 0000
#define I2C_SR1_TxE                    I2C_SR1_TxE_MASK                        //Bit 7 TxE: Data Register Empty (transmitters)

#define I2C_SR1_BERR_POSITION          (8U)
#define I2C_SR1_BERR_MASK              (0x1UL << I2C_SR1_BERR_POSITION)        //0b 0000 0001 0000 0000
#define I2C_SR1_BERR                   I2C_SR1_BERR_MASK                       //Bit 8 BERR: Bus Error

#define I2C_SR1_ARLO_POSITION          (9U)
#define I2C_SR1_ARLO_MASK              (0x1UL << I2C_SR1_ARLO_POSITION)        //0b 0000 0010 0000 0000
#define I2C_SR1_ARLO                   I2C_SR1_ARLO_MASK                       //Bit 9 ARLO: Arbitration Lost (master mode)

#define I2C_SR1_AF_POSITION            (10U)
#define I2C_SR1_AF_MASK                (0x1UL << I2C_SR1_AF_POSITION)          //0b 0000 0100 0000 0000
#define I2C_SR1_AF                     I2C_SR1_AF_MASK                         //Bit 10 AF: Acknowledge Failure

#define I2C_SR1_OVR_POSITION           (11U)
#define I2C_SR1_OVR_MASK               (0x1UL << I2C_SR1_OVR_POSITION)         //0b 0000 1000 0000 0000
#define I2C_SR1_OVR                    I2C_SR1_OVR_MASK                        //Bit 11 OVR: Overrun/Underrun

#define I2C_SR1_PECERR_POSITION        (12U)
#define I2C_SR1_PECERR_MASK            (0x1UL << I2C_SR1_PECERR_POSITION)      //0b 0001 0000 0000 0000
#define I2C_SR1_PECERR                 I2C_SR1_PECERR_MASK                     //Bit 12 PECERR: PEC Error in reception

#define I2C_SR1_TIMEOUT_POSITION       (14U)
#define I2C_SR1_TIMEOUT_MASK           (0x1UL << I2C_SR1_TIMEOUT_POSITION)     //0b 0100 0000 0000 0000
#define I2C_SR1_TIMEOUT                I2C_SR1_TIMEOUT_MASK                    //Bit 14 TIMEOUT: Timeout or Tlow Error

#define I2C_SR1_SMBALERT_POSITION      (15U)
#define I2C_SR1_SMBALERT_MASK          (0x1UL << I2C_SR1_SMBALERT_POSITION)    //0b 1000 0000 0000 0000
#define I2C_SR1_SMBALERT               I2C_SR1_SMBALERT_MASK                   //Bit 15 SMBALERT: SMBus Alert

//I2C_SR2 register
#define I2C_SR2_MSL_POSITION           (0U)
#define I2C_SR2_MSL_MASK               (0x1UL << I2C_SR2_MSL_POSITION)         //0b 0000 0000 0000 0001
#define I2C_SR2_MSL                    I2C_SR2_MSL_MASK                        //Bit 0 MSL: Master/Slave

#define I2C_SR2_BUSY_POSITION          (1U)
#define I2C_SR2_BUSY_MASK              (0x1UL << I2C_SR2_BUSY_POSITION)        //0b 0000 0000 0000 0010
#define I2C_SR2_BUSY                   I2C_SR2_BUSY_MASK                       //Bit 1 BUSY: Bus Busy

#define I2C_SR2_TRA_POSITION           (2U)
#define I2C_SR2_TRA_MASK               (0x1UL << I2C_SR2_TRA_POSITION)         //0b 0000 0000 0000 0100
#define I2C_SR2_TRA                    I2C_SR2_TRA_MASK                        //Bit 2 TRA: Transmitter/Receiver

#define I2C_SR2_GENCALL_POSITION       (4U)
#define I2C_SR2_GENCALL_MASK           (0x1UL << I2C_SR2_GENCALL_POSITION)     //0b 0000 0000 0001 0000
#define I2C_SR2_GENCALL                I2C_SR2_GENCALL_MASK                    //Bit 4 GENCALL: General Call Address (Slave mode)

#define I2C_SR2_SMBDEFAULT_POSITION    (5U)
#define I2C_SR2_SMBDEFAULT_MASK        (0x1UL << I2C_SR2_SMBDEFAULT_POSITION)  //0b 0000 0000 0010 0000
#define I2C_SR2_SMBDEFAULT             I2C_SR2_SMBDEFAULT_MASK                 //Bit 5 SMBDEFAULT: SMBus Device Default Address (Slave mode)

#define I2C_SR2_SMBHOST_POSITION       (6U)
#define I2C_SR2_SMBHOST_MASK           (0x1UL << I2C_SR2_SMBHOST_POSITION)     //0b 0000 0000 0100 0000
#define I2C_SR2_SMBHOST                I2C_SR2_SMBHOST_MASK                    //Bit 6 SMBHOST: SMBus Host Header (Slave mode)

#define I2C_SR2_DUALF_POSITION         (7U)
#define I2C_SR2_DUALF_MASK             (0x1UL << I2C_SR2_DUALF_POSITION)       //0b 0000 0000 1000 0000
#define I2C_SR2_DUALF                  I2C_SR2_DUALF_MASK                      //Bit 7 DUALF: Dual Flag (Slave mode)

#define I2C_SR2_PEC_POSITION           (8U)
#define I2C_SR2_PEC_MASK               (0xFFUL << I2C_SR2_PEC_POSITION)        //0b 1111 1111 0000 0000
#define I2C_SR2_PEC                    I2C_SR2_PEC_MASK                        //Bits 15:8 PEC: Packet Error Checking Register

//I2C_CCR register
#define I2C_CCR_CCR_POSITION           (0U)
#define I2C_CCR_CCR_MASK               (0xFFFUL << I2C_CCR_CCR_POSITION)       //0b 0000 1111 1111 1111
#define I2C_CCR_CCR                    I2C_CCR_CCR_MASK                        //Bits 11:0 CCR[11:0]: Clock control register in Fast/Standard mode

#define I2C_CCR_DUTY_POSITION          (14U)
#define I2C_CCR_DUTY_MASK              (0x1UL << I2C_CCR_DUTY_POSITION)        //0b 0100 0000 0000 0000
#define I2C_CCR_DUTY                   I2C_CCR_DUTY_MASK                       //Bit 14 DUTY: Fast mode duty cycle

#define I2C_CCR_FS_POSITION            (15U)
#define I2C_CCR_FS_MASK                (0x1UL << I2C_CCR_FS_POSITION)          //0b 1000 0000 0000 0000
#define I2C_CCR_FS                     I2C_CCR_FS_MASK                         //Bit 15 F/S: I2C Master mode selection

//I2C_TRISE register
#define I2C_TRISE_TRISE_POSITION       (0U)
#define I2C_TRISE_TRISE_MASK           (0x3FUL << I2C_TRISE_TRISE_POSITION)    //0b 0000 0000 0011 1111
#define I2C_TRISE_TRISE                I2C_TRISE_TRISE_MASK                    //Bits 5:0 TRISE[5:0]: Maximum rise time in Fast/Standard mode


//==========================================================================================
//======================================Generic Macros======================================
//==========================================================================================


#endif /* INC_STM32F103X6_H_ */
