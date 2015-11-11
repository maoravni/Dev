/* xmodem.c:
 *	Basic implementation of Xmodem. 
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
#include "genlib.h"
#include "stddefs.h"
#include "flash.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "cli.h"
#include "ether.h"
#include "timer.h"

#if INCLUDE_XMODEM

#define PUTCHAR putchar


/* struct xinfo:
 * Used to contain information pertaining to the current transaction.
 * The structure is built by the command Xmodem, then passed to the other
 * support functions (Xup, Xdown, etc..) for reference and update.
 */
struct xinfo {
	uchar	sno;			/* Sequence number. */
	int		xfertot;		/* Running total of transfer. */
	int		pktlen;			/* Length of packet (128 or 1024). */
	int		pktcnt;			/* Running tally of number of packets processed. */
	int		filcnt;			/* Number of files transferred by ymodem. */
	long	size;			/* Size of upload. */
	ulong	flags;			/* Storage for various runtime flags. */
	ulong	base;			/* Starting address for data transfer. */
	ulong	dataddr;		/* Running address for data transfer. */
	int		errcnt;			/* Keep track of errors (used in verify mode). */
	int		nakresend;		/* Time between each NAK sent at Xdown startup. */
	char	*firsterrat;	/* Pointer to location of error detected when */
							/* transfer is in verify mode. */
	char	fname[TFSNAMESIZE];
};

/* Runtime flags: */
#define	USECRC	(1<<0)

/* Current xmodem operation: */
#define XNULL	0
#define XUP		1
#define XDOWN	2

/* X/Ymodem protocol: */
#define SOH		0x01
#define STX		0x02
#define EOT		0x04
#define ACK		0x06
#define NAK		0x15
#define CAN		0x18
#define EOF		0x1a
#define ESC		0x1b

#define PKTLEN_128	128

#define BYTE_TIMEOUT	1000

static int Xup(struct xinfo *);
static int Xdown(struct xinfo *);
static int getPacket(uchar *,struct xinfo *);
static int putPacket(uchar *,struct xinfo *);

char *XmodemHelp[] = {
	"Xmodem file transfer",
	"-[a:BcdF:f:i:s:u]",
#if INCLUDE_VERBOSEHELP
	"Options:",
	" -a{##}     address (overrides default of APPRAMBASE)",
#if INCLUDE_FLASH
	" -B         boot sector reload",
#endif
	" -c         use crc (default = checksum)",
	" -d         download",
#if INCLUDE_TFS
	" -F{name}   filename",
	" -f{flags}  file flags (see tfs)",
	" -i{info}   file info (see tfs)",
#endif
	" -s{##}     size (overrides computed size)",
	" -u         upload",
	"Notes:",
	" * Either -d or -u must be specified (-B implies -d).",
	" * Each additional 'd' option cuts the nak-resend rate in half.",
	" * XMODEM forces a 128-byte modulo on file size.  The -s option",
	"   can be used to override this when transferring a file to TFS.",
	" * File upload requires no address or size (size will be mod 128).",
	" * When using -B, it should be the ONLY command line option,",
	"   it's purpose is to reprogram the boot sector, so be careful!",
#endif
	(char *)0,
};

int nakat;

