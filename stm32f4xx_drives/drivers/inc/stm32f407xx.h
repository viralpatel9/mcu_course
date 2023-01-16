/*
 * stm32f407xx.h
 *
 *  Created on: Nov 5, 2022
 *      Author: ViralPatel
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include<stddef.h>

#define __vo volatile
#define __weak __attribute__((weak))

/*
 *  Processsor specific detials
 *  ARM NVIC Registers for ISERx Registers
 */
#define NVIC_ISER0					((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1					((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2					((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3					((__vo uint32_t*)0xE000E10c)


// ARM NVIC Register for the ICERx Register Adresses
#define NVIC_ICER0					((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1					((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2					((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3					((__vo uint32_t*)0xE000E18c)

#define NVIC_PR_BASE_ADDR			((__vo uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED 		4

// define the flash and SRAM memories
#define FLASH_BASEADDR				0x08000000U			// main memory in the datasheet is called flash memory
#define SRAM1_BASEADDR				0x20000000U			//112kb bytes of SRM1
#define SRMA2_BASEADDR				0x20001C00U			//after SRM1 112kb=112*1024 = some bytes and then convert it into hex
#define ROM							0x1FFF0000U
#define SRAM 						SRAM1_BASEADDR

// define the base addresses of the Bus Peripheral

#define PERIPH_BASE 				0x40000000U
#define APB1PERIPH_BASE 			PERIPH_BASE
#define APB2PERIPH_BASE				0X40010000U
#define AHB1PERIPH_BASE 			0X40020000U
#define AHB2PERIPH_BASE 			0X50000000U

// define each peripheral hanging to the AHB1 bus
// to do : complete for all the other peripherals..

#define GPIOA_BASEADDR 				(AHB1PERIPH_BASE+0x0000)
#define GPIOB_BASEADDR 				(AHB1PERIPH_BASE+0x0400)
#define GPIOC_BASEADDR 				(AHB1PERIPH_BASE+0x0800)
#define GPIOD_BASEADDR 				(AHB1PERIPH_BASE+0x0C00)
#define GPIOE_BASEADDR 				(AHB1PERIPH_BASE+0x1000)
#define GPIOF_BASEADDR 				(AHB1PERIPH_BASE+0x1400)
#define GPIOG_BASEADDR 				(AHB1PERIPH_BASE+0x1800)
#define GPIOH_BASEADDR 				(AHB1PERIPH_BASE+0x1C00)
#define GPIOI_BASEADDR 				(AHB1PERIPH_BASE+0x2000)
#define RCC_BASEADDR				(AHB1PERIPH_BASE+0x3800)
// Base addr of the peripheral of hanging to APB1 bus

#define I2C1_BASEADDR 				(APB1PERIPH_BASE+0x5400)
#define I2C2_BASEADDR 				(APB1PERIPH_BASE+0x5800)
#define I2C3_BASEADDR 				(APB1PERIPH_BASE+0x5C00)

#define SPI2_BASEADDR 				(APB1PERIPH_BASE+0x3800)
#define SPI3_BASEADDR 				(APB1PERIPH_BASE+0x3C00)

#define USART2_BASEADDR 			(APB1PERIPH_BASE+0x4400)
#define USART3_BASEADDR 			(APB1PERIPH_BASE+0x4800)
#define UART4_BASEADDR 				(APB1PERIPH_BASE+0x4C00)
#define UART5_BASEADDR 				(APB1PERIPH_BASE+0x5000)

// Base addr of the peripheral that are hanging to the APB2 Bus

#define SPI1_BASEADDR 				(APB2PERIPH_BASE+0x3000)
#define USART1_BASEADDR 			(APB2PERIPH_BASE+0x1000)
#define USART6_BASEADDR 			(APB2PERIPH_BASE+0x1400)
#define EXTI_BASEADDR 				(APB2PERIPH_BASE+0x3C00)
#define SYSCFG_BASEADDR 			(APB2PERIPH_BASE+0x3800)

/************peripheral register definition*******************/

typedef struct {
	__vo uint32_t MODER;					//defining structural
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];				// AFR[0] is AFLR and AFR[1] is the AFHR
} GPIO_RegDef_t;

typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
}SPI_RegDef_t;

typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t OAR1;
	__vo uint32_t OAR2;
	__vo uint32_t DR;
	__vo uint32_t SR1;
	__vo uint32_t SR2;
	__vo uint32_t CCR;
	__vo uint32_t TRISE;
	__vo uint32_t FLTR;
}I2C_RegDef_t;

typedef struct
{
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t BRR;
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t CR3;
	__vo uint32_t GTPR;
}USART_RegDef_t;

typedef struct {
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	uint32_t RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	uint32_t RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	uint32_t RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;
	__vo uint32_t CKGATENR;
	__vo uint32_t DCKCFGR2;
} RCC_RegDef_t;


/*
 * 			PEripheral register definition structure of EXTI
 */

typedef struct
{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_RegDef_t;


/*
 * 	Peripheral register structure for SYSCFG
 */

typedef struct
{
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	uint32_t RESERVED1[2];
	__vo uint32_t CMPCR;
	__vo 	uint32_t RESERVED2[2];
	uint32_t CFGR;
}SYSCFG_RegDef_t;


/******** peripheral definitions *********/

#define GPIOA 						((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB 						((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 						((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 						((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 						((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF 						((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG 						((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH 						((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI 						((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC							((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI						((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG						((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define SPI1						((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2						((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3						((SPI_RegDef_t*)SPI3_BASEADDR)

#define I2C1						((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2						((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3						((I2C_RegDef_t*)I2C3_BASEADDR)

// Clock enable macros for the GPIOx peripherals

#define GPIOA_PCLK_EN() 	(RCC->AHB1ENR |=(1<<0))			// to enable the clock of the gpio, it is Bit 0 where the clock is present so that is enable as 1.(RCC AHB1ENR)
#define GPIOB_PCLK_EN() 	(RCC->AHB1ENR |=(1<<1))
#define GPIOC_PCLK_EN() 	(RCC->AHB1ENR |=(1<<2))
#define GPIOD_PCLK_EN() 	(RCC->AHB1ENR |=(1<<3))
#define GPIOE_PCLK_EN() 	(RCC->AHB1ENR |=(1<<4))
#define GPIOF_PCLK_EN() 	(RCC->AHB1ENR |=(1<<5))
#define GPIOG_PCLK_EN() 	(RCC->AHB1ENR |=(1<<6))
#define GPIOH_PCLK_EN() 	(RCC->AHB1ENR |=(1<<7))
#define GPIOI_PCLK_EN() 	(RCC->AHB1ENR |=(1<<8))

// Clock enable macros for I2C Peripheral

#define I2C1_PCLK_EN() 		(RCC->APB1ENR |=(1<<21))
#define I2C2_PCLK_EN() 		(RCC->APB1ENR |=(1<<22))
#define I2C3_PCLK_EN() 		(RCC->APB1ENR |=(1<<23))

// Clock enable macros for SPIx peripheral
#define SPI1_PCLK_EN()			(RCC->APB2ENR |=(1<<12))
#define SPI2_PCLK_EN()			(RCC->APB1ENR |=(1<<14))
#define SPI3_PCLK_EN()			(RCC->APB1ENR |=(1<<15))
#define SPI4_PCLK_EN()			(RCC->APB2ENR |=(1<<13))

// Clock enable macros for USARTx peripherals
#define USART1_PCLK_EN()			(RCC->APB2ENR |=(1<<4))
#define USART2_PCLK_EN()			(RCC->APB1ENR |=(1<<17))
#define USART3_PCLK_EN()			(RCC->APB1ENR |=(1<<18))
#define USART6_PCLK_EN()			(RCC->APB2ENR |=(1<<5))

// Clock enable macros for SYSCFG peripheral
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |=(1<<14))

//  Clock disable macros for the GPIOx peripherals
#define GPIOA_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<0))
#define GPIOB_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<1))
#define GPIOC_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<2))
#define GPIOD_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<3))
#define GPIOE_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<4))
#define GPIOF_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<5))
#define GPIOG_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<6))
#define GPIOH_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<7))
#define GPIOI_PCLK_DI() 	(RCC->AHB1ENR &=~(1<<8))

// Clock disable macros for I2C Peripheral
#define I2C1_PCLK_DI() 		(RCC->APB1ENR &=~(1<<21))
#define I2C2_PCLK_DI() 		(RCC->APB1ENR &=~(1<<22))
#define I2C3_PCLK_DI() 		(RCC->APB1ENR &=~(1<<23))

