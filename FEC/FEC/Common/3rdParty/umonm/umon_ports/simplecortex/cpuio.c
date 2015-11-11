#include "config.h"
#include "led.h"
#include "vcom.h"
#include "cpuio.h"
#include "lpc17xx.h"
#include "lpc_timer.h"
#include "lpc17xx_gpio.h"
#include "genlib.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_rit.h"

unsigned long
target_timer(void)
{
	return(LPC_RIT->RICOUNTER);
}

/* ledSet():
 * These may or may not actually work, depending on how the simple
 * cortex board is being used.  The pins assigned to these four LEDs
 * are also used by other functionality in the system...
 */

void
ledSet(int lednum, int state)
{
	uint32_t bit = 0;

	switch(lednum) {
		case 6:
			if ((LPC_PINCON->PINSEL3 & 0x00000030) == 0)
				bit = (uint32_t)(1 << 18);
			break;
		case 7:
			if ((LPC_PINCON->PINSEL3 & 0x000000c0) == 0)
				bit = (uint32_t)(1 << 19);
			break;
		case 8:
			if ((LPC_PINCON->PINSEL3 & 0x00030000) == 0)
				bit = (uint32_t)(1 << 24);
			break;
		case 9:
			if ((LPC_PINCON->PINSEL3 & 0x000c0000) == 0)
				bit = (uint32_t)(1 << 25);
			break;
		default:
			return;
	}

	/* If we're here and the 'bit' value has not be set to 
	 * something, then there was either a PINSEL conflict or
	 * and invalid led number was used.  In either case,
	 * we DON'T touch the corresponding GPIO.
	 */
	if (bit == 0) {
		printf("LED %d conflict\n",lednum);
	}
	else {
		/* Set inverse GPIO state based on LED drive state */
		if (state)
			GPIO_ClearValue(1,bit);
		else
			GPIO_SetValue(1,bit);
	}
}

void
ledCfg(void)
{
	uint32_t ledbits = 0;

	/* For the LED setup, we assume that all PINSELX bits have been set
	 * so we can more dynamically determine which pins can legally be
	 * established as GPIO-based LED bits.
	 */
	if ((LPC_PINCON->PINSEL3 & 0x00000030) == 0)	/* LED6: P1.18 */
		ledbits |= (uint32_t)(1 << 18);
	if ((LPC_PINCON->PINSEL3 & 0x000000c0) == 0)	/* LED7: P1.19 */
		ledbits |= (uint32_t)(1 << 19);
	if ((LPC_PINCON->PINSEL3 & 0x00030000) == 0)	/* LED8: P1.24 */
		ledbits |= (uint32_t)(1 << 24);
	if ((LPC_PINCON->PINSEL3 & 0x000c0000) == 0)	/* LED9: P1.25 */
		ledbits |= (uint32_t)(1 << 25);
	
	
	/* Configure GPIO for the simplecortex LEDs... */
	GPIO_SetDir(1,ledbits,1);

	/* Blink them all on/off... */
	GPIO_SetValue(1,ledbits);

#if 0
	delayMs(0,500);
	LED6_OFF();
	delayMs(0,500);
	LED7_OFF();
	delayMs(0,500);
	LED8_OFF();
	delayMs(0,500);
	LED9_OFF();
#else
	delayMs(0,500);
	GPIO_ClearValue(1,ledbits);
	delayMs(0,500);
#endif
}

void
initCPUio(void)
{
	/* Enable P1 Ethernet Pins. */
    LPC_PINCON->PINSEL2 = 0x50150105;
    LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0x0000000F) | 0x00000005;

    SystemCoreClockUpdate();

}

#ifdef USR_HEADER_FUNC
void
showRSID(void)
{
	/* Read the reset source ID register to determine
 	 * the reason for the reset:
	 */
	switch(LPC_SC->RSID & 0xf) {
		case 1:
			printf("Power-on");
			break;
		case 2:
			printf("Ext-sig");
			break;
		case 4:
			printf("Watch-dog");
			break;
		case 8:
			printf("Brown-out");
			break;
	}
	printf(" reset\n");
	LPC_SC->RSID = 0;
}

void
postUsrHeaderFunction(int center)
{
	showRSID();
}
#endif

