/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: 24 Nov 2022
 *      Author: ViralPatel
 */

#include "stm32f407xx_spi_driver.h"


void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
			if (pSPIx == SPI1)
			{
				SPI1_PCLK_EN();
			}
			else if (pSPIx == SPI2)
			{
				SPI2_PCLK_EN();
			}
			else if (pSPIx == SPI3)
			{
				SPI3_PCLK_EN();
			}
	}
	else
	{
			if (pSPIx == SPI1)
			{
				SPI1_PCLK_DI();
			}
			else if (pSPIx == SPI2)
			{
				SPI2_PCLK_DI();
			}
			else if (pSPIx == SPI3)
			{
				SPI3_PCLK_DI();
			}
	}
}

/*
 * 	Init and Deint of the peripheral
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//first lets configgure the spi_cr1 register
	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle -> SPIConfig.SPI_DeviceMode <<SPI_CR1_MSTR;

	//2. configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg &=~(1<<SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// bidi mode should be set
		tempreg|=(1<<SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// bisi mode should be cleared
		tempreg &=~(1<<SPI_CR1_BIDIMODE);
		//rxonly bit must be selected
		tempreg |=(1<<SPI_CR1_RXONLY);
	}

	//3. configure the spi serial clock speed(baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed<<SPI_CR1_BR;

	//4. configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF<<SPI_CR1_DFF;

	//5. configure the CPOL
	tempreg |= pSPIHandle-> SPIConfig.SPI_CPOL<<SPI_CR1_CPOL;

	//6. configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA<<SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1=tempreg;

}


void SPI_DeInt(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1) {
			SPI1_REG_RESET();
		} else if (pSPIx == SPI2) {
			SPI2_REG_RESET();
		} else if (pSPIx == SPI3) {
			SPI3_REG_RESET();
		}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		 return FLAG_SET;
	}
	return FLAG_RESET;
}

// data send and receive using the spi pin

// blocking(non interrupts) and non blocking the api

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len>0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//2. check the DFF bit in CR1
		if((pSPIx->CR1 & (1<<SPI_CR1_DFF)))
		{
			//16 bit DFF
			//1. Load the data in to the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;			//the len is decreae by 2 because of the 16 bit
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			//8 bit DFF
			//1. Load the data in to the DR
			pSPIx->DR = *pTxBuffer;
			Len--;					// the len is decrease by 1 bcoz of 8 bbit
			pTxBuffer++;
		}

	}
}


void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len>0)
	{
		//1. wait until RXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		//2. check the DFF bit in CR1
		if((pSPIx->CR1 & (1<<SPI_CR1_DFF)))
		{
			//16 bit DFF
			//1. Load the data froim DR to RX buffer
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;
			Len--;
			Len--;			//the len is decreae by 2 because of the 16 bit
			(uint16_t*)pRxBuffer++;
		}
		else
		{
			//8 bit DFF
			//1. Load the data from DR to Rxbuffer address
			*pRxBuffer=pSPIx->DR;
			Len--;					// the len is decrease by 1 bcoz of 8 bbit
			pRxBuffer++;
		}

	}
}


void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi ==ENABLE)
	{
		 pSPIx->CR1 |= (1<<SPI_CR1_SPE); 			//this enables the SPI
	}
	else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SPE);			//this disables the SPI
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi ==ENABLE)
	{
		 pSPIx->CR1 |= (1<<SPI_CR1_SSI); 			//this enables the SPI
	}
	else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SSI);			//this disables the SPI
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi ==ENABLE)
	{
		 pSPIx->CR1 |= (1<<SPI_CR2_SSOE); 			//this enables the SPI
	}
	else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR2_SSOE);			//this disables the SPI
	}
}
