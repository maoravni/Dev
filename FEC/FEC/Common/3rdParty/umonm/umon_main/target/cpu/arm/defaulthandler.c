/* 
 * defaulthandler.c:
 * This code is a mix of CooCox, uMon and 
 * http://helgaenharrie.dyndns.org/simplecortex suggestions.
 * The goal is to have ONE snippet of code for default and
 * hardfault handlers regardless of the CPU.
 */
#include "config.h"
#include "genlib.h"
#include "warmstart.h"
#include "device/MK64F12/MK64F12.h"
#include "core_cm4.h"
#include "core_cmFunc.h"

#ifndef VERBOSE_HARDFAULT
#define VERBOSE_HARDFAULT 0
#endif

#ifndef USE_SYNTAX_UNIFIED
#define USE_SYNTAX_UNIFIED 0
#endif

/**
  * @brief  This is the code that gets called when the processor receives an 
  *         unexpected interrupt.  This simply enters an infinite loop, 
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None  
  */
void Default_Handler(void)
{
    __asm(
#if USE_SYNTAX_UNIFIED
    ".syntax unified\n"
#endif
        "MOVS   R0, #4  \n"
        "MOV    R1, LR  \n"
        "TST    R0, R1  \n"
        "BEQ    _DefMSP    \n"
        "MRS    R0, PSP \n"
        "B      Default_HandlerC      \n"
    "_DefMSP:  \n"
        "MRS    R0, MSP \n"
        "B      Default_HandlerC      \n"
#if USE_SYNTAX_UNIFIED
    ".syntax divided\n"
#endif
    ) ;
}

void Default_HandlerC(unsigned long *hardfault_args)
{
    printf("EXCEPTION @ 0x%08x, VecNo: %d\n",hardfault_args[6],__get_IPSR() & 0xff);
    ExceptionType = 0;
    ExceptionAddr = hardfault_args[6];
    monrestart(EXCEPTION);
}

__attribute__((naked)) void				// 'naked' attribute disables 'c' stacking
HardFault_Handler(void)
{
	/* At this point, R0-R3,R12,LR,PC & PSR are automatically
	 * pushed to the stack (if floating point is enabled, then
	 * S0-S15,FPSCR follow that).
	 * LR (after having been pushed onto the stack) is automatically
	 * loaded with an EXC_RETURN value where all but the lower 5 bits are
	 * set to 1...
	 * We test bit 2 (0x04): if set, exception return uses PSP stack; else MSP.
	 * We could also test bit 4...
	 *   if set, exception return uses floating point state on stack
	 *   (but we don't here).
	 *
	 * Refer to section 2.3.7 "Exception entry and return" of PM0214
	 * "STM32F3xxx and STM32F4xxx Cortex-M4 programming
	 * manual" from ST.
	 */
	__asm(
#if USE_SYNTAX_UNIFIED
	".syntax unified\n"
#endif
		"MOVS   R0, #4  \n"
		"MOV    R1, LR  \n"
		"TST    R0, R1  \n"
		"BEQ    _MSP    \n"
		"MRS    R0, PSP \n"
		"B      HardFault_HandlerC      \n"
	"_MSP:  \n"
		"MRS    R0, MSP \n"
		"B      HardFault_HandlerC      \n"
#if USE_SYNTAX_UNIFIED
	".syntax divided\n"
#endif
	) ;
}


/**
 * HardFaultHandler_C:
 */
void
HardFault_HandlerC(unsigned long *hardfault_args)
{
    printf("HARD_FAULT @ 0x%08x, VecNo: %d\n",hardfault_args[6],__get_IPSR() & 0xff);
#if VERBOSE_HARDFAULT
	printf ("R0-3: %08x %08x %08x %08x\n",
		((unsigned long) hardfault_args[0]), ((unsigned long) hardfault_args[1]),
		((unsigned long) hardfault_args[2]), ((unsigned long) hardfault_args[3]));
	printf ("LR:   %08x\n", ((unsigned long) hardfault_args[5]));
	printf ("R12:  %08x\n", ((unsigned long) hardfault_args[4]));
	printf ("PSR:  %08x\n", ((unsigned long) hardfault_args[7]));
#endif

    ExceptionType = 0;
    ExceptionAddr = hardfault_args[6];
    monrestart(EXCEPTION);
}
