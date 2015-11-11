#include <stdint.h>
#include "config.h"
#include "genlib.h"
#if CONSOLE_IS_VCOM
#include "usbapi.h"
#include "usbvcom.h"
#else
#include "lpc17xx_uart.h"
#endif
#include "timer.h"
#include "led.h"
#include "cpuio.h"
#include "lpc17xx.h"

/* target-specific putchar():
 * When buffer has space available, load the incoming character
 * into the UART.
 */
int
target_putchar(char c)
{
#if CONSOLE_IS_VCOM
	while(VCOM_putchar(c) != (int)c);
#else
	while (!(CONSOLE_UART->LSR & UART_LSR_THRE));
	CONSOLE_UART->THR = c;
#endif
	return((int)c);
}

int
target_console_empty(void)
{
#if CONSOLE_IS_VCOM
	return(1);
#else
	return((CONSOLE_UART->LSR & UART_LSR_THRE));
#endif
}

/* target-specific getchar():
 * Assume there is a character in the UART's input buffer and just
 * pull it out and return it.
 */
int
target_getchar(void)
{
#if CONSOLE_IS_VCOM
	return(VCOM_getchar());
#else
	return(CONSOLE_UART->RBR);
#endif
}

int
target_gotachar(void)
{
#if INCLUDE_BLINKLED
	static uint8_t ledstate;
	static struct elapsed_tmr tmr;

#ifndef BLINKON_MSEC
#define BLINKON_MSEC 500
#define BLINKOFF_MSEC 500
#endif

	switch(ledstate) {
		case 0:
			startElapsedTimer(&tmr,BLINKON_MSEC);
			ledstate = 1;
			STATLED_ON();
			break;
		case 1:
			if(msecElapsed(&tmr)) {
				STATLED_OFF();
				ledstate = 2;
				startElapsedTimer(&tmr,BLINKOFF_MSEC);
			}
			break;
		case 2:
			if(msecElapsed(&tmr)) {
				STATLED_ON();
				ledstate = 1;
				startElapsedTimer(&tmr,BLINKON_MSEC);
			}
			break;
	}
#endif
#if CONSOLE_IS_VCOM
	if (VCOM_Available())
        return(1);
#else
	return(CONSOLE_UART->LSR & UART_LSR_RDR);
#endif
    return(0);	
}

void
target_reset(void)
{
	LPC_WDT->WDTC = 0xfff;
	LPC_WDT->WDMOD = 3;	/* WDEN | WDRESET */
	LPC_WDT->WDFEED = 0xaa;
	LPC_WDT->WDFEED = 0x55;
	//intsoff();
	while(1)
		target_gotachar();
}
