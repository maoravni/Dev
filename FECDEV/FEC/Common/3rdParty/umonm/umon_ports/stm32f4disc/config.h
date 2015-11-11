/* Simple-Cortex MicroMonitor configuration file.
 * Homepage: http://www.brc-electronics.nl/
 */
#define CPU_LE
#define SYSCLKFREQ		168000000
#define PLATFORM_NAME	"STM32F4DISCOVERY"
#define CPU_NAME		"STM32F407VGT6"
#define CMDLINESIZE		80	
#define getopt umon_getopt

/* When using an absolute address to access a thumb function 
 * we must add one to the address...
 * (Refer to "pointers to functions in Thumb state" at
 * infocenter.arm.com).
 */
#define ADJUST_THUMB_ADDRESS(a)	(a|1)

#define	DEFAULT_ETHERADD "00:11:22:33:44:55"
#define	DEFAULT_IPADD "192.168.1.100"

/* If the ENET_LINK_IS_UP macro is defined as some function, then
 * that function will be called every half second for some number of
 * ticks.  The default tick count is set by the definition of
 * LINKUP_TICK_MAX also defined in that file.  This can be overridden
 * here if necessary.  Refer to the function EthernetWaitforLinkup() in
 * ethernet.c (umon_main/target/common/ethernet.c) for complete details.
 *
 * The function defined by ENET_LINK_IS_UP (shown below as phy_linkup())
 * is assumed to return 1 if the link is up; else 0. 
 *
#define ENET_LINK_IS_UP EMAC_LinkStatus
#define LINKUP_TICK_MAX 10
 *
 * The purpose of this is to provide a common way to wait for the up-state
 * of the link prior to allowing other commands to execute from uMon at
 * startup.
 */

/* One of four different TFS defrag modes:
 * Only 1 should be enabled; each has its pros/cons...
 *
 * DEFRAGTYPE_PWRSAFE:
 *   Provides powersafe defragmentation using spare sector.
 * DEFRAGTYPE_NPWRSAFE_SPARE:
 *   Not powersafe and uses spare (saves RAM).
 * DEFRAGTYPE_NPWRSAFE_APPRAMBASE:
 *   Not powersafe, assumes there is at least enough ram starting at
 *   APPRAMBASE to copy the largest sector to (saves life of sector,
 *   but assumes the ram will be available when needed).
 * DEFRAGTYPE_NPWRSAFE_RAMBLOCK:
 *   Not powersafe, allocates a static array in RAM that is the size
 *	 of the largest sector.
 */ 
#define DEFRAGTYPE_PWRSAFE				0
#define DEFRAGTYPE_NPWRSAFE_SPARE		0
#define DEFRAGTYPE_NPWRSAFE_APPRAMBASE	1
#define DEFRAGTYPE_NPWRSAFE_RAMBLOCK	0

/* Flash bank configuration:
 * Basic information needed to configure the flash driver.
 * Fill in port specific values here.
 * The flash on this device is occupies 0x08000000 thru 0x080fffff.
 * This flash requires that the functions that operate on it be
 * running out of ram; however, we do NOT use the FLASH_COPY_TO_RAM
 * feature typically used by uMon.  The function relocation is
 * handled by the linker.
 */
#define FLASHBANKS				1
#define FLASH_BANK0_BASE_ADDR  	0x08000000
#define FLASH_PROTECT_RANGE  	"0-4"	
#define FLASH_LARGEST_SECTOR    0x20000

/* For this device, the flash erase operation is done
 * on different widths based on the voltage...
 * Only one of the defins below should be active:
 */
//#define FLASH_PSIZE FLASH_PSIZE_BYTE			// 1.8v-2.1v: byte-wide access
//#define FLASH_PSIZE FLASH_PSIZE_HALF_WORD		// 2.1v-2.7v: half-word-wide access
#define FLASH_PSIZE FLASH_PSIZE_WORD			// 2.7v-3.6v: word-wide access
//#define FLASH_PSIZE FLASH_PSIZE_DOUBLE_WORD	// extern v : double-word-wide access

/* TFS definitions:
 * Values that configure the flash space that is allocated to TFS.
 * Fill in port specific values here.
 * Note that if we're using tfsclean4.c (simpler, not powersafe),
 * and we'er
 */
#if (DEFRAGTYPE_PWRSAFE | DEFRAGTYPE_NPWRSAFE_SPARE)
#define TFSSPARESIZE    		0x20000
#define TFSSTART        		0x080c0000
#define TFSEND          		0x080dffff
#define TFSSPARE        		(TFSEND+1)
#define TFSSECTORCOUNT			1	/* Does not include SPARE */
#else
#define TFSSPARESIZE    		0
#define TFSSTART        		0x080c0000
#define TFSEND          		0x080fffff
#define TFSSPARE        		0
#define TFSSECTORCOUNT			2
#endif

#define ALLOCSIZE 		(4*1024)
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
#define INCLUDE_EDIT            1
#define INCLUDE_UNZIP           0
#define INCLUDE_ETHERNET        1
#define INCLUDE_ICMP			INCLUDE_ETHERNET
#define INCLUDE_TFTP            INCLUDE_ETHERNET
#define INCLUDE_FLASH           1
#define INCLUDE_TFS             1
#define INCLUDE_TFSAPI          1
#define INCLUDE_TFSSCRIPT       1
#define INCLUDE_TFSCLI          INCLUDE_TFS
#define INCLUDE_LINEEDIT        1
#define INCLUDE_DHCPBOOT        INCLUDE_ETHERNET
#define INCLUDE_IPV6			1
#define INCLUDE_EE              0
#define INCLUDE_STOREMAC        0
#define INCLUDE_VERBOSEHELP     0
#define INCLUDE_HWTMR	 	    1
#define INCLUDE_BLINKLED		0

#define TIMER_TICKS_PER_MSEC (SYSCLKFREQ/(1000*2))

//#define LOOPS_PER_SECOND		100000

/* Some fine tuning (if needed)...
 * If these #defines are not in config.h, they default to '1' in
 * various other include files within uMon source; hence, they're
 * really only useful if you need to turn off ('0') the specific
 * facility or block of code.
 */
#define INCLUDE_TFTPSRVR		INCLUDE_ETHERNET
#define INCLUDE_ETHERVERBOSE	1
#define INCLUDE_MONCMD			INCLUDE_ETHERNET
#define INCLUDE_READ			0

#include "inc_check.h"
