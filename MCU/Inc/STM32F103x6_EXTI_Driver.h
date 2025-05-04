/*
 * STM32F103x6_EXTI_Driver.h
 *
 *  Created on: Aug 21, 2024
 *      Author: Mohamed Hamdy
 */

#ifndef INC_STM32F103X6_EXTI_DRIVER_H_
#define INC_STM32F103X6_EXTI_DRIVER_H_

//==========================================================================================
//=========================================Includes=========================================
//==========================================================================================
#include "STM32F103x6.h"
#include "STM32F103x6_GPIO_Driver.h"

//==========================================================================================
//======================================Generic Macros======================================
//==========================================================================================
#define EXTI0            (uint8_t)0
#define EXTI1            (uint8_t)1
#define EXTI2            (uint8_t)2
#define EXTI3            (uint8_t)3
#define EXTI4            (uint8_t)4
#define EXTI5            (uint8_t)5
#define EXTI6            (uint8_t)6
#define EXTI7            (uint8_t)7
#define EXTI8            (uint8_t)8
#define EXTI9            (uint8_t)9
#define EXTI10           (uint8_t)10
#define EXTI11           (uint8_t)11
#define EXTI12           (uint8_t)12
#define EXTI13           (uint8_t)13
#define EXTI14           (uint8_t)14
#define EXTI15           (uint8_t)15


//==========================================================================================
//============================User type definitions (structures)============================
//==========================================================================================

typedef struct
{
	GPIOx_REG_t* GPIO_Port;		//@STM32F103x6.h @Peripheral instants
	uint16_t GPIO_PinNumber;	//@STM32F103x6_GPIO_Driver.h @Macros Configuration References
	uint8_t EXTI_LineNumber;	//@STM32F103x6_EXTI_Driver.h @Generic Macros
	uint8_t EXTI_IrqNumber;		//@STM32F103x6.h @NVIC EXTI IRQ number Macros
}EXTI_PinConfig_t;
//NOTE: EXTI_PinConfig_t MUST NOT be used individually or assigned member my member,
//it MUST be used as a member of EXTI_Config_t and totally assigned @reference EXTI_Pin_Define

typedef struct
{
	EXTI_PinConfig_t EXTI_Pin;		//This parameter must be a value of @reference EXTI_Pin_Define
	uint8_t EXTI_Trigger;		 	//This parameter must be a value of @reference EXTI_Trigger_Define
	uint8_t EXTI_EN_OR_DIS; 			//This parameter must be a value of @reference EXTI_EN_OR_DIS_Define
	void(*ptr_CallbackFUN)(void);
}EXTI_Config_t;

//==========================================================================================
//==============================Macros Configuration References=============================
//==========================================================================================

/*
	@reference EXTI_Pin_Define
*/
#define PA0_EXTI0		(EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}
#define PB0_EXTI0		(EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}
#define PC0_EXTI0		(EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}
#define PD0_EXTI0		(EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}

#define PA1_EXTI1		(EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}
#define PB1_EXTI1		(EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}
#define PC1_EXTI1		(EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}
#define PD1_EXTI1		(EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}

#define PA0_EXTI0       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}
#define PB0_EXTI0       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}
#define PC0_EXTI0       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}
#define PD0_EXTI0       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_0, EXTI0, EXTI0_iqrNUMBER}

#define PA1_EXTI1       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}
#define PB1_EXTI1       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}
#define PC1_EXTI1       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}
#define PD1_EXTI1       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_1, EXTI1, EXTI1_iqrNUMBER}

#define PA2_EXTI2       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_2, EXTI2, EXTI2_iqrNUMBER}
#define PB2_EXTI2       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_2, EXTI2, EXTI2_iqrNUMBER}
#define PC2_EXTI2       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_2, EXTI2, EXTI2_iqrNUMBER}
#define PD2_EXTI2       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_2, EXTI2, EXTI2_iqrNUMBER}

#define PA3_EXTI3       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_3, EXTI3, EXTI3_iqrNUMBER}
#define PB3_EXTI3       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_3, EXTI3, EXTI3_iqrNUMBER}
#define PC3_EXTI3       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_3, EXTI3, EXTI3_iqrNUMBER}
#define PD3_EXTI3       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_3, EXTI3, EXTI3_iqrNUMBER}

