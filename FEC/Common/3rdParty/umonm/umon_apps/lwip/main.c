/* 
 * main.c:
 * This is the code that initializes LWIP and hooks it to uMon's
 * packet interface.  This code is *almost* entirely target
 * independent.  The only code that is cpu/target specific is
 * the code that retrieves the time base.
 */

#include <string.h>
#include <stdlib.h>
#include "monlib.h"
#include "cfg.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/init.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "httpd.h"
#include "monsrvr.h"
#include "umonlwip.h"

unsigned long AppStack[APPSTACKSIZE/4];

extern int console_poll(void);
extern int console_init(void);
int main(int, char **);

// Locations established in linker file for copying flash-based
// data to sram; and for clearing bss...
extern unsigned long _sidata, _sdata, _edata;
extern unsigned long _sbss, _ebss;

int
Cstart(void)
{
    int     argc;
    char    **argv;
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
	 * Wrap the call with the Lwip startup/shutdown calls...
	 */
	mon_appexit(main(argc,argv));

	/* We won't get here. */
	return(0);
}

void
usage(char *arg0)
{
	mon_printf("Usage: %s {command} [command-specific-args]\n",arg0);
	mon_printf(" Commands:\n");
	mon_printf("  * srvrs\n");
	mon_printf("    run HTTP & MONCMD servers\n");
	mon_printf("\n\n");
}

int
main(int argc,char *argv[])
{
	if (argc < 2) {
		usage(argv[0]);
		return(-1);
	}
	if (strcmp(argv[1],"srvrs") == 0) {
		mon_printf("Running HTTP & MONCMD servers...\n");
		lwip_umon_startup();
		httpd_init();
		websocd_init();
		monsrvr_init();
		console_init();
		do {
			lwip_poll();
			websocd_poll();
		} while (console_poll() != -1);
		lwip_umon_shutdown();
	}
	else {
		usage(argv[0]);
	}

	return(0);
}

#include "ctype.h"

unsigned char	ctypetbl[] = {
	 0,		_C,		_C,		_C,		_C,		_C,		_C,		_C,
	_C,		_S|_C,	_S|_C,	_S|_C,	_S|_C,	_S|_C,	_C,		_C,
	_C,		_C,		_C,		_C,		_C,		_C,		_C,		_C,
	_C,		_C,		_C,		_C,		_C,		_C,		_C,		_C,
	_S|_B,	_P,		_P,		_P,		_P,		_P,		_P,		_P,
	_P,		_P,		_P,		_P,		_P,		_P,		_P,		_P,
	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,
	_N|_X,	_N|_X,	_P,		_P,		_P,		_P,		_P,		_P,
	_P,		_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U,
	_U,		_U,		_U,		_U,		_U,		_U,		_U,		_U,
	_U,		_U,		_U,		_U,		_U,		_U,		_U,		_U,
	_U,		_U,		_U,		_P,		_P,		_P,		_P,		_P,
	_P,		_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L,
	_L,		_L,		_L,		_L,		_L,		_L,		_L,		_L,
	_L,		_L,		_L,		_L,		_L,		_L,		_L,		_L,
	_L,		_L,		_L,		_P,		_P,		_P,		_P,		_C,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0,
	 0,		 0,		 0,		 0,		 0,		 0,		 0,		 0
};
