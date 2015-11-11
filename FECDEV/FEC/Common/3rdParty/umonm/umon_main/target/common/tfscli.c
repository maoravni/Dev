/* tfscli.c:
 *	This file contains the TFS code that is only needed if the "tfs" command
 *	is included in the command set.
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
#include "genlib.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "cli.h"

#if INCLUDE_TFSCLI

/* tfsprflags():
 *   Print the specified set of flags.
 */
static void
tfsprflags(long flags, int verbose)
{
	struct	tfsflg	*tfp;

	if (verbose)
		printf(" Flags: ");
	tfp = tfsflgtbl;
	while(tfp->sdesc) {
		if ((flags & tfp->mask) == tfp->flag) {
			if (verbose) {
				printf("%s", tfp->ldesc);
				if ((tfp+1)->flag)
					printf(", ");
			}
			else
				putchar(tfp->sdesc);
		}
		tfp++;
	}
	if (!(flags & TFS_NSTALE)) 
		printf("stale");
	
	if (verbose)
		printf("\n");
}

static char *
tfsmodebtoa(ulong mode,char *buf)
{
	char	*pipe, *bp;

	pipe = "";
	bp = buf;
	*bp = 0;
	if (mode & TFS_RDONLY) {
		bp += sprintf(bp,"rdonly");
		pipe = "|";
	}
	if (mode & TFS_CREATE) {
		bp += sprintf(bp,"%screate",pipe);
		pipe = "|";
	}
	if (mode & TFS_APPEND)
		sprintf(bp,"%sappend",pipe);

	return(buf);
}

/* listfilter():
 *	If the incoming filename (fname) passes the incoming filter, then
 *	return 1; else return 0.
 *
 *	Examples:
 *		if filter is "*.html" and fname is "index.html" return 1.
 *		if filter is "dir SLASH ASTERISK" and fname is "dir/abc" return 1.
 *		if filter is "abc" and fname is "abc" return 1.
 *
 *	Notes:
 *		* A valid filter will have the asterisk as either the first or last
 *		  character of the filter.  If first, assume filter is a suffix, 
 *		  if last (or none at all), assume filter is a prefix.
 *		* If there is an asterisk in the middle of the filter, it is chopped
 *		  at the asterisk without warning.
 */
static int
listfilter(char *filter,char *fname)
{
	int		flen;
	char	*prefix, *suffix, *asterisk, *sp;

	if (!filter)		/* No filter means match everything. */
		return(1);

	flen = 0;
	prefix = suffix = (char *)0;
	asterisk = strchr(filter,'*');

	/* If no asterisk, then just compare filter to fname... */
	if (!asterisk) {
		if (!strcmp(filter,fname))
			return(1);
	}
	else if (asterisk == filter) {
		suffix = asterisk+1;
		flen = strlen(suffix);
		sp = fname + strlen(fname) - flen;
		if (!strcmp(suffix,sp))
			return(1);
	}
	else {
		*asterisk = 0;
		prefix = filter;
		flen = strlen(prefix);
		if (!strncmp(prefix,fname,flen)) {
			*asterisk = '*';
			return(1);
		}
		*asterisk = '*';
	}
	return(0);
}

/* tfsvlist():	verbose list...
 *	Display all files currently stored.  Do not put them in alphabetical
 *	order; display them as they are physically listed in the file system.
 *	Display complete structure of file header for each file.
 *	Note1: This version of file listing is only called if "tfs -vv ls"
 *	or "tfs -vvv ls" is called.  The first level of verbosity is handled
 *	by tfsqlist to simply display the "dot" files.
 */
