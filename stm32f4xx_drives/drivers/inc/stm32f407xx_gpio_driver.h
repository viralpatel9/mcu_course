/*
 * stm32f407xx_gio_driver.h
 *
 *  Created on: Nov 6, 2022
 *      Author: ViralPatel
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

#endif /* INC_STM32F407XX_GIO_DRIVER_H_ */

// this is the configuration strucure of the GPIO pin
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;


//  this is the handle structure for a GPIO pin
typedef struct
{
	// pointer to hold the base address of the GPIO peripheral
	GPIO_RegDef_t *pGPIOx;				// this hold the basse address of the GPIo port to whoch the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;		//to hold the configure of the peripheral.

}GPIO_Handle_t;


/******************************************************
 *
 * 					APIs Supported by the driver
 *
 ******************************************************/
/*
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
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR Handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);




















