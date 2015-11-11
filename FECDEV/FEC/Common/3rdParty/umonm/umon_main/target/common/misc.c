/* misc.c:
 *	This file contains miscellaneous functions that are used by all target
 *	versions of the monitor.
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
#include "stddefs.h"
#include "cli.h"
#include "monlib.h"
#include "genlib.h"
#include "devices.h"
#include "cpuio.h"
#include "ctype.h"
#include "ether.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "warmstart.h"
#include "timer.h"
#include "version.h"
#include "target_version.h"

#ifdef USR_HEADER_FUNC
extern void USR_HEADER_FUNC(void);
#endif

/* extWatchDog():
 * This function pointer is used by the application to override the
 * monitor's configured watchdog functionality.  Refer to config.h
 * in umon_ports/template for more information on the WATCHDOG macro.
 */
#ifdef WATCHDOG_ENABLED
int (*extWatchDog)(void);
#endif

/* AppExitStatus:
 * Global copy of the application's exit status.
 */
int	AppExitStatus;

char *
monVersion(void)
{
	static char buf[16];

	/* Build the version string as "X.Y.Z"
	 */
	snprintf(buf,sizeof(buf),"%d.%d.%d",
		MAJOR_VERSION,MINOR_VERSION,TARGET_VERSION);

	return(buf);
}

/* ShowVersion():
 *	Display the build time and date of the running monitor.
 */
void
ShowVersion(void)
{
	printf("Monitor core release: %d.%d, target version %d (built: %s)\n",
		MAJOR_VERSION,MINOR_VERSION,TARGET_VERSION,monBuilt());
}

/* monHeader():
 *	Dump the common stuff that is displayed at the console port when
 *	the monitor first starts up.  If called with 'center' set, then
 *	the printed output is centered on an 80-character screen width.
 */
void
monHeader(int center)
{
	char	*ip, *mac;

	printf("\n%s\n",PLATFORM_NAME);
	printf("uMon-M %s\n",monVersion());
	printf("Built: %s\n",monBuilt());
	printf("AppRamBase: 0x%06x\n",APPLICATION_RAMSTART);
	mac = getenv("ETHERADD");
	if (mac)
		printf("MAC: %s\n",mac);
	ip = getenv("IPADD");
	if (ip)
		printf("IP: %s\n",ip);

#ifdef USR_HEADER_FUNC
	USR_HEADER_FUNC();
#endif
}

/* stkchk():
 *	The monitor's stack is a statically allocated array (MonStack).
 *	At initialization the stack pointer is set to the top of this stack
 *	and the bottom is loaded with 0xdeaddead.  This function simply 
 *	verifies that the bottom of the stack is sane (still reads 0xdeaddead).
 *	If not, an error is printed.
 */
int
stkchk(char *msg)
{
	extern ulong	MonStack[];

	if (MonStack[0] != 0xdeaddead) {
		printf("\n***** Monitor stack overflow (%s) *****\n",msg);
		MonStack[0] = 0xdeaddead;
		return(-1);
	}
	return(0);
}

void
stkusage(void)
{
	int		i;
	ulong	*sp, stackval;
	extern ulong	MonStack[];

	printf("Stack: bottom=0x%lx, size=%d",(long)MonStack,MONSTACKSIZE);

	sp = MonStack;
	sp++;	/* Increment past 0xdeaddead */

	/* If the very first entry in stack space is not set to
	 * STACK_PREINIT_VAL, then we assume that stkinit() was not called
	 * at startup so don't print any stack usage information...
	 */
	if (*sp == STACK_PREINIT_VAL) {
		for(i=0;i<MONSTACKSIZE;i+=4) {
			if (*sp++ != STACK_PREINIT_VAL)
				break;
		}
		printf(" (%d%% used)",((MONSTACKSIZE-i)*100)/MONSTACKSIZE);
	}
	printf("\n");

	/* Finally, check to make sure that the current stack pointer
	 * resides within the MonStack[] array...
	 */
	if ((&stackval < MonStack) || (&stackval > &MonStack[MONSTACKSIZE/4]))
		printf("WARNING: SP outside MonStack[]\n");
}

/* stkinit():
 * This function should be called very early in uMon startup prior to
 * the stack pointer being set to the end of MonStack[].  It
 * simply initializes the MonStack[] array to a defined value so
 * that the stack can be observed for fullness during runtime.
 */
void
stkinit(void)
{
	extern ulong	MonStack[];
	volatile ulong *sp, *spend;

	sp = (ulong *)MonStack;
	spend = MonStack+MONSTACKSIZE/4;
	while(sp < spend)
		*sp++ = STACK_PREINIT_VAL;
}

/* writeprompt():
 *	Called to print out the monitor prompt.  If the shell variable PROMPT
 *	exists, then use the content of that variable as a prompt; otherwise,
 *	just print uMON>.
 */
void
writeprompt(void)
{
	char	*prompt;

	prompt = getenv("PROMPT");
	if (!prompt)
		prompt = "uMON>";
	else if (strcmp(prompt,"NULL") == 0)
		return;

	putstr(prompt);
}

/* CommandLoop():
 *	This function is called at the end of monitor initialization.
 *	The monitor spends most of its time here, waiting for incoming
 *	characters on the console interface.
 */
