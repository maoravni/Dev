#define USR_HEADER_FUNC	reset_message

#define ENET_LINK_IS_UP linkpoll
#define LINKUP_TICK_MAX 10

#define ALLOCSIZE 		(64*1024)	// Large to support lua demo.
#define MONSTACKSIZE	(4*1024)

/* INCLUDE_XXX Macros:
 * Set/clear the appropriate macros depending on what you want
 * to configure in your system.
 * Note that this is a reasonably "loaded" configuration.  For
 * a real system, several of these facilities are likely to be
 * off...
 */
								
#define INCLUDE_MALLOC			1
#define INCLUDE_MEMCMDS         0
#define INCLUDE_DM              1
#define INCLUDE_PM              1
#define INCLUDE_SHELLVARS		1
#define INCLUDE_XMODEM          0
#define INCLUDE_UNZIP           0
#define INCLUDE_ETHERNET        1
#define INCLUDE_ICMP			INCLUDE_ETHERNET
#define INCLUDE_TFTP            INCLUDE_ETHERNET
#define INCLUDE_FLASH           1
#define INCLUDE_TFS             1
#define INCLUDE_TFSAPI          INCLUDE_TFS
#define INCLUDE_TFSSCRIPT       INCLUDE_TFS
#define INCLUDE_TFSCLI          INCLUDE_TFS
#define INCLUDE_EDIT            INCLUDE_TFS
#define INCLUDE_LINEEDIT        1
#define INCLUDE_DHCPBOOT        INCLUDE_ETHERNET
#define INCLUDE_IPV6			0
#define INCLUDE_EE              0
#define INCLUDE_STOREMAC        0
#define INCLUDE_VERBOSEHELP     0
#define INCLUDE_HWTMR	 	    1
#define INCLUDE_BLINKLED		0
#define INCLUDE_LEDCMD			1

#define TIMER_TICKS_PER_MSEC (20971520/1000)

/* Some fine tuning (if needed)...
 * If these #defines are not in config.h, they default to '1' in
 * various other include files within uMon source; hence, they're
 * really only useful if you need to turn off ('0') the specific
 * facility or block of code.
 */
#define INCLUDE_TFTPSRVR		INCLUDE_ETHERNET
#define INCLUDE_ETHERVERBOSE	1
#define INCLUDE_MONCMD			INCLUDE_ETHERNET
#define INCLUDE_READ			1

#include "inc_check.h"
