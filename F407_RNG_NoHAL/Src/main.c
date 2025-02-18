/**
 ******************************************************************************
*
*
*
*		Project:	F407_RNG_NoHAL
*
*
*
*
 ******************************************************************************
 */
#define STM32F407xx
#include "stm32f4xx.h"
#include <stdint.h>



int main(void)
{

	uint32_t random_number = 0;			// Generated random number

	RCC->AHB2ENR |= (1 << 6);			// Enable RNG clock


	RCC->CR |= (1 << 24); 				//turn on the PLL


	while((RCC->CR & RCC_CR_PLLRDY) == 0); //wait until PLL is active


	RNG->CR |= (1 << 2);				// Enable the random number generator

	RNG->DR;							// Ignore the first number generated

	while(1)							// When new data is available, grab it.
	{
		while((RNG->SR & RNG_SR_DRDY) == 0);
		random_number = RNG->DR;
	}

}
