#include "config.h"
#include "cpuio.h"
#include "stddefs.h"
#include "genlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"

#define CONSOLE_UART	UART0_BASE
#define TIMER_BASE		TIMER0_BASE

unsigned long
target_timer(void)
{
	return(~*(unsigned long *)(TIMER_BASE + 0x48));
}

void
SysCtlDelay(unsigned long dly)
{
	volatile int i;
	for(i=0;i<dly;i++);
}

void
SystemInit(void)
{
    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);
}

void
initCPUio(void)
{
    //
    // Enable the peripherals used by this example.
    // The UART itself needs to be enabled, as well as the GPIO port
    // containing the pins that will be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Set up TIMER0 to be used for uMon's HW-Timer...
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	*(unsigned long *)(TIMER_BASE + 0) &= ~7;	// 32-bit timer config
	*(unsigned long *)(TIMER_BASE + 0x0c) |= 1;	// Enable the timer

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void
devInit(int baud)
{
    //
    // Configure the UART for 'baud' 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), baud,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
}

void
delayMs(uint8_t timer_num, uint32_t delayInMs)
{
	return;
}

/* Stubs needed for building uMon standard:
 */
int ExceptionType;
unsigned long ExceptionAddr;

void
vinit()
{
}

void
intsrestore(unsigned long val)
{
}

unsigned long
intsoff(void)
{
	return(0);
}

void
cacheInitForTarget()
{
}

int
ConsoleBaudSet(int baud)
{
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), baud,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
	return(0);

}

char *
ExceptionType2String(int type)
{
	return("???");
}


/* target-specific putchar():
 * When buffer has space available, load the incoming character
 * into the UART.
 */
int
target_putchar(char c)
{
	UARTCharPut(CONSOLE_UART,c);
	return((int)c);
}

int
target_console_empty(void)
{
	return(UARTBusy(CONSOLE_UART));
}

/* target-specific getchar():
 * Assume there is a character in the UART's input buffer and just
 * pull it out and return it.
 */
int
target_getchar(void)
{
	return(UARTCharGet(CONSOLE_UART));
}

int
target_gotachar(void)
{
	return(!(HWREG(CONSOLE_UART + UART_O_FR) & UART_FR_RXFE));
}

void
target_reset(void)
{
	SysCtlReset();
}
