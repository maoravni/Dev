/* tfs.c:
 *	Tiny File System
 *	TFS supports the ability to store/access files in flash.  The TFS
 *	functions provide a command at the monitor's user interface (the
 *	"tfs" command) as well as a library of functions that are available to
 *	the monitor/application code on this target (TFS api).
 *
 *	The code that supports TFS in the MicroMonitor package spans across 
 *	several files.  This is done so that various pieces of TFS can optionally
 *	be compiled in or out (using INCLUDE_XXX macros in config.h) of the
 *	monitor package...
 *
 *	tfs.c:
 *		Core TFS code that cannot be optionally omitted without eliminating
 *		the TFS facility from the monitor.
 *	
 *	tfsapi.c:
 *		This file contains the code that supports the application's ability
 *		to use the TFS api.  Since some of the api is used by the monitor
 *		itself, not all of the api-specific code is there, some of it is
 *		in tfs.c.
 *
 *	tfscleanX.c:
 *		TFS can be configured with one of several different flash defrag
 *		mechanisms.  Currently, tfsclean[123].c are available.
 *
 *	tfscli.c:
 *		If you don't need the "tfs" command in your command line interface,
 *		then the code in this file can be omitted.
 *
 *	tfsloader.c:
 *		TFS can support COFF, ELF or A.OUT binary file formats.  The code
 *		to load each of these formats from flash to RAM is here.
 *
 *	NOTES:
 *	* Dealing with multiple task access:
 *	  Since the monitor is inherently a single threaded program
 *	  potentially being used in a multi-tasking environment, the monitor's
 *	  access functions (API) must be provided with a lock/unlock 
 *	  wrapper that will guarantee sequential access to all of the monitor 
 *	  facilities.  Refer to monlib.c to see this implementation.  This
 *	  provides the protection needed by TFS to keep multiple "mon_"
 *	  functions from being executed by different tasks.
 *	  Note that originally this was supported with tfsctrl(TFS_MUTEX ) and
 *	  it only protected the tfs API functions.  This turned out to be
 *	  insufficient because it did not prevent other tasks from calling
 *	  other non-tfs functions in the monitor while tfs access (and
 *	  potentially, flash update) was in progress.  This meant that a flash
 *	  update could be in progress and some other task could call mon_getenv()
 *	  (for example).  This could screw up the flash update because
 *	  mon_getenv() might be fetched out of the same flash device that
 *	  the TFS operation is being performed on.
 *
 *	* Dealing with cache coherency:
 *	  I believe the only concern here is that Icache must be invalidated
 *	  and Dcache must be flushed whenever TFS does a memory copy that may
 *	  ultimately be executable code.  This is handled at the end of the
 *	  tfsmemcpy function by calling flushDcache() and invalidateIcache().
 *	  It is the application's responsibility to give the monitor the
 *	  appropriate functions (see assigncachefuncs()) if necessary.
 *
 *	* Configuring a device to run as TFS memory:
 *	  Assuming you are using power-safe cleanup...
 *	  TFS expects that on any given device used for storage of files, the
 *	  device is broken up into some number of sectors with the last sector
 *	  being the largest and used as the spare sector for defragmentation.
 *	  All other sector sizes must be smaller than the SPARE sector and the
 *	  sector just prior to the spare is used for defragmentation state
 *	  overhead.  This sector should be large enough to allow the overhead 
 *	  space to grow down from the top without filling the sector.  For most
 *	  flash devices, these two sectors (spare and overhead) are usually the
 *	  same size and are large.  For FlashRam, the device should be configured
 *	  so that these two sectors are large.  The spare sector will never be
 *	  allowed to contain any file information (because it is 100% dedicated to
 *	  the defragmentation process) and the sector next to this can have files
 *	  in it, but the overhead space is also in this sector.
 *
 *	* Testing TFS:
 *	  There are three files dedicated to testing the file system.  Two of them
 *	  (tfstestscript & tfstestscript1) are scripts that are put into the
 *	  file system and run.  The third file (tfstest.c) is a piece of code 
 *	  that can be built into a small application that runs out of TFS to test
 *	  all of the API functionality.
 *	  - tfstestscript:
 *		This script is used to simply bang on normal defragmentation.  It
 *		builds files with sizes and names based on the content of memory
 *		starting at $APPRAMBASE.  Changing the content of memory starting at
 *		$APPRAMBASE will change the characteristics of this test so it is
 *		somewhat random.  It is not 100% generic, but can be used as a
 *		base for testing TFS on various systems.
 *	  - tfstestscript1:
 *		This script is used to bang on the power-safe defragmentation of
 *		TFS.  It simulates power hits that might occur during defragmentation.
 *		This script assumes that the monitor has been built with the
 *		DEFRAG_TEST_ENABLED flag set.
 *	  - tfstest.c:
 *		This code can be built into a small application that will thoroughly
 *		exercise the TFS API.  This file can also be used as a reference for
 *		some examples of TFS api usage.
 *
 *	General notice:
 *	This code is part of a boot-monitor package developed as a generic base
 *	platform for embedded system designs.  As such, it is likely to be
 *	distributed to various projects beyond the control of the original
 *	author.  Please notify the author of any enhancements made or bugs found
 *	so that all may benefit from the changes.  In addition, notification back
 *	to the author will allow the new user to pick up changes that may have
 *	been made by other users after this version of the code was distributed.
 *
 *	Note1: the majority of this code was edited with 4-space tabs.
 *	Note2: as more and more contributions are accepted, the term "author"
 *		   is becoming a mis-representation of credit.
 *
 *	Original author:	Ed Sutter
 *	Email:				esutter@lucent.com
 *	Phone:				908-582-2351
 */
#include "config.h"
#include "cpu.h"
#include "stddefs.h"
#include "genlib.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "tfsdev.h"
#include "flash.h"
#include "cli.h"

#if INCLUDE_TFS

int		tfsrun_abortableautoboot(char **arglist,int verbose);
char	*(*tfsGetAtime)(long,char *,int);
long	(*tfsGetLtime)(void);
int		(*tfsDocommand)(char *,int);
TDEV	tfsDeviceTbl[TFSDEVTOT+1];
TFILE	**tfsAlist;
struct	tfsdat tfsSlots[TFS_MAXOPEN];
int		TfsCleanEnable;
char	tfsInitialized;

static void 	pre_tfsautoboot_hook(void);

static int		tfsAlistSize, tfsOldDelFlagCheckActive;
static int		tfsMonrcActive;

/* alt_tfsdevtbl[]:
 * This pre-initialized table of "flash-empty" tfsdev structures allows
 * the user to override the default configuration of TFS as it is defined
 * in tfsdev.h in the port directory.  This is primarily useful for cases
 * where uMon is being deployed on an evaluation board and the users have
 * varying needs for TFS and it's use of the on-board flash memory.
 *
 * If TFS_ALTDEVTBL_BASE is defined, then the flash space used as the
 * alternate device table can be anywhere in flash memory.  Obviously
 * it has to be in space that is not used by anything else (and is erased
 * if not in use); however, this does provide the user with the option to
 * allocate one sector of flash for this; thus allowing the structure to
 * be re-written by simply eraseing that sector.
 */
#ifdef TFS_ALTDEVTBL_BASE

struct tfsdev *alt_tfsdevtbl = (struct tfsdev *)TFS_ALTDEVTBL_BASE;

#else

/* The following initialized array uses a GNU-extension to
 * force an array of structures and an array of arrays be initialized
 * to 0xff in flash.  This may break for other compilers (in that case,
 * define TFS_ALTDEVTBL_BASE).  This syntax is discussed in gcc.gnu.org
 * online documentation under the topic "Designated Initializers".
 */
struct tfsdev alt_tfsdevtbl[TFSDEVTOT] = {
	[0 ... (TFSDEVTOT-1)] = 
	{ (char *)0xffffffff, 0xffffffff, 0xffffffff,
		0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff }
};

#endif

#define TFS_ALTDEVTBL_SIZE	(TFSDEVTOT * sizeof(struct tfsdev))

/* tfsflgtbl & tfserrtbl:
 *	Tables that establish an easy lookup mechanism to convert from
 *	bitfield to string or character.
 *	Note that TFS_ULVL0 is commented out.  I leave it in here as a place
 *	holder (comment), but it actually is not needed becasue ulvl_0 is the
 *	default if no other ulvl is specified.
 */
struct tfsflg tfsflgtbl[] = {
	{ TFS_BRUN,			'b',	"run_at_boot",			TFS_BRUN },
	{ TFS_QRYBRUN,		'B',	"qry_run_at_boot",		TFS_QRYBRUN },
	{ TFS_EXEC,			'e',	"executable",			TFS_EXEC },
	{ TFS_UNREAD,		'u',	"ulvl_unreadable", 		TFS_UNREAD },
	{ 0, 0, 0, 0 }
};

static struct tfserr tfserrtbl[] = {
	{ TFS_OKAY,				"no error" },
	{ TFSERR_NOFILE,		"file not found" },
	{ TFSERR_NOSLOT,		"max fps opened" },
	{ TFSERR_EOF,			"end of file" },
	{ TFSERR_BADARG,		"bad argument" },
	{ TFSERR_NOTEXEC,		"not executable" },
	{ TFSERR_BADCRC,		"bad crc" },
	{ TFSERR_FILEEXISTS,	"file already exists" },
	{ TFSERR_FLASHFAILURE,	"flash operation failed" },
	{ TFSERR_WRITEMAX,		"max write count exceeded" },
	{ TFSERR_RDONLY,		"file is read-only" },
	{ TFSERR_BADFD,			"invalid descriptor" },
	{ TFSERR_BADHDR,		"bad binary executable header" },
	{ TFSERR_CORRUPT,		"corrupt file" },
	{ TFSERR_MEMFAIL,		"memory failure" },
	{ TFSERR_FLASHFULL,		"out of flash space" },
	{ TFSERR_USERDENIED,	"user level access denied" },
	{ TFSERR_NAMETOOBIG,	"name or info field too big" },
	{ TFSERR_FILEINUSE,		"file in use" },
	{ TFSERR_NOTCPRS,		"file is not compressed" },
	{ TFSERR_NOTAVAILABLE,	"tfs facility not available" },
	{ TFSERR_BADFLAG,		"bad flag" },
	{ TFSERR_CLEANOFF,		"defragmentation is disabled" },
	{ TFSERR_FLAKEYSOURCE,	"dynamic source data" },
	{ TFSERR_BADEXTENSION,	"invalid file extension" },
	{ TFSERR_LINKERROR,		"file link error" },
	{ TFSERR_BADPREFIX,		"invalid device prefix" },
	{ TFSERR_ALTINUSE,		"alternate devcfg in use" },
	{ TFSERR_NORUNMONRC,	"can't run from monrc" },
	{ TFSERR_DSIMAX,		"out of DSI space" },
	{ TFSERR_TOOSMALL,		"partition size too small" },
	{ 0,0 }
};