// Clock disable macros for SPIx peripheral
#define SPI1_PCLK_DI()			(RCC->APB2ENR &=~(1<<12))
#define SPI2_PCLK_DI()			(RCC->APB1ENR &=~(1<<14))
#define SPI3_PCLK_DI()			(RCC->APB1ENR &=~(1<<15))
#define SPI4_PCLK_DI()			(RCC->APB2ENR &=~(1<<13))

// Clock disable macros for USARTx peripherals
#define USART1_PCLK_DI()			(RCC->APB2ENR &=~(1<<4))
#define USART2_PCLK_DI()			(RCC->APB1ENR &=~(1<<17))
#define USART3_PCLK_DI()			(RCC->APB1ENR &=~(1<<18))
#define USART6_PCLK_DI()			(RCC->APB2ENR &=~(1<<5)

//Clock disable macro for sys clock
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &=~(1<<14))

//macros to reset GPIOx peripherals
#define GPIOA_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<0)); }while(0)
#define GPIOB_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<1)); }while(0)
#define GPIOC_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<2)); }while(0)
#define GPIOD_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<3)); }while(0)
#define GPIOE_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<4)); }while(0)
#define GPIOF_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<5)); }while(0)
#define GPIOG_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<6)); }while(0)
#define GPIOH_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<7)); }while(0)
#define GPIOI_REG_RESET()			do{(RCC->AHB1RSTR |=(1<<0));	(RCC->AHB1RSTR &=~(1<<8)); }while(0)


//macros to reset the SPIx peripherals
#define SPI1_REG_RESET()			do{(RCC->APB2RSTR |=(1<<0));	(RCC->APB2RSTR &=~(1<<0)); }while(0)
#define SPI2_REG_RESET()			do{(RCC->APB1RSTR |=(1<<0));	(RCC->APB1RSTR &=~(1<<2)); }while(0)
#define SPI3_REG_RESET()			do{(RCC->APB1RSTR |=(1<<0));	(RCC->APB1RSTR &=~(1<<3)); }while(0)



// This methodis called c conditional operator

#define GPIOA_BASEADDR_TO_CODE(x)	((x== GPIOA)? 0:\
									 (x== GPIOB)? 1:\
									 (x== GPIOC)? 2:\
									 (x== GPIOD)? 3:\
									 (x== GPIOE)? 4:\
									 (x== GPIOF)? 5:\
									 (x== GPIOG)? 6:\
									 (x== GPIOH)? 7:\
									 (x== GPIOI)? 8:0)

/*
 *   IRQ number for MCU for GPIO
 */
#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40

/*
 *  		IRQ number definition for MCU for SPI peripheral
 */
#define IRQ_NO_SPI1			35
#define IRQ_NO_SPI2			36
#define IRQ_NO_SPI3			51

/*
 * 			IRQ number definition for I2C
 */
#define	IRQ_NO_I2C1_EV		31
#define	IRQ_NO_I2C1_ER		32
#define	IRQ_NO_I2C2_EV		33
#define	IRQ_NO_I2C2_ER		34
#define	IRQ_NO_I2C3_EV		72
#define	IRQ_NO_I2C3_ER		73


/*
 * mACROS FOR ALL THE POSSIBLITY PRIORITY
 */
#define NVIC_IRQ_PRI0 		0
#define NVIC_IRQ_PRI15 		15


//Some generic macros
#define ENABLE 		1
#define DISABLE 	0
#define SET 		ENABLE
#define RESET 		DISABLE
#define GPIO_PIN_SET SET
#define GPIO_PIN_RESET RESET
#define FLAG_RESET		RESET
#define FLAG_SET		SET

/****************************************************************
 * ***********Bit position definitions of SPI Peripherals********
 ****************************************************************/
//bit position for SPI_CR1 register
#define SPI_CR1_CPHA 			0
#define SPI_CR1_CPOL 			1
#define SPI_CR1_MSTR 			2
#define SPI_CR1_BR				3
#define SPI_CR1_SPE 			6
#define SPI_CR1_LSBFIRST		7
#define SPI_CR1_SSI 			8
#define SPI_CR1_SSM	 			9
#define SPI_CR1_RXONLY 			10
#define SPI_CR1_DFF 			11
#define SPI_CR1_CRCNEXT			12
#define SPI_CR1_CRCEN 			13
#define SPI_CR1_BIDIOE 			14
#define SPI_CR1_BIDIMODE		15