static int
tfsvlist(char *filter[], int verbose, int more)
{
	TDEV	*tdp;
	TFILE	*fp;
	int		tot, sizetot;
	char	tbuf[32], **fltrptr;

	tot = 0;
	sizetot = 0;
	for(tdp=tfsDeviceTbl;tdp->start != TFSEOT;tdp++) {
		fltrptr = filter;
		while(1) {
			fp = (TFILE *)tdp->start;
			while(validtfshdr(fp)) {

				if ((TFS_DELETED(fp)) && (verbose < 3)) {
					fp = nextfp(fp,tdp);
					continue;
				}
				if (!listfilter(*fltrptr,TFS_NAME(fp))) {
					fp = nextfp(fp,tdp);
					continue;
				}
				if ((fp->flags & TFS_UNREAD) && (TFS_USRLVL(fp)>getUsrLvl())) {
					fp = nextfp(fp,tdp);
					continue;
				}
				printf(" Name:  '%s'%s\n",
					fp->name,TFS_DELETED(fp) ? " (deleted)" : "");
				printf(" Info:  '%s'\n", fp->info);
				if (TFS_FILEEXISTS(fp))
					tfsprflags(fp->flags, 1);
				printf(" Addr:  0x%lx (hdr @ 0x%lx, nxtptr = 0x%lx)\n",
					(ulong)(TFS_BASE(fp)),(ulong)fp,(ulong)(fp->next));
				printf(" Size:  %ld bytes (crc=0x%lx)",
					fp->filsize,fp->filcrc);
#if INCLUDE_FLASH
				if ((tdp->devinfo & TFS_DEVTYPE_MASK) != TFS_DEVTYPE_RAM) {
					int	start_sector, end_sector;
					addrtosector((uchar *)fp,&start_sector,0,0);
					addrtosector((uchar *)fp+fp->filsize+TFSHDRSIZ,
						&end_sector,0,0);
					if (start_sector == end_sector) {
						printf(" in sector %d",start_sector);
					}
					else {
						printf(" spanning sectors %d-%d",
							start_sector,end_sector);
					}
				}
#endif
				putchar('\n');
				sizetot += (fp->filsize + TFSHDRSIZ + DEFRAGHDRSIZ);
				if (TFS_TIME(fp) != TIME_UNDEFINED)
					printf(" Time:  %s\n",
						tfsGetAtime(TFS_TIME(fp),tbuf,sizeof(tbuf)));
				printf("\n");
				tot++;
				fp = nextfp(fp,tdp);
				if ((more) && ((tot % more) == 0)) {
					if (!More())
						return(TFS_OKAY);
				}
			}
			/* If this or the next pointer is null, terminate loop now... */
			if (!*fltrptr) break;
			fltrptr++;
			if (!*fltrptr) break;
		}
	}
	printf("Total: %d accessible file%s (%d bytes).\n",
		tot,tot == 1 ? "" : "s",sizetot);
	return (TFS_OKAY);
}

/* tfsqlist():	quick list...
 *	Display list of files in alphabetical order.
 *	Display only the name and flag summary.
 *
 *	To support listing of files in a bit of an orderly manner, if this
 *	function sees that a filename has a slash in it, it will only print the
 *	characters upto and including the first slash.  This gives the appearance
 *	of a directory structure, even though there really isn't one.
 *	For example, if there are three files...
 *			CONFIG/file1
 *			CONFIG/file2
 *			CONFIG/file3
 *	then if no filter is specified, then that listing will be replaced with
 *			CONFIG/
 *	printed only once.  To display all the files prefixed with CONFIG/, the
 *	user must type "tfs ls CONFIG/\*".
 *	
 *	Note: a file with a leading dot ('.') is invisible unless verbose is set.
 */