/* dummyAtime() & dummyLtime():
 *	These two functions are loaded into the function pointers as defaults
 *	for the time-retrieval stuff used in TFS.
 */
static char *
dummyAtime(long tval,char *buf,int buflen)
{
/*	strcpy(buf,"Fri Sep 13 00:00:00 1986"); */
	*buf = 0;
	return(buf);
}

static long
dummyLtime(void)
{
	return(TIME_UNDEFINED);
}

/* getdfsdev():
 *	Input is a file pointer; based on that pointer return the appropriate
 *	device header pointer.  If error, just return 0.
 *	A "device" in TFS is some block of some type of memory that is assumed
 *	to be contiguous space that can be configured as a block of sectors (to
 *	look like flash).  For most systems, there is only one (the flash); 
 *	other systems may have battery-backed RAM, etc...
 *	Note that this is not fully implemented.
 */
static TDEV *
gettfsdev(TFILE *fp)
{
	TDEV *tdp;

	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		if ((fp >= (TFILE *)tdp->start) &&
			(fp < (TFILE *)tdp->end))
			return(tdp);
	}
	return(0);
}

/* tfsspace():
 * Return 1 if the incoming address is within TFS space; else
 * return 0.
 */
int
tfsspace(char *addr)
{
	TDEV *tdp;

	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		if ((addr >= (char *)tdp->start) &&
			(addr <= (char *)tdp->end))
			return(1);
		if ((addr >= (char *)tdp->spare) &&
			(addr < (char *)(tdp->spare+tdp->sparesize)))
			return(1);
	}
	return(0);
}

#ifndef TFS_NON_STANDARD_FLASH_INTERFACE
/* tfsflasherase(), tfsflasheraseall() & tfsflashwrite():
 *	Wrappers for corresponding flash operations.  
 *
 * In almost all cases, this code is included here; however, there
 * are some cases where the flash access functions that TFS uses must
 * be port-specific; hence, TFS_NON_STANDARD_FLASH_INTERFACE would be defined
 * in config.h and they would be provided by the port .
 */
int
tfsflasheraseall(TDEV *tdp)
{
#if INCLUDE_FLASH
	int	snum, last;

	/* Erase the sectors within the device that are used for file store...
	 */
	if (addrtosector((uchar *)tdp->start,&snum,0,0) < 0)
		return(TFSERR_MEMFAIL);
	last = snum + tdp->sectorcount;

	while(snum < last) {
		if (AppFlashErase(snum++) <= 0)
			return(TFSERR_MEMFAIL);
	}

	/* Erase the spare (if there is one)...
	 * (if this system is configured with tfsclean2.c, then
	 * there is no need for a spare sector).
	 */
	if (tdp->spare) {
		if (addrtosector((uchar *)tdp->spare,&snum,0,0) < 0)
			return(TFSERR_MEMFAIL);
		if (AppFlashErase(snum) <= 0)
			return(TFSERR_MEMFAIL);
	}
#else
	memset((void *)tdp->start,(int)0xff,(int)(tdp->end-tdp->start));
#endif
	return(TFS_OKAY);
}

int
tfsflasherase(int snum)
{
#if INCLUDE_FLASH
	return(AppFlashErase(snum));
#else
	return(TFSERR_NOTAVAILABLE);
#endif
}

int
tfsflashwrite(uchar *dest,uchar *src,long bytecnt)
{
#if INCLUDE_FLASH
	if (bytecnt < 0)
		return(TFSERR_BADARG);
	
	if (AppFlashWrite(dest,src,bytecnt) == 0)
		return(TFS_OKAY);
	else
		return(TFSERR_FLASHFAILURE);
#else
	return(TFSERR_NOTAVAILABLE);
#endif
}

#endif	/* TFS_NON_STANDARD_FLASH_INTERFACE */

/* tfserrmsg():
 *	Return the error message string that corresponds to the incoming
 *	tfs error number.
 */
char *
tfserrmsg(int errno)
{
	struct	tfserr	*tep;
	
	tep = tfserrtbl;
	while(tep->msg) {
		if (errno == tep->err)
			return(tep->msg);
		tep++;
	}
	return("unknown tfs errno");
}

/* tfsflagsbtoa():
 *	Convert binary flags to ascii and return the string.
 */
char *
tfsflagsbtoa(long flags,char *fstr)
{
	int	i;
	struct	tfsflg	*tfp;

	if ((!flags) || (!fstr))
		return((char *)0);

	i = 0;
	tfp = tfsflgtbl;
	*fstr = 0;
	while(tfp->sdesc) {
		if ((flags & tfp->mask) == tfp->flag)
			fstr[i++] = tfp->sdesc;
		tfp++;
	}
	fstr[i] = 0;
	return(fstr);
}

/* tfsflagsatob():
 *	Convert ascii flags to binary and return the long.
 */
int
tfsflagsatob(char *fstr, long *flag)
{
	struct	tfsflg	*tfp;

	*flag = 0;

	if (!fstr)
		return(TFSERR_BADFLAG);

	while(*fstr) {
		tfp = tfsflgtbl;
		while(tfp->sdesc) {
			if (*fstr == tfp->sdesc) {
				*flag |= tfp->flag;
				break;
			}
			tfp++;
		}
		if (!tfp->flag)
			return(TFSERR_BADFLAG);
		fstr++;
	}
	return(TFS_OKAY);
}

/* hdrcrc():
 * The crc of the file header was originally calculated (in tfsadd())
 * with the header crc and next pointer nulled out; so a copy must
 * be made and these two fields cleared.  Also, note that the
 * TFS_NSTALE and TFS_ACTIVE flags are forced to be set in the copy.
 * This is done because it is possible that either of these bits may
 * have been cleared due to other TFS interaction; hence, they need
 * to be set prior to crc calculation.
 * Note also that earlier versions of TFS deleted a file by clearing
 * the entire flags field.  This made it impossible to do a header crc
 * check on a deleted file; deletion has been changed to simply clear
 * the TFS_ACTIVE bit in the flags, so now a deleted file's header can
 * can be crc tested by simply forcing the TFS_ACTIVE bit high as was
 * mentioned above.
 */
ulong
tfshdrcrc(TFILE *hdr)
{
	TFILE hdrcpy;

	memcpy((char *)&hdrcpy,(char *)hdr,sizeof(TFILE));
	hdrcpy.next = 0;
	hdrcpy.hdrcrc = 0;
	hdrcpy.flags |= (TFS_NSTALE | TFS_ACTIVE);
    { int i;
    for(i=0;i<4;i++)
        hdrcpy.altflags[i] = 0xffffffff;
    }
	return(crc32((uchar *)&hdrcpy,TFSHDRSIZ));
}

/* validtfshdr():
 *	Return 1 if the header pointed to by the incoming header pointer is valid.
 *	Else return 0.  The header crc is calculated based on the hdrcrc
 *	and next members of the structure being zero.
 *	Note that if the file is deleted, then just ignore the crc and return 1.
 */
int
validtfshdr(TFILE *hdr)
{
	/* A few quick checks... */
	if (!hdr || hdr->hdrsize == ERASED16)
		return(0);

	if (tfshdrcrc(hdr) == hdr->hdrcrc) {
		return(1);
	}
	else {
		/* Support transition to new deletion flag method...
		 */
		if ((hdr->flags == 0) && tfsOldDelFlagCheckActive)
			return(1);					

		printf("Bad TFS hdr crc @ 0x%lx\n",(ulong)hdr);
		return(0);
	}
}

/* nextfp():
 *	Used as a common means of retrieving the next file header pointer.  It
 *	does some sanity checks based on the fact that all pointers must fall
 *	within the TFSSTART<->TFSEND memory range and since each file is placed
 *	just after the previous one in linear memory space, fp->next should
 *	always be greater than fp.
 */
TFILE *
nextfp(TFILE *fp, TDEV *tdp)
{
	if (!tdp)
		tdp = gettfsdev(fp);

	/* Make some basic in-range checks...
	 */
	if ((!tdp) || (fp < (TFILE *)tdp->start) || (fp > (TFILE *)tdp->end) ||
		(fp->next < (TFILE *)tdp->start) || (fp->next > (TFILE *)tdp->end)  ||
		(fp->next <= fp)) {
		printf("Bad TFS hdr ptr @ 0x%lx\n",(ulong)fp);
		return(0);
	}
	return(fp->next);
}

/* tfsflasherased():
 *	Jump to the point in flash after the last file in TFS, then verify
 *	that all remaining flash  that is dedicated to TFS is erased (0xff).
 *	If erased, return 1; else return 0.
 */
int
tfsflasherased(TDEV *tdp, int verbose)
{
	ulong	*lp;
	TFILE	*tfp;

	tfp = (TFILE *)tdp->start;
	while(validtfshdr(tfp))
		tfp = nextfp(tfp,tdp);

	lp = (ulong *)tfp;
	while (lp < (ulong *)tdp->end) {
		if (*lp != ERASED32) {
			if (verbose)
				printf("End of TFS on %s not erased at 0x%lx\n",
					tdp->prefix,(ulong)lp);
			return(0);
		}
#ifdef WATCHDOG_ENABLED
		if (((ulong)lp & 0x3f) == 0)
			WATCHDOG_MACRO;
#endif
		lp++;
	}
	return(1);
}

