/*
 * stm32f4xx_i2c_driver.c
 *
 *  Created on: 6 Dec 2022
 *      Author: ViralPatel
 */
#include "stm32f407xx_i2c_driver.h"

uint16_t AHB_PreScalar[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScalar[4] = { 2,4,8,16};

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);


static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)			///send the slave address to the master
{
	SlaveAddr = SlaveAddr <<1;				//move tthe bit with 1 register
	SlaveAddr &=~(1);				//slaveAddr is the Slave Adress and clear off the first bit address
	pI2Cx->DR = SlaveAddr;			//store the slave address to the DR
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)			///send the slave address to the master
{
	SlaveAddr = SlaveAddr <<1;				//move tthe bit with 1 register
	SlaveAddr |=1;				//slaveAddr is the Slave Adress and clear off the first bit address
	pI2Cx->DR = SlaveAddr;			//store the slave address to the DR
}

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummy_read;
	// check for device mode
	if(pI2CHandle-> pI2Cx->SR2 & (1 <<I2C_SR2_MSL))
	{
		//device is in master mode
		if(pI2CHandle->TxRxState==I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize ==1 )
			{
				// first disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

				//clear the ADDR flag(read SR1, read SR2)
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}
		}
		else
		{
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;
		}
	}
	else
	{
		//device is in slave mode
		//straight away clear the ADDR flag
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}
}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_STOP);
}

void I2C_SlaveEnableDisableCallbackEvents(I2C_RegDef_t *pI2Cx,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx-> CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2Cx-> CR2 |= (1 << I2C_CR2_ITERREN);
		pI2Cx-> CR2 |= (1 << I2C_CR2_ITBUFEN);
	}
	else
	{
		pI2Cx-> CR2 |= ~(1 << I2C_CR2_ITEVTEN);
		pI2Cx-> CR2 |= ~(1 << I2C_CR2_ITERREN);
		pI2Cx-> CR2 |= ~(1 << I2C_CR2_ITBUFEN);
	}
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else
	{
		pI2Cx->CR1 &= ~(1 << 0);
	}

}

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}
}

uint32_t  RCC_GetPLLOutputClock()
{

	return 0;
}

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;
	uint8_t clksrc, temp,ahbp, apb1p;

	clksrc= ((RCC->CFGR>>2)&0x3);			//bring those 2 bits to the lsb position and mask
	if(clksrc==0)
	{
		SystemClk= 16000000;
	}
	else if(clksrc==1)
	{
		SystemClk= 8000000;
	}
	else if(clksrc==2)
	{
		SystemClk= RCC_GetPLLOutputClock();
	}

	//for ahb1
	temp=((RCC->CFGR>>4)&0xF);

	if(temp<8)
	{
		ahbp=1;
	}
	else
	{
		 ahbp=AHB_PreScalar[temp-8];
	}

	//for apb1
	temp=((RCC->CFGR>>10)&0x7);

	if(temp<4)
	{
		apb1p=1;
	}
	else
	{
		 apb1p=APB1_PreScalar[temp-4];
	}

	pclk1=(SystemClk/ahbp)/apb1p;

	return pclk1;
}
/*
 * 		@ I2C initialisation driver
 */

