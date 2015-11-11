/* lpc17xx_flash.c:
 * Support for LPC17XX internal flash.
 *
 * This code was built to fit into the Micromonitor framework based
 * on the discussion of chapter 32 of the LPC17XX user manual (UM10360).
 *
 * Specfically, it deals with the IAP commands
 * (IAP = in-application-programming).
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "system_LPC17xx.h"

#define DEBUG_PRINT 0

#if DEBUG_PRINT
#define DBGPRINT(a)	printf a 
#else
#define DBGPRINT(a)
#endif

#if LPC17XX_512
#define SECTORTOT	30
#elif LPC17XX_256
#define SECTORTOT	22
#elif LPC17XX_128
#define SECTORTOT	18
#elif LPC17XX_064
#define SECTORTOT	16
#elif LPC17XX_032
#define SECTORTOT	8
#else
#error: Flash type not set
#endif

#define IAP_LOCATION	0x1fff1ff1
typedef void (*IAP)(unsigned long *, unsigned long *);

IAP iap_entry;

static unsigned cpuclk_khz;
static unsigned long iap_cmd[5];
static unsigned long iap_rslt[5];

#define IAPSTAT_SUCCESS					0
#define IAPSTAT_INVALID_COMMAND			1
#define IAPSTAT_SRC_ADDR_ERROR			2
#define IAPSTAT_DST_ADDR_ERROR			3
#define IAPSTAT_SRC_ADDR_NOT_MAPPED		4
#define IAPSTAT_DST_ADDR_NOT_MAPPED		5
#define IAPSTAT_COUNT_ERROR				6
#define IAPSTAT_INVALID_SECTOR			7
#define IAPSTAT_SECTOR_NOT_BLANK		8
#define IAPSTAT_SECTOR_NOT_PREPARED		9
#define IAPSTAT_COMPARE_ERROR			10
#define IAPSTAT_BUSY					11

#define IAPCMD_PREP4WRITE			50
#define IAPCMD_CPYRAM2FLASH			51
#define IAPCMD_ERASESECTOR			52
#define IAPCMD_BLANKCHECKSECTOR		53
#define IAPCMD_READPARTID			54
#define IAPCMD_READBOOTCODEVSN		55
#define IAPCMD_COMPARE				56
#define IAPCMD_REINVOKEISP			57
#define IAPCMD_READSERIALNO			58

static int
prepare4write(int s1, int s2)
{
	iap_cmd[0] = IAPCMD_PREP4WRITE;
	iap_cmd[1] = s1;
	iap_cmd[2] = s2;
	iap_entry(iap_cmd,iap_rslt);
	if (iap_rslt[0] != IAPSTAT_SUCCESS) {
		DBGPRINT(("prepare4write failed\n"));
		return(-1);
	}
	return(0);
}

int
lpc17xx_erase(struct flashinfo *fdev,int snum)
{
	if (prepare4write(snum,snum) == -1)
		return(-1);

	iap_cmd[0] = IAPCMD_ERASESECTOR;
	iap_cmd[1] = snum;
	iap_cmd[2] = snum;
	iap_cmd[3] = cpuclk_khz;
	iap_entry(iap_cmd,iap_rslt);
	if (iap_rslt[0] != IAPSTAT_SUCCESS) {
		DBGPRINT(("erase failed %d\n",iap_rslt[0]));
		return(-1);
	}

	return(0);
}

#define BLKSIZ	256
#define BLKMSK	0xff

static int
ramtoflash(char *dest, char *src, int line)
{
	int snum;

	if (addrtosector((unsigned char *)dest,&snum,0,0) < 0)
		return(-1);
	if (prepare4write(snum,snum) == -1)
		return(-1);

	DBGPRINT(("r2f: 0x%x 0x%x\n",dest,src));
	iap_cmd[0] = IAPCMD_CPYRAM2FLASH;
	iap_cmd[1] = (unsigned long)dest;
	iap_cmd[2] = (unsigned long)src;
	iap_cmd[3] = BLKSIZ;
	iap_cmd[4] = cpuclk_khz;
	iap_entry(iap_cmd,iap_rslt);
	if (iap_rslt[0] != IAPSTAT_SUCCESS) {
		DBGPRINT(("ramtoflash failed %d (ln=%d)\n",iap_rslt[0],line));
		return(-1);
	}

	return(0);
}

static unsigned long blk[BLKSIZ/4];

#define MEMCPY(a,b,c) memcpy((void *)(a),(void *)(b),c)
#define RAM2FLASH(a,b) ramtoflash((char *)(a),(char *)(b),__LINE__)

/* lpc17xx_write():
 * Each call to copyblock() must be exactly 256 bytes to a destination address
 * that is 256-byte aligned..
 */
