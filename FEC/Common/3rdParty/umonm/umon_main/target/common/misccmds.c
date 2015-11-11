/* misccmds:
 *	More monitor commands...
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
#include "tfs.h"
#include "tfsprivate.h"
#include "genlib.h"
#include "ether.h"
#include "devices.h"
#include "cli.h"
#include "ctype.h"
#include "warmstart.h"

int		(*extgetUsrLvl)(void);

int
getUsrLvl(void)
{
	return(MAXUSRLEVEL);
}

#if INCLUDE_VERSIONCMD
char *VersionHelp[] = {
		"Version information",
		"",
		0,
};

int
Version(int argc,char *argv[])
{
	extern	void ShowVersion(void);

	ShowVersion();
	return(CMD_SUCCESS);
}
#endif

#if INCLUDE_TFSSCRIPT
char *EchoHelp[] = {
	"Print string to local terminal",
	"[arg1] ... [argn]",
	0,
};

int
Echo(int argc,char *argv[])
{
	char	*cp;
	int		i, done;

	for(i=optind;i<argc;i++) {
		cp = argv[i];
		done = 0;
		while(!done && *cp)
			putchar(*cp++);
		if (i != argc-1) {
			putchar(' ');
		}
	}
	putchar('\n');
	flush_console_out();
	return(CMD_SUCCESS);
}
#endif

#if INCLUDE_CALLCMD
/* Call():
 *	This function is called when the user wants to execute an 
 *	embedded function. 
 *  The the argument is preceded by an ampersand, then a pointer
 *  to the argument is passed to the function instead of a 
 *	strtol() conversion.
 */
char *CallHelp[] = {
	"Call embedded function",
	"-[Av:] {address} [arg1] [arg2] ...",
#if INCLUDE_VERBOSEHELP
	" -v {var} put return val in varname",
#endif
	0,
};

int
Call(int argc,char *argv[])
{
	char	monargs;
	int		i, j, opt;
	long	args[10], func_addr;
	int		(*func)(long,long,long,long,long,long,long,long,long,long);

	monargs = 0;
	while((opt=getopt(argc,argv,"A")) != -1) {
		switch(opt) {
		case 'A':
			monargs = 1;
			break;
		default:
			return(CMD_PARAM_ERROR);
		}
	}

	if ((argc < optind+1) || (argc > optind+11))
		return(CMD_PARAM_ERROR);

    func_addr = strtol(argv[optind],(char **)0,0);

#ifdef ADJUST_THUMB_ADDRESS // If THUMB, then add 1...
    func_addr = ADJUST_THUMB_ADDRESS(func_addr);
#endif

    func = (int(*)(long,long,long,long,long,long,long,long,long,long))func_addr;

	if ((func == 0) && (isdigit(argv[optind][0]) == 0)) {
		return(CMD_PARAM_ERROR);
	}

	/* If -A option is specified, then we push all command line args
	 * through putargv() for use by getargv() in the application;
	 * otherwise just retrieve and convert args from command line.
	 */
	if (monargs) {
		for(j=0,i=optind;i<argc;i++,j++)
			putargv(j,argv[i]);
		putargv(j,(char *)0);
	}
	else {
		/* If the first character of the argument is an ampersand (&),
		 * then a pointer to the argument is passed; otherwise, the
		 * argument is converted to a long integer using strtol()...
		 */
		for(j=0,i=optind+1;i<argc;i++,j++) {
			if (argv[i][0] == '&')
				args[j] = (ulong)&argv[i][1];
			else
				args[j] = strtol(argv[i],(char **)0,0);
		}
	}

	func(args[0],args[1],args[2],args[3],args[4],args[5],args[6],
		args[7],args[8],args[9]);

	return(CMD_SUCCESS);
}
#endif

#if INCLUDE_RESETCMD
/* Reset():
 *	Used to re-initialize the monitor through the command interface.
 */

char *ResetHelp[] = {
	"Reset monitor firmware",
	"-[x]",
#if INCLUDE_VERBOSEHELP
	" -x      app_exit",
#endif
	0,
};

int
Reset(int argc,char *argv[])
{
	extern	void appexit(int);
	int	opt;

	intsoff();

	/* For some systems, the reset occurs while characters are in the
	 * UART FIFO (so they don't get printed).  Adding this delay will
	 * hopefully allow the characters in the FIFO to drain...
	 */
	monDelay(250);

	while((opt=getopt(argc,argv,"x")) != -1) {
		switch(opt) {
		case 'x':
			appexit(0);
			break;
		default:	
			return(CMD_PARAM_ERROR);
		}
	}
	target_reset();
	return(CMD_SUCCESS);
}
#endif
