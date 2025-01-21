# MISC

## Projects

### BlackPill_Init 
###### This project just sets up the Black Pill hardware, as seen in the IOC file; there is no code in "main.c" that runs behind it.  The LED is set as a GPIO output on Port C, pin 13.  The user button is set as a GPIO input on Port A, pin 0.  USART1 is configured for 921600 baud on pins PA9 (Tx) and PA10 (Rx), and there is a function in the "main.c" file that enables sending data to UART1 using "printf" statements.  The system clock is set to maximum - 100 MHz - using the high speed external oscillator. 
