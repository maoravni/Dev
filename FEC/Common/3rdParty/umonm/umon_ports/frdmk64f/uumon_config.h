#define MONSTACKSIZE	(1*1024)

/* INCLUDE_XXX Macros:
 * Set/clear the appropriate macros depending on what you want
 * to configure in your system.
 * Note that this is a reasonably "loaded" configuration.  For
 * a real system, several of these facilities are likely to be
 * off...
 */
								
#define INCLUDE_MALLOC			0
#define INCLUDE_MEMCMDS         0
#define INCLUDE_DM              1
#define INCLUDE_PM              0
#define INCLUDE_SHELLVARS		0
#define INCLUDE_XMODEM          0
#define INCLUDE_UNZIP           0
#define INCLUDE_ETHERNET        1
#define INCLUDE_ICMP			INCLUDE_ETHERNET
#define INCLUDE_TFTP            INCLUDE_ETHERNET
#define INCLUDE_FLASH           1
#define INCLUDE_TFS             0
#define INCLUDE_TFSAPI          INCLUDE_TFS
#define INCLUDE_TFSSCRIPT       INCLUDE_TFS
#define INCLUDE_TFSCLI          INCLUDE_TFS
#define INCLUDE_EDIT            INCLUDE_TFS
#define INCLUDE_LINEEDIT        0
#define INCLUDE_DHCPBOOT        0
#define INCLUDE_IPV6			0
#define INCLUDE_EE              0
#define INCLUDE_STOREMAC        0
#define INCLUDE_VERBOSEHELP     0
#define INCLUDE_HWTMR	 	    1
#define INCLUDE_BLINKLED		0

#define TIMER_TICKS_PER_MSEC (20971520/1000)

/* Some fine tuning (if needed)...
 * If these #defines are not in config.h, they default to '1' in
 * various other include files within uMon source; hence, they're
 * really only useful if you need to turn off ('0') the specific
 * facility or block of code.
 */
#define INCLUDE_TFTPSRVR		0
#define INCLUDE_ETHERVERBOSE	0
#define INCLUDE_MONCMD			0
#define INCLUDE_READ			0
#define INCLUDE_PRINT_IPSTUFF	0
#define INCLUDE_PRINT_LONGLONG	0
#define INCLUDE_FLASHLOCK		0
#define INCLUDE_FLASHEWRITE		0
#define INCLUDE_ICMPCMD			0
#define INCLUDE_ETHERCMD		0
#define INCLUDE_LEDCMD			0
#define INCLUDE_VERSIONCMD		0
#define INCLUDE_CALLCMD			1
#define INCLUDE_RESETCMD		1
#define INCLUDE_FANCYDOCOMMAND	0
#define INCLUDE_FLASHENV		1

#if INCLUDE_FLASHENV
#define FLASHENV_SNUM 255
#define FLASHENV_BASE ((char *)0x000ff000)
#define FLASHENV_END  ((char *)0x000fffff)
#endif

#include "inc_check.h"

#define PRE_COMMANDLOOP_HOOK()	flashenv_boot()
