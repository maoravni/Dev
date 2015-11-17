/* chario.c:
 *	This code supports some basic character io functionality.
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
#include "cpuio.h"
#include "genlib.h"
#include "monlib.h"
#include "stddefs.h"
#include "timer.h"
#include "ether.h"
#include "fbi.h"

#define CTLC	0x03	/* control-c */

int rawmode, ConsoleBaudRate;

/* extWatchDog():
 * This function pointer is used by the application to override the
 * monitor's configured watchdog functionality.  Refer to config.h
 * in umon_ports/template for more information on the WATCHDOG macro.
 */
#ifdef WATCHDOG_ENABLED
int (*remoteWatchDog)(void);
#endif

#if INCLUDE_MONLIB
/* remotexxxx() function pointers:
 * These function pointers provide the application with the ability to
 * override the monitor's default putchar/getchar/gotachar functions.
 * See comments above InitRemoteIO() function below for more info.
 */
int	(*remoterawon)(void);
int (*remoterawoff)(void);
int	(*remoteputchar)(int);
int (*remotegetchar)(void);
int (*remotegotachar)(void);
#endif

static char console_echo_off;

/* console_echo():
 * Enable/disable the output of the console...
 */
void
console_echo(int enable)
{
	if (enable)
		console_echo_off = 0;
	else
		console_echo_off = 1;
}

/* putchar():
 * 	Output a character to the stdout RS232 port.
 *  If console_echo_off is set, then we DON'T send the
 *  character to the console.
 */
int
putchar(char c)
{
#if INCLUDE_MONLIB
	/* If the remoteputchar function pointer is non-zero, then we
	 * assume that the default putchar function has been overridden
	 * by some overlaying application using mon_com(CHARFUNC_PUTCHAR,),
	 * so we use that redefined function instead...
	 * New for uMon-M: test the return of remoteputchar...
	 *
	 * REMOTE_PUTCHAR_REPLACE (and default):
	 *	Call remote function and return (same as original).
	 *
	 * REMOTE_PUTCHAR_INSERT:
	 *	Call remote function, then continue.
	 *
	 * REMOTE_PUTCHAR_INSERT:
	 *	Call remote function, then ONLY do target_putchar().
	 */
	if (remoteputchar) {
		switch(remoteputchar(c)) {
			case REMOTE_PUTCHAR_INSERT:
				if (getenv("PDBG"))
					target_putchar('y');
				break;
			case REMOTE_PUTCHAR_INSERTT:
				if (!console_echo_off) {
					if (c == '\n')
						target_putchar('\r');
					target_putchar(c);
				}
				if (getenv("PDBG"))
					target_putchar('x');
				return((int)c);
			default:
			case REMOTE_PUTCHAR_REPLACE:
				return((int)c);
		}
	}
#endif

	if (rawmode) {
		if (!console_echo_off)
			target_putchar(c);
		fbi_putchar(c);
		return((int)c);
	}

	/* Call the target-specific function that puts the character out on
	 * the console port (precede '\n' with '\r').
	 * For each call to target_putchar(), call SendIPMonChar so that if
	 * this output is generated by moncmd, it will be sent back to the
	 * moncmd client.  Note that if INCLUDE_ETHERNET is 0, then the
	 * function SendIPMonChar is NULL (see ether.h).
	 */
	if (c == '\n') {
#if INCLUDE_MONCMD
		SendIPMonChar('\r',0);
#endif
		fbi_putchar('\r');
		if (!console_echo_off)
			target_putchar('\r');
	}
#if INCLUDE_MONCMD
	SendIPMonChar(c,0);
#endif
	if (!console_echo_off)
		target_putchar(c);
	fbi_putchar(c);
	WATCHDOG_MACRO;
	return((int)c);
}

int 
getchar(void)
{
	/* If the remotegetchar function pointer is non-zero, then we
	 * assume that the default getchar function has been overridden
	 * by some overlaying application using mon_com(CHARFUNC_GETCHAR,),
	 * so we use that redefined function instead...
	 */
#if INCLUDE_MONLIB
    if (remotegetchar)
        return(remotegetchar());
#endif
    
	while(!gotachar()) {
		/* While waiting for an incoming character, call pollethernet()
		 * to process any incoming packets.  Note that if INCLUDE_ETHERNET
		 * is 0 in config.h, then this function is NULL (see ether.h).
		 */
		WATCHDOG_MACRO;
    	pollethernet();
	}

	return(target_getchar());
}