/* tfsmemuse():
 *	Step through one (or all) TFS devices and tally up various memory usage
 *	totals.  See definition of tfsmem structure for more details.
 *	If incoming tdpin pointer is NULL, then tally up for all TFS devices;
 *	otherwise, tally up for only the one device pointed to by tdpin.
 */
int
tfsmemuse(TDEV *tdpin, TINFO *tinfo, int verbose)
{
	TFILE	*tfp;
	TDEV	*tdp, *dsimax;
	int		devtot, devidx, ftot, fmax;
	char	*cfgerr, varname[TFSNAMESIZE+16];

	/* Start by clearing incoming structure...
	 */
	tinfo->pso = 0;
	tinfo->sos = 0;
	tinfo->memtot = 0;
	tinfo->liveftot = 0;
	tinfo->deadftot = 0;
	tinfo->livedata = 0;
	tinfo->deaddata = 0;
	tinfo->liveovrhd = 0;
	tinfo->deadovrhd = 0;
	tinfo->sectortot = 0;

	if (verbose) {
		printf("TFS Memory Usage...\n     ");
		printf(" name    start       end       spare     spsize  scnt type\n");
	}

	devtot = fmax = 0;
	for (tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) 
		devtot++;

	devidx = 0;
	dsimax = 0;
	for (tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		if (!tdpin || (tdpin == tdp)) {
			tfp = (TFILE *)tdp->start;

			cfgerr = (char *)0;

			/* Do some sanity checks on the configuration...
			 */
			if ((tdp->spare >= tdp->start) && (tdp->spare <= tdp->end)) {
				cfgerr = "spare within storage space";
			}
			if (cfgerr) {
				printf("Bad %s TFS config: %s.\n",tdp->prefix,cfgerr);
			}

			/* Store TFS device info to shell variables.
			 */
			sprintf(varname,"TFS_PREFIX_%d", devidx);
			shell_sprintf(varname,"%s",tdp->prefix);

			sprintf(varname,"TFS_START_%d", devidx);
			shell_sprintf(varname,"0x%lx",tdp->start);

			sprintf(varname,"TFS_END_%d", devidx);
			shell_sprintf(varname,"0x%lx",tdp->end);

			sprintf(varname,"TFS_SPARE_%d", devidx);
			shell_sprintf(varname,"0x%lx",tdp->spare);

			sprintf(varname,"TFS_SPARESZ_%d", devidx);
			shell_sprintf(varname,"0x%lx",tdp->sparesize);

			sprintf(varname,"TFS_SCNT_%d", devidx);
			shell_sprintf(varname,"%d",tdp->sectorcount);

			sprintf(varname,"TFS_DEVINFO_%d", devidx);
			shell_sprintf(varname,"0x%lx",tdp->devinfo);

			if (verbose) {
				printf("%10s: 0x%08lx|0x%08lx|",
					tdp->prefix,(ulong)(tdp->start),(ulong)(tdp->end));

				if (TFS_DEVTYPE_ISRAM(tdp))
					printf("  - NA -  | - NA - | NA ");
				else
					printf("0x%08lx|0x%06lx|%4ld",(ulong)(tdp->spare),
						tdp->sparesize,tdp->sectorcount);

				printf("|0x%lx\n",(ulong)(tdp->devinfo));
			}
			tinfo->memtot += ((tdp->end - tdp->start) + 1) + tdp->sparesize;
			tinfo->pso += (tdp->sectorcount * 4) + 16;
			tinfo->sos += tdp->sparesize;
			tinfo->sectortot += tdp->sectorcount;
			ftot = 0;
			while(validtfshdr(tfp)) {
				if (TFS_FILEEXISTS(tfp)) {
					ftot++;
					tinfo->liveftot++;
					tinfo->livedata += TFS_SIZE(tfp);
					tinfo->liveovrhd += (TFSHDRSIZ + DEFRAGHDRSIZ);
				}
				else {
					tinfo->deadftot++;
					tinfo->deaddata += TFS_SIZE(tfp);
					tinfo->deadovrhd += TFSHDRSIZ;
				}
				tfp = nextfp(tfp,tdp);
			}

#if INCLUDE_FLASH
			/* Keep track of how many files are stored, relative to the
			 * size of the sector used to store DSI info at defrag time...
			 */
			if (!TFS_DEVTYPE_ISRAM(tdp)) {
				int		sector_ovrhd, ssize;

				if (addrtosector((uchar *)(tdp->end),0,&ssize,0) < 0)
					return(TFSERR_MEMFAIL);
				sector_ovrhd = tdp->sectorcount * sizeof(struct sectorcrc);

				if ((((ftot + 1) * DEFRAGHDRSIZ) + sector_ovrhd) > ssize)
					dsimax = tdp;

				fmax += (ssize - sector_ovrhd)/DEFRAGHDRSIZ;
			}
#endif
		}
		devidx++;
	}
	shell_sprintf("TFS_DEVTOT","%d",devtot);

	tinfo->memused = tinfo->livedata + tinfo->liveovrhd +
			tinfo->deaddata + tinfo->deadovrhd + tinfo->pso + tinfo->sos +
			tinfo->sectortot * sizeof(struct sectorcrc);
	tinfo->memfree = tinfo->memtot - tinfo->memused; 

	/* Remaining space may not even be big enough to contain the
	 * file overhead, if this is the case, show a remaining space
	 * of zero rather than a negative number...
	 */
	tinfo->memfordata =
		tinfo->memfree - (devtot * (TFSHDRSIZ + DEFRAGHDRSIZ));
	if (tinfo->memfordata < 0)
		tinfo->memfordata = 0;


	if (verbose) {
		printf("\n Total memory: %d bytes (used=%d, avail=%d (%d for data)).\n",
			tinfo->memtot,tinfo->memused,tinfo->memfree, tinfo->memfordata);
		printf(" Per-device overhead: %d bytes ",tinfo->pso+tinfo->sos);
		printf("(defrag-state=%d spare-sector=%d).\n",tinfo->pso,tinfo->sos);
		printf(" File data space: %d bytes (live=%d, dead=%d).\n",
			tinfo->livedata+tinfo->deaddata,
			tinfo->livedata,tinfo->deaddata);
		printf(" File overhead space: %d bytes (live=%d, dead=%d).\n",
			tinfo->liveovrhd+tinfo->deadovrhd,
			tinfo->liveovrhd,tinfo->deadovrhd);
		printf(" Sector overhead space: %d bytes.\n",
			tinfo->sectortot*sizeof(struct sectorcrc));
		printf(" File count: %d (live=%d, dead=%d).\n",
			tinfo->liveftot+tinfo->deadftot,tinfo->liveftot,tinfo->deadftot);
		printf(" Defrag will release %d bytes\n",
			tinfo->deadovrhd+tinfo->deaddata);
		printf(" Max # files storable in flash: %d\n",fmax);
		if (dsimax) 
			printf(" Device '%s' has max Defrag-State-Info\n",dsimax->prefix);
		printf("\n");
	}
	return(tinfo->liveftot + tinfo->deadftot);
}

/* tfscheck():
 *	Step through each file in a particular device making a few checks...
 *	- First look at the header.  If hdrsize is erased, it "should" indicate
 *	  the end of the linear list of files.  To be anal about it, verify that
 *	  the entire header is erased.  If it is, we truly are at the end of the
 *	  list; otherwise, header error.
 *	- Second, do a crc32 on the header. 
 *	- Third, if the file is not deleted, then do a crc32 on the data portion
 *	  of the file (if the file is deleted, then it really doesn't matter if
 *	  there is a crc32 error on that data).
 *	- Finally, if the header is not corrupted, index to the next pointer and
 *	  continue.  If the header is corrupt, see if enough information
 *	  in the header is valid to allow us to step to the next file.  Do this
 *	  by calculating where the next pointer should be (using current pointer,
 *	  file+header size and mod16 adjustment) and then see if that matches the
 *	  value stored in the actual "next" pointer.  If yes, go to next file;
 *	  else break out of the loop.
 *	
 *	The purpose is to do more sophisticated file system checks than are
 *	done in normal TFS operations.
 */
#define TFS_CORRUPT		1
#define HDR_CORRUPT		2
#define DATA_CORRUPT	4

int
tfscheck(TDEV *tdp, int verbose)
{
	TFILE	*fp, *fp1;
	int		tfscorrupt, filtot, err;

	/* If the incoming device pointer is null, then loop through all
	 * devices in TFS, recursively calling tfscheck() with each pointer...
	 */
	if (!tdp) {
		for (tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
			err = tfscheck(tdp,verbose);
			if (err != TFS_OKAY)
				return(err);
		}
		return(TFS_OKAY);
	}

	if (verbose)
		printf("TFS device %s check:\n",tdp->prefix);

	filtot = tfscorrupt = 0;

	fp = (TFILE *)tdp->start;
	while(1) {
		tfscorrupt &= ~(HDR_CORRUPT | DATA_CORRUPT);

		/* If hdrsize is ERASED16, then verify that the whole header is
		 * also ERASED16, if yes, we're at the end of the linear list of
		 * files; otherwise, we have a corrupt header.
		 */
		if (fp->hdrsize == ERASED16) {
			int		i;
			ushort	*sp;

			/* If this is right at the edge of the end of the TFS device,
			 * then break with no further checks to this header.
			 */
			if ((fp+1) > (TFILE *)tdp->end)
				break;

			/* Make sure the entire header is erased...
			 */
			sp = (ushort *)fp;
			for(i=0;i<TFSHDRSIZ;i+=2,sp++) {
				if (*sp != ERASED16) {
					if (verbose)
						printf(" Corrupt hdr @ 0x%lx",(ulong)fp);
					tfscorrupt = HDR_CORRUPT | TFS_CORRUPT;
					break;
				}
			}
			if (!(tfscorrupt & HDR_CORRUPT))
				break;
			else
				goto nextfile;
		}

		/* Run a crc check on the header even if file is deleted...
		 */
		if (tfshdrcrc(fp) != fp->hdrcrc) {
			if (verbose)
				printf(" CRC error in hdr @ 0x%lx\n",(ulong)fp);
			tfscorrupt = HDR_CORRUPT | TFS_CORRUPT;
			goto nextfile;
		}

		/* If file exists, run a crc check on data...
		 */
		if (TFS_FILEEXISTS(fp)) { 
			filtot++;
			if (verbose)
				printf(" %s...",fp->name);
		
			if (crc32((uchar *)(TFS_BASE(fp)),fp->filsize) != fp->filcrc) {
					if (verbose)
						printf(" CRC error in data");
					tfscorrupt = DATA_CORRUPT | TFS_CORRUPT;
			}
			else {
				if (verbose)
					printf(" ok");
			}
		}

		/* Prior to incrementing to the next file pointer, if the header
		 * is corrupted, attempt to salvage the next pointer...
		 * If the value of the next pointer matches what is	calculated
		 * from the file size and header size, then assume it is ok
		 * and allow the tfscheck() loop to continue; otherwise break.
		 */
nextfile:
		if (tfscorrupt & HDR_CORRUPT) {
			if (fp->next) {
				ulong modnext;				

				modnext = (ulong)((int)(fp+1) + fp->filsize);
				if (modnext & 0xf) {
					modnext += 16;
					modnext &= ~0xf;
				}
				if (verbose)
					printf(" (next ptr ");
				if (fp->next != (TFILE *)modnext) {
					if (verbose)
						printf("damaged)\n");
					break;
				}
				else {
					if (verbose)
						printf("salvaged)");
				}
			}
		}
		fp1 = nextfp(fp,tdp);
		if (!fp1) {
			tfscorrupt = HDR_CORRUPT | TFS_CORRUPT;
			break;
		}
		if ((verbose) && (TFS_FILEEXISTS(fp) || tfscorrupt))
			putchar('\n');
		fp = fp1;
	}
	tfsflasherased(tdp,verbose);
	if (tfscorrupt)
		return(TFSERR_CORRUPT);
	if (verbose)
		printf(" PASSED\n");
	return (TFS_OKAY);
}

