/*
 * main.c
 *
 *  Created on: 13 Nov 2022
 *      Author: ViralPatel
 */

#include "stm32f407xx.h"

int main(void)
{
	return 0;
}


void EXTI0_IRQHandler(void)
{
	//handle the interrupt
	GPIO_IRQHandling(0);
}
