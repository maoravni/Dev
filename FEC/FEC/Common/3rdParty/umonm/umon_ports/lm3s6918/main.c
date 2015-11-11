#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"

int main(void)
{
    char cThisChar;

    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    //
    // Enable the peripherals used by this example.
    // The UART itself needs to be enabled, as well as the GPIO port
    // containing the pins that will be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

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

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    //
    // Put a character to show start of example.  This will display on the
    // terminal.
    //
    while(1) {
    	int i;
		for(i=0;i<10;i++)
			UARTCharPut(UART0_BASE, '!');

		//
		// Enter a loop to read characters from the UART, and write them back
		// (echo).  When a line end is received, the loop terminates.
		//
		do
		{
			//
			// Read a character using the blocking read function.  This function
			// will not return until a character is available.
			//
			cThisChar = UARTCharGet(UART0_BASE);

			//
			// Write the same character using the blocking write function.  This
			// function will not return until there was space in the FIFO and
			// the character is written.
			//
			UARTCharPut(UART0_BASE, cThisChar);

		//
		// Stay in the loop until either a CR or LF is received.
		//
		} while((cThisChar != '\n') && (cThisChar != '\r'));

		//
		// Put a character to show the end of the example.  This will display on
		// the terminal.
		//
		UARTCharPut(UART0_BASE, '\r');
		UARTCharPut(UART0_BASE, '\n');
    }

    //
    // Return no errors
    //
    return(0);
}

void
SysCtlDelay(unsigned long delay)
{
	volatile unsigned long i;

	for(i=0;i<delay;i++);
}
