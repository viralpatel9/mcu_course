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


//dEVICE MODE
#define SPI_DEVICE_MODE_MASTER 		1
#define SPI_DEVICE_MODE_SLAVE		0

// SPI Bus Config
#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	3

//SPI SclkSpeed
#define SPI_SCLK_SPEED_DIV2				0
#define SPI_SCLK_SPEED_DIV4				1
#define SPI_SCLK_SPEED_DIV8				2
#define SPI_SCLK_SPEED_DIV16			3
#define SPI_SCLK_SPEED_DIV32			4
#define SPI_SCLK_SPEED_DIV64			5
#define SPI_SCLK_SPEED_DIV128			6
#define SPI_SCLK_SPEED_DIV256			7

//SPI_DFF
#define SPI_DFF_8BITS 		0
#define SPI_DFF_16BITS 		1

//for SPI_CPOL
#define SPI_CPOL_HIGH 		1
#define SPI_CPOL_LOW 		0

//for SPI_CPHA
#define SPI_CPHA_HIGH 		1
#define SPI_CPHA_LOW 		0

// for SPI_SSM
#define SPI_SSM_EN			1
#define SPI_SSM_DI			0

//spi related status flag defiition
#define SPI_TXE_FLAG 		(1<<SPI_SR_TXE)
#define SPI_RXNE_FLAG		(1<<SPI_SR_RXNE)
#define SPI_BUSY_FLAG		(1<<SPI_SR_BSY)


/*
 * ******* API supported by the drivers
 *
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * 	Init and Deint of the peripheral
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInt(SPI_RegDef_t *pSPIx);


// data send and receive using the spi pin

// blocking(non interrupts) and non blocking the api



void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer, uint32_t Len);


void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