int
Xmodem(int argc,char *argv[])
{
#if INCLUDE_TFS
	TFILE	*tfp;
#endif
	struct	xinfo xi;
	int	opt, xop, newboot;
	char	*info, *flags;

	xop = XNULL;
	newboot = 0;
	info = (char *)0;
	flags = (char *)0;
	xi.fname[0] = 0;
	xi.size = 0;
	xi.flags = 0;
	xi.filcnt = 0;
	xi.nakresend = 2;
	xi.pktlen = PKTLEN_128;
	xi.base = xi.dataddr = getAppRamStart();
	while ((opt=getopt(argc,argv,"a:BcdF:f:i:ns:u")) != -1) {
		switch(opt) {
		case 'a':
			xi.dataddr = xi.base = strtoul(optarg,(char **)0,0);
			break;
		case 'B':
			xop = XDOWN;
			newboot = 1;
			break;
		case 'c':
			xi.flags |= USECRC;
			break;
		case 'n':
			printf("nakat: %d\n",nakat);
			return(CMD_SUCCESS);
		case 'd':
			xi.nakresend *= 2;		/* -ddd increases Xdown() resend delay */
			xop = XDOWN;
			break;
#if INCLUDE_TFS
		case 'F':
			strncpy(xi.fname,optarg,TFSNAMESIZE);
			break;
		case 'f':
			flags = optarg;
			break;
		case 'i':
			info = optarg;
			break;
#endif
		case 's':
			xi.size = strtol(optarg,(char **)0,0);
			break;
		case 'u':
			xop = XUP;
			break;
		default:
			return(CMD_PARAM_ERROR);
		}
	}

	nakat = 0;
	/* There should be no arguments after the option list. */
	if (argc != optind)
		return(CMD_PARAM_ERROR);

	if (xop == XUP) {
			if (xi.fname[0]) {	/* Causes -a and -s options to be ignored. */
#if INCLUDE_TFS
			tfp = tfsstat(xi.fname);
			if (!tfp) {
				printf("%s: file not found\n",xi.fname);
				return(CMD_FAILURE);
			}
			xi.base = xi.dataddr = (ulong)TFS_BASE(tfp);
			xi.size = TFS_SIZE(tfp);
#endif
		}
		Xup(&xi);
	}
	else if (xop == XDOWN) {
		long	tmpsize;

		if (xi.fname[0] != 0) {
			if ((xi.base = (ulong)tfsnxtdataptr()) == 0)
				return(CMD_FAILURE);
			xi.dataddr = xi.base;
		}
		tmpsize = (long)Xdown(&xi);
		if (tmpsize == -1)
			return(CMD_FAILURE);

		if ((!xi.size) || (tmpsize < 0))
			xi.size = tmpsize;

#if INCLUDE_TFS
		if ((xi.fname[0]) && (xi.size > 0)) {
			int	err;

			printf("Writing to file '%s'...\n",xi.fname);
			err = tfsadd(xi.fname,info,flags,(uchar *)xi.base,xi.size);
			if (err != TFS_OKAY) {
				printf("%s: %s\n",xi.fname,(char *)tfsctrl(TFS_ERRMSG,err,0));
				return(CMD_FAILURE);
			}
		}
#if INCLUDE_FLASH
		else
#endif
#endif
#if INCLUDE_FLASH
		if ((newboot) && (xi.size > 0)) {
			extern	int FlashProtectWindow;
			int		rc;
			char	*bb;
			ulong	bootbase;

			bb = getenv("BOOTROMBASE");
			if (bb)
				bootbase = strtoul(bb,0,0);
			else
				bootbase = BOOTROM_BASE;
#ifdef TO_FLASH_ADDR
			/* The address the program is linked to is not necessarily the
			 * physical address where flash operations can be performed on.
			 * A typical use could be that the program is linked to run in
			 * a cacheable region but writing to the flash can only be done
			 * in an uncached region.
			 * "config.h" is a good place to define the TO_FLASH_ADDR macro.
			 */
			bootbase = (ulong)TO_FLASH_ADDR(bootbase);
#endif
			printf("Reprogramming boot @ 0x%lx from 0x%lx, %ld bytes.\n",
				bootbase,xi.base,xi.size);
			if (askuser("OK?")) {
				int first, last;

				/* If sector(s) need to be unlocked, do that now...
				 */
				if (addrtosector((uchar *)bootbase,&first,0,0) == -1)
					return(CMD_FAILURE);

				if (addrtosector((uchar *)bootbase+xi.size-1,&last,0,0) == -1)
					return(CMD_FAILURE);

				if (flashlock(first,FLASH_LOCKABLE) != -1) {
					FlashProtectWindow = 1;
					while(first < last)
						flashlock(first++,FLASH_UNLOCK);
				}

				FlashProtectWindow = 1;
				rc = flashewrite((uchar *)bootbase,(uchar *)xi.base,xi.size);
				if (rc == -1) {
					printf("failed\n");
					return(CMD_FAILURE);
				}
			}
		}
#endif
	}
	else
		return(CMD_PARAM_ERROR);
	shell_sprintf("XMODEMGET","%ld",xi.size);
	return(CMD_SUCCESS);
}

/* xgetchar():
 * Wrap getchar with a timer, so that after 5 seconds of waiting
 * we giveup...
 */