int
gotachar(void)
{
	/* If the remotegotachar function pointer is non-zero, then we
	 * assume that the default gotachar function has been overridden
	 * by some overlaying application using mon_com(CHARFUNC_GOTACHAR,),
	 * so we use that redefined function instead...
	 */
#if INCLUDE_MONLIB
    if (remotegotachar)
        return(remotegotachar());
#endif
    
	fbi_cursor();
	WATCHDOG_MACRO;
	return(target_gotachar());
}

/* flush_console_out():
 * Call this function to wait for the console UART to flush all outgoing
 * characters.  Note that this function has a timeout in the loop.
 */
void
flush_console_out(void)
{
	int timeout = 0;

#if INCLUDE_MONLIB
    if (remoteputchar)
        return;
#endif

	while(timeout++ < 50000) {
		if (target_console_empty()) {
			monDelay(10);
			break;
		}
	}
}

/* flush_console_in(void):
 * While there is input queued up from the console, 
 * flush it to the bit bucket...
 */
void
flush_console_in(void)
{
	while(gotachar())
		getchar();
}


/* rawon() & rawoff():
 * Used primarily by xmodem.  When xmodem runs, it must be assured that
 * the interface is in RAW mode.  For the case of the monitor alone, it
 * will always be in a raw mode.  These functions are primarily for use
 * when an application has re-loaded the serial driver and may have put
 * it in a non-raw mode.  The mon_com() calls CHARFUNC_RAWMODEON and
 * CHARFUNC_RAWMODEOFF establish these pointers.
 */
void
rawon(void)
{
#if INCLUDE_MONLIB
    if (remoterawon)
        remoterawon();
#endif
	rawmode = 1;
}

void
rawoff(void)
{
#if INCLUDE_MONLIB
    if (remoterawoff)
        remoterawoff();
#endif
	rawmode = 0;
}

/* puts() & putstr():
 * Two string-output functions.  
 * puts: normal "puts" as is available in standard c libs.
 * putstr: same as "puts", but no ending newline.
 */

void
putstr(char *string)
{
	while(*string) {
		putchar(*string);
		string++;
	}
}

void
puts(char *string)
{
	putstr(string);
	putchar('\n');
}


/* getfullline():
 * Basic line retrieval; but with a few options...
 * This function is accessed through the getline_xx functions
 * below.
 * Args...
 *	buf:	 pointer to buffer to be used to place the incoming characters.
 *  max:	 size of the buffer.
 *  ledit:	 if set, then allow the line-editor to be used if ESC is input.
 *  timeout: if positive, then after 'timeout' number of seconds waiting
 *                per character, giveup.
 *           if negative, then after 'timeout' number of seconds waiting
 *                total, giveup.
 *           if zero, then wait forever.
 *  prefill: if set, prefill the buffer with that string and show the user.
 *  echo:    if set, characters are echoed as they are entered.
 */
int
getfullline(char *buf,int max,int ledit, int timeout,char *prefill, int echo)
{
	char	*base;
	struct	elapsed_tmr tmr;
	static	unsigned char  crlf;
	int		tot, idx, cumulativetimeout;

	cumulativetimeout = 0;
	tot = idx = 0;
	base = buf;
	max -= 1;		/* Make sure there is space for the null terminator. */

	if (prefill) {
		strcpy(base,prefill);
		tot = strlen(prefill);
		putstr(prefill);
		buf += tot;
		idx = tot;
	}

	/* If the timeout parameter is negative, then assume that this is
	 * to be run with a cumulative timeout rather than a timeout that
	 * is re-started after each character...
	 */
	if (timeout < 0) { 
		cumulativetimeout = 1;
		timeout = abs(timeout);
	}

	for(;idx<max;idx++) {
		if (timeout > 0) {
			startElapsedTimer(&tmr,timeout);
			while(!msecElapsed(&tmr)) {
				if (gotachar())
					break;
				pollethernet();
			}
			if (cumulativetimeout)
				timeout = msecRemaining(&tmr);

			if (ELAPSED_TIMEOUT(&tmr)) {
				*buf = 0;
				return(-1);	/* Return negative to indicate timeout */
			}
		}
		if (cumulativetimeout && timeout == 0) {
			*buf = 0;
			return(-1);
		}

		*buf = (char)getchar();

		if (getenv("CLI_HEXOUT"))
			printf("<%02x>",*buf);
		
		if (!*buf) {
			idx--;
			continue;
		}
#if INCLUDE_LINEEDIT
		if ((*buf == 0x1b) && (ledit)) {
			(void)line_edit(base);
			break;
		}
		else
#endif
		{
			if ((*buf == '\r') || (*buf == '\n')) {
				if ((crlf) && (*buf != crlf)) {
					crlf = 0;
					continue;
				}
				puts("\r");
				crlf = *buf;
				*buf = 0;
				break;
			}
			if (*buf == '\b' || *buf == 0x7f) {
				if (tot) {
					idx -= 2;
					buf--; 
					tot--;
					if (echo)
						putstr("\b \b");
				}
			}
			else if (*buf == CTLC) {
				puts("^C");
				*base = 0;
				return(0);
			}
			else {
				if (echo)
					putchar(*buf);
				tot++; 
				buf++;
			}
			crlf = 0;
		}
	}
	if (idx == max) {
		printf((char *)"\007\nInput too long (exceeds %d bytes).\n",max);
		*buf = 0;
		return(0);
	}
#if INCLUDE_LINEEDIT
	if (ledit)
		historylog(base);
#endif
	return(strlen(base));
}

