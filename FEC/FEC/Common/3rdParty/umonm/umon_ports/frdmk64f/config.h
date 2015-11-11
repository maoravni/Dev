/* K64F FRDM MicroMonitor configuration file.
 */
#define CPU_LE
#define SYSCLKFREQ		120000000
#define PLATFORM_NAME	"FRDM-K64F"
#define CPU_NAME		"MK64FN1M0VMD12"
#define CMDLINESIZE		80	
#define getopt umon_getopt

/* When using an absolute address to access a thumb function 
 * we must add one to the address...
 * (Refer to "pointers to functions in Thumb state" at
 * infocenter.arm.com).
 */
#define ADJUST_THUMB_ADDRESS(a)	(a|1)

#define	DEFAULT_ETHERADD "00:00:00:00:00:00"
#define	DEFAULT_IPADD "192.168.1.100"

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
 * The FLASH_PROTECT_RANGE should be set to cover the sectors
 * used by boot.bin burned into flash.
 * You can figure this out by  running
 */
#define PSECTOR_SIZE			0x00001000
#define PBLOCK_SIZE				0x00100000
#define FLASHBANKS				1
#define FLASH_BANK0_BASE_ADDR  	0x00000000
#if BUILD_FOR_UUMON
#define FLASH_PROTECT_RANGE  	"0-15"
#elif BUILD_FOR_UMONM
#define FLASH_PROTECT_RANGE  	"0-31"
#endif
#define FLASH_LARGEST_SECTOR    PSECTOR_SIZE

/* TFS definitions:
 * Values that configure the flash space that is allocated to TFS.
 * Fill in port specific values here.
 * Note that if we're using tfsclean4.c (simpler, not powersafe),
 * and we'er
 */
#if (DEFRAGTYPE_PWRSAFE | DEFRAGTYPE_NPWRSAFE_SPARE)
#define TFSSPARESIZE    		PSECTOR_SIZE
#define TFSSTART        		0x000f0000
#define TFSEND          		0x000fefff
#define TFSSPARE        		(TFSEND+1)
#define TFSSECTORCOUNT			15	// does not include SPARE
#else
#define TFSSTART        		0x000f0000
#define TFSEND          		0x000fffff
#define TFSSECTORCOUNT			16
#define TFSSPARE        		0	// not used
#define TFSSPARESIZE    		0	// not used
#endif

/* The make file should specify how to build this...
 */
#if BUILD_FOR_UUMON
#include "uumon_config.h"
#elif BUILD_FOR_UMONM
#include "umonm_config.h"
#else
#error: Specify BUILDTYPE in makefile
#endif
