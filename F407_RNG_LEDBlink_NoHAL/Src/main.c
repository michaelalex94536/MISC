/**
 ******************************************************************************
*
*		Project:	F407_RNG_LEDBlink_NoHAL
*
*		Use the orange LED on Port D, Pin 13 and the blue LED on Port D, Pin 15
*		Turn the LED on and off for a duration based on the random number generator.
*
*		Need to experimentally determine how much to reduce the delay,
*		as the RNG produces a 32-bit number.
*
 ******************************************************************************
 */
#define STM32F407xx
#include "stm32f4xx.h"
#include <stdint.h>

#define GPIOD_EN		(1 << 3)		     	// RCC enable bit for Port D on AHB1
#define GPIOD_MODE		((01 << 26)|(01 << 30))	// Mode bits of Pins 13 and 15 to make them outputs

#define RNGCLK_EN		(1 << 6)				// Enable the RNG clock
#define PLL_EN			(1 << 24)				// PLL enable
#define	RNG_EN			(1 << 2)				// Enable the RNG

void make_delay(uint32_t delay_count);

int main(void)
{

	uint32_t random_number[] = {0, 0, 0, 0};	// Generated random number

	// Set up the LED pins
	RCC->AHB1ENR |= GPIOD_EN;				// Enable port D clock on the AHB1 bus
	GPIOD->MODER |= GPIOD_MODE;				// Make Pin 13 of port D an output
	GPIOD->ODR &= ~(1 << 13);				// Set Pin 13 low to keep LED off
	GPIOD->ODR &= ~(1 << 15);				// Set Pin 15 low to keep LED off

	// Set up the RNG
	RCC->AHB2ENR |= RNGCLK_EN;				// Enable RNG clock
	RCC->CR |= PLL_EN; 						// Turn on the PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0); 	// Wait until PLL is active
	RNG->CR |= RNG_EN;						// Enable the random number generator
	RNG->DR;								// Ignore the first number generated

	while(1)								// When new data is available, grab it.
	{
		for(uint8_t i = 0; i < 4; i++)		// Generate two numbers
		{
			while((RNG->SR & RNG_SR_DRDY) == 0);
			random_number[i] = RNG->DR;
		}

		GPIOD->ODR |= (1 << 13);				// Turn the LED on by setting Pin 13 high
		make_delay(random_number[0] >> 14);		// Reduce the delay

		GPIOD->ODR &= ~(1 << 13);				// Turn the LED off by setting Pin 13 low
		make_delay(random_number[1] >> 14);		// Reduce the delay

		GPIOD->ODR |= (1 << 15);				// Turn the LED on by setting Pin 15 high
		make_delay(random_number[2] >> 14);		// Reduce the delay

		GPIOD->ODR &= ~(1 << 15);				// Turn the LED off by setting Pin 15 low
		make_delay(random_number[3] >> 14);		// Reduce the delay
	}

}

void make_delay(uint32_t delay_count)
{
	uint32_t i;
	for(i = 0; i <= delay_count; i++){}
}
