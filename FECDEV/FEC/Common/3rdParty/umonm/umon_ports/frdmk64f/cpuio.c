#include "config.h"
#include "cpuio.h"
#include "stddefs.h"
#include "timer.h"
#include "cli.h"
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fsl_debug_uart.h"
#include "fsl_uart_hal.h"
#include "gpio_pins.h"
#include "device/MK64F12/MK64F12.h"
#include "device/MK64F12/MK64F12_wdog.h"
#include "device/fsl_device_registers.h"
#include "hal/pit/fsl_pit_hal.h"
#include "fsl_clock_manager.h"

#define PIT_TIMER	0

extern void ledSetup(void);

static uint32_t busClock;

int ExceptionType;
unsigned long ExceptionAddr;
unsigned long LoopsPerMillisecond;

unsigned long
target_timer(void)
{
	return(~pit_hal_read_timer_count(PIT_TIMER));
}

void
pitSetup(void)
{
    /* Un-gate port clock*/
    clock_manager_set_gate(kClockModulePIT, 0, true);

	pit_hal_enable();
	pit_hal_configure_timer_run_in_debug(1);
	pit_hal_set_timer_period_count(PIT_TIMER, 0xffffffff);
	pit_hal_configure_timer_chain(PIT_TIMER, 0);
	pit_hal_timer_start(PIT_TIMER);
}

void
initCPUio(void)
{
	extern void hardware_init(void);

	hardware_init();
	clock_manager_get_frequency(kBusClock, &busClock);
#if INCLUDE_LEDCMD
	ledSetup();
#endif
	pitSetup();
	pin_mux_UART(BOARD_DEBUG_UART_INSTANCE);

	/* Disable the MPU for now: */
	BW_MPU_CESR_VLD(0);
}


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
    __ASM volatile ("cpsid i" : : : "memory");
	return(0);
}

void
cacheInitForTarget()
{
	FMC_PFB0CR &= ~(FMC_PFB0CR_B0SEBE_MASK | FMC_PFB0CR_B0IPE_MASK | FMC_PFB0CR_B0DPE_MASK | FMC_PFB0CR_B0ICE_MASK | FMC_PFB0CR_B0DCE_MASK);
	FMC_PFB1CR &= ~(FMC_PFB1CR_B1SEBE_MASK | FMC_PFB1CR_B1IPE_MASK | FMC_PFB1CR_B1DPE_MASK | FMC_PFB1CR_B1ICE_MASK | FMC_PFB1CR_B1DCE_MASK);
}

int
ConsoleBaudSet(int baud)
{
#ifdef USE_UART_HAL
    debug_uart_init(BOARD_DEBUG_UART_INSTANCE, baud);
#else
    uint32_t freq;
    uint16_t sbr, brfa;
    clock_names_t sourceClock;

    clock_manager_set_gate(kClockModuleUART, BOARD_DEBUG_UART_INSTANCE, true);

	HW_UART_C2_CLR(BOARD_DEBUG_UART_INSTANCE, BM_UART_C2_TE);
	HW_UART_C2_CLR(BOARD_DEBUG_UART_INSTANCE, BM_UART_C2_RE);

	sourceClock = (BOARD_DEBUG_UART_INSTANCE < 2) ? kSystemClock : kBusClock;
	clock_manager_get_frequency(sourceClock, &freq);
	sbr = freq / (baud * 16);

    /* write the sbr value to the BDH and BDL registers*/
    BW_UART_BDH_SBR(BOARD_DEBUG_UART_INSTANCE, (uint8_t)(sbr >> 8));
    BW_UART_BDL_SBR(BOARD_DEBUG_UART_INSTANCE, (uint8_t)sbr);

    /* determine if a fractional divider is needed to fine tune closer to the desired baud
     * each value of brfa is in 1/32 increments, hence the multiply-by-32
     */
	brfa = (32*freq/(baud*16)) - 32*sbr;

    /* write the brfa value to the register*/
    BW_UART_C4_BRFA(BOARD_DEBUG_UART_INSTANCE, brfa);

	BW_UART_C1_M(BOARD_DEBUG_UART_INSTANCE, 8);					/* Bits/char: 8 */
	HW_UART_C1_CLR(BOARD_DEBUG_UART_INSTANCE, BM_UART_C1_PE);	/* Parity: none */
	HW_UART_C1_CLR(BOARD_DEBUG_UART_INSTANCE, BM_UART_C1_PE);	/* Parity disabled */
	BW_UART_BDH_SBNS(BOARD_DEBUG_UART_INSTANCE, 1);				/* Stop bit: 1 */

	BW_UART_S2_RXINV(BOARD_DEBUG_UART_INSTANCE, 0);
	BW_UART_C3_TXINV(BOARD_DEBUG_UART_INSTANCE, 0);

    /* finally, enable the UART transmitter and receiver*/
	HW_UART_S1_RD(BOARD_DEBUG_UART_INSTANCE);
	HW_UART_C2_SET(BOARD_DEBUG_UART_INSTANCE, BM_UART_C2_TE);
	HW_UART_C2_SET(BOARD_DEBUG_UART_INSTANCE, BM_UART_C2_RE);
#endif
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
#ifdef USE_UART_HAL
	while(!uart_hal_is_transmit_data_register_empty(BOARD_DEBUG_UART_INSTANCE));
	uart_hal_putchar(BOARD_DEBUG_UART_INSTANCE, (uint8_t)c);
#else
	while(!HW_UART_S1(BOARD_DEBUG_UART_INSTANCE).B.TDRE);
	HW_UART_S1_RD(BOARD_DEBUG_UART_INSTANCE);
	HW_UART_D_WR(BOARD_DEBUG_UART_INSTANCE, c);
#endif
	return((int)c);
}