int
xgetchar(char *cp)
{
	struct	elapsed_tmr tmr;

	startElapsedTimer(&tmr,5000);
	while(!msecElapsed(&tmr) && !gotachar());
	if (!gotachar()) {
		return(-1);
	}
	*cp = getchar();
	return(0);
}

/* putPacket():
 * Used by Xup to send packets.
 */
static int
putPacket(uchar *tmppkt, struct xinfo *xip)
{
	char	c;
	int		i;
	ushort	chksm;

	chksm = 0;
	
	PUTCHAR(SOH);
	PUTCHAR((char)(xip->sno));
	PUTCHAR((char)~(xip->sno));

	if (xip->flags & USECRC) {
		for(i=0;i<xip->pktlen;i++) {
			PUTCHAR((char)*tmppkt);
			chksm = (chksm<<8)^xcrc16tab[(chksm>>8)^*tmppkt++];
		}
    	/* An "endian independent way to extract the CRC bytes. */
		PUTCHAR((char)(chksm >> 8));
		PUTCHAR((char)chksm);
	}
	else {
		for(i=0;i<xip->pktlen;i++) {
			PUTCHAR((char)*tmppkt);
			chksm = ((chksm+*tmppkt++)&0xff);
		}
		PUTCHAR((char)(chksm&0x00ff));
	}

	if (xgetchar(&c) == -1)			/* Wait for ack */
		return(-1);

	return((int)c);
}

/* getPacket():
 * Used by Xdown to retrieve packets.
 */
static int
getPacket(uchar *tmppkt, struct xinfo *xip)
{
	char	*pkt;
	int		i,rcvd;
	uchar	seq[2];

	if ((rcvd = getbytes_t((char *)seq,2,BYTE_TIMEOUT)) != 2) {
nakat = __LINE__;
		PUTCHAR(NAK);
		return(0);
	}

	rcvd = getbytes_t((char *)tmppkt,xip->pktlen,BYTE_TIMEOUT);
	if (rcvd != xip->pktlen) {
nakat = __LINE__;
		PUTCHAR(NAK);
		return(0);
	}
	pkt = (char *)tmppkt;

	if (xip->flags & USECRC) {
		char c;
		ushort	crc, xcrc;
		
    	/* An "endian independent way to combine the CRC bytes. */
		if (xgetchar(&c) == -1)
			return(0);
		crc = (ushort)c;
		crc <<= 8;
		if (xgetchar(&c) == -1)
			return(0);
	    crc += (ushort)c;
		xcrc = xcrc16((uchar *)pkt,(ulong)(xip->pktlen));
		if (crc != xcrc) {
nakat = __LINE__;
			PUTCHAR(NAK);
			return(0);
		}
	}
	else {
		uchar	csum, xcsum;

		if (xgetchar((char *)&xcsum) == -1)
			return(0);
		csum = 0;
		for(i=0;i<xip->pktlen;i++)
			csum += *pkt++;
		if (csum != xcsum) {
nakat = __LINE__;
			PUTCHAR(NAK);
			return(0);
		}
	}

	/* Test the sequence number compliment...
	 */
	if ((uchar)seq[0] != (uchar)~seq[1]) {
nakat = __LINE__;
		PUTCHAR(NAK);
		return(0);
	}

	/* Verify that the incoming sequence number is the expected value...
	 */
	if ((uchar)seq[0] !=  xip->sno) {
		/* If the incoming sequence number is one less than the expected
		 * sequence number, then we assume that the sender did not recieve
		 * our previous ACK, and they are resending the previously received
		 * packet.  In that case, we send  ACK and don't process the
		 * incoming packet...
	 	 */
		if ((uchar)seq[0] == xip->sno-1) {
			PUTCHAR(ACK);
			return(0);
		}

		/* Otherwise, something's messed up...
		 */
		PUTCHAR(CAN);
		return(-1);
	}

	xip->sno++;
	xip->pktcnt++;
	PUTCHAR(ACK);
	return(0);
}

/* Xup():
 * Called when a transfer from target to host is being made (considered
 * an upload).
 */
