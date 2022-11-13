/*
 * stm32f407xx_gio_driver.h
 *
 *  Created on: Nov 6, 2022
 *      Author: ViralPatel
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

// this is the configuration strucure of the GPIO pin
typedef struct {
	uint8_t GPIO_PinNumber;									// @GPIO_PIN_NUMBER
	uint8_t GPIO_PinMode;									//@GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

//  this is the handle structure for a GPIO pin
typedef struct {
	// pointer to hold the base address of the GPIO peripheral
	GPIO_RegDef_t *pGPIOx;// this hold the basse address of the GPIo port to whoch the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;//to hold the configure of the peripheral.

} GPIO_Handle_t;

/********************GPIO Modes*****************@GPIO_PIN_MODES  ****/
#define GPIO_MODE_IN 		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_ALTFN		2
#define GPIO_MODE_ANALOG	3
#define GPIO_MODE_IT_FT		4				//Interrupts in the initialisation of the pin
#define GPIO_MODE_IT_RT		5
#define GPIO_MODE_IT_RFT	6

//GPIO pin output types
#define	GPIO_OP_TYPE_PP		0
#define	GPIO_OP_TYPE_OD		1

//GPIO pin output speed
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_FAST		2
#define GPIO_SPEED_HIGH		3

// GPIO Pull up and pull down configuration macros
#define GPIO_NO_PUPD		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2

// GPIO PIN NUMBER    @GPIO_PIN_NUMBER
#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1		1
#define GPIO_PIN_NO_2		2
#define GPIO_PIN_NO_3		3
#define GPIO_PIN_NO_4		4
#define GPIO_PIN_NO_5		5
#define GPIO_PIN_NO_6		6
#define GPIO_PIN_NO_7		7
#define GPIO_PIN_NO_8		8
#define GPIO_PIN_NO_9		9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15

/******************************************************
 *
 * 					APIs Supported by the driver
 *
 ******************************************************/
/*
 *
 * 			Peripheral Cock Setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * 	Init and Deint of the peripheral
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInt(GPIO_RegDef_t *pGPIOx);

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber,
		uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR Handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F407XX_GIO_DRIVER_H_ */