int
lpc17xx_write(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt)
{
	unsigned char *buf;
	int boffset, tot;
	unsigned long daddr, saddr;

	buf = (unsigned char *)blk;
	daddr = (unsigned long)dest;
	saddr = (unsigned long)src;
	tot = bytecnt;

	/* Deal with the possibility that the destination is not 256-byte aligned,
	 * as well as the possibility that the total is less that 256 bytes...
	 */
	boffset = (daddr & BLKMSK);
	if (boffset) {
		MEMCPY(buf,daddr-boffset,boffset);
		if (tot >= (BLKSIZ-boffset)) {
			MEMCPY(buf+boffset,saddr,BLKSIZ-boffset);
			tot -= (BLKSIZ-boffset);
			saddr += (BLKSIZ-boffset);
			if (RAM2FLASH(daddr-boffset,buf) < 0)
				return(-1);
			daddr = daddr - boffset + BLKSIZ;
		}
		else {
			MEMCPY(buf+boffset,saddr,tot);
			MEMCPY(buf+boffset+tot,daddr+tot,BLKSIZ-boffset-tot);
			tot -= tot;
			if (RAM2FLASH(daddr-boffset,buf) < 0)
				return(-1);
		}
	}
	if (tot == 0)
		return(0);

	/* Destination address should be aligned now...
	 */
	if (daddr & BLKMSK) {
		DBGPRINT(("huh1?\n"));
		return(-1);
	}
	
	while(tot > BLKSIZ) {
		MEMCPY(buf,saddr,BLKSIZ);
		if (RAM2FLASH(daddr,buf) < 0)
			return(-1);
		tot -= BLKSIZ;
		saddr += BLKSIZ;
		daddr += BLKSIZ;
	}

	if (tot == 0)
		return(0);

	MEMCPY(buf,saddr,tot);
	MEMCPY(buf+tot,daddr+tot,BLKSIZ-tot);
	if (RAM2FLASH(daddr,buf) < 0)
		return(-1);
	
	return(0);
}

int
lpc17xx_ewrite(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt)
{
	printf("Ewrite not implemented yet.\n");
	return(-1);
}

int
lpc17xx_type(struct flashinfo *fdev)
{
	iap_cmd[0] = 54;
	iap_entry(iap_cmd,iap_rslt);
//	printf("rslt: 0x%x 0x%x 0x%x 0x%x 0x%x\n",
//		iap_rslt[0],iap_rslt[1],iap_rslt[2],iap_rslt[3],iap_rslt[4]);

	if (iap_rslt[0] == IAPSTAT_SUCCESS)
		fdev->id = iap_rslt[1];
	else
		fdev->id = -1;

	return(fdev->id);
}

/* lpc17xx doesn't support lock so...
 */
int
lpc17xx_lock(struct flashinfo *fdev,int snum,int operation)
{
	if (operation == FLASH_LOCKABLE)
		return(0);	
	return(-1);
}

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
#define LPC1769_ID 0x26113F37
#define LPC1768_ID 0x26013F37
#define LPC1767_ID 0x26012837
#define LPC1766_ID 0x26013F33
#define LPC1765_ID 0x26013733
#define LPC1764_ID 0x26011922
#define LPC1759_ID 0x25113737
#define LPC1758_ID 0x25013F37
#define LPC1756_ID 0x25011723
#define LPC1754_ID 0x25011722
#define LPC1752_ID 0x25001121
#define LPC1751_ID 0x25001118

struct flashdesc FlashNamId[] = {
	{ LPC1769_ID,	"LPC1769" }
};

struct sectorinfo sinfoLPC17[SECTORTOT];

/* FlashBankInit():
 * Initialize flash structures and determine flash device type.
 */
int
FlashBankInit(struct flashinfo *fbnk, int snum)
{
	int	i, ssize;
	unsigned long addr;

	addr = (unsigned long)(fbnk->base);
	fbnk->sectorcnt = SECTORTOT;

	for(i=0;i<SECTORTOT;i++) {
		fbnk->sectors[i].snum = snum+i;
		if (i < 16)
			ssize = 0x1000;
		else
			ssize = 0x8000;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = (unsigned char *)addr;
		fbnk->sectors[i].end = (unsigned char *)(addr + ssize - 1);
		fbnk->sectors[i].protected = 0;
		addr += ssize;
	}
	fbnk->end = (unsigned char *)(addr - 1);
	return(SECTORTOT);
}

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit(void)
{
	int		snum;
	struct	flashinfo *fbnk;

	iap_entry = (IAP)IAP_LOCATION;

	snum = 0;
	FlashCurrentBank = 0;
	cpuclk_khz = SystemCoreClock / 1000; 

	fbnk = &FlashBank[0];
	lpc17xx_type(fbnk);
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->width = 0;	/* Not needed for this device. */

	fbnk->fltype = lpc17xx_type;
	fbnk->flerase = lpc17xx_erase;
	fbnk->flwrite = lpc17xx_write;
	fbnk->flewrite = lpc17xx_ewrite;
	fbnk->fllock = lpc17xx_lock;
	fbnk->fllock = FlashLockNotSupported;

	fbnk->sectors = sinfoLPC17;
	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

#if FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT, &FlashBank[FLASHRAM_BANKNUM],
		sinfoRAM, 0);
#endif

	return(0);
}

#endif	/* INCLUDE_FLASH */