Status
uart_set_divisors(LPC_UART_TypeDef *UARTx, uint32_t baudrate)
{
	Status errorStatus = ERROR;

	uint32_t uClk;
	uint32_t calcBaudrate = 0;
	uint32_t temp = 0;

	uint32_t mulFracDiv, dividerAddFracDiv;
	uint32_t divisor = 0 ;
	uint32_t mulFracDivOptimal = 1;
	uint32_t dividerAddOptimal = 0;
	uint32_t diviserOptimal = 0;

	uint32_t relativeError = 0;
	uint32_t relativeOptimalError = 100000;

	/* get UART block clock */
	if (UARTx == LPC_UART0)
	{
		uClk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_UART0);
	}
	else if (UARTx == (LPC_UART_TypeDef *)LPC_UART1)
	{
		uClk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_UART1);
	}
	else if (UARTx == LPC_UART2)
	{
		uClk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_UART2);
	}
	else if (UARTx == LPC_UART3)
	{
		uClk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_UART3);
	}


	uClk = uClk >> 4; /* div by 16 */
	/* In the Uart IP block, baud rate is calculated using FDR and DLL-DLM 
	 * registers. The formula is :
	 * BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv)/(16*(DLL)
	 * It involves floating point calculations.  That's the reason the
	 * formulae are adjusted with multiply and divide method.  The value of
	 * mulFracDiv and dividerAddFracDiv should comply to the following
	 * expressions:
	 * 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15
	 */
	for (mulFracDiv = 1 ; mulFracDiv <= 15 ;mulFracDiv++)
	{
	for (dividerAddFracDiv = 0 ; dividerAddFracDiv <= 15 ;dividerAddFracDiv++)
	{
	  temp = (mulFracDiv * uClk) / ((mulFracDiv + dividerAddFracDiv));

	  divisor = temp / baudrate;
	  if ((temp % baudrate) > (baudrate / 2))
		divisor++;

	  if (divisor > 2 && divisor < 65536)
	  {
		calcBaudrate = temp / divisor;

		if (calcBaudrate <= baudrate)
		  relativeError = baudrate - calcBaudrate;
		else
		  relativeError = calcBaudrate - baudrate;

		if ((relativeError < relativeOptimalError))
		{
		  mulFracDivOptimal = mulFracDiv ;
		  dividerAddOptimal = dividerAddFracDiv;
		  diviserOptimal = divisor;
		  relativeOptimalError = relativeError;
		  if (relativeError == 0)
			break;
		}
	  } /* End of if */
	} /* end of inner for loop */
	if (relativeError == 0)
	  break;
	} /* end of outer for loop  */

	if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR)/100))
	{
		UARTx->LCR |= UART_LCR_DLAB_EN;
		UARTx->DLM = UART_LOAD_DLM(diviserOptimal);
		UARTx->DLL = UART_LOAD_DLL(diviserOptimal);
		/* Then reset DLAB bit */
		UARTx->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
		UARTx->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) \
				| UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;
		errorStatus = SUCCESS;
	}

	return errorStatus;
}

void
devInit(int baud)
{
#if CONSOLE_IS_VCOM
    InitializeVituralComPort();
#else
	uint8_t tmp = 0;

	/* We're using UART0 with TX/RX on P0.2/P0.3 */
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART0, ENABLE);
	LPC_PINCON->PINSEL0 &= ~0x000000f0;
	LPC_PINCON->PINSEL0 |= 0x00000050;
	CONSOLE_UART->FCR = 0;
	while (CONSOLE_UART->LSR & UART_LSR_RDR)
		tmp = CONSOLE_UART->RBR;
	CONSOLE_UART->TER = UART_TER_TXEN;
	while (!(CONSOLE_UART->LSR & UART_LSR_THRE));
	CONSOLE_UART->TER = 0;
	CONSOLE_UART->IER = 0;
	CONSOLE_UART->LCR = 0;
	CONSOLE_UART->ACR = 0;
	tmp = CONSOLE_UART->LSR;
	tmp = tmp;
	uart_set_divisors(CONSOLE_UART,baud);
	CONSOLE_UART->LCR = (UART_LCR_WLEN8 | UART_STOPBIT_1);
	CONSOLE_UART->TER = UART_TER_TXEN;
#endif

    ledCfg();

    RIT_Init(LPC_RIT);
}

void
delayMs(uint8_t timer_num, uint32_t delayInMs)
{

	LPC_TIM_TypeDef *tmr;
	switch(timer_num) {
		case 0:
			tmr = LPC_TIM0;
			break;
		case 1:
			tmr = LPC_TIM1;
			break;
		case 2:
			tmr = LPC_TIM2;
			break;
		case 3:
			tmr = LPC_TIM3;
			break;
		default:
			return;
	}
	tmr->TCR = 0x02;		/* reset timer */
	tmr->PR  = 0x00;		/* set prescaler to zero */
	tmr->MR0 = (SystemCoreClock / 4) / (1000/delayInMs);  //enter delay time
	tmr->IR  = 0xff;		/* reset all interrrupts */
	tmr->MCR = 0x04;		/* stop timer on match */
	tmr->TCR = 0x01;		/* start timer */
  
	/* wait until delay time has elapsed */
	while (LPC_TIM0->TCR & 0x01);
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
#if CONSOLE_IS_VCOM
	return(-1);
#else
	uart_set_divisors(CONSOLE_UART,baud);
	return(0);
#endif

}

char *
ExceptionType2String(int type)
{
	return("???");
}
