/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: 24 Nov 2022
 *      Author: ViralPatel
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"


// SPI configuration structure
typedef struct {
	uint8_t SPI_DeviceMode;									// @GPIO_PIN_NUMBER
	uint8_t SPI_BusConfig;									//@GPIO_PIN_MODES
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
} SPI_Config_t;


// handle structure for the SPIx peripheral
typedef struct
{
	SPI_RegDef_t	*pSPIx;				// base address of the SPIx peripheral
	SPI_Config_t 	SPIConfig;
}SPI_Handle_t;


/*
 * ******* API supported by the drivers
 *
 */




#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