void
tfsclear(TDEV *tdp)
{
	int i;

	/* Clear the fileslot[] table indicating that no files are opened.
	 * Only clear the slots applicable to the incoming TDEV pointer.
	 */
	for (i = 0; i < TFS_MAXOPEN; i++) {
		ulong offset;

		offset = tfsSlots[i].offset;
		if (offset != (ulong)-1) {
			if ((tdp == (TDEV *)0) ||
				((offset >= tdp->start) && (offset <= tdp->end)))
				tfsSlots[i].offset = -1;
		}
	}

	/* If the incoming TDEV pointer is NULL, then we can assume a global
	 * clear and go ahead and cleanup everything; otherwise, we just return
	 * here.
	 */
	if (tdp != (TDEV *)0)
		return;

	/* Init the time retrieval function pointers to their dummy values.
	 */
	tfsGetAtime = dummyAtime;
	tfsGetLtime = dummyLtime;

	/* Default to using standard docommand() within scripts.
	 */
	tfsDocommand = docommand;

	/* Start off with a buffer for 16 files.  This is probably more than
	 * will be used, so it avoids reallocations in tfsreorder().
	 *
	 * Note that this function may be called as a result of the monitor
	 * doing an application exit.  In that case, the heap is not
	 * re-initialized; hence, tfsAlist may already be allocated.
	 * If it is, then just leave it alone...
	 */
	if (tfsAlist == 0) {
		tfsAlistSize = 16;
		tfsAlist = (TFILE **)malloc((tfsAlistSize+1) * sizeof(TFILE **));
		if (!tfsAlist) {
			printf("tfsclear(): tfsAlist allocation failed\n");
			tfsAlistSize = 0;
		}
	}
}

/* tfsdevtblinit():
 *	Transfer the information in tfsdevtbl (in tfsdev.h) to tfsDeviceTbl[].
 *	In most cases, this will be a simple copy.  If the device flag is set
 *	to indicate that the initalization is dynamic, then use the flash 
 *	ops to retrieve the information from the specified bank.
 *
 *	For dynamic configuration, the "start" member of the tfsdev structure
 *	must be set in tfsdev.h and the "devinfo & TFS_DEVINFO_BANKMASK" area
 *	must contain the number of the last flash bank that is to be part of
 *	the TFS device.  Typically this value is the same bank number as the
 *	starting bank, but it could span across multiple contiguous banks
 *	if the hardware is set up that way.
 *
 *	To support the use of top-boot devices, plus the TFS requirement that
 *	the SPARE sector be at-least as large as any other sector in the device,
 *	this code will automatically step down the sector list until it finds
 *	the first large sector below all the small ones usually at the top of
 *	a top-boot device.  The call to lastlargesector() takes care of this.
 *
 *	NOTE:
 *	 This dynamic configuration assumes that the end of the TFS space is
 *	 just below the beginning of the spare space.
 *
 */
void
tfsdevtblinit(void)
{
	int		i;
#if INCLUDE_FLASH
	int		startsector, endsector, bank;
#endif
	TDEV	*tDp, *tdp;

	for(i=0;i<TFSDEVTOT;i++) {
		if ((alt_tfsdevtbl != (struct tfsdev *)0xffffffff) &&
			(alt_tfsdevtbl[i].prefix != (char *)0xffffffff)) {
			tdp = &alt_tfsdevtbl[i];

			/* do some sanity checking on the alternate table:
			 */
			if ((tdp->start >= tdp->end) ||
				(tdp->sparesize > (1024*1024)) ||
				(tdp->sectorcount > (1024*16))) {
				printf("TFS altdevtbl[%d] appears corrupt\n",i);
				tdp = &tfsdevtbl[i];
			}
		}
		else
			tdp = &tfsdevtbl[i];

		tDp = &tfsDeviceTbl[i];
		memcpy((char *)tDp,(char *)tdp,sizeof(struct tfsdev));
		if (i == TFSDEVTOT-1)
			break;

#if INCLUDE_FLASH
		if (tdp->devinfo & TFS_DEVINFO_DYNAMIC) {
			bank = tDp->devinfo & TFS_DEVINFO_BANKMASK;

			/* The spare sector may not be the last sector in the device...
			 * device.  Especially if the device is TopBoot type.
			 */
			if (lastlargesector(bank, (uchar *)tDp->start,
				tDp->sectorcount+1, &endsector,
				(int *)&tDp->sparesize,(uchar **)&tDp->spare) == -1)
				break;

			tDp->end = tDp->spare - 1;
			if (addrtosector((uchar *)tDp->start,&startsector,0,0) == -1)
				break;
			tDp->sectorcount = endsector - startsector;

			/*
			printf( "tfsdevtblinit - dynamic TFS allocation\n"
				"    prefix........ %s\n"
				"    start......... 0x%08X\n"
				"    end........... 0x%08X\n"
				"    spare......... 0x%08X\n"
				"    sparesize..... 0x%08X\n"
				"    sectorcount... %d\n",
				tDp->prefix, tDp->start, tDp->end, tDp->spare,
				tDp->sparesize, tDp->sectorcount);
			 */
		}
#endif
	}
}

/* tfsstartup():
 *	Called at system startup to get things properly initialized.
 */
void
tfsstartup()
{
	TDEV	*tdp;

	if (!tfsInitialized)
		tfsdevtblinit();

	tfsclear((TDEV *)0);

	/* No need to walk through the entire TFS init if it has already
	 * been done...
	 */
	if (tfsInitialized)
		return;

	/* Step through the table looking for TFS devices that may need to
	 * be automatically initialized at startup.  There are two cases:
	 *  - if the devtype is TFS_DEVTYPE_RAM.
	 *  - if the devtype is TFS_DEVTYPE_NVRAM with AUTOINIT set.
	 */
	for(tdp = tfsDeviceTbl;tdp->prefix != 0;tdp++) {
		if (((tdp->devinfo & TFS_DEVTYPE_MASK) == TFS_DEVTYPE_RAM) ||
			(((tdp->devinfo & TFS_DEVTYPE_MASK) == TFS_DEVTYPE_NVRAM) &&
			 (tdp->devinfo & TFS_DEVINFO_AUTOINIT))) {
				_tfsinit(tdp);
		}
	}

	tfsfixup(3,0);
	tfsInitialized = 1;
}

/* struct tfsran:
	Used by tfsrunboot only.  No need to put this in tfs.h.
 */
struct tfsran {
	char name[TFSNAMESIZE+1];
};

/* tfsrunboot():
 *	This function is called at monitor startup.  It scans the list of
 *	files built by tfsreorder() and executes each file in the list that has
 *	the BRUN flag set.  As each file is run its name is added to the
 *	ranlist[] table.
 *	
 *	After each file is run, there is a check made to see if the flash has
 *	been modified.  If yes, then tfsreorder() is run again and we start 
 *	over at the top of the list of files organized by tfsreorder().  As
 *	we step through the tfsAlist[] array, if the file has a BRUN flag set
 *	but it is already in the ranlist[] table, it is not run again.
 *	   
 *	This scheme allows a file in the initial list of BRUN files to modify
 *	the file list without confusing the list of files that are to be run.
 *	Files (even new BRUN files) can be added to the list by some other BRUN
 *	file, and these new files will be run.
 */
