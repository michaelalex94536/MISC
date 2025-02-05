/**
<<<<<<< HEAD
=======
 *
 *	Here we set the system clock to 100 MHz for the Black Pill board, and to confirm it works,
 *	we blink the on-board LED.  No HAL was used here.
 *
 *
>>>>>>> dadf48eadef76ffdf32c8554389f9ea492dda575
 *	At startup, the system clock is 16 MHz.  We want to enable the HSE (running at 25 MHz) and
 *	configure the system clock to maximum, or 100 MHz. The HSE is on pins PH0 and PH1.
 *
 *	From STM32Cube, the PLL settings need to be:
 *	/ M = / 12
 *	* N = X 96
 *	/ P = / 2
 *
 *	and ...
 *
 *	AHB prescaler: 		/ 1
 *	APB1 prescaler: 	/ 2
 *	APB2 prescaler:		/ 1
 *
 *
 *
 * Procedure:
 * 1a) Configure the power interface clock, bit 28 (PWREN) of APB1
 * 1b) Set voltage to Scale 1 for 100 MHz capability by setting both bits 15:14 (VOS) to 1 in the power control register, PWR_CR.
 *
 * 2a) Enable the HSE (Set bit 16 (HSEON) of RCC_CR)
 * 2b) Make sure HSE is ready (Bit 17 (HSERDY) of RCC_CR is set)
 *
 * 3) Set the flash as in CubeMX using the Flash access control register (FLASH_ACR) by enabling
 * 	  Instruction cache, Data cache and Prefetch buffer. Set latency to 3.
 *
 * 4) Configure AHB, APB1, and APB2 prescalers
 *
 * 5) Configure PLL
 *
 * 6) Turn on the PLL and be sure it has locked
 *
 * 7) Select clock source and wait until it is ready our source is the PLL
 ******************************************************************************
 */


#include "stm32f4xx.h"

void ConfigSysClock(void);
void LED_Config(void);
void make_delay(uint32_t delay_count);


int main(void)
{

	ConfigSysClock();

	LED_Config();

    /* Loop forever */
	while(1)
	{
		GPIOC->ODR &= ~(1 << 13);		// Turn the LED on by setting Pin 13 low
		make_delay(1000000);

		GPIOC->ODR |= (1 << 13);		// Turn the LED off by setting Pin 13 high
		make_delay(1000000);
	}
}


void ConfigSysClock(void)
{
	#define PLL_M 	(12 << 0)		// Starts at bit 0 in the RCC PLL configuration register (6 bits)
	#define PLL_N  	(96 << 6)		// Starts at bit 6 in the RCC PLL configuration register (9 bits)
	#define PLL_P  	(0 << 16)		// Starts at bit 16 in the RCC PLL configuration register (2 bits)
	#define PLL_SRC	(1 << 22)		// Use HSE as PLL source
	#define PLL_EN	(1 << 24)		// Enable the PLL

	// 1) Power
	RCC->APB1ENR |= (0x1UL << (28U));					// Enable power interface clock
	PWR->CR |= ((0x1UL << (15U))|(1UL << (14U)));		// Set bits 15 and 14 high (Scale 1 mode)

	// 2) HSE
	RCC->CR |= (0x1UL << (16U));				// Enable HSEON bit (16)
	while(!(RCC->CR & (0x1UL << (17U))));		// Wait until HSERDY bit (17) goes high

	// 3) Configure flash as HAL would have done it:
	FLASH->ACR |= (FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN |FLASH_ACR_LATENCY_3WS);


	// 4) Configure prescaler settings
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;		// AHB
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;		// APB1
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;		// APB2

	// 5) Configure the PLL using HSE as the source
	RCC->PLLCFGR |= (PLL_M | PLL_N | PLL_P | PLL_SRC);

	// 6) Enable the PLL and confirm it has locked before moving on
	RCC->CR |= PLL_EN;
	while(!(RCC->CR & (0x1UL << (25U))));

	// 7) Select clock source as PLL and confirm it
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}

void make_delay(uint32_t delay_count)
{
	uint32_t i;
	for(i = 0; i <= delay_count; i++){}
}

void LED_Config(void)
{
	#define GPIOC_EN		(1 << 2)		// RCC enable bit for Port C on AHB1
	#define GPIOC_MODE		(01 << 26)		// Mode bits of Pin 13 to make it an output

	RCC->AHB1ENR |= GPIOC_EN;				// Enable port C clock on the AHB1 bus
	GPIOC->MODER |= GPIOC_MODE;				// Make Pin 13 of port C an output
	GPIOC->ODR |= (1 << 13);				// Set Pin 13 high to keep LED off
}