#define PA4_EXTI4       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_4, EXTI4, EXTI4_iqrNUMBER}
#define PB4_EXTI4       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_4, EXTI4, EXTI4_iqrNUMBER}
#define PC4_EXTI4       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_4, EXTI4, EXTI4_iqrNUMBER}
#define PD4_EXTI4       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_4, EXTI4, EXTI4_iqrNUMBER}

#define PA5_EXTI5       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_5, EXTI5, EXTI5_iqrNUMBER}
#define PB5_EXTI5       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_5, EXTI5, EXTI5_iqrNUMBER}
#define PC5_EXTI5       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_5, EXTI5, EXTI5_iqrNUMBER}
#define PD5_EXTI5       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_5, EXTI5, EXTI5_iqrNUMBER}

#define PA6_EXTI6       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_6, EXTI6, EXTI6_iqrNUMBER}
#define PB6_EXTI6       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_6, EXTI6, EXTI6_iqrNUMBER}
#define PC6_EXTI6       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_6, EXTI6, EXTI6_iqrNUMBER}
#define PD6_EXTI6       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_6, EXTI6, EXTI6_iqrNUMBER}

#define PA7_EXTI7       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_7, EXTI7, EXTI7_iqrNUMBER}
#define PB7_EXTI7       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_7, EXTI7, EXTI7_iqrNUMBER}
#define PC7_EXTI7       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_7, EXTI7, EXTI7_iqrNUMBER}
#define PD7_EXTI7       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_7, EXTI7, EXTI7_iqrNUMBER}

#define PA8_EXTI8       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_8, EXTI8, EXTI8_iqrNUMBER}
#define PB8_EXTI8       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_8, EXTI8, EXTI8_iqrNUMBER}
#define PC8_EXTI8       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_8, EXTI8, EXTI8_iqrNUMBER}
#define PD8_EXTI8       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_8, EXTI8, EXTI8_iqrNUMBER}

#define PA9_EXTI9       (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_9, EXTI9, EXTI9_iqrNUMBER}
#define PB9_EXTI9       (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_9, EXTI9, EXTI9_iqrNUMBER}
#define PC9_EXTI9       (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_9, EXTI9, EXTI9_iqrNUMBER}
#define PD9_EXTI9       (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_9, EXTI9, EXTI9_iqrNUMBER}

#define PA10_EXTI10     (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_10, EXTI10, EXTI10_iqrNUMBER}
#define PB10_EXTI10     (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_10, EXTI10, EXTI10_iqrNUMBER}
#define PC10_EXTI10     (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_10, EXTI10, EXTI10_iqrNUMBER}
#define PD10_EXTI10     (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_10, EXTI10, EXTI10_iqrNUMBER}

#define PA11_EXTI11     (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_11, EXTI11, EXTI11_iqrNUMBER}
#define PB11_EXTI11     (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_11, EXTI11, EXTI11_iqrNUMBER}
#define PC11_EXTI11     (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_11, EXTI11, EXTI11_iqrNUMBER}
#define PD11_EXTI11     (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_11, EXTI11, EXTI11_iqrNUMBER}

#define PA12_EXTI12     (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_12, EXTI12, EXTI12_iqrNUMBER}
#define PB12_EXTI12     (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_12, EXTI12, EXTI12_iqrNUMBER}
#define PC12_EXTI12     (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_12, EXTI12, EXTI12_iqrNUMBER}
#define PD12_EXTI12     (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_12, EXTI12, EXTI12_iqrNUMBER}

#define PA13_EXTI13     (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_13, EXTI13, EXTI13_iqrNUMBER}
#define PB13_EXTI13     (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_13, EXTI13, EXTI13_iqrNUMBER}
#define PC13_EXTI13     (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_13, EXTI13, EXTI13_iqrNUMBER}
#define PD13_EXTI13     (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_13, EXTI13, EXTI13_iqrNUMBER}

#define PA14_EXTI14     (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_14, EXTI14, EXTI14_iqrNUMBER}
#define PB14_EXTI14     (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_14, EXTI14, EXTI14_iqrNUMBER}
#define PC14_EXTI14     (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_14, EXTI14, EXTI14_iqrNUMBER}
#define PD14_EXTI14     (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_14, EXTI14, EXTI14_iqrNUMBER}