int
tfsrunboot(void)
{
	static	struct	tfsran *ranlist;
	char	*argv[2];
	int		rancnt, aidx, ridx, err;

#if TFS_RUN_DISABLE
	return(TFSERR_NOTAVAILABLE);
#endif

	if (!tfsInitialized) {
		printf("TFS not initialized, tfsrunboot aborting.\n");
		return(0);
	}

	/* The argv[] array is used by tfsrun(); argv[0] is name of file to be
	 * executed, argv[1] must be nulled to indicate no command line args
	 * passed to the BRUN file/script.
	 */
	argv[1] = (char *)0;

	/* Create list of file pointers (tfsAlist[]) in alphabetical order
	 * based on name...
	 */
	if ((err = tfsreorder()) < 0) {
		printf("tfsrunboot() reorder1: %s\n",tfserrmsg(err));
		return(-1);
	}

	/* Clear the ranlist pointer. This pointer is the base address of a
	 * list of file names that have	been run.
	 */
	rancnt = 0;
	ranlist = (struct tfsran *)0;

	for (aidx=0;tfsAlist[aidx];aidx++) {
		char	fname[TFSNAMESIZE+1];
		int		alreadyran;
		TFILE	*fp;
		struct	tfsran *rp;

		fp = tfsAlist[aidx];
		strcpy(fname,TFS_NAME(fp));

		/* If the file has no BRUN flag set, just continue.  If a BRUN flag
		 * is set, then see if the file has already been run.  If yes, then
		 * just continue; else run the file.
		 */
		alreadyran = 0;
		if (fp->flags & (TFS_BRUN | TFS_QRYBRUN)) {
			for(ridx=0;ridx<rancnt;ridx++) {
				if (!strcmp(ranlist[ridx].name,fname)) {
					alreadyran = 1;
					break;
				}
			}
		}
		else
			continue;			/* No BRUN flag set. */

		if (alreadyran) {		/* BRUN flag set, but file has already		*/
			continue;			/* been run.								*/
		}

		err = TFS_OKAY;
		argv[0] = fname;

		/* At this point we know the file is a BRUN type, so just see if
		 * the query should precede the run...
		 */
		if (fp->flags & TFS_QRYBRUN) {
			int pollval;
			char query[TFSNAMESIZE+8];

			sprintf(query,"%s?",fname);
			pollval = pollConsole(query);
#ifdef TFS_AUTOBOOT_CANCEL_CHAR
			if (pollval == (int)TFS_AUTOBOOT_CANCEL_CHAR)
				continue;
#else
			if (pollval)
				continue;
#endif
		}
		/* Increase the size of the ranlist[] table and add the file that
		 * is about to be run to that list...
		 */
		rancnt++;
		rp = (struct tfsran*)realloc((char *)ranlist,
			rancnt*sizeof(struct tfsran));

		if (!rp) {
			if (ranlist)
				free((char *)ranlist);
			printf("tfsrunboot() runlist realloc failure\n");
			return(-1);
		}

		ranlist = rp;
		strcpy(ranlist[rancnt-1].name,fname);

		/* Run the executable...
		 */
		if (fp->flags & TFS_BRUN) {
			err = tfsrun_abortableautoboot(argv,0);
		}
		else {
			pre_tfsautoboot_hook();
			err = tfsrun(argv,0);
		}
	
		if (err != TFS_OKAY)
			printf("%s: %s\n",fname,tfserrmsg(err));
	}
	if (ranlist)
		free((char *)ranlist);
	return(rancnt);
}

/* tfsreorder():
 *	Populate the tfsAlist[] array with the list of currently active file
 *	pointers, but put in alphabetical (lexicographical using strcmp()) order
 *	based on the filename.
 *	Note that after each file addition/deletion, this must be re-run.
 */
int
tfsreorder(void)
{
	TFILE	*fp;
	TDEV	*tdp;
	int		i, j, tot;

	/* Determine how many valid files exist, and create tfsAlist array:
	 */
	tot = 0;

	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		fp = (TFILE *)tdp->start;
		while(validtfshdr(fp)) {
			if (TFS_FILEEXISTS(fp))
				tot++;
			fp = nextfp(fp,tdp);
		}
	}

	/* If tfsAlist already exists, and is already big enough, then
	 * don't do any allocation; otherwise, create the array with one extra
	 * slot for a NULL pointer used elsewhere as an end-of-list indicator.
	 */
	if (tot > tfsAlistSize) {
		tfsAlist = (TFILE **)realloc((char *)tfsAlist,
			(tot+1) * sizeof(TFILE **));
		if (!tfsAlist) {
			tfsAlistSize = 0;
			return(TFSERR_MEMFAIL);
		}
		tfsAlistSize = tot;
	}

	/* Clear the entire table (plus the extra one at the end):
	 */
	for(i=0;i<=tot;i++)
		tfsAlist[i] = (TFILE *)0;

	/* Populate tfsAlist[] with a pointer to each active file
	 * in flash as they exist in memory...
	 */
	i = 0;

	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		fp = (TFILE *)tdp->start;
		while(validtfshdr(fp)) {
			if (TFS_FILEEXISTS(fp)) {
				tfsAlist[i++] = fp;
			}
			fp = nextfp(fp,tdp);
		}
	}

	/* Now run a bubble sort on that list based on the lexicographical
	 * ordering returned by strcmp...
	 */
	for(i=1;i<tot;++i) {
		for(j=tot-1;j>=i;--j) {
			if (strcmp(TFS_NAME(tfsAlist[j-1]),TFS_NAME(tfsAlist[j])) > 0) {
				fp = tfsAlist[j-1];
				tfsAlist[j-1] = tfsAlist[j];
				tfsAlist[j] = fp;
			}
		}
	}
	return(tot);
}

/* tfsheadroom():
 * Based on the current offset into the file specified by the incoming
 * descriptor, return the gap between the current offset and the end
 * of the file.
 */
static long
tfsheadroom(int fd)
{
	struct tfsdat *tdat;

	if ((fd < 0) || (fd >= TFS_MAXOPEN))
		return(TFSERR_BADARG);

	tdat = &tfsSlots[fd];
	if (tdat->flagmode & TFS_RDONLY)
		return(tdat->hdr.filsize - tdat->offset);
	else
		return(tdat->hwp - tdat->offset);
}

/* tfstell():
 *	Return the offset into the file that is specified by the incoming
 *	descriptor.
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */
long
tfstell(int fd)
{
	if ((fd < 0) || (fd >= TFS_MAXOPEN))
		return(TFSERR_BADARG);
	return(tfsSlots[fd].offset);
}

/* tfscompare():
 *	Compare the content of the file specified by tfp with the content pointed
 *	to by the remaining arguments.  If identical, return 0; else return -1.
 */
static int
tfscompare(TFILE *tfp,char *name, char *info, char *flags, uchar *src, int size)
{
	long bflags;

	/* Compare size, name, info field, flags and data:
	 */

	/* Size...
	 */
	if (TFS_SIZE(tfp) != size)
		return(-1);

	/* Name...
	 */
	if (strcmp(name,TFS_NAME(tfp)))
		return(-1);

	/* Info field...
	 */
	if (info) {
		if (strcmp(info,TFS_INFO(tfp)))
			return(-1);
	}
	else {
		if (TFS_INFO(tfp)[0] != 0)
			return(-1);
	}

	/* Flags...
	 */
	if (tfsflagsatob(flags, &bflags) == -1)
		return(-1);
	if (bflags != (TFS_FLAGS(tfp) & 0x7ff))
		return(-1);
	
	/* Data...
	 */
	if (memcmp(TFS_BASE(tfp),(char *)src,size)) 
		return(-1);

	return(0);
}

/* tfsinit():
 *	Clear out all the flash that is dedicated to the file system.
 *	This removes all currently stored files and erases the flash.
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */
int
_tfsinit(TDEV *tdpin)
{
	int		ret;
	TDEV *tdp;

	/* Step through the table of TFS devices and erase each sector...
	 */
	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		if (!tdpin || (tdp == tdpin)) {
			ret = tfsflasheraseall(tdp);
			if (ret != TFS_OKAY)
				return(ret);
		}
	}
	return(TFS_OKAY);
}

int
tfsinit(void)
{
	return(_tfsinit(0));
}


/* tfsFtot():
 *	Return the number of files in a device, or all devices if tdpin is null.
 */
int
tfsFtot(TDEV *tdpin)
{
	int		ftot;
	TFILE	*fp;
	TDEV	*tdp;

	ftot = 0;
	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		if (!tdpin || (tdpin == tdp)) {
			fp = (TFILE *)tdp->start;
			while (fp->hdrsize != ERASED16) {
				ftot++;
				fp = nextfp(fp,tdp);
			}
		}
	}
	return(ftot);
}

/* tfsFileIsOpened():
 *	Return 1 if file is currently opened; else 0.
 */
int
tfsFileIsOpened(char *name)
{
	int	 i;
	struct	tfsdat *slot;

	slot = tfsSlots;
	for (i=0;i<TFS_MAXOPEN;i++,slot++) {
		if ((slot->offset >= 0) && !strcmp(slot->hdr.name,name))
			return(1);
	}
	return(0);
}

/* tfsunopen():
 *	If the incoming file descriptor is valid, mark that file as no-longer
 *	opened and return TFS_OKAY; else return TFSERR_BADARG.
 *	descriptor.
 */
static long
tfsunopen(int fd)
{
	if ((fd < 0) || (fd >= TFS_MAXOPEN))
		return(TFSERR_BADARG);
	if (tfsSlots[fd].offset == -1)
		return(TFSERR_BADARG);
	tfsSlots[fd].offset = -1;
	return(TFS_OKAY);
}