static int
Xup(struct xinfo *xip)
{
	uchar	c;
	int		tmp, done;

	if (xip->size & 0x7f) {
		xip->size += 128;
		xip->size &= 0xffffff80L;
	}

	printf("Upload %ld bytes from 0x%lx\n",xip->size,(ulong)xip->base);

	/* Startup synchronization... */
	/* Wait to receive a NAK or 'C' from receiver. */
	done = 0;
	while(!done) {
		if (xgetchar((char *)&c) == -1)
			return(0);
		switch(c) {
		case NAK:
			done = 1;
			break;
		case 'C':
			xip->flags |= USECRC;
			done = 1;
			break;
		case 'q':	/* ELS addition, not part of XMODEM spec. */
			return(0);
		default:
			break;
		}
	}

	rawon();

	done = 0;
	xip->sno = 1;
	xip->pktcnt = 0;
	while(!done) {
		if ((tmp = putPacket((uchar *)(xip->dataddr),xip)) == -1)
			return(0);
		c = (uchar)tmp;
		switch(c) {
		case ACK:
			xip->sno++;
			xip->pktcnt++;
			xip->size -= xip->pktlen;
			xip->dataddr += xip->pktlen;
			break;
		case NAK:
			break;
		case CAN:
			done = -1;
			break;
		case EOT:
			done = -1;
			break;
		default:
			done = -1;
			break;
		}
		if (xip->size <= 0) {
			char tmp;

			PUTCHAR(EOT);
			if (xgetchar(&tmp) == -1)	/* Flush the ACK */
				return(0);
			break;
		}
	}

	rawoff();
	return(0);
}


/* Xdown():
 * Called when a transfer from host to target is being made (considered
 * an download).
 * Note that if we don't have INCLUDE_MALLOC set (in config.h), then
 * we allocate a 128-byte static buffer and only support the 128-byte
 * packet size here.
 */

static int
Xdown(struct xinfo *xip)
{
	char	c;
	int		i, done;
	struct	elapsed_tmr tmr;
	static char pkt[PKTLEN_128];

	rawon();

	xip->sno = 0x01;
	xip->pktcnt = 0;
	xip->errcnt = 0;
	xip->xfertot = 0;
	xip->firsterrat = 0;

	/* Startup synchronization... */
	/* Continuously send NAK or 'C' until sender responds. */
	for(i=0;i<32;i++) {
		if (xip->flags & USECRC)
			PUTCHAR('C');
		else
			PUTCHAR(NAK);

		startElapsedTimer(&tmr,xip->nakresend * 1000);
		while(!msecElapsed(&tmr) && !gotachar());
		if (gotachar())
			break;
	}
	if (i == 32) {
		return(-1);
	}

	done = 0;
	while(done == 0) {
		if (xgetchar(&c) == -1)
			return(-1);
		switch(c) {
		case SOH:				/* 128-byte incoming packet */
			xip->pktlen = PKTLEN_128;
			done = getPacket((uchar *)pkt,xip);
			if (InFlashSpace((uchar *)(xip->dataddr),xip->pktlen)) {
				int wrlen = xip->pktlen;

				if (xip->size != 0) {
					int tot = xip->size - (xip->xfertot + xip->pktlen);
					if (tot < 0)
						wrlen = PKTLEN_128 + tot;
				}
				if (AppFlashWrite((uchar *)(xip->dataddr),(uchar *)pkt,wrlen) < 0) {
					done = -1;
				}
			}
			else {
				memcpy((void *)xip->dataddr,(void *)pkt,xip->pktlen);
			}
            xip->dataddr += xip->pktlen;
            xip->xfertot += xip->pktlen;
			break;
		case CAN:
			done = -1;
			break;
		case EOT:
			PUTCHAR(ACK);
			done = xip->xfertot;
			rawoff();
			printf("\nRcvd %d pkt%c (%d bytes)\n",xip->pktcnt,
					xip->pktcnt > 1 ? 's' : ' ',xip->xfertot);

	 		/* If the transfer is complete and no file add is to
			 * be done, then we flush d-cache and invalidate
			 * i-cache across the memory space that was just
			 * copied to.  This is necessary in case the
	 		 * binary data that was just transferred is code.
			 */
			flushDcache((char *)xip->base,xip->xfertot);
			invalidateIcache((char *)xip->base,xip->xfertot);
			break;
		case ESC:		/* User-invoked abort */
			done = -1;
			break;
		case EOF:		/* 0x1a sent by MiniCom, just ignore it. */
			break;
		default:
			done = -1;
			break;
		}
	}
	rawoff();
	return(done);
}


#endif
