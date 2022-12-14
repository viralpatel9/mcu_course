/*
 * stm32f4xx_i2c_driver.c
 *
 *  Created on: 6 Dec 2022
 *      Author: ViralPatel
 */
#include "stm32f407xx_i2c_driver.h"

uint16_t AHB_PreScalar[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScalar[4] = { 2,4,8,16};


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

static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx ->SR1;
	dummyRead = pI2Cx ->SR2;
	(void)dummyRead;
}

static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_STOP);
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
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

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
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

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
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

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
void I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
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



void I2C_MasterReceiveDataIT(I2C_Handle_t  *pI2CHandle, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
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

void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}


void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{

}

void I2C_EV_IRQHandling(I2C_Handle_t  *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device

	//1. handle for interrupt generated by SB event
	// Note: SB flag is only applicable in Master mode

	//2. Handle for interrupt generated by ADDR Event
	// Note: When master mode: Address is sent
	//		 When slave mode: Address matched with own address

	//3. Handle for interrupt generated by BTF(byte Transfer Finished) event

	//4. Handle for iterrupt generated by STOPF Event
	//NotL Stop detection flag is applicable only in slave mode, for master this flag

	//5. Handle for interrupt generated by TXE event

	//6. Handle for interrupt generated by RXNE Event

}

void I2C_ER_IRQHandling(I2C_Handle_t  *pI2CHandle)
{

}