void
CommandLoop(void)
{
	static	char cmdline[CMDLINESIZE];

	while(1) {
		stkchk("Cmdloop");				/* Verify valid monitor stack */
		writeprompt();					/* Issue prompt */
		memset(cmdline,0,CMDLINESIZE);	/* Clear command line buffer */
		if (getline(cmdline,CMDLINESIZE,INCLUDE_LINEEDIT) == 0)
			continue;
		docommand(cmdline,0);
#if INCLUDE_FLASH
 		LowerFlashProtectWindow();
#endif
	}
}
/* exceptionAutoRestart():
 *	Serves three purposes:
 *	1. Copies the verbose description of the exception to the
 *	   shell variable EXCEPTION_TYPE.
 *	2. If there is an EXCEPTION_SCRIPT shell variable, then see if the
 *		user wants it to be executed.
 *	3. If there is no NO_EXCEPTION_RESTART variable, then 
 *		call monrestart with the incoming value (usually INITIALIZE).
 */
void
exceptionAutoRestart(int restartvalue)
{
	char *script;
	char *arglist[2];

	setenv("EXCEPTION_TYPE",ExceptionType2String(ExceptionType));
	script = getenv("EXCEPTION_SCRIPT");
	if ((script) &&
		(!pollConsole("Press any key to stop exception script.\n"))) {
		arglist[0] = script;
		arglist[1] = (char *)0;
		tfsrun(arglist,0);
	}

	if (!getenv("NO_EXCEPTION_RESTART")) {
		if (!pollConsole("Press any key to stop auto restart.\n"))
			monrestart(restartvalue);
	}
}

/* getAppRamStart():
 *	First looks for the content of APPRAMBASE shell variable;
 *	if present, that string is converted to a long and returned,
 *	else the value of APPLICATION_RAMSTART is returned.
 */
ulong
getAppRamStart(void)
{
	char	*apprambase;
	ulong	value;

	apprambase = getenv("APPRAMBASE");
	if (apprambase)
		value = strtoul(apprambase,0,0);
	else
		value = APPLICATION_RAMSTART;
	return(value);
}

/* inUmonBssSpace()
 *	Return 1 if the address range falls within MicroMonitor's
 *	own	RAM space.
 */
int
inUmonBssSpace(char *start,char *end)
{
	static int nowarn;

	if (nowarn || getenv("NO_UMONBSS_WARNING")) {
		nowarn = 1;
		return(0);
	}

	if (((start >= (char *)&bss_start) && (start < (char *)&bss_end)) ||
		((end >= (char *)&bss_start) && (end < (char *)&bss_end)) ||
		((start <= (char *)&bss_start) && (end >= (char *)&bss_end))) {
		printf("\nError: addr range 0x%lx <-> 0x%lx violates uMon ram.\n",
			(long)start,(long)end);
		return(1);
	}

	return(0);
}

void
monrestart(int val)
{
	/* Allow UART0 transmit FIFO to empty...
	 */
	flush_console_out();
	intsoff();
    warmstart(val);
}

void
appexit(int val)
{
	AppExitStatus = val;
	monrestart(APP_EXIT);
}

/* monWatchDog():
 * This function is hooked to the monitor API so that the application
 * can use the same function used by monitor code for exciting the
 * watchdog hardware.
 * Note that if there is no defined WATCHDOG_MACRO, then this function
 * returns -1; else it executes the macro and returns 0.  This
 * return value can be used by the application to decide whether or
 * not the call to mon_watchdog() even needs to occur.
 */
int
monWatchDog(void)
{
#ifdef WATCHDOG_ENABLED
	WATCHDOG_MACRO;
	return(0);
#else
	return(-1);
#endif
}


/* putargv() & getargv():
 * The getargv() function provides a hook allowing the downloaded
 * application code to handle main(argc,argv) in a painless way.
 * The Argv[] array location is returned by mon_getargv() and is
 * assumed to contain the NULL terminated list of argv pointers...
 * Immediately after the NULL termination is the data that the argv
 * pointers are referencing.  
 *
 * MONLIB NOTICE: getargv() is accessible through monlib.c.
 */

/* ArgvList:
 * Used by the code that is behind the mon_getargv() API.
 */
static char		*ArgvList[32];

int
putargv(int argnum,char *argptr)
{
#if INCLUDE_SHELLVARS
	int	i;
	char buf[8];
#endif

	if (argnum >= (sizeof(ArgvList)/sizeof(char *)))
		return(-1);

	if (argnum == 0) {
#if INCLUDE_SHELLVARS
		for(i=0;i<(sizeof(ArgvList)/sizeof(char *));i++) {
			sprintf(buf,"ARG%d",i);
			setenv(buf,0);
		}
		setenv("ARGC",0);
#endif
		ArgvList[0] = (char *)0;
	}

	ArgvList[argnum] = argptr;

#if INCLUDE_SHELLVARS
	if (argptr) {
		sprintf(buf,"ARG%d",argnum);
		setenv(buf,ArgvList[argnum]);
	}
	else {
		shell_sprintf("ARGC","%d",argnum);
	}
#endif

	return(0);
}

void
getargv(int *argc,char ***argv)
{
	int	i;

	if (argv)
		*argv = &ArgvList[0];
	for(i=0;;i++)
		if (ArgvList[i] == 0)
			break;
	if (argc)
		*argc = i;
}