/* tfsctrl():
 *	Provides an ioctl-like interface to tfs.
 *	Requests supported:
 *		TFS_ERRMSG:		Return error message (char *) corresponding to
 *						the incoming error number (arg1).
 *		TFS_MEMUSE:		Return the total amount of memory currently in use by
 *						TFS.
 *		TFS_MEMAVAIL:	Return the amount of memory currently avaialable for
 *						use in TFS.
 *		TFS_MEMDEAD:	Return the amount of memory currently in use by
 *						dead files in TFS.
 *		TFS_DEFRAG:		Mechanism for the application to issue
 *						a defragmentation request.
 *						Arg1: if 1, then reset after defrag is complete.
 *						Arg2: verbosity level.
 *		TFS_TELL:		Return the offset into the file specified by the
 *						incoming file descriptor (arg1).
 *		TFS_FATOB:		Return the binary equivalent of the TFS flags string
 *						pointed to by arg1.
 *		TFS_FBTOA:		Return the string equivalent of the TFS flags (long)
 *						in arg1, destination buffer in arg2.
 *		TFS_UNOPEN:		In TFS, a the data is not actually written to FLASH
 *						until the tfsclose() function is called.  This argument
 *						to tfsctrl() allows a file to be opened and possibly
 *						written to, then unopened without actually modifying
 *						the FLASH.  The value of arg1 file descriptor to 
 *						apply the "unopen" to.
 *		TFS_TIMEFUNCS:	This ctrl call is used to tell TFS what function
 *						to call for time information...
 *						Arg1 is a pointer to:
 *							(long)getLtime(void)
 *							- Get Long Time...
 *							Returns a long representation of time.
 *						Arg2 is a pointer to:
 *							(char *)getAtime(long tval,char *buf).
 *							- Get Ascii Time...
 *							If tval is zero, the buf is loaded with a string
 *							representing the current time;
 *							If tval is non-zero, then buf is loaded with a
 *							string conversion of the value of tval.
 *						Note that since it is up to these functions to 
 *						make the conversion between binary version of time
 *						and ascii version, we don't define the exact meaning
 *						of the value returne by getBtime().
 *		TFS_DOCOMMAND:	Allows the application to redefine the function
 *						that is called to process each line of a script.
 *						This is useful if the application has its own
 *						command interpreter, but wants to use the scripting
 *						facilities of the monitor.
 *						Arg1 is a pointer to the docommand function to be
 *						used instead of the standard;
 *						Arg2 is a pointer to a location into which the current
 *						docommand function pointer can be stored.
 *						If arg1 is 0, load standard docommand;
 *						if arg2 is 0, don't load old value.
 *		TFS_INITDEV:	Allows the application to initialize one of TFS's
 *						devices.  Arg1 is a pointer to the device name prefix.
 *		TFS_DEFRAGDEV:	Allows the application to defrag one of TFS's
 *						devices.  Arg1 is a pointer to the device name prefix.
 *		TFS_CHECKDEV:	Allows the application to check one of TFS's
 *						devices.  Arg1 is a pointer to the device name prefix.
 *		TFS_RAMDEV:  	Allows the application to create (or remove) a
 *						special temporary TFS device in RAM.
 *
 *
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */

long
tfsctrl(int rqst,long arg1,long arg2)
{
	long	retval, flag;
	TDEV	*tdp;
	TINFO	tinfo;

	switch(rqst) {
		case TFS_ERRMSG:
			retval = (long)tfserrmsg(arg1);
			break;
		case TFS_MEMUSE:
			tfsmemuse(0,&tinfo,0);
			retval = tinfo.memused;
			break;
		case TFS_MEMAVAIL:
			tfsmemuse(0,&tinfo,0);
			retval = tinfo.memfordata;
			break;
		case TFS_MEMDEAD:
			tfsmemuse(0,&tinfo,0);
			retval = tinfo.deadovrhd+tinfo.deaddata;
			break;
		case TFS_DEFRAG:
			for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) 
				tfsclean(tdp,(int)arg1);
			retval = 0;
			break;
		case TFS_DEFRAGON:
			retval = tfsclean_on();
			break;
		case TFS_DEFRAGOFF:
			retval = tfsclean_off();
			break;
		case TFS_UNOPEN:
			retval = tfsunopen((int)arg1);
			break;
		case TFS_FATOB:
			retval = tfsflagsatob((char *)arg1,&flag);
			if (retval == TFS_OKAY)
				retval = flag;
			break;
		case TFS_FBTOA:
			retval = (long)tfsflagsbtoa(arg1,(char *)arg2);
			if (retval == 0)
				retval = TFSERR_BADARG;
			break;
		case TFS_HEADROOM:
			retval = tfsheadroom(arg1);
			break;
		case TFS_TELL:
			retval = tfstell(arg1);
			break;
		case TFS_TIMEFUNCS:
			tfsGetLtime = (long(*)(void))arg1;
			tfsGetAtime = (char *(*)(long,char *,int))arg2;
			retval = TFS_OKAY;
			break;
		case TFS_DOCOMMAND:
			if (arg2)
				*(long *)arg2 = (long)tfsDocommand;
			if (arg1)
				tfsDocommand = (int(*)(char *,int))arg1;
			else
				tfsDocommand = docommand;
			retval = TFS_OKAY;
			break;
		default:
			retval = TFSERR_BADARG;
			break;
	}
	return(retval);
}

/* tfsNameToDevice():
 * Given the filename, return the device pointer associated with
 * that filename.  The device used depends on the prefix of
 * the incoming file name.  If the incoming prefix doesn't match
 * any of the devices in the table, then return a pointer to the
 * first device in the table (assumed to be the default).
 */
TDEV *
tfsNameToDevice(char *name)
{
	TDEV *tdp;

	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		if (!strncmp(name,tdp->prefix,strlen(tdp->prefix)))
			break;
	}
	if (tdp->start == TFSEOT)
		tdp = tfsDeviceTbl;

	return(tdp);
}

uchar *
tfsnxtdataptr(void)
{
	TFILE	*fp;
	TDEV	*tdp;

	tdp=tfsDeviceTbl;

	fp = (TFILE *)tdp->start;

	/* Find end of current storage: */
	while (fp) {
		if (fp->hdrsize == ERASED16)
			break;
		fp = nextfp(fp,tdp);
	}
	if (!fp)	/* If fp is 0, then nextfp() (above) detected corruption. */
		return(0);

	return((uchar *)(fp+1));
}

/* tfsadd():
 *	Add a file to the current list.
 *	If the file already exists AND everything is identical between the
 *	old and the new (flags, info and data), then return and do nothing;
 *	else remove the old file prior to adding the new one.
 *  New for M3 (minimal ram type) devices...
 *  This function typically assumes that the source of the file data
 *  is some contiguous block of memory, and typically its RAM because
 *  tftp and xmodem usually transfer a file to ram, then call tfsadd()
 *  to transfer the data to TFS flash.
 *  For these small-footprint devices, there may not be enough RAM to put
 *  the file in prior to doing the flash burn, so we support the ability
 *  to write the data directly to the *next* point in TFS flash that would
 *  be where the file would exist just above the header.
 *  The only thing needed in tfsadd() is for it to ignore the possiblity
 *  that the area to be written to may not be empty.
 */
int
tfsadd(char *name, char *info, char *flags, uchar *src, int size)
{
	TDEV	*tdp;
	TFILE	*fp, tf;
	long	bflags;
	ulong	endoftfsflash, nextfileaddr, thisfileaddr;
	ulong	crc_pass1, crc_pass2;
	int		ftot, err, fileexists, rc;

	if (!info) info = "";
	if (!flags) flags = "";

	/* Check for valid size and name:
	 */
	if ((size < 0) || (!name) || (*name == 0))
		return(TFSERR_BADARG);

	/* If name or info field length is too long, abort now...
	 */
	if ((strlen(name) > TFSNAMESIZE) ||
		((info) && (strlen(info) > TFSINFOSIZE)))
		return(TFSERR_NAMETOOBIG);

	/* The incoming filename cannot match any of the existing 
	 * TFS device names...
	 */
	tdp = tfsDeviceTbl;
	while(tdp->start != TFSEOT) {
		if (strcmp(tdp->prefix,name) == 0) 
			return(TFSERR_FILEEXISTS);
		tdp++;
	}

	/* If the file is currently opened, then don't allow the add...
	 */
	if (tfsFileIsOpened(name))
		return(TFSERR_FILEINUSE);

	/* If incoming flags are illegal, abort now...
	 */
	if (*flags == 0) {
		bflags = 0;
	}
	else {
		err = tfsflagsatob(flags,&bflags);
		if (err != TFS_OKAY)
			return(err);

		/* If we're adding a link, then the size better be zero...
		 */
		if ((bflags & TFS_SYMLINK) && (size != 0))
			return(TFSERR_LINKERROR);
	}

	/* If size is zero, but the request is not a link, then
	 * error...
	 */
	if ((size == 0) && ((bflags & TFS_SYMLINK) != TFS_SYMLINK))
		return(TFSERR_BADARG);

	fileexists = 0;

	/* Take snapshot of source crc. */
	crc_pass1 = crc32(src, size);

	/* Establish the device that is to be used for the incoming file
	 * addition request...
	 */
	tdp = tfsNameToDevice(name);

	fp = (TFILE *)tdp->start;

	/* Find end of current storage: */
	ftot = 0;
	while (fp) {
		if (fp->hdrsize == ERASED16)
			break;
		if (TFS_FILEEXISTS(fp)) {
			ftot++;
			if (!strcmp(TFS_NAME(fp),name)) {
				if (!(TFS_STALE(fp))) {
					/* If destination file exists, but we do not meet the
					 * user level requirements, return error now.
					 */
					if (TFS_USRLVL(fp) > getUsrLvl()) 
						return(TFSERR_USERDENIED);

					/* If file of the same name exists AND it is identical to
					 * the new file to be added, then return TFS_OKAY and be
					 * done; otherwise, set a flag so that the old one is
					 * removed prior to writing the new one.
					 * For uMon-M we don't do this compare if the source
					 * is within TFS flash space because tftp and xmodem write
					 * directly to TFS flash, then call tfsadd().
					 */
					if (!tfsspace((char *)src)) {
						if (!tfscompare(fp,name,info,flags,src,size)) {
							printf("Same data 0x%x 0x%x\n",(long)TFS_BASE(fp),(long)src);
							return(TFS_OKAY);
						}
					}
				
					fileexists = 1;
				}
			}
		}
		fp = nextfp(fp,tdp);
	}
	if (!fp)	/* If fp is 0, then nextfp() (above) detected corruption. */
		return(TFSERR_CORRUPT);

	/* Calculate location of next file (on mod16 address).  This will be 
	 * initially used to see if we have enough space left in flash to store
	 * the current request; then, if yes, it will become part of the new
	 * file's header.
	 */
	thisfileaddr = (ulong)(fp+1);
	nextfileaddr = thisfileaddr + size;
	if (nextfileaddr & 0xf)
		nextfileaddr = (nextfileaddr | 0xf) + 1;
	
	/* Make sure that the space is available for writing to flash...
	 * Remember that the end of useable flash space must take into
	 * account the fact that some space must be left over for the
	 * defragmentation state tables.  Also, the total space needed for
	 * state tables cannot exceed the size of the sector that will contain
	 * those tables.  
	 */
	if (TFS_DEVTYPE_ISRAM(tdp)) {
		endoftfsflash = tdp->end;
	}
	else {
#if INCLUDE_FLASH
		int ssize;
		ulong	state_table_overhead;

		/* The state table overhead cannot exceed one additional
		 * sector's space, so we need to check for that...
		 */
		state_table_overhead = ((ftot+1) * DEFRAGHDRSIZ) +
			(tdp->sectorcount * sizeof(struct sectorcrc));

		if (addrtosector((uchar *)(tdp->end),0,&ssize,0) < 0)
			return(TFSERR_MEMFAIL);

#if DEFRAGTYPE_PWRSAFE
		if (state_table_overhead >= (ulong)ssize)
			return(TFSERR_DSIMAX);
#endif

		endoftfsflash = (tdp->end + 1) - state_table_overhead;
#else
		return(TFSERR_NOTAVAILABLE);
#endif
	}

	if ((nextfileaddr >= endoftfsflash) || (nextfileaddr < thisfileaddr))
		return(TFSERR_FLASHFULL);

	if (!flasherased((uchar *)fp,(uchar *)fp + (size+TFSHDRSIZ))) {
		if ((uchar *)(fp+1) != (uchar *)src)
			return(TFSERR_FLASHFULL);
	}

	memset((char *)&tf,0,TFSHDRSIZ);

	/* Do another crc on the source data.  If crc_pass1 != crc_pass2 then
	 * somehow the source is changing.  This is typically caused by the fact
	 * that the source address is within TFS space that was automatically
	 * defragmented above.  There is no need to check source data if the
	 * source is in-place-modifiable.
	 */
	crc_pass2 = crc32(src,size);
	if (crc_pass1 != crc_pass2)
		return(TFSERR_FLAKEYSOURCE);

	/* Now that we have determined that we have enough space to do the 
	 * copy, if the "fileexists" flag was set (indicating that there is already
	 * a file in TFS with the same name as the incoming file), we must now
	 * remove it...
	 */
	if (fileexists) {
		err = _tfsunlink(name);
		if (err != TFS_OKAY) {
			printf("%s: %s\n",name,tfserrmsg(err));
			return(err);
		}
	}

	/* Copy name and info data to header.
	 */
	strcpy(tf.name, name);
	strcpy(tf.info, info);
	tf.hdrsize = TFSHDRSIZ;
	tf.hdrvrsn = TFSHDRVERSION;
	tf.filsize = size;
	tf.flags = bflags;
	tf.flags |= (TFS_ACTIVE | TFS_NSTALE);
	tf.filcrc = crc_pass2;
	tf.modtime = tfsGetLtime();
	{
	int i;
	for(i=0;i<4;i++)
		tf.altflags[i] = 0xffffffff;
	}
	tf.next = 0;
	tf.hdrcrc = 0;
	tf.hdrcrc = crc32((uchar *)&tf,TFSHDRSIZ);
	tf.next = (TFILE *)nextfileaddr;

	/* Now copy the file and header to flash.
	 * Note1: the header is copied AFTER the file has been
	 * successfully copied.  If the header was written successfully,
	 * then the data write failed, the header would be incorrectly
	 * pointing to an invalid file. To avoid this, simply write the
	 * data first.
	 * Note2: if the file is in-place-modifiable, then there is no
	 * file data to be written to the flash.  It will be left as all FFs
	 * so that the flash can be modified by tfsipmod() later.
	 */

	/* Write the file to flash:
	 */
	if (TFS_DEVTYPE_ISRAM(tdp))
		memcpy((char *)(fp+1),(char *)src,size);
	else {
		rc = tfsflashwrite((uchar *)(fp+1),(uchar *)src,size);
		if (rc != TFS_OKAY)
			return(rc);
	}

	/* Write the file header to flash:
	 */
	if (TFS_DEVTYPE_ISRAM(tdp))
		memcpy((char *)fp,(char *)&tf,TFSHDRSIZ);
	else {
		rc = tfsflashwrite((uchar *)fp,(uchar *)(&tf),TFSHDRSIZ);
		if (rc != TFS_OKAY)
			return(rc);
	}

	/* Double check the CRC now that it is in flash.
	 */
	if (crc32((uchar *)(fp+1), size) != tf.filcrc)
		return(TFSERR_BADCRC);

	return(TFS_OKAY);
}