// bit position for SPI_CR2 register
#define SPI_CR2_RXDMAEN			0
#define SPI_CR2_TXDMAEN			1
#define SPI_CR2_SSOE			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE			5
#define SPI_CR2_RXNEIE			6
#define SPI_CR2_TXEIE			7

//bit position for SPI_SR
#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8

//bit position for I2C_CR1
#define I2C_CR1_PE				0
#define I2C_CR1_SMBUS			1
#define I2C_CR1_SMBTYPE			3
#define I2C_CR1_ENARP			4
#define I2C_CR1_ENPEC			5
#define I2C_CR1_ENGC			6
#define I2C_CR1_NOSTRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_POS				11
#define I2C_CR1_PEC				12
#define I2C_CR1_ALERT			13
#define I2C_CR1_SWRST			15

//bit position for I2C_CR2
#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN			8
#define I2C_CR2_ITEVTEN			9
#define I2C_CR2_ITBUFEN			10
#define I2C_CR2_DMAEN			11
#define I2C_CR2_LAST			12

//bit position for I2C_SR1
#define I2C_SR1_SB				0
#define I2C_SR1_ADDR			1
#define I2C_SR1_BTF				2
#define I2C_SR1_ADD10			3
#define I2C_SR1_STOPF			4
#define I2C_SR1_RXNE			6
#define I2C_SR1_TXE				7
#define I2C_SR1_BERR			8
#define I2C_SR1_ARLO			9
#define I2C_SR1_AF				10
#define I2C_SR1_OVR				11
#define I2C_SR1_PECERR			12
#define I2C_SR1_TIMEOUT			14
#define I2C_SR1_SMBALERT		15

//bit position for I2C_SR2
#define I2C_SR2_MSL				0
#define I2C_SR2_BUSY			1
#define I2C_SR2_TRA				2
#define I2C_SR2_GENCALL			4
#define I2C_SR2_SMBDEFAULT		5
#define I2C_SR2_SMBHOST			6
#define I2C_SR2_DUALF			7
#define I2C_SR2_PEC				8

//bit position for I2C_CCR
#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

//bit position definition for USART
#define USART_SR_PE				0
#define USART_SR_FE				1
#define USART_SR_NF				2
#define USART_SR_ORE			3
#define USART_SR_IDLE			4
#define USART_SR_RXNE			5
#define USART_SR_TC				6
#define USART_SR_TXE			7
#define USART_SR_LBD			8
#define USART_SR_CTS			9

#define USART_DR				0

#define USART_DIV_FRACTION		0
#define USART_DIV_MANTISSA		4

#define	USART_CR1_SBK			0
#define	USART_CR1_RWU			1
#define	USART_CR1_RE			2
#define	USART_CR1_TE			3
#define	USART_CR1_IDLEIE		4
#define	USART_CR1_RXNEIE		5
#define	USART_CR1_TCIE			6
#define	USART_CR1_TXEIE			7
#define	USART_CR1_PEIE			8
#define	USART_CR1_PS			9
#define	USART_CR1_PCE			10
#define	USART_CR1_WAKE			11
#define	USART_CR1_M				12
#define	USART_CR1_UE			13
#define	USART_CR1_OVER8			15

#define	USART_CR2_ADD			0
#define	USART_CR2_LBDL			5
#define	USART_CR2_LBDIE			6
#define	USART_CR2_LBCL			8
#define	USART_CR2_CPHA			9
#define	USART_CR2_CPOL			10
#define	USART_CR2_CLKEN			11
#define	USART_CR2_STOP			12
#define	USART_CR2_LENEN			14

#define	USART_CR3_EIE			0
#define	USART_CR3_IREN			1
#define	USART_CR3_IRLP			2
#define	USART_CR3_HDSEL			3
#define	USART_CR3_NACK			4
#define	USART_CR3_SCEN			5
#define	USART_CR3_DMAR			6
#define	USART_CR3_DMAT			7
#define	USART_CR3_RTSE			8
#define	USART_CR3_CTSE			9
#define	USART_CR3_CTSIE			10
#define	USART_CR3_ONEBIT		11

#define	USART_GTPR_PSC			0
#define	USART_GTPR_GT			8


#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_i2c_driver.h"

#endif /* INC_STM32F407XX_H_ */