void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg=0;

	//enable the clock for
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	//configure the ack control bit
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl<<10;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//configure the FREQ field of CR"
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value()/1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	//configure the FREQ field for OAR
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress<<1;
	tempreg |= (1<<14);				//becasue datasheet says that
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR Calculations
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed<= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		ccr_value = (RCC_GetPCLK1Value()/(2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF);
	}
	else
	{
		//fast mode
		tempreg |= (1<<15);
		tempreg |= (1<<pI2CHandle->I2C_Config.I2C_FMDutyCycle<<14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value()/(3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			ccr_value = (RCC_GetPCLK1Value()/(25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	//TRISE Configuration
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed<= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		tempreg = (RCC_GetPCLK1Value()/1000000U)+1;
	}
	else
	{
		//mod eis fast mode
		tempreg = ((RCC_GetPCLK1Value()*300)/1000000000U)+1;
	}
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}


/*
 * 				De initiatlise the I2C function
 */
void I2C_DeInt(I2C_RegDef_t *pI2Cx)
{

}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate the START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completerd by checkin gthe SB flag in the SR1
	// note: until SB is cleared SCL will be stretched(pulled to Low)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. SEND THE ADDRESS FOO THE SLAVE WITH R/NW BIT SET TO THE W(0)(total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);

	//4. COnfirm that the address phase is completed bby checking the Addr flag in thhe SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. clear the ADDR flag according to its software sequence
	// Note: until ADDr is cleared SCL will be stretched(pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	//6. send the data until Len becomes 0
	while(Len>0)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE)); 			//wait till TXE is Set
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	//7. When Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP Condition
	// not: TXE=1, BTF=1, means that both SR and DR are empty and next transmission should begin
	//when BTF =1 SCL will be stretched(pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));

	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate STOP Condition and master need not to wait for the completion of stop condition.
	//note: Geenrating STOP, Automatically clear the BTF
	if(Sr==I2C_DISABLE_SR)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}
}

void I2C_MasterReceiveData(I2C_Handle_t  *pI2CHandle, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	//1. Generate the START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm thatstart generaton is completed by checking the SB flag in the SR1
	//Note: until SB is cleared SCL will be stretched(pulled to low)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to R(1)(total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Wait until address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));


	//procedure to read only 1 byte from slave
	if(Len==1)
	{
		// disable acking
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		//clear the addr flag
		I2C_ClearADDRFlag(pI2CHandle);

		//wait until RXNE becomes 1
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		//generate the stop condition
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

	//procedure to readfrom slave when Len>1
	if(Len>1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//read the data unitl Len becomes zero
		for ( uint32_t i=Len;i>0;i--)
		{
			//wait until RXNE becomes 1
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			if(i==2)			//if last 2 bytes are remaining
			{
				//clear the ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				//generate STOP condition

				if(Sr==I2C_DISABLE_SR)
					{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}

			}

			//read the data froom data register in to the buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//increment the buffer address
			pRxBuffer++;
		}
	}

	//re-enable Acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl==I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
	}

}


