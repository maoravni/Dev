/* 
 * This file is a simple example of an application that could be run
 * on top of the monitor.
 *
 * Cstart():
 * The Cstart() function depends on the setting of MONCOMPTR in config.h.
 * It demonstrates the use of monConnect and the first mon_XXX function
 * typically called by an application, mon_getargv().
 *
 * main():
 * The main() function demonstrates argument processing (thanks to
 * the call to mon_getargv() in start()), environment variables and
 * a simple use of TFS to dump the content of an ASCII file.
 * Also, if the first argument is "strace_demo", then the strace_demo()
 * function is called.  Refer to strace.c for details.
 */

#include <string.h>
#include <stdlib.h>
#include "monlib.h"
#include "demo_cfg.h"

#ifdef APPSTACKSIZE
unsigned long AppStack[APPSTACKSIZE/4];
#endif

/* Global variables added just to be able to use this
 * app to demonstrate hookup with gdb...
 */
int argtot;
long apprambase = 0x400;


int
main(int argc,char *argv[])
{
	int i;
	for(i=0;i<argc;i++)
		mon_printf("argv[%d]: '%s'\n",i,argv[i]);
	mon_printf("Hello world! 0x%lx (@0x%lx)\n",apprambase,(long)&apprambase);
	apprambase += 16;
	mon_printf("Modified: 0x%lx (@0x%lx)\n",apprambase,(long)&apprambase);
	return(0);
}

// Locations established in linker file for copying flash-based
// data to sram; and for clearing bss...
extern unsigned long _sidata, _sdata, _edata;
extern unsigned long _sbss, _ebss;

int
Cstart(void)
{
	int		argc;
	char	**argv;
	unsigned long *pulSrc, *pulDest;

	/* Copy the data segment initializers from flash to SRAM */
	pulSrc = &_sidata;
	for(pulDest = &_sdata; pulDest < &_edata; )
		*(pulDest++) = *(pulSrc++);

	/* Zero fill the bss segment.  This is done with inline assembly
	 * since this will clear the value of pulDest if it is not kept
	 * in a register.
	 */
  __asm("  ldr     r0, =_sbss\n"
        "  ldr     r1, =_ebss\n"
        "  mov     r2, #0\n"
        "  .thumb_func\n"
        "zero_loop:\n"
        "    cmp     r0, r1\n"
        "    it      lt\n"
        "    strlt   r2, [r0], #4\n"
        "    blt     zero_loop");

  
	/* Connect the application to the monitor.  This must be done
	 * prior to the application making any other attempts to use the
	 * "mon_" functions provided by the monitor.
	 */
	monConnect((int(*)())(*(unsigned long *)MONCOMPTR),(void *)0,(void *)0);

	/* When the monitor starts up an application, it stores the argument
	 * list internally.  The call to mon_getargv() retrieves the arg list
	 * for use by this application...
	 */
	mon_getargv(&argc,&argv);

	/* Call main, then exit to monitor.
	 */
	main(argc,argv);
	
#ifdef APPSTACKSIZE
	mon_appexit(0);
#endif

	/* Won't get here if mon_appexit() is called */
	return(0);
}