int
getline(char *buf, int max, int ledit)
{
	return(getfullline(buf,max,ledit,0,0,1));
}

int
getline_t(char *buf, int max, int timeout)
{
	return(getfullline(buf,max,0,timeout,0,1));
}

int
getline_p(char *buf, int max, int ledit, char *prefill)
{
	return(getfullline(buf,max,ledit,0,prefill,1));
}

/* getbytes():
 *	Similar to gets() except that the caller specifies the number
 *	of characters and whether or not to block.
 *  If the copy to the buffer fails, abort and return the current
 *  count.
 */

int
getbytes(char *buf,int cnt,int block)
{
	int	i;
	volatile char *bp;
	char c;

	bp = (volatile char *)buf;

	for(i=0;i<cnt;i++) {
		if (!block && !gotachar())
			break;
		c = (char)getchar();
		*bp = c;
		if (*bp != c)
			break;
		bp++;
	}
	return(i);
}

/* getbytes_t():
 *	Similar to getbytes() except that the caller specifies the allowed
 *  timeout between two consecutive bytes received.
 *  If the copy to the buffer fails, or timeout occures, abort and return
 *	the current count.
 */

int
getbytes_t(char *buf,int cnt,int timeout)
{
	int	i;
	struct elapsed_tmr tmr;
	volatile char *bp;
	char c;

	bp = (volatile char *)buf;

	for(i=0;i<cnt;i++) {
		if (!gotachar()) {
			startElapsedTimer(&tmr,timeout);
			while(!gotachar() && !msecElapsed(&tmr));
			if (!gotachar())
				break;
		}
		c = (char)getchar();
		*bp = c;
		if (*bp != c)
			break;
		bp++;
	}
	return(i);
}

int
putbytes(char *buf, int cnt)
{
	char *end;

	end = buf + cnt;

	while(buf < end) {
		putchar(*buf++);
	}
	return(cnt);
}

int
askuser(char *msg)
{
	int	yes, len;

#if INCLUDE_MONCMD
	/* If command was issued from UDP (i.e. moncmd), then
	 * immediately return 1...
	 */
	if (IPMonCmdActive)
		return(1);
#endif

	putstr(msg);
	len = strlen(msg);
	switch((char)getchar()) {
		case ' ':
		case 'y':
		case '\r':
		case '\n':
			yes = 1;
			break;
		default:
			yes = 0;
			break;
	}
	while(len) {
		putstr("\b \b");
		len--;
	}
	return(yes);
}

int
More(void)
{
	return(askuser((char *)"more?"));
}

int
hitakey(void)
{
	return(askuser((char *)"hit any key to continue..."));
}

/* RemoteIO functions:
 * The idea of "remote io" is to allow the monitor commands to still
 * run when the application has taken over the system.  The monitor's
 * connection to the serial port is a simple polled interface.  When 
 * the application comes up, it is very likely that it will overlay a
 * new driver onto the serial port.  If this happens, and the user at
 * the console interface of the application wants to execute a monitor
 * command, then the monitor's putchar/getchar/gotachar functions must
 * use functions that are part of the application.  These remote io
 * function pointers, if set, will point to those functions.
 */

void
InitRemoteIO(void)
{
#if INCLUDE_MONLIB
	/* Null out the remote put/getchar functions. */
	remoterawon = 0;
	remoterawoff = 0;
	remoteputchar = 0;
	remotegetchar = 0;
	remotegotachar = 0;
#endif
}