// For I2C Event handler
/*********************************************************************
 * @fn      		  - I2C_MasterSendDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -  Complete the below code . Also include the function prototype in header file

 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);

	}

	return busystate;

}



uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t  *pI2CHandle, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1<< I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return busystate;
}



void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi== I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->CR1 |= (1<<I2C_CR1_ACK);
	}
	else
	{
		//disable the ack
		pI2Cx->CR1 &=~(1<<I2C_CR1_ACK);
	}
}

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}

}


/*********************************************************************
 * @fn      		  - I2C_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen> 0)
					{
						//1. Load the data in to DR
						pI2CHandle->pI2Cx-> DR = *(pI2CHandle->pTxBuffer);

						//2. Decrement the Txlen
						pI2CHandle->TxLen--;

						//3. Increment the buffer address
						pI2CHandle->pTxBuffer++;
					}
}
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN Control Bit
    pI2CHandle->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);
     //Implement the code to disable ITEVFEN Control Bit
    pI2CHandle->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);
    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pRxBuffer = NULL;
    pI2CHandle->RxLen =0;
    pI2CHandle->RxSize = 0;

    if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
    {
    	I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
    }
}


void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN Control Bit
    pI2CHandle-> pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);
     //Implement the code to disable ITEVFEN Control Bit
    pI2CHandle->pI2Cx -> CR2 &= ~(1<< I2C_CR2_ITEVTEN);

    pI2CHandle-> TxRxState = I2C_READY;
    pI2CHandle-> pTxBuffer = NULL;
    pI2CHandle-> RxLen = 0;

}

void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data)
{
	pI2C-> DR = data;
}

uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C)
{
	return (uint8_t) pI2C->DR;
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{

					//we have to do the data reception
					if(pI2CHandle->RxSize==1)
					{
						*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
						pI2CHandle->RxLen--;
					}

					if(pI2CHandle->RxSize>1)
					{
						if(pI2CHandle->RxLen == 2)
						{
							//clear the ack bit
							I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
						}

						//read DR
						*pI2CHandle-> pRxBuffer = pI2CHandle-> pI2Cx->DR;
						pI2CHandle->pRxBuffer++;
						pI2CHandle->RxLen--;
					}

					if(pI2CHandle->RxLen == 0)
						{
						//close the I2C data receptio and notify the application
						//1. Generate the stop condition
						if(pI2CHandle->Sr == I2C_DISABLE_SR)
							I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

						//2. close the I2C Rx
						I2C_CloseReceiveData(pI2CHandle);

						//3. Notify the application
						I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
						}
}


void I2C_EV_IRQHandling(I2C_Handle_t  *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device

	uint32_t temp1, temp2, temp3;

	temp1 = pI2CHandle-> pI2Cx->CR2 & (1<< I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle-> pI2Cx->CR2 & (1<<I2C_CR2_ITBUFEN);

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<< I2C_SR1_SB);

	//1. handle for interrupt generated by SB event
	// Note: SB flag is only applicable in Master mode
	if(temp1 && temp3)
	{
		//SB flag is set
		//the interrupt is generated because of the SB mode
		//this block will not be executed in slave mode because for the slave SB is always zero
		/// in this block lets execute the address phase
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}


	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<< I2C_SR1_ADDR);

	//2. Handle for interrupt generated by ADDR Event
	// Note: When master mode: Address is sent
	//		 When slave mode: Address matched with own address
	if(temp1 && temp3)
	{
		//ADDR flag is set
		// interrupt is generated because of ADDR event
		I2C_ClearADDRFlag(pI2CHandle);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<< I2C_SR1_BTF);
	//3. Handle for interrupt generated by BTF(byte Transfer Finished) event
	if(temp1 && temp3)
	{
		//BTF flag is set
		if(pI2CHandle-> TxRxState == I2C_BUSY_IN_TX)
		{
			// make sure the TXE is also set
			if(pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_TXE))
			{
				//BTF, TXE = 1
				if(pI2CHandle->TxLen ==0)
				{
				// 1. Generate the STOP condition
				if(pI2CHandle->Sr == I2C_DISABLE_SR)
				{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}

				//2. Reset all the member elements of the handle structure
				I2C_CloseSendData(pI2CHandle);

				//3. notify the application abou the transmission complete
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_STOPF);
	//4. Handle for iterrupt generated by STOPF Event
	//NotL Stop detection flag is applicable only in slave mode, for master this flag
	//this code block will not be executed by the master since the STOPF will not set in master mode

	if(temp1 && temp3)
	{
		//STOF flag is already set
		//clear the STOPF flag(i.e first read SR1 then write to CR1)
		pI2CHandle->pI2Cx->CR1 |= 0x0000;
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_TXE);
	//5. Handle for interrupt generated by TXE event
	if(temp1 && temp2 && temp3)
	{
		//check for device mode
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			//TXE flag is set
			//we have to the data transmission
			if(pI2CHandle-> TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}
		else
		{
			//slave
			//make sure that the slave is really in transmitter mode
			if(pI2CHandle->pI2Cx->SR2 & (1 <<I2C_SR2_TRA))
			{
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_RXNE);
	//6. Handle for interrupt generated by RXNE Event
	if(temp1 && temp2 && temp3)
	{
		//check for DEVICE MODE
		if(pI2CHandle->pI2Cx->SR2 & (1<< I2C_SR2_MSL))
		{
			// THE DEVICE IS MASTER
			//RXNE flag is set
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}
		else
		{
			//slave
			//make sire that the slave is in receiver mode
			if(!(pI2CHandle->pI2Cx->SR2 & (1<< I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}
}

void I2C_ER_IRQHandling(I2C_Handle_t  *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1<< I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1<< I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1<< I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}

}
