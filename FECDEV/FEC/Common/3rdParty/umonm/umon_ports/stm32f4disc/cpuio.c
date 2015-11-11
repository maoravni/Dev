#include "config.h"
#include "cpuio.h"
#include "stddefs.h"
#include "genlib.h"
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

unsigned long
target_timer(void)
{
	return(TIM2->CNT);
}

void
initCPUio(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOC & TIM2 clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  TIM2->CR1 = 1;
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

#ifdef VECT_TAB_SRAM  
__attribute__ ((used,section(".ram_vtable")))
void (*ramVectors[98])(void);
#endif

void
vinit()
{
#ifdef VECT_TAB_SRAM  
	extern int SizeOfRomVtable;
	extern void (* g_pfnVectors[])(void);
	extern void Default_Handler(void);

	int i;

	for(i=0;i<98;i++) {
		if (i < SizeOfRomVtable)
			ramVectors[i] = g_pfnVectors[i];
		else
			ramVectors[i] = Default_Handler;
	}
#endif
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
	/* USARTx configured as follow:
        - BaudRate = 'baud'
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
	*/
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(USART6, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(USART6, ENABLE);
	return(0);

}

void
devInit(int baud)
{
	ConsoleBaudSet(baud);
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
	while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	USART_SendData(USART6,(uint8_t)c);
	return((int)c);
}

int
target_console_empty(void)
{
	if (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == SET);
		return(0);
	return(1);
}

/* target-specific getchar():
 * Assume there is a character in the UART's input buffer and just
 * pull it out and return it.
 */
int
target_getchar(void)
{
	return(USART_ReceiveData(USART6)); 
}

int
target_gotachar(void)
{
	if (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET)
		return(1);
	return(0);
}

void
target_reset(void)
{
	IWDG->KR = 0xcccc;
	while(1);
}
