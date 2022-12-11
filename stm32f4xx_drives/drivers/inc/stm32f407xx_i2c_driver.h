/*
 * stm32f4xx_i2c_driver.h
 *
 *  Created on: 6 Dec 2022
 *      Author: ViralPatel
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

//configuration structure for I2Cx peripheral
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint32_t I2C_DeviceAddress;
	uint32_t I2C_ACKControl;
	uint32_t I2C_FMDutyCycle;
}I2C_Config_t;

/*
 *   Handle Structure for I2Cx Peripheral
 */
typedef struct
{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
}I2C_Handle_t;

/*
 * 		@I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM		100000
#define I2C_SCL_SPEED_FM4K		400000
#define I2C_SCL_SPEED_FM2K		200000

/*
 * 		@ACK Control
 */
#define I2C_ACK_ENABLE 			1
#define I2C_ACK_DISABLE			0

/*
 * 		@ I2C_FMDUtyCycle
 */
#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1


//I2C related status flag defiition.... this can be found in status registers
#define I2C_FLAG_TXE 			(1<<I2C_SR1_TXE)
#define I2C_FLAG_RXNE			(1<<I2C_SR1_RXNE)
#define I2C_FLAG_SB				(1<<I2C_SR1_SB)
#define I2C_FLAG_OVR			(1<<I2C_SR1_OVR)
#define I2C_FLAG_AF				(1<<I2C_SR1_AF)
#define I2C_FLAG_ARLO			(1<<I2C_SR1_ARLO)
#define I2C_FLAG_BERR			(1<<I2C_SR1_BERR)
#define I2C_FLAG_STOPF			(1<<I2C_SR1_STOPF)
#define I2C_FLAG_ADD10			(1<<I2C_SR1_ADD10)
#define I2C_FLAG_BTF			(1<<I2C_SR1_BTF)
#define I2C_FLAG_ADDR			(1<<I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT		(1<<I2C_SR1_TIMEOUT)


/*
 * ******* API supported by the drivers**********
 *
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * 	Init and Deint of the peripheral
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInt(I2C_RegDef_t *pI2Cx);


// data send and receive using the I2C pin

// blocking(non interrupts) and non blocking the api



void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr);


void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);


/*
 * 	Other peripheral control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

uint32_t  RCC_GetPLLOutputClock(void);

/*
 * application callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);


#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */