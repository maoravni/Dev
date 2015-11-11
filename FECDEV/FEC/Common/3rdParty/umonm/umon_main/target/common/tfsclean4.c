/* tfsclean4.c:
 *	This is a non-powersafe version of tfsclean() that uses a block of
 *	memory (can be configured as RAM or a flash sector depending on
 *  the DEFRAGTYPE_NNN macro setting (see below).
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
 *	Email:				ed.sutter@alcatel-lucent.com
 *	Phone:				908-582-2351
 */
#include "config.h"
#include "cpu.h"
#include "stddefs.h"
#include "genlib.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "flash.h"
#include "warmstart.h"

#if INCLUDE_TFS

#if ((DEFRAGTYPE_NPWRSAFE_SPARE + \
	  DEFRAGTYPE_NPWRSAFE_APPRAMBASE + \
	  DEFRAGTYPE_NPWRSAFE_RAMBLOCK + \
	  DEFRAGTYPE_PWRSAFE) != 1)
#error: Too many DEFRAGTYPEs enabled, pick one.
#endif

#if DEFRAGTYPE_PWRSAFE
#error: tfsclean4  is not powersafe.
#elif DEFRAGTYPE_NPWRSAFE_SPARE
#elif DEFRAGTYPE_NPWRSAFE_APPRAMBASE
#elif DEFRAGTYPE_NPWRSAFE_RAMBLOCK
static long	spareInRAM[FLASH_LARGEST_SECTOR/sizeof(long)];
#endif

static char *spareBase;

#if !INCLUDE_FLASH
#error: If flash is not included, then use tfsclean2.c
#endif

/* defragSerase():
 * Common function to call from within tfsclean() to erase a sector
 * and generate an error message if necessary.
 */
static int
defragSerase(int tag, int snum)
{
	int	ret = 0;

	ret = tfsflasherase(snum);
	if (ret <= 0) {
		printf("tfsclean() serase erase failed: %d,%d,%d\n",snum,tag,ret);
	}
	return(ret);
}

/* defragFwrite():
 * Common function to call from within tfsclean() to write to flash
 * and generate an error message if necessary.
 */
static int
defragFwrite(int tag, uchar *dest,uchar *src,int size)
{
	int	ret = 0;

	ret = tfsflashwrite(dest,src,size);
	if (ret != TFS_OKAY) {
		printf("tfsclean() fwrite failed: 0x%lx,0x%lx,%d,%d\n",
			(ulong)dest,(ulong)src,size,tag);
	}
	return(ret);
}

/* defragGetSpantype():
 * With the incoming sector base and end (s_base, s_end),
 * determine the type of span that the incoming file (f_base, f_end)
 * has across it.  There are six different ways the spanning can
 * occur:
 *   1. begin and end in previous active sector (bpep);
 *   2. begin in previously active sector, end in this one (bpec);
 *   3. begin in previously active sector, end in later one (bpel);
 *   4. begin and end in this active sector (bcec);
 *   5. begin in this active sector, end in later one (bcel);
 *   6. begin and end in later active sector (blel);
 */
static int
defragGetSpantype(char *s_base,char *s_end,char *f_base,char *f_end)
{
	int	spantype;

	if (f_base < s_base) {
		if ((f_end > s_base) && (f_end <= s_end))
			spantype = SPANTYPE_BPEC;
		else if (f_end > s_end)
			spantype = SPANTYPE_BPEL;
		else
			spantype = SPANTYPE_BPEP;
	}
	else {
		if (f_base > s_end)
			spantype = SPANTYPE_BLEL;
		else if (f_end <= s_end)
			spantype = SPANTYPE_BCEC;
		else
			spantype = SPANTYPE_BCEL;
	}
	return(spantype);
}

/* defragGetSpantypeStr():
 * Return a string that corresponds to the incoming state value.
 */
static char *
defragGetSpantypeStr(int spantype)
{
	char *str;

	switch(spantype) {
	case SPANTYPE_BPEC:
		str = "BPEC";
		break;
	case SPANTYPE_BLEL:
		str = "BLEL";
		break;
	case SPANTYPE_BPEL:
		str = "BPEL";
		break;
	case SPANTYPE_BPEP:
		str = "BPEP";
		break;
	case SPANTYPE_BCEC:
		str = "BCEC";
		break;
	case SPANTYPE_BCEL:
		str = "BCEL";
		break;
	default:
		str = "???";
		break;
	}
	return(str);
}

#if DEFRAGTYPE_NPWRSAFE_SPARE
/* defragEraseSpare():
 * Erase the spare sector associated with the incoming TFS device.
 * The underlying flash driver SHOULD have a check so that it only
 * erases the sector if the sector is not already erased, so this
 * extra check (call to flasherased()) may not be necessary in
 * most cases.
 */
static int
defragEraseSpare(TDEV *tdp)
{
	int	snum, ssize;
	uchar *sbase;

	if (addrtosector((unsigned char *)spareBase,&snum,&ssize,&sbase) < 0)
		return(TFSERR_FLASHFAILURE);

	if (!flasherased(sbase,sbase+(ssize-1))) {
		if (defragSerase(1,snum) < 0) {
			return(TFSERR_FLASHFAILURE);
		}
	}
	return(TFS_OKAY);
}
#endif

#if 0
/* defragGetStateStr():
 * Return a string that corresponds to the incoming state value.
 */
static char *
defragGetStateStr(int state)
{
	char *str;

	switch(state) {
	case SECTOR_DEFRAG_INACTIVE:
		str = "SectorDefragInactive";
		break;
	case SECTOR_DEFRAG_ABORT_RESTART:
		str = "DefragRestartAborted";
		break;
	case SCANNING_ACTIVE_SECTOR_1:
		str = "ScanningActiveSector1";
		break;
	case SCANNING_ACTIVE_SECTOR_2:
		str = "ScanningActiveSector2";
		break;
	case SCANNING_ACTIVE_SECTOR_3:
		str = "ScanningActiveSector3";
		break;
	case SCANNING_ACTIVE_SECTOR_4:
		str = "ScanningActiveSector4";
		break;
	case SCANNING_ACTIVE_SECTOR_5:
		str = "ScanningActiveSector5";
		break;
	case SECTOR_DEFRAG_ALMOST_DONE:
		str = "DefragAlmostDone";
		break;
	default:
		str = "???";
		break;
	}
	return(str);
}

/* defragRestart():
 * Poll the console allowing the user to abort the auto-restart of
 * the defragmentation.  If a character is received on the console,
 * then return 0 indicating that the defrag should not be restarted;
 * else return 1.
 */
int
defragRestart(int state,int snum)
{
	printf("TFS defrag restart state: %s sector %d\n",
		defragGetStateStr(state),snum);
	if (pollConsole("Hit any key to abort..."))
		return(0);
	return(1);
}
#endif

/* inSector():
 * We are trying to figure out if the address space that we want to copy
 * from is within the active sector.  If it is, then we need to adjust
 * our pointers so that we retrieve the at least some of data from the
 * spare. 
 * If the range specified by 'i_base' and 'i_size' overlays (in any way)
 * the address space used by the sector specified by 'snum', 
 * then return the address in the spare and the size of the overlay.
 */
static int
inSector(TDEV *tdp,int snum,uchar *i_base,int i_size,uchar **saddr,int *ovlysz)
{
	int		s_size;
	uchar	*s_base, *s_end, *i_end;

	/* Retrieve information about the sector: */
	if (sectortoaddr(snum,&s_size,&s_base) == -1)
		return(TFSERR_MEMFAIL);

	i_end = i_base + i_size;
	s_end = s_base + s_size;

	if ((i_end < s_base) || (i_base > s_end)) {
		*ovlysz = 0;
		return(0);
	}

	if (i_base < s_base) {
		if (i_end > s_end) {
			*ovlysz = s_size;
		}
		else {
			*ovlysz = (i_size - (s_base - i_base));
		}
		*saddr = (uchar *)spareBase;
	}
	else {
		if (i_end > s_end) {
			*ovlysz = (i_size - (i_end - s_end));
		}
		else {
			*ovlysz = i_size;
		}
		*saddr = (uchar *)spareBase + (i_base - s_base);
	}
	return(0);
}

/* struct fillinfo & FILLMODE definitions:
 * Structure used by the "Fill" functions below.
 */
#define FILLMODE_FWRITE			1	/* Do the flash write */
#define FILLMODE_SPAREOVERLAP	2	/* Determine if there is SPARE overlap */
#define FILLMODE_CRCONLY		3	/* Calculate a 32-bit crc on the data */

struct fillinfo {
	struct defraghdr *dhp;	/* pointer to defrag header table */
	TDEV	*tdp;			/* pointer to TFS device */
	ulong	crc;			/* used in FILLMODE_CRCONLY mode */
	int		crcsz;			/* size of crc calculation */
	int		fhdr;			/* set if we're working on a file header */
	int		asnum;			/* the active sector */
	int		mode;			/* see FILLMODE_xxx definitions */
};

/* defragFillFlash():
 * This function is called by the defragFillActiveSector() function
 * below.  It covers the four different cases of a file spanning over
 * the active sector, plus it deals with the possibility that the source
 * of the file data may be the same sector as the active one (meaning that
 * the source is taken from the spare).  It is within this function that
 * the active sector is actually modified and it assumes that the portion
 * of the active sector to be written to is already erased.
 * 
 * 
 * SPANTYPE_BCEC:
 * In this case, the file starts in the active sector and ends in
 * the active sector...
 * -----------|----------|----------|----------|---------|---------|----------
 * |          |          |          |          |         |         |         |
 * |          |          |<-active->|          |         |         |  SPARE  |
 * |          |          |  sector  |          |         |         |  SECTOR |
 * |          |          |          |          |         |         |         |
 * |          |          | newfile  |          |         |         |         |
 * |          |          | |<-->|   |          |         |         |         |
 * -----------|----------|----------|----------|---------|---------|----------
 *
 * 
 * SPANTYPE_BPEC:
 * In this case, the file starts in a sector prior to the currently active
 * sector and ends in the active sector...
 * -----------|----------|----------|----------|---------|---------|----------
 * |          |          |          |          |         |         |         |
 * |          |          |          |<-active->|         |         |  SPARE  |
 * |          |          |          |  sector  |         |         |  SECTOR |
 * |          |          |          |          |         |         |         |
 * |          |      |<----newfile----->|      |         |         |         |
 * |          |          |          |          |         |         |         |
 * -----------|----------|----------|----------|---------|---------|----------
 *
 * 
 * SPANTYPE_BPEL:
 * In this case, the file starts in some sector prior to the currently
 * active sector and ends in some sector after the currently active
 * sector...
 * -----------|----------|----------|----------|---------|---------|----------
 * |          |          |          |          |         |         |         |
 * |          |          |<-active->|          |         |         |  SPARE  |
 * |          |          |  sector  |          |         |         |  SECTOR |
 * |          |          |          |          |         |         |         |
 * |       |<---------- newfile------------------->|     |         |         |
 * |          |          |          |          |         |         |         |
 * -----------|----------|----------|----------|---------|---------|----------
 *
 *
 * SPANTYPE_BCEL:
 * In this case, the file starts in the active sector and ends in
 * a later sector.
 * -----------|----------|----------|----------|---------|---------|----------
 * |          |          |          |          |         |         |         |
 * |          |<-active->|          |          |         |         |  SPARE  |
 * |          |  sector  |          |          |         |         |  SECTOR |
 * |          |          |          |          |         |         |         |
 * |          |      |<----newfile----->|      |         |         |         |
 * |          |      ****|          |          |         |         |         |
 * -----------|----------|----------|----------|---------|---------|----------
 */
static int
defragFillFlash(struct fillinfo *fip,int spantype,char **activeaddr,int verbose)
{
	char 	*hp;
	TFILE	nfhdr;
	struct	defraghdr *dhp;	
	int		ohdroffset, nhdroffset;
	uchar	*ovly, *src, *activesbase;
	int		ovlysz, srcsz, activessize;

	src = 0;
	ovly = 0;
	srcsz = 0;
	nhdroffset = ohdroffset = 0;
	dhp = fip->dhp;

	if (verbose >= 2) {
		printf("   defragFillFlash %s %s (%s %d)\n",fip->fhdr ? "hdr" : "dat",
			defragGetSpantypeStr(spantype), dhp->fname,fip->asnum);
	}

	if (spantype == SPANTYPE_BCEC) {
		if (fip->fhdr) {
			src = (uchar *)dhp->ohdr;
			srcsz = TFSHDRSIZ;
		}
		else {
			src = (uchar *)dhp->ohdr+TFSHDRSIZ;
			srcsz = dhp->filsize;
		}
	}
	else if (spantype == SPANTYPE_BPEC) {
		if (fip->fhdr) {
			/* Calculate the offset into the header at which point a
			 * sector boundary occurs.  Do this for both the old (before
			 * defrag relocation) and new (after defrag relocation)
			 * location of the header.
			 */
			/* Changed as of Dec 2010, based on error found by Leon...
			 * We need to figure out how the header overlaps the sector
			 * boundary.  Prior to 12/2010, this code did not account for
			 * the case where the file size spans beyond the currently
			 * active sector.
			 */
			if ((dhp->neso > dhp->filsize) && (dhp->oeso > dhp->filsize)) {
				nhdroffset = TFSHDRSIZ - (dhp->neso - dhp->filsize);
				ohdroffset = TFSHDRSIZ - (dhp->oeso - dhp->filsize);
				srcsz = (dhp->oeso - dhp->filsize) + (ohdroffset - nhdroffset);
				src = (uchar *)dhp->ohdr + nhdroffset;
			}
			else {
				int ssz;
				int sno = dhp->nesn;
				int fsz = dhp->filsize;
				while(sno > fip->asnum) {
					if (sectortoaddr(sno,&ssz,0) < 0)
						return(TFSERR_MEMFAIL);
					if (fsz == dhp->filsize)
						fsz -= dhp->neso;
					else
						fsz -= ssz;
					sno--;
				}
				if (sectortoaddr(fip->asnum,&ssz,0) < 0)
					return(TFSERR_MEMFAIL);
				srcsz = ssz - fsz;
				src = (uchar *)dhp->ohdr;
				src += (TFSHDRSIZ-srcsz);
				nhdroffset = (TFSHDRSIZ-srcsz);
			}
		}
		else {
			src = (uchar *)dhp->ohdr + TFSHDRSIZ + (dhp->filsize - dhp->neso);
			srcsz = dhp->neso;
		}
	}
	else if (spantype == SPANTYPE_BCEL) {
		if (sectortoaddr(fip->asnum,&activessize,&activesbase) == -1)
			return(TFSERR_MEMFAIL);

		if (fip->fhdr) {
			src = (uchar *)dhp->ohdr;
		}
		else {
			src = (uchar *)dhp->ohdr+TFSHDRSIZ;
		}
		srcsz = (activesbase + activessize) - (uchar *)*activeaddr;
	}
	else if (spantype == SPANTYPE_BPEL) {
		if (sectortoaddr(fip->asnum,&activessize,0) == -1)
			return(TFSERR_MEMFAIL);

		if (fip->fhdr) {
			src = (uchar *)dhp->ohdr;
		}
		else {
			src = (uchar *)dhp->ohdr+TFSHDRSIZ;
		}

		src += ((*activeaddr - dhp->nda) - TFSHDRSIZ);
		srcsz = activessize; 
	}
	else {
		return(0);
	}

	/* Do some error checking on the computed size:
	 */
	if (srcsz < 0) {
		printf("defragFillFlash: srcsz < 0\n");
		return(TFSERR_MEMFAIL);
	}

	if (fip->fhdr) {
		if (srcsz > TFSHDRSIZ) {
			printf("defragFillFlash: srcsz > TFSHDRSIZ\n");
			return(TFSERR_MEMFAIL);
		}
	}
	else {
		if (srcsz > dhp->filsize) {
			printf("defragFillFlash: srcsz > filsize\n");
			return(TFSERR_MEMFAIL);
		}
	}

	/* Determine if any portion of the source was part of the sector that
	 * is now the active sector..  If yes (ovlysz > 0), then we must
	 * deal with the fact that some (or all) of the fill source is in the
	 * spare sector...
	 */
	if (inSector(fip->tdp,fip->asnum,src,srcsz,&ovly,&ovlysz) < 0)
		return(TFSERR_MEMFAIL);

	/* If the mode is not FILLMODE_FWRITE, then we don't do any of the
	 * flash operations.  We are in this function only to determine
	 * if we need to copy the active sector to the spare prior to 
	 * starting the modification of the active sector.
	 */
	if (fip->mode == FILLMODE_FWRITE) {
		if (fip->fhdr) {
			hp = (char *)&nfhdr;
			if (ovlysz) {
				memcpy((char *)hp+nhdroffset,(char *)ovly,ovlysz);
				if (ovlysz != srcsz) {
					memcpy(hp+nhdroffset+ovlysz,(char *)src+ovlysz,
						srcsz-ovlysz);
				}
			}
			else {
				/* next line was <nfhdr = *dhp->ohdr> ... */
				memcpy((char *)&nfhdr,(char *)dhp->ohdr,sizeof(TFILE));
			}
			nfhdr.next = dhp->nextfile;
			if (defragFwrite(2,(uchar *)*activeaddr,(uchar *)hp+nhdroffset,srcsz) == -1)
				return(TFSERR_FLASHFAILURE);
		}
		else {
			if (ovlysz) {
				if (defragFwrite(3,(uchar *)*activeaddr,(uchar *)ovly,ovlysz) == -1)
					return(TFSERR_FLASHFAILURE);
				if (ovlysz != srcsz) {
					if (defragFwrite(4,(uchar *)*activeaddr+ovlysz,(uchar *)src+ovlysz,
						srcsz-ovlysz) == -1)
						return(TFSERR_FLASHFAILURE);
				}
			}
			else {
				if (defragFwrite(5,(uchar *)*activeaddr,(uchar *)src,srcsz) == -1)
					return(TFSERR_FLASHFAILURE);
			}
		}
	}
	else if (fip->mode == FILLMODE_CRCONLY) {
		register uchar	*bp;
		int		sz, temp;

		if (fip->fhdr) {
			hp = (char *)&nfhdr;
			/* next line was <nfhdr = *dhp->ohdr> ... */
			memcpy((char *)&nfhdr,(char *)dhp->ohdr,sizeof(TFILE));
			nfhdr.next = dhp->nextfile;
			bp = (uchar *)hp + nhdroffset;
		}
		else {
			bp = (uchar *)src;
		}
		sz = srcsz;
		fip->crcsz += sz;
		while(sz) {
			temp = (fip->crc ^ *bp++) & 0x000000FFL;
			fip->crc = ((fip->crc >> 8) & 0x00FFFFFFL) ^ crc32tab[temp];
			sz--;
		}
	}
	*activeaddr += srcsz;

	if ((spantype == SPANTYPE_BCEC || spantype == SPANTYPE_BPEC) &&
		(!fip->fhdr) && ((ulong)*activeaddr & 0xf)) {
		int		sz, temp, modfixsize;

		modfixsize = (TFS_FSIZEMOD - ((ulong)*activeaddr & (TFS_FSIZEMOD-1)));
		*activeaddr += modfixsize;
		if (fip->mode == FILLMODE_CRCONLY) {
			sz = modfixsize;
			fip->crcsz += sz;
			while(sz) {
				temp = (fip->crc ^ 0xff) & 0x000000FFL;
				fip->crc = ((fip->crc >> 8) & 0x00FFFFFFL) ^ crc32tab[temp];
				sz--;
			}
		}
	}

	/* Return ovlysz so that the caller will know if this function
	 * needed the spare sector.  This is used in the "mode = SPARE_OVERLAP"
	 * pass of defragFillActiveSector().
	 */
	return(ovlysz);
}

/* defragFillActiveSector():
 * This and defragFillFlash() are the workhorses of the tfsclean() function.
 * The bulk of this function is used to determine if we need to do anything
 * to the active sector and if so, do we need to copy the active sector to
 * the spare prior to erasing it.
 * The first loop in this function determines whether we need to do anything
 * at all with this sector (it may not be touched by the defragmentation).
 * The second loop determines if we have to copy the active sector to the
 * spare prior to erasing the active sector.
 * The final loop in this function does the call to defragFillFlash()
 * to do the actual flash writes.
 */
static int
defragFillActiveSector(TDEV *tdp, int ftot, int snum, struct defraghdr *dhp, int verbose)
{
	int		firstsnum;		/* number of first TFS sector */
	int		activesnum;		/* number of sector currently being written to */
	int		activessize;	/* size of active sector */
	char	*activeaddr;	/* offset being written to in the active sector */
	uchar	*activesbase;	/* base address of active sector */
	char	*activesend;	/* end address of active sector */
	int		fullsize;		/* size of file and header */
	char	*new_dend;		/* new end of data */
	char	*new_dbase;		/* new base of data */
	char	*new_hend;		/* new end of header */
	char	*new_hbase;		/* new base of header */
	char	*new_fend;		/* new end of file */
	char	*new_fbase;		/* new base of file */
	int		new_fspan;		/* span type for new file */
	int		new_hspan;		/* span type for new header */
	int		new_dspan;		/* span type for new data */
	int		fillstat;		/* result of defragFillFlash() function call */
	int		noreloctot;		/* number of files spanning the active sector */
							/* that do not have to be relocated */
	int		tmptot;			/* temps used for the "SPARE_OVERLAP" mode */
	char	*tmpactiveaddr;
	struct	defraghdr *tmpdhp;
	struct	fillinfo finfo;
	int		lastsnum, tot;
	int		copytospare;
	int		lastfileisnotrelocated;

	/* Retrieve number of first TFS sector: */
	if (addrtosector((uchar *)tdp->start,&firstsnum,0,0) < 0)
		return(TFSERR_MEMFAIL);

	activesnum = snum + firstsnum;

	/* Retrieve information about active sector: */
	if (sectortoaddr(activesnum,&activessize,&activesbase) == -1)
		return(TFSERR_MEMFAIL);

	if (verbose)
		printf(" Active sector: %3d @ 0x%lx\n",activesnum,(ulong)activesbase);

	if (addrtosector((uchar *)tdp->end,&lastsnum,0,0) < 0)
		return(TFSERR_MEMFAIL);

	activeaddr = (char *)activesbase;
	tmpactiveaddr = (char *)activesbase;
	activesend = (char *)activesbase + activessize - 1;

	/* FIRST LOOP:
	 * See if we need to do anything...
	 * In this state, we are simply checking to see if anything is going
	 * to cause the currently active sector to be written to.
	 * If yes, then we need to copy it to the spare and start the
	 * modification process; else, we just return and do nothing
	 * to this sector.
	 * For each file in the defrag header table that is destined for
	 * the address space occupied by the currently active sector, copy
	 * that file (header and data) to the active sector...
	 * Note that it may only be a partial copy, depending on the size
	 * of the file and the amount of space left in the active sector.
	 */

	noreloctot = 0;
	new_fspan = SPANTYPE_UNDEF;
	lastfileisnotrelocated = 0;
	for(tot=0;tot<ftot;tot++,dhp++) {
		fullsize = TFSHDRSIZ + dhp->filsize;
		new_fbase = dhp->nda;
		new_fend = (new_fbase + fullsize);

		/* We must figure out how the new version of the file will
		 * span across the active sector.
		 * See defragGetSpantype() for details.
		 */
		new_fspan = defragGetSpantype((char *)activesbase,(char *)activesend,
			(char *)new_fbase,(char *)new_fend);

		/* If the file we are looking at entirely spans a sector that is
		 * prior to the currently active sector, then we just continue
		 * through the list.
		 * If the file entirely spans a sector that is after the
		 * currently active sector, then we are done with this active sector.  
		 * If the file falls within the active sector in any way, and its
		 * new location does not match its old location, then we
		 * break out of this loop and begin the modification of this
		 * active sector...
		 */
		if (new_fspan == SPANTYPE_BLEL)
			return(0);
		
		if (new_fspan != SPANTYPE_BPEP) {
			if (dhp->nda == (char *)dhp->ohdr) {
				noreloctot++;
				if (tot == ftot-1) {
					lastfileisnotrelocated = 1;
					if (verbose > 1)
						printf("  last file not relocated\n");
				}
			}
			else
				break;
		}
	}

	/* If tot == ftot, then we got through the entire loop above without
	 * finding a file that needs to be relocated into this active sector.
	 * This means one of two things: either all the files fall into a 
	 * sector prior to this active sector, or the files in this active
	 * sector do not need to be relocated.  In either case, we simply
	 * return without touching this sector.
	 * Note that we also keep track of the possibility that the last file
	 * may not be relocated.  If this ends up to be the case, then we are
	 * simply cleaning up one or more dead files after a full set of active
	 * files, so we should clean up the sector.
	 */
	if ((tot == ftot) && (lastfileisnotrelocated == 0))
		return(0);

	/* If tot != ftot, then we must subtract noreloctot from tot so that
	 * we establish 'tot' as the index into the first file that must be
	 * copied to the active sector...
	 */
	if (noreloctot) {
		tot -= noreloctot;
		dhp -= noreloctot;
	}

	/* Since we got here, we know that we have to do some work on the
	 * currently active sector.  We may not have to copy it to the spare,
	 * but we will erase the spare anyway because the sector erase is
	 * supposed to be smart enough to avoid the erase if it is already 
	 * erased.  This should be handled by the flash driver because in
	 * ALL cases the erase should be avoided if possible.
	 */
#if DEFRAGTYPE_NPWRSAFE_SPARE
	if (defragEraseSpare(tdp) < 0)
		return(TFSERR_FLASHFAILURE);
#endif

	/* If the active sector is the last sector (which would contain the
	 * defrag header table), then we reference the copy of the table that
	 * is in the spare...
	 * Also, if this is the last sector, then we HAVE to copy it to 
	 * spare, so we can skip the 2nd loop that attempts to determine
	 * if we need to do it.
	 */
	if (activesnum == lastsnum) {
		copytospare = 1;
	}
	else {
		/* SECOND LOOP:
		 * See if we need to copy the active sector to the spare...
		 * We do this by continuing the loop we started above.  Notice that
		 * we do an almost identical loop again below this.
		 * On this pass through the loop we are only checking to see if it is
		 * necessary to copy this active sector to the spare.
		 */
		tmptot = tot;
		tmpdhp = dhp;
		copytospare = 0;
		finfo.mode = FILLMODE_SPAREOVERLAP;
		for(;tmptot<ftot;tmptot++,tmpdhp++) {
			finfo.tdp = tdp;
			finfo.dhp = tmpdhp;
			finfo.asnum = activesnum;
	
			fullsize = TFSHDRSIZ + tmpdhp->filsize;
			new_fbase = tmpdhp->nda;
			new_fend = (new_fbase + fullsize);
	
			new_fspan = defragGetSpantype((char *)activesbase,(char *)activesend,
				(char *)new_fbase,(char *)new_fend);
	
			if (new_fspan == SPANTYPE_BPEP) 
				continue;
			else if (new_fspan == SPANTYPE_BLEL)
				break;
	
			/* Now retrieve span information about header and data
			 * portions of the file (new and orig)...
			 */
			new_hbase = new_fbase;
			new_hend = new_hbase + TFSHDRSIZ;
			new_dbase = new_hbase + TFSHDRSIZ;
			new_dend = new_fend;
	
			new_hspan = defragGetSpantype((char *)activesbase,(char *)activesend,
				(char *)new_hbase,(char *)new_hend);
			new_dspan = defragGetSpantype((char *)activesbase,(char *)activesend,
				(char *)new_dbase,(char *)new_dend);
	
			/* If defragFillFlash() returns positive (with mode ==
			 * FILLMODE_SPAREOVERLAP set above), then we know that the
			 * spare sector must be loaded with a copy of this active
			 * sector, so we can break out of this loop at that point...
			 */
			finfo.fhdr = 1;
			fillstat = defragFillFlash(&finfo,new_hspan,&tmpactiveaddr,verbose);
			if (fillstat < 0)
				return(fillstat);	
			if (fillstat > 0) {
				copytospare = 1;
				break;
			}
			if (new_hspan == SPANTYPE_BCEL)
				break;
	
			finfo.fhdr = 0;
			fillstat = defragFillFlash(&finfo,new_dspan,&tmpactiveaddr,verbose);
			if (fillstat < 0)
				return(fillstat);	
			if (fillstat > 0) {
				copytospare = 1;
				break;
			}
			if (new_dspan == SPANTYPE_BCEL || new_dspan == SPANTYPE_BPEL)
				break;
		}
	}

	finfo.mode = FILLMODE_FWRITE;

	if (copytospare) {
#if DEFRAGTYPE_NPWRSAFE_SPARE
		if (defragFwrite(6,(uchar *)spareBase,activesbase,activessize) == -1) {
			printf("Failed to copy active %d to spare\n",activesnum);
			return(TFSERR_FLASHFAILURE);
		}
#else
		memcpy((void *)spareBase,(void *)activesbase,activessize);
#endif
	}

	/* We can now begin actual modification of the active sector,
	 * so start off by eraseing it...
	 */
	if (defragSerase(4,activesnum) < 0)
		return(TFSERR_FLASHFAILURE);

	/* THIRD LOOP:
	 * Now we pass through the loop to do the real flash modifications...
	 */
	for(;tot<ftot;tot++,dhp++) {
		finfo.tdp = tdp;
		finfo.dhp = dhp;
		finfo.asnum = activesnum;

		fullsize = TFSHDRSIZ + dhp->filsize;
		new_fbase = dhp->nda;
		new_fend = (new_fbase + fullsize);

		new_fspan = defragGetSpantype((char *)activesbase,(char *)activesend,
			(char *)new_fbase,(char *)new_fend);

		if (new_fspan == SPANTYPE_BPEP) 
			continue;
		else if (new_fspan == SPANTYPE_BLEL)
			break;

		if (verbose)
			printf("  File: %s\n",dhp->fname);

		/* Now retrieve span information about header and data
		 * portions of the file (new and orig)...
		 */
		new_hbase = new_fbase;
		new_hend = new_hbase + TFSHDRSIZ;
		new_dbase = new_hbase + TFSHDRSIZ;
		new_dend = new_fend;

		new_hspan = defragGetSpantype((char *)activesbase,(char *)activesend,
			(char *)new_hbase,(char *)new_hend);
		new_dspan = defragGetSpantype((char *)activesbase,(char *)activesend,
			(char *)new_dbase,(char *)new_dend);

		/* At this point we have all the information we need to copy
		 * the appropriate amount of the file from orignal space
		 * to new space.
		 * We have to break the write up into two parts, the header
		 * (new_hspan) and the data (new_dspan) so we have to look
		 * at the spantype for each to determine what part of the
		 * header and/or data we are going to copy.
		 *
		 * Also, we must consider the possibility that the source
		 * data may be in the spare sector.  This would be the case
		 * if the active sector is the same sector that the original
		 * data was in.  If the source data is in the spare sector,
		 * then an added complication is the fact that it may not
		 * all be there, we may have to copy some from the spare,
		 * then some from the original space.
		 */
		finfo.fhdr = 1;
		fillstat = defragFillFlash(&finfo,new_hspan,&activeaddr,verbose);
		if (fillstat < 0)
			return(fillstat);	
		if (new_hspan == SPANTYPE_BCEL)
			break;

		finfo.fhdr = 0;
		fillstat = defragFillFlash(&finfo,new_dspan,&activeaddr,verbose);
		if (fillstat < 0)
			return(fillstat);	
		if (new_dspan == SPANTYPE_BCEL || new_dspan == SPANTYPE_BPEL)
			break;
	}
	return(0);
}

/* _tfsclean():
 * This is the front-end of the defragmentation process, following are the
 * basic steps of defragmentation...
 *
 * Build the Defrag State Information (DSI) area:
 * 1. Create a table of 32-bit CRCs, two for each sector.  One is the CRC
 *	  of the sector prior to beginning defragmentation and the other is
 *	  what will be the CRC of the sector after defragmentation has completed.
 *	  These CRCs are used to help recover from an interrupted defragmentation.
 * 2. Create a table of struct defraghdr structures, one for each file in
 *	  TFS that is currently active (not dead).
 * 3. Create a CRC of the tables created in steps 1 & 2.
 *
 * The data created in steps 1-3 is stored at the end of the last sector
 * used by TFS for file storage.  After this is created, the actual flash
 * defragmentation process starts.
 *
 * File relocation:
 * 4. Step through each sector in TFS flash space, process each file whose
 *	  relocated space overlaps with that sector.  As each sector is being
 *	  re-built, the original version of that sector is stored in the spare.
 *
 * End of flash cleanup:
 * 5. Run through the remaining, now unsused, space in TFS flash and make
 *	  sure it is erased.
 *
 * File check:
 * 6. Run a check of all of the relocated files to make sure everything is
 *	  still sane.
 * 
 * Defragmentation success depends on some coordination with tfsadd()...
 * Whenever a file is added to TFS, tfsadd() must verify that the space
 * needed for defrag overhead (defrag state & header tables) will be
 * available.  Also, tfsadd() must make sure that the defrag overhead will
 * always fit into one sector (the sector just prior to the spare).
 */

int
_tfsclean(TDEV *tdp, int notused, int verbose)
{
	int		firstsnum;		/* Number of first sector in TFS device. */
	int		lastsnum;		/* Number of last sector in TFS device. */
	int		lastssize;		/* Size of last sector in TFS device. */
	uchar	*lastsbase;		/* Base address of last sector in TFS device. */
	int		sectorcheck;	/* Used to verify proper TFS configuration. */
	struct	defraghdr *dht;	/* Pointer to defrag header table. */
	int		chkstat;		/* Result of tfscheck() after defrag is done. */
	int		ftot;			/* Total number of active files in TFS. */
	int		dtot;			/* Total number of deleted files in TFS. */
	int		fcnt;			/* Running file total, used in hdrtbl build. */
	TFILE	*tfp;			/* Misc file pointer */
	char	*newaddress;	/* Used to calculate "new" location of file. */
	int		activesnum;		/* Sector being worked on restarted defrag. */
	int		defrag_state;
	int		sidx, snum;
	char	*end;

	/* Based on the configuration we initially establish the base of
	 * the memory space we will use as the spare sector...
	 */
#if DEFRAGTYPE_NPWRSAFE_SPARE
	spareBase = (char *)tdp->spare;
#elif DEFRAGTYPE_NPWRSAFE_APPRAMBASE
	spareBase = (char *)getAppRamStart();
#elif DEFRAGTYPE_NPWRSAFE_RAMBLOCK
	spareBase = (char *)spareInRAM;
#endif
	
	if (TfsCleanEnable < 0)
		return(TFSERR_CLEANOFF);

	/* If incoming TFS device pointer is NULL, return error
	 */
	if (!tdp)
		return(TFSERR_BADARG);

	activesnum = 0;
	defrag_state = SECTOR_DEFRAG_INACTIVE;

	if (verbose) 
		printf("TFS device '%s' defrag...\n",tdp->prefix);

	if (addrtosector((uchar *)tdp->start,&firstsnum,0,0) < 0)
		return(TFSERR_MEMFAIL);
	lastsnum = firstsnum + tdp->sectorcount - 1;
	if (addrtosector((uchar *)tdp->end,&sectorcheck,0,0) < 0)
		return(TFSERR_MEMFAIL);
	if (lastsnum != sectorcheck) {
		/* If this error occurs, it is an indication that TFS was not
		 * properly configured in config.h, this error should not occur
		 * if TFS is properly configured.
		 */
		printf("%s: SECTORCOUNT != TFSSTART <-> TFSEND\n", tdp->prefix);
		printf("First TFS sector = %d, last = %d\n",firstsnum,sectorcheck);
		return(TFSERR_MEMFAIL);
	}

	activesnum = firstsnum;

	/* Retrieve information about last sector:
	 */
	if (sectortoaddr(lastsnum,&lastssize,&lastsbase) == -1)
		return(TFSERR_MEMFAIL);

	/* Retrieve the number of "dead" and "living" files:
	 * If there are no dead files, then there is no need to defrag.
	 * If there are no "living" files, then we can just init the flash.
	 */
	ftot = dtot = 0;
	tfp = (TFILE *)tdp->start;
	while(validtfshdr(tfp)) {
		if (TFS_FILEEXISTS(tfp))
			ftot++;
		else
			dtot++;
		tfp = nextfp(tfp,tdp);
	}
	if (dtot == 0) {
		if (verbose)
			printf("No dead files in %s.\n",tdp->prefix);
		if (tfsflasherased(tdp,verbose))
			return(0);
		if (verbose)
			printf("Cleaning up end of flash...\n");
	}
	if (ftot == 0) {
		if (verbose)
			printf("No active files detected, erasing all %s flash...\n",
				tdp->prefix);
		_tfsinit(tdp);
		return(0);
	}

	/* Now that we know how many files are in TFS, we can establish 
	 * a pointer to the defrag header table, and the size of the table...
	 */
	if ((dht = (struct defraghdr *)malloc(ftot*sizeof(struct defraghdr))) == 0) {
		printf("DHT allocation failed\n");
		return(TFSERR_MEMFAIL);
	}
		
	if (defrag_state == SECTOR_DEFRAG_INACTIVE) {
		if (verbose) {
			printf("TFS defrag: building DSI space...\n");
		}

#if DEFRAGTYPE_NPWRSAFE_SPARE
		/* Erase the spare then copy the last TFS sector.
		 */
		if (defragEraseSpare(tdp) < 0)
			return(TFSERR_FLASHFAILURE);
	
		if (defragFwrite(11,(uchar *)spareBase,lastsbase,lastssize) == -1) {
			return(TFSERR_FLASHFAILURE);
		}
#else
		memcpy((void *)spareBase,(void *)lastsbase,lastssize);
#endif

		/* Build the header table:
		 */
		fcnt = 0;
		tfp = (TFILE *)tdp->start;
		newaddress = (char *)tdp->start;

		if (verbose > 2) {
			printf("\nDEFRAG HEADER DATA (dht=0x%lx, ftot=%d):\n",
				(ulong)dht,ftot);
		}
	
		while(validtfshdr(tfp)) {
			if (TFS_FILEEXISTS(tfp)) {
				uchar	*base, *eof, *neof, *nbase;
				int		size, slot;
				struct	tfsdat *slotptr;
				struct	defraghdr	*dfhdr;
	
				dfhdr = &dht[fcnt];
				strcpy(dfhdr->fname,TFS_NAME(tfp));
				dfhdr->ohdr = tfp;
				dfhdr->ohdrcrc = tfp->hdrcrc;
				dfhdr->filsize = TFS_SIZE(tfp);
				if (addrtosector((uchar *)tfp,0,0,&base) < 0)
					return(TFSERR_MEMFAIL);

				eof = (uchar *)(tfp+1)+TFS_SIZE(tfp)-1;
				if (addrtosector((uchar *)eof,0,0,&base) < 0)
					return(TFSERR_MEMFAIL);
				dfhdr->oeso = eof - base + 1;

				neof = (uchar *)newaddress+TFSHDRSIZ+TFS_SIZE(tfp)-1;
				if (addrtosector((uchar *)neof,&dfhdr->nesn,0,&nbase) < 0)
					return(TFSERR_MEMFAIL);
				dfhdr->neso = neof - nbase + 1;

				dfhdr->crc = 0;
				dfhdr->idx = fcnt;
				dfhdr->nda = newaddress;
	
				/* If the file is currently opened, adjust the base address. */
				slotptr = tfsSlots;
				for (slot=0;slot<TFS_MAXOPEN;slot++,slotptr++) {
					if (slotptr->offset != -1) {
						if (slotptr->base == (uchar *)(TFS_BASE(tfp))) {
							slotptr->base = (uchar *)(newaddress+TFSHDRSIZ);
						}
					}
				}
				size = TFS_SIZE(tfp) + TFSHDRSIZ;
				if (size & 0xf) {
					size += TFS_FSIZEMOD;
					size &= ~(TFS_FSIZEMOD-1);
				}
				newaddress += size;
				dfhdr->nextfile = (TFILE *)newaddress;
				dfhdr->crc = crc32((uchar *)dfhdr,DEFRAGHDRSIZ);
				fcnt++;
			}
			tfp = nextfp(tfp,tdp);
		}

		defrag_state = SCANNING_ACTIVE_SECTOR;
	}

	if (defrag_state == SCANNING_ACTIVE_SECTOR) {

		if (verbose) {
			printf("TFS: updating sectors %d-%d...\n",
				activesnum,lastsnum);
		}

		/* Now we begin the actual defragmentation.  We have built enough
		 * state information (defrag header and state table) into the last
		 * TFS sector, so now we can start the cleanup.
		 */
		for(sidx = activesnum - firstsnum; sidx < tdp->sectorcount; sidx++) {
			if (defragFillActiveSector(tdp,ftot,sidx,dht,verbose) < 0)
				return(TFSERR_FLASHFAILURE);
		}

		defrag_state = SECTOR_DEFRAG_ALMOST_DONE;
	}

	if (defrag_state == SECTOR_DEFRAG_ALMOST_DONE) {

		/* We've completed the relocation of all files into a defragmented
		 * area of TFS flash space.  Now we have to erase all sectors after
		 * the sector used by the last file in TFS (including the spare)...
		 * If the last file in TFS uses the last sector, then the defrag
		 * header table will be erased and there is nothing left to do
		 * except erase the spare.
		 */
		if (verbose) {
			printf("TFS: clearing available space...\n");
		}
		if (dht[ftot-1].crc != ERASED32) {
			end = (dht[ftot-1].nda + dht[ftot-1].filsize + TFSHDRSIZ) - 1;
			if (addrtosector((uchar *)end,&snum,0,0) < 0)
				return(TFSERR_FLASHFAILURE);
			snum++;
			while(snum <= lastsnum) {
				if (defragSerase(7,snum) < 0)
					return(TFSERR_FLASHFAILURE);
				snum++;
			}
		}
#if DEFRAGTYPE_NPWRSAFE_SPARE
		if (defragEraseSpare(tdp) < 0)
			return(TFSERR_FLASHFAILURE);
#endif

		/* All defragmentation is done, so verify sanity of files... */
		chkstat = tfscheck(tdp,verbose);
	}
	else {
		chkstat = TFS_OKAY;
	}

	if (verbose)
		printf("Defragmentation complete\n");

	free((void *)dht);
	return(chkstat);
}

/* tfsfixup():
 *	Called at system startup to finish up a TFS defragmentation if one
 *	was in progress.
 */
int
tfsfixup(int verbose, int dontquery)
{
	return(0);
}

#endif	/* INCLUDE_TFS */