#define PA15_EXTI15     (EXTI_PinConfig_t){GPIOA, GPIO_PinNumber_15, EXTI15, EXTI15_iqrNUMBER}
#define PB14_EXTI15     (EXTI_PinConfig_t){GPIOB, GPIO_PinNumber_15, EXTI15, EXTI15_iqrNUMBER}
#define PC14_EXTI15     (EXTI_PinConfig_t){GPIOC, GPIO_PinNumber_15, EXTI15, EXTI15_iqrNUMBER}
#define PD14_EXTI15     (EXTI_PinConfig_t){GPIOD, GPIO_PinNumber_15, EXTI15, EXTI15_iqrNUMBER}

/*
	@reference EXTI_Trigger_Define
*/
#define EXTI_Trigger_RISING				(uint8_t)0
#define EXTI_Trigger_FALLING			(uint8_t)1
#define EXTI_Trigger_RISING_FALLING		(uint8_t)2

/*
	@reference EXTI_EN_OR_DIS_Define
*/
#define EXTI_DISABLED				(uint8_t)0
#define EXTI_ENABLED				(uint8_t)1

//==========================================================================================
//===========================APIs Supported by "MCAL EXTI DRIVER"===========================
//==========================================================================================

/**==================================================
 * @brief  Initializes an EXTI line.
 *
 * This function configures the EXTI line, trigger selection, interrupt handler,
 * and enables or disables the interrupt.
 *
 * @param  EXTI_Config  Pointer to an EXTI_Config_t structure that contains
 *                      the configuration information for the specified EXTI line.
 *
 * @retval None
 *
 * @note    - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *          - Ensure that the GPIO driver is included (STM32F103x6_GPIO_Driver.h) and compiled
 *            (STM32F103x6_GPIO_Driver.c, STM32F103x6_GPIO_Driver.o or STM32F103x6_GPIO_Driver.a).
 *          - The EXTI input pin is configured as floating input, so you are free to setup
 *            your external circuit.
 *
 * @example
 * EXTI_Config_t extiConfig;
 * extiConfig.EXTI_Pin = PA0_EXTI0;
 * extiConfig.EXTI_Trigger = EXTI_Trigger_RISING;
 * extiConfig.EXTI_EN_OR_DIS = EXTI_ENABLED;
 * extiConfig.ptr_CallbackFUN = &myEXTIhandlerfunction;
 * MCAL_EXTI_Init(&extiConfig);
 */
void MCAL_EXTI_Init(EXTI_Config_t* EXTI_Config);

/**==================================================
 * @brief  Updates the configuration of an EXTI line.
 *
 * This function updates the configuration of an already initialized EXTI line,
 * including trigger selection, interrupt handler and enables or disables the interrupt.
 *
 * @param  EXTI_Config  Pointer to an EXTI_Config_t structure that contains
 *                      the updated configuration information for the specified pre-defined EXTI line.
 *
 * @retval None
 *
 * @note    - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *          - Ensure that the GPIO driver is included (STM32F103x6_GPIO_Driver.h) and compiled
 *            (STM32F103x6_GPIO_Driver.c, STM32F103x6_GPIO_Driver.o or STM32F103x6_GPIO_Driver.a).
 *          - Ensure that you use this function with pre-defined EXTI line to update
 *            EXTI_Trigger, EXTI_EN_OR_DIS or ptrCallbackFUN.
 *
 * @example
 * EXTI_Config_t extiConfig;
 * extiConfig.EXTI_Pin = PA0_EXTI0;
 * extiConfig.EXTI_Trigger = EXTI_Trigger_FALLING;
 * extiConfig.EXTI_EN_OR_DIS = EXTI_ENABLED;
 * extiConfig.ptr_CallbackFUN = &myEXTIhandlerfunction;
 * MCAL_EXTI_Update(&extiConfig);
 */
void MCAL_EXTI_Update(EXTI_Config_t* EXTI_Config);

/**==================================================
 * @brief  Deinitializes the EXTI and NVIC peripheral
 *
 * This function resets the EXTI and NVIC configuration to its default state and disables all EXTI lines.
 *
 * @retval None
 *
 * @note    - Ensure that the GPIO and AFIO peripheral clocks are enabled before calling this function.
 *          - Ensure that the GPIO driver is included (STM32F103x6_GPIO_Driver.h) and compiled
 *            (STM32F103x6_GPIO_Driver.c, STM32F103x6_GPIO_Driver.o or STM32F103x6_GPIO_Driver.a).
 *
 * @example
 * MCAL_EXTI_DeInit();
 */
void MCAL_EXTI_DeInit(void);


#endif /* INC_STM32F103X6_EXTI_DRIVER_H_ */