static int
tfsqlist(char *filter[], int verbose, int more)
{
	TFILE	*fp;
	char	dirname[TFSNAMESIZE+1], tmpname[TFSNAMESIZE+1];
	char	*name, fbuf[16], **fltrptr, *slash, *flags;
	int		idx, sidx, filelisted, err, sizetot;

	if ((err = tfsreorder()) < 0)
		return(err);

	filelisted = 0;
	sizetot = 0;
	dirname[0] = 0;
	fltrptr = filter;
	printf(" Name                        Size   Location   Flags  Info\n");
	while(1) {
		idx = 0;
		while((fp = tfsAlist[idx])) {
			name = TFS_NAME(fp);
			if (((name[0] == '.') && (!verbose)) ||
				(!listfilter(*fltrptr,name)) ||
				((fp->flags & TFS_UNREAD) && (TFS_USRLVL(fp) > getUsrLvl()))) {
				idx++;
				continue;
			}

			/* If name contains a slash, process it differently (but ignore */
			/* any leading slashes) */
			strcpy(tmpname,TFS_NAME(fp));
			for(sidx=0;sidx<TFSNAMESIZE;sidx++) {
				if (tmpname[sidx] != '/')
					break;
			}
			slash = strchr(&tmpname[sidx],'/');
			if (slash && !*fltrptr) {
				char tmp;

				tmp = *(slash+1);
				*(slash+1) = 0;
				if (strcmp(dirname,tmpname)) {
					filelisted++;
					printf(" %-34s  (dir)\n",tmpname);
					strcpy(dirname,tmpname);
					*(slash+1) = tmp;
				}
				else {
					idx++;
					*(slash+1) = tmp;
					continue;
				}
			}
			else {
				filelisted++;
				sizetot += TFS_SIZE(fp);
				flags = tfsflagsbtoa(TFS_FLAGS(fp),fbuf);
				if ((!flags) || (!fbuf[0]))
					flags = " ";
				printf(" %-23s  %7ld  0x%08lx  %-5s  %s\n",TFS_NAME(fp),
					TFS_SIZE(fp),(ulong)(TFS_BASE(fp)),flags,TFS_INFO(fp));
			}
			idx++;
			if ((more) && !(filelisted % more)) {
				if (!More())
					return(TFS_OKAY);
			}
		}
		/* If this or the next pointer is null, terminate loop now... */
		if (!*fltrptr) break;
		fltrptr++;
		if (!*fltrptr) break;
	}
	printf("\nTotal: %d item%s listed (%d bytes).\n",filelisted,
		filelisted == 1 ? "" : "s",sizetot);
	return (TFS_OKAY);
}

/* tfsrm():
 *	Remove all files that pass the incoming filter.
 *	This replaces the older tfs rm stuff in Tfs().
 */
static int
tfsrm(char *filter[])
{
	TFILE	*fp;
	char	*name, **fltrptr;
	int		idx, err, rmtot;

	if ((err = tfsreorder()) < 0)
		return(err);

	fltrptr = filter;
	while (*fltrptr) {
		idx = rmtot = 0;
		while((fp = tfsAlist[idx])) {
			name = TFS_NAME(fp);
			if (listfilter(*fltrptr,name)) {
				if ((err = tfsunlink(name)) != TFS_OKAY) 
					printf("%s: %s\n",name,(char *)tfsctrl(TFS_ERRMSG,err,0));
				rmtot++;
			}
			idx++;
		}
		/* This function will potentially delete many files, but if the */
		/* filter doesn't match at least one file, indicate that... */
		if (!rmtot)
			printf("%s: file not found\n",*fltrptr);
		fltrptr++;
	}

	return(TFS_OKAY);
}

/* tfscat():
 *	Print each character of the file until NULL terminate. Replace
 *	each instance of CR or LF with CRLF.
 */
static void
tfscat(TFILE *fp, int more)
{
	int	i, lcnt;
	char	*cp;

	lcnt = 0;
	cp = (char *) (TFS_BASE(fp));
	for(i=0;i<fp->filsize;i++) {
		if (*cp == 0x1a)	/* EOF or ctrl-z */
			break;
		putchar(*cp);
		if ((*cp == '\r') || (*cp == '\n')) {
			lcnt++;
			if (lcnt == more) {
				if (More() == 0)
					break;
				lcnt = 0;
			}
		}
		cp++;
	}
}