/* tfsunlink():
 *	Delete a file from the current list of files. Note that there
 *	is no attempt to de-fragment the flash; it simply nulls out the flags
 *	field of the file.  If successful return 0; else return error number.
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */
int
tfsunlink(char *name)
{
	/* If the file is currently opened, then don't allow the deletion...
	 */
	if (tfsFileIsOpened(name))
		return(TFSERR_FILEINUSE);

	return(_tfsunlink(name));
}

int
_tfsunlink(char *name)
{
	int		rc;
	TFILE	*fp;
	TDEV	*tdp;
	ulong	flags_marked_deleted;

	fp = _tfsstat(name);
	if (!fp)
		return(TFSERR_NOFILE);

	if (TFS_USRLVL(fp) > getUsrLvl())
		return(TFSERR_USERDENIED);

	flags_marked_deleted = fp->flags & ~TFS_ACTIVE;

	tdp = tfsNameToDevice(name);
	if (TFS_DEVTYPE_ISRAM(tdp))
		memcpy((char *)&fp->flags,(char *)&flags_marked_deleted,sizeof(long));
	else {
#ifdef USE_16BYTE_ERASE
        rc = tfsflashwrite((uchar *)&fp->altflags,
            (uchar *)&flags_marked_deleted, sizeof(fp->altflags));
#else
		rc = tfsflashwrite((uchar *)&fp->altflags[0],
			(uchar *)&flags_marked_deleted, sizeof(long));
#endif
		if (rc != TFS_OKAY)
			return(rc);
	}

	return (TFS_OKAY);
}
			
static void
pre_tfsautoboot_hook(void)
{
#ifdef PRE_TFSAUTOBOOT_HOOK
	extern void PRE_TFSAUTOBOOT_HOOK();

	if (tfsMonrcActive == 0)
	{
		PRE_TFSAUTOBOOT_HOOK();
	}
#endif
}

/*
 * tfsrun_abortableautoboot():
 *
 * Aborting execution of the monrc & non-query-autoboot files:
 * The code wrapped within the #ifdef TFS_AUTOBOOT_ABORTABLE definition
 * is an implementation of an idea from Jason DiDonato (Jan 2004).
 * These files can be aborted by an escape character under two
 * circumstances:
 *
 *		1. There is no password file installed
 *		2. The user correctly enters the level-three password when
 *		   prompted by the abort interaction below.
 *
 * This mechanism maintains security, but only when security is desired.
 * A system that needs security will have a password file installed;
 * hence, if no password file is found, the escape character
 * (AUTOBOOT_ABORT_CHAR) is all that is needed at startup to abort an
 * otherwise non-abortable file.
 * If a password file is found, then the user must know the level-3 password
 * to abort the execution; otherwise, after a few seconds, the interaction
 * will terminate and the file will be executed.
 *
 * Prior to this implementation, autobootables were not abortable in any
 * way.  This is still the default, which is overridden by the definition
 * of TFS_AUTOBOOT_ABORTABLE in config.h
 */


#define AUTOBOOT_ABORT_NULL		0
#define AUTOBOOT_ABORT_NO		1
#define AUTOBOOT_ABORT_YES		2
#define AUTOBOOT_ABORT_FAILED	3

#ifndef AUTOBOOT_ABORT_CHAR
#ifdef TFS_AUTOBOOT_CANCEL_CHAR
#define AUTOBOOT_ABORT_CHAR		TFS_AUTOBOOT_CANCEL_CHAR
#else
#define AUTOBOOT_ABORT_CHAR		0x03	/* CTRL-C */
#endif
#endif

int
tfsrun_abortableautoboot(char **arglist,int verbose)
{
#ifdef TFS_AUTOBOOT_ABORTABLE
	int	err;
	static int	autoboot_abort;

	/* If a character has been detected at the console, and the
	 * character is AUTOBOOT_ABORT_CHAR, then, if a password file
	 * exists, require that the user enter the level-3 password
	 * to abort the autoboot file...
	 * Note that this is only done on the first pass through this
	 * function.  All subsequent passes use the result of the initial
	 * pass.
	 */
	if (autoboot_abort == AUTOBOOT_ABORT_NULL) {
		autoboot_abort = AUTOBOOT_ABORT_NO;

		if (gotachar() && (getchar() == AUTOBOOT_ABORT_CHAR)) {
			autoboot_abort = AUTOBOOT_ABORT_YES;
		}
	}

	err = TFS_OKAY;

	switch(autoboot_abort) {
		case AUTOBOOT_ABORT_NO:
			pre_tfsautoboot_hook();
			err = tfsrun(arglist,verbose);
			break;
		case AUTOBOOT_ABORT_YES:
			printf("%s aborted\n",arglist[0]);
			break;
		case AUTOBOOT_ABORT_FAILED:
			printf("%s abort attempt failed\n",arglist[0]);
			pre_tfsautoboot_hook();
			err = tfsrun(arglist,verbose);
			break;
	}
	return(err);
#else
	pre_tfsautoboot_hook();
	return(tfsrun(arglist,verbose));
#endif	/* TFS_AUTOBOOT_ABORTABLE */
}


/* tfsrun():
 *	Run the named file.  Based on the file flags, the file is either
 *	executed as a COFF/ELF file with all relocation data in the file
 *	or run as a simple script of monitor commands.
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */

int
tfsrun(char **arglist,int verbose)
{
	int		i, err;
	TFILE	fstat, *fp;
	char	*name;

	name = arglist[0];
	fp = tfsstat(name);
	
	if (!fp)
		return (TFSERR_NOFILE);

	tfsfstat(name,&fstat);

	if (TFS_USRLVL(fp) > getUsrLvl())
		return(TFSERR_USERDENIED);

#if INCLUDE_TFSSCRIPT
	/* Store away the argument list so that it is accessible by the script
	 * or executable application about to be run:
	 */
	for(i=0;arglist[i];i++)
		putargv(i,arglist[i]);
	putargv(i,(char *)0);
#endif

	/* Executable file can be script or binary...
	 */
	if (!(fp->flags & (TFS_EXEC)))
		return(TFSERR_NOTEXEC);

	if (crc32((uchar *)(TFS_BASE(fp)), fp->filsize) != fp->filcrc)
		return(TFSERR_BADCRC);
	/* Machine code or script...
	 * If machine code, then block it if monrc is active.
	 */
	err = tfsscript(&fstat,verbose);

	return(err);
}

/* tfsrunrcfile():
 * Called at system startup to run the monitor's run-control file (monrc).
 * Three attempts are made to find the monrc file...
 * 1. Call tfsstat("monrc")
 * 2. Call tfsstat("monrc.bak")
 * 3. For each device, prepend the device prefix to the monrc name
 *	  and run tfsstat() on that string (i.e.. tfsstat(//A/monrc)).
 *	  This supports a multi-device system in which the monrc file is not
 *	  in the first TFS device.
 *
 * If one of the above is found, it will be automatically executed.
 * Note that only the first one found will be executed.
 * If the file is flagged as autobootable, post a warning message.
 * This lets the user know that the monrc file is going to be run twice
 * (probably not what they want, so when the warning is seen, the flag
 * should be changed by the user).
 *
 */

void
tfsrunrcfile(void)
{
	int		err;
	TDEV	*tdp;
	TFILE	*tfp;
	char	*arglist[2], bigname[TFSNAMESIZE+1], *name;
	
#if TFS_RUN_DISABLE
	return;
#endif

	if (!tfsInitialized) {
		printf("TFS not initialized, tfsrunrcfile aborting.\n");
		return;
	}

	name = TFS_RCFILE;
	tfp = tfsstat(name);
	if (!tfp) {
		name = TFS_RCFILE ".bak";
		tfp = tfsstat(name);
	}
	if (!tfp) {
		for(tdp=tfsDeviceTbl; (tdp->start != TFSEOT) ; tdp++) {
			sprintf(bigname,"%smonrc",tdp->prefix);
			if ((tfp = tfsstat(bigname))) {
				name = bigname;
				break;
			}
		}
	}
	if (!tfp)
		return;

	if (TFS_FLAGS(tfp) & (TFS_BRUN | TFS_QRYBRUN))
		printf("Warning: %s is autobootable\n",name);

	arglist[0] = name;
	arglist[1] = (char *)0;

	tfsMonrcActive = 1;
	err = tfsrun_abortableautoboot(arglist,0);
	tfsMonrcActive = 0;
	if (err != TFS_OKAY)
		printf("%s: %s\n",name,tfserrmsg(err));
	return;
}

int
tfsRunningMonrc(void)
{
	return(tfsMonrcActive);
}

/* tfsnext():
 *	Called to retrieve the "next" file in the tfs list.  If
 *	incoming argument is NULL then return the first file in the list.  If no
 *	more files, return NULL; else return the tfshdr structure pointer to the
 *	next (or first) file in the tfs.
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */
TFILE *
tfsnext(TFILE *fp)
{
	TDEV	*tdp;
	TFILE *fpnext;

	if (!fp) {
		tdp = tfsDeviceTbl;
		fpnext = (TFILE *) tfsDeviceTbl[0].start;
	}
	else {
		tdp = gettfsdev(fp);
		fpnext = nextfp(fp,0);
	}

	while(tdp->start != TFSEOT) {
		while(validtfshdr(fpnext)) {
			if (TFS_FILEEXISTS(fpnext))
				return (fpnext);
			fpnext = nextfp(fpnext,0);
		}
		tdp++;
		fpnext = (TFILE *)tdp->start;
	}
	return ((TFILE *) 0);
}

/* tfsstat():
 *	Steps through the list of files until it finds the specified
 *	filename or reaches the end of the list.  If found, a pointer to that
 *	file's structure is returned; else return 0.
 *	MONLIB NOTICE: this function is accessible through monlib.c.
 */
TFILE *
tfsstat(char *name)
{
	if (!tfsInitialized) {
		printf("TFS not initialized, tfsstat aborting.\n");
		return(0);
	}

	return(_tfsstat(name));
}

TFILE *
_tfsstat(char *name)
{
	int     len;
	TFILE   *fp;
	TDEV    *tdp;
	char    *prefix, *dotslash = "./";

	/* Account for the possibility that the filename might have the
	 * device name prefixed for the first device in the table (or "./").
	 */
	tdp = &tfsDeviceTbl[0];
	if (strncmp(name,tdp->prefix,strlen(tdp->prefix)) == 0) {
		len = strlen(tdp->prefix);
		prefix = tdp->prefix;
	}
	else if (strncmp(name,dotslash,2) == 0) {
		len = 2;
		prefix = dotslash;
	}
	else {
		len = 0;
		prefix = 0;
	}

	if (prefix) {
		fp = (TFILE *) tdp->start;
		while(validtfshdr(fp)) {
			if (TFS_FILEEXISTS(fp) && (strcmp(name+len, fp->name) == 0)) {
				return(fp);
			}
			fp = nextfp(fp,tdp);
		}
	}

	/* Then, if not found, walk through all TFS devices normally...
	 */
	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		fp = (TFILE *) tdp->start;
		while(validtfshdr(fp)) {
			if (TFS_FILEEXISTS(fp) && (strcmp(name, fp->name) == 0)) {
				return(fp);
			}
			fp = nextfp(fp,tdp);
		}
	}
	return ((TFILE *) 0);
}

/* tfsfstat():
 * Very similar in purpose to tfsstat().  This version is provided to the 
 * API as a "defrag-safe" version of tfsstat()...
 * If tfsstat() is called (returning a pointer into TFS memory space), then
 * a defragmentation occurs, that pointer is stale; hence, the need for 
 * an alternative that will load the content of the TFILE structure into
 * an application-supplied block of memory (usually a pointer to a local
 * TFILE structure).  Using tfsfstat avoids this because if a defrag occurs,
 * it does not affect the content of the locally stored TFILE structure.
 * NOTE:
 * addition of this function to the TFS API was due to the fact that
 * I did not consider the above described condition when first adding
 * tfsstat() to the TFS API.  In general, tfsfstat() should be considered
 * a replacement for all tfsstat() situations that will dereference the
 * pointer.
 * NOTE1:
 * The return value is similar to standard "stat"... Return 0 if
 * successful, else -1.
 */
int
tfsfstat(char *name, TFILE *apptfp)
{
	TFILE	*tfp = tfsstat(name);
	
	if (!tfp)
		return(-1);

	memcpy((char *)apptfp,(char *)tfp,sizeof(TFILE));
	return(0);
}

int
showTfsError(int errno, char *msg)
{
	if (errno == TFS_OKAY)
		return(TFS_OKAY);

	if (msg)
		printf("%s: %s\n",msg,tfserrmsg(errno));
	else
		printf("%s\n",tfserrmsg(errno));
	return(errno);
}

int
tfsclean_on(void)
{
	TfsCleanEnable++;
	return(TfsCleanEnable);
}

int
tfsclean_off(void)
{
	TfsCleanEnable--;
	return(TfsCleanEnable);
}

/* tfsclean():
 * Wrapper to the underlying _tfsclean() function.
 * The original intent of this was to enable a call to appexit() if there
 * is an error returned by tfsclean().  This is used for testing TFS...
 * If a script is running to try to cause tfsclean to break, then we want
 * it to halt if tfsclean does return an error.


 * As of uMon1.0, if this function detects that the device is volatile
 * RAM, then the clean is not powersafe; hence, doesn't have to go
 * through the rigorous sector-based defragmentation.
 */
int
tfsclean(TDEV *tdp,int verbose)
{
	TFILE *tfp, *tfp1, *tfpnxt;
	int	cleanresult, size;
	char *cp;

	if (TFS_DEVTYPE_ISRAM(tdp)) {
		cp = 0;
		tfp = (TFILE *)tdp->start;
		while(validtfshdr(tfp)) {
			tfpnxt = nextfp(tfp,tdp);
			if (TFS_DELETED(tfp)) {
				if (cp == 0)
					cp = (char *)tfp;
			}
			else {
				if (cp != 0) {
					size = TFS_SIZE(tfp) + TFSHDRSIZ;
					if (size & 0xf)
						size = (size | 0xf) + 1;
					memcpy(cp,(char *)tfp,size);
					tfp1 = (TFILE *)cp;
					tfp1->next = (TFILE *)(cp+size);
					tfp1->hdrcrc = tfshdrcrc(tfp1);
					cp += size;
				}
			}
			tfp = tfpnxt;
		}
		if (cp)
			memset(cp,0xff,(char *)(tdp->end)-cp);
		cleanresult = TFS_OKAY;
	}
	else {
		cleanresult = _tfsclean(tdp,0,verbose);
		if (cleanresult != TFS_OKAY) {
			if (getenv("APP_EXITONCLEANERROR"))
				appexit(0);
#if INCLUDE_TFSSCRIPT
			if (getenv("SCR_EXITONCLEANERROR"))
				ScriptExitFlag = EXIT_SCRIPT;
#endif
		}
	}

	return(cleanresult);
}

#else	/* INCLUDE_TFS */

char *
tfserrmsg(int errno)
{
	return(0);
}
int
tfsinit(void)
{
	return(TFSERR_NOTAVAILABLE);
}

int
tfsfstat(char *name, TFILE *apptfp)
{
	return(TFSERR_NOTAVAILABLE);
}

TFILE *
tfsstat(char *name)
{
	return ((TFILE *) 0);
}

TFILE *
tfsnext(TFILE *fp)
{
	return ((TFILE *) 0);
}

int
tfsrun(char **arglist,int verbose)
{
	return(TFSERR_NOTAVAILABLE);
}

int
tfsunlink(char *name)
{
	return(TFSERR_NOTAVAILABLE);
}

int
tfsadd(char *name, char *info, char *flags, uchar *src, int size)
{
	return(TFSERR_NOTAVAILABLE);
}

long
tfsctrl(int rqst,long arg1,long arg2)
{
	return(TFSERR_NOTAVAILABLE);
}

long
tfstell(int fd)
{
	return(TFSERR_NOTAVAILABLE);
}

int
tfsRunningMonrc(void)
{
	return(0);
}

int
tfsspace(char *addr)
{
	return(0);
}

#endif	/* INCLUDE_TFS else */