int
target_console_empty(void)
{
#ifdef USE_UART_HAL
	return(uart_hal_is_transmit_data_register_empty(BOARD_DEBUG_UART_INSTANCE));
#else
	return(HW_UART_S1(BOARD_DEBUG_UART_INSTANCE).B.TDRE);
#endif
}

/* target-specific getchar():
 * Assume there is a character in the UART's input buffer and just
 * pull it out and return it.
 */
int
target_getchar(void)
{
	uint8_t onechar;
#ifdef USE_UART_HAL
	uart_hal_getchar(BOARD_DEBUG_UART_INSTANCE, &onechar);
#else
	HW_UART_S1_RD(BOARD_DEBUG_UART_INSTANCE);
	onechar = HW_UART_D_RD(BOARD_DEBUG_UART_INSTANCE);
#endif
	return((int)onechar);
}

int
target_gotachar(void)
{
#ifdef USE_UART_HAL
	return(uart_hal_is_receive_data_register_full(BOARD_DEBUG_UART_INSTANCE));
#else
	return(HW_UART_S1(BOARD_DEBUG_UART_INSTANCE).B.RDRF);
#endif
}


void
target_reset(void)
{
    BW_WDOG_UNLOCK_WDOGUNLOCK(0xC520U);
    BW_WDOG_UNLOCK_WDOGUNLOCK(0xD928U);
    BW_WDOG_TOVALH_TOVALHIGH(0);
    BW_WDOG_TOVALL_TOVALLOW(0x100);
	HW_WDOG_STCTRLH_WR(0x1d3 | BM_WDOG_STCTRLH_WDOGEN);
	while(1);
}

#ifdef USR_HEADER_FUNC
void
reset_message(void)
{
    if (RCM->SRS1 & RCM_SRS1_SW_MASK)
        printf("Software Reset\n");
    if (RCM->SRS1 & RCM_SRS1_LOCKUP_MASK)
        printf("Core Lockup Event Reset\n");
    if (RCM->SRS1 & RCM_SRS1_JTAG_MASK)
        printf("JTAG Reset\n");
    if (RCM->SRS0 & RCM_SRS0_POR_MASK)
        printf("Power-on Reset\n");
    if (RCM->SRS0 & RCM_SRS0_PIN_MASK)
        printf("External Pin Reset\n");
    if (RCM->SRS0 & RCM_SRS0_WDOG_MASK)
        printf("Watchdog(COP) Reset\n");
    if (RCM->SRS0 & RCM_SRS0_LOC_MASK)
        printf("Loss of Clock Reset\n");
    if (RCM->SRS0 & RCM_SRS0_LVD_MASK)
        printf("Low-voltage Detect Reset\n");
    if (RCM->SRS0 & RCM_SRS0_WAKEUP_MASK)
        printf("LLWU Reset\n");
}
#endif

/* STUBS:
 * Something in the build is pulling these in...
 * Need to figure out why.
 */
void _exit() {}
void _sbrk() {}
void _kill() {}
void _getpid() {}
void _close() {}
void _fstat() {}
void _isatty() {}
void _lseek() {}
#ifndef USE_UART_HAL
void _read() {}
void _write() {}
#endif