char *TfsHelp[] = {
	"Tiny File System Interface",
	"-[df:i:mv] operation [args]...",
#if INCLUDE_VERBOSEHELP
	"",
	"Options:",
	" -d    tfs device",
	" -f    flags (see below)",
	" -i    info",
	" -m    enable more throttle",
	" -v    enable verbosity",
	"",
	"Operations (alphabetically):",
	" add {name} {src_addr} {size}, base {file} {var}, cat {name}",
	" check [var], clean",
	" cp {from} {to_name | addr}",
	" info {file} {var}",
	" ls [filter]",
	" qclean [ramstart] [ramlen]",
	" rm {filter}, run {name}, size {file} {var}, stat",
	"",
	"Flags:",
	" e=exec_script, c=compressed, l=symlink",
	" b=run_at_boot, B=qry_run_at_boot, i=inplace_modifiable",
#endif
	0,
};

/* Tfs():
 *	Entry point for the tfs command through the monitor's command line
 *	interface.  This function provides access to most TFS functionality
 *	through the CLI.
 */

int
Tfs(int argc, char *argv[])
{
	TDEV	*tdp, *tdptmp;
	TFILE	*fp;
	TINFO	tinfo;
	long	size;
	int		opt, verbose, i, more, retval, status;
	char	*src, *name, *info, *flags, *to, *from;
	char	*arg1, *arg2, *arg3, *arg4;

	tdp = 0;
	more = 0;
	retval = CMD_SUCCESS;
	verbose = 0;
	status = TFS_OKAY;
	info = (char *)0;
	flags = (char *)0;
	while ((opt = getopt(argc, argv, "vmf:i:")) != -1) {
		switch (opt) {
		case 'f':
			flags = optarg;
			break;
		case 'i':
			info = optarg;
			break;
		case 'm':
			more++;
			break;
		case 'v':
			verbose++;
			break;
		default:
			return(CMD_PARAM_ERROR);
		}
	}

	arg1 = argv[optind];
	arg2 = argv[optind+1];
	arg3 = argv[optind+2];
	arg4 = argv[optind+3];

	if (argc == optind) {
		retval = CMD_PARAM_ERROR;
	}
	else if (strcmp(arg1, "stat") == 0) {
		char buf[32];
		int	 opencnt;
		struct	tfsdat *slot;

		/* Display current TFS memory usage: */
		tfsmemuse(tdp,&tinfo,1);
		printf("TFS Hdr size: %d\n",TFSHDRSIZ);

		/* Display currently opened files: */
		opencnt = 0;
		slot = tfsSlots;
		for (i=0;i<TFS_MAXOPEN;i++,slot++) {
			if (slot->offset >= 0) {
				printf("%3d: 0x%08lx %s %s\n",i,(ulong)(slot->base),
					tfsmodebtoa(slot->flagmode,buf),slot->hdr.name);
				opencnt++;
			}
		}
		printf("Total files currently opened: %d\n",opencnt);
	}
	else if (((strcmp(arg1, "base") == 0) ||
			 (strcmp(arg1, "info") == 0) ||
			 (strcmp(arg1, "size") == 0)) && (argc == (optind+3))) {
		fp = tfsstat(arg2);
		if ((!fp) ||
			((fp->flags & TFS_UNREAD) && (TFS_USRLVL(fp) > getUsrLvl())))
			setenv(arg3,0);
		else {
			switch(arg1[0]) {
				case 'b':
					shell_sprintf(arg3,"0x%x",(char *)fp+TFSHDRSIZ);
					break;
				case 'i':
					setenv(arg3,fp->info);
					break;
				case 's':
					shell_sprintf(arg3,"%ld",fp->filsize);
					break;
			}
		}
	}
	else if (strcmp(arg1, "ls") == 0) {
		if (verbose > 1)
			status = tfsvlist(&argv[optind+1],verbose,more << 1);
		else
			status = tfsqlist(&argv[optind+1],verbose,more << 3);

		showTfsError(status,"ls");
	}
	else if ((strcmp(arg1, "cp") == 0) && (argc == (optind+3))) {
		char	buf[16], linfo[TFSINFOSIZE];

		from = arg2;
		to = arg3;
		fp = tfsstat(from);
		if (fp) {
			if (flags) {
				if (strcmp(flags,"0") == 0)
					flags = (char *)0;
			}
			else {
				flags = tfsflagsbtoa(fp->flags,buf);
			}
			if (!info)
				strcpy(linfo,fp->info);
			else
				strcpy(linfo,info);
			if ((fp->flags & TFS_UNREAD) &&
				(TFS_USRLVL(fp) > getUsrLvl())) {
				status = showTfsError(TFSERR_USERDENIED,from);
			}
			else if (to[0] == '0' && to[1] == 'x') {
				memcpy((char *)strtol(to,0,16),TFS_BASE(fp),TFS_SIZE(fp));
				flushDcache((char *)strtol(to,0,16), TFS_SIZE(fp));
				invalidateIcache((char *)strtol(to,0,16), TFS_SIZE(fp));
			}
			else {
				int freespace;

				/* First check to see if a defrag is needed...
				 */
				freespace = tfsctrl(TFS_MEMAVAIL,0,0);
				if (freespace <= TFS_SIZE(fp)) {
					tfsctrl(TFS_DEFRAG,0,0);
					fp = tfsstat(from);
				}
				status = tfsadd(to,linfo,flags,(uchar *)TFS_BASE(fp),
					TFS_SIZE(fp));
				showTfsError(status,to);
			}
		}
		else
			status = showTfsError(TFSERR_NOFILE,from);
	}
	else if ((strcmp(arg1, "cat") == 0) && (argc >= (optind+2))) {
		for(i=optind+1;i<argc;i++) {
			name = argv[i];
			fp = tfsstat(name);
			if (fp) {
				if ((fp->flags & TFS_UNREAD) &&
					(TFS_USRLVL(fp) > getUsrLvl())) {
					status = showTfsError(TFSERR_USERDENIED,name);
				}
				else {
					tfscat(fp,more << 3);	/* more times 8 */
				}
			}
			else
				status = showTfsError(TFSERR_NOFILE,name);
		}
	}
	else if ((strcmp(arg1, "rm") == 0) && (argc >= (optind+2))) {
		status = tfsrm(&argv[optind+1]);
		showTfsError(status,0);
	}
	else if (strcmp(arg1, "clean") == 0) {
		if (argc != optind+1)
			return(CMD_PARAM_ERROR);

		/* If tdp has been set by the -d option, only defrag the affected
		 * device; else, defrag all devices...
		 */
		for (tdptmp=tfsDeviceTbl;tdptmp->start != TFSEOT;tdptmp++) {
			if (!tdp || (tdp == tdptmp)) {
				status = tfsclean(tdptmp,verbose);
				showTfsError(status,tdptmp->prefix);
			}
		}
	}
	else if (strcmp(arg1, "check") == 0) {
		if (argc == optind+1)
			verbose = 1;
	
		if (tfscheck(tdp,verbose) != TFS_OKAY)
			status = TFSERR_CORRUPT;

		/* If an additional argument is provided after the "check" command
		 * then assume it is a shell variable name that is to be populated
		 * with the pass/fail status of the check...
		 */
		if (argc == optind+2)
			setenv(arg2,status == TFS_OKAY ? "PASS" : "FAIL");
	}
	else if ((strcmp(arg1, "run") == 0) && (argc >= (optind+2))) {
		status = tfsrun(&argv[optind+1],verbose);
		showTfsError(status,arg2);
	}
	else if ((!(strcmp(arg1, "add"))) && (argc == (optind+4))) {
		src = (char *) strtol(arg3, (char **) 0, 0);
		size = strtol(arg4, (char **) 0, 0);
		status = tfsadd(arg2, info, flags, (uchar *)src, size);
		showTfsError(status,arg2);
		if (status != TFS_OKAY)
			retval = CMD_FAILURE;
	}
	else {
		retval = CMD_PARAM_ERROR;
	}
	return(retval);
}
#endif
