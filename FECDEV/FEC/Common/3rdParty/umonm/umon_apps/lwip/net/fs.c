/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 * Modified significantly: Ed Sutter (ed.sutter@alcatel-lucent.com)
 * Changed to support interface with uMon's TFS file system.
 *
 */
#include <strings.h>
#include <ctype.h>
#include "lwip/def.h"
#include "httpd.h"
#include "monlib.h"
#include "tfs.h"

char *nofile = "<html>\n<head><title>Page Not Found</title></head>\n<body bgcolor=\"white\" text=\"black\">\n<body>\n<center><h1><br><br><b>Sorry!</h1></b><br>The page you're looking for is not available<br></center></body></html>\n";

/* ssi code:
 * Server Side Include (SSI) support code.
 * The idea here is that the http server will parse all .shtml files for SSI-strings.
 * As of this writing, an SSI string is formatted as follows:
 *
 *  SSI_PREFIX + COMMAND + ":" + OPTIONAL_COMMAND_ARGS + SSI_SUFFIX
 *
 * As of this writing, the only command supported is 'echo', which tells
 * the server that the command-arguments are a combination of strings and
 * shell variables...
 *
 *  <!--#echo:${PLATFORM}<br>IP=${IPADD}<br>GW=${GIPADD}-->
 *
 * See ssievaluate() below for the guts of the parsing.
 */

char *
getSsiStateStr(int state)
{
	char *str = "???";

	switch(state) {
		case SSISTATE_OFF:
			str = "OFF";
			break;
		case SSISTATE_INPROGRESS:
			str = "INPROGRESS";
			break;
		case SSISTATE_GOTPREFIX:
			str = "GOTPREFIX";
			break;
	}
	return(str);
}

void
gotoSsiState(struct fs_file *fsfp, int state)
{
	if (fsfp) {
		//mon_printf("SSI: %s->%s\n",getSsiStateStr(fsfp->ssistate),getSsiStateStr(state));
		fsfp->ssistate = state;
	}
}

/* ssiprefix():
 * If the buffer pointed to by 'buf' contains the SSI prefix ("<!--#"), or it
 * with some portion of the prefix, return the offset at which it begins;
 * else return -1.
 */
int
ssiprefix(char *buf, int sz)
{
	int i;
	char *pp, *prefix;

	prefix = SSI_PREFIX;
	pp = prefix;
	for (i=0;i<sz;i++) {
		if (buf[i] == *pp) {
			pp++;
			if (*pp == 0) {
				// This block contains the entire prefix
				return(i+1-SIZEOF_PREFIX);
			}
		}
		else {
			pp = prefix;
		}
	}
	if (pp != prefix) {
		// This block ends with an initial portion of the prefix
		return(i-(pp-prefix));
	}
	else {
		// This block contains no portion of the prefix
		return(-1);
	}
}

int
ssisuffix(char *buf, int sz)
{
	int i;
	char *sp, *suffix;

	sp = suffix = SSI_SUFFIX;
	for (i=0;i<sz;i++) {
		if (buf[i] == *sp) {
			sp++;
			if (*sp == 0) {
				// This block contains the entire suffix
				return(i-SIZEOF_SUFFIX);
			}
		}
		else {
			sp = suffix;
		}
	}
	if (sp != suffix) {
		// This block ends with an initial portion of the prefix
		return(i-(sp-suffix));
	}
	else {
		// This block contains no portion of the prefix
		return(-1);
	}
}

#define STATE_BEGIN			0
#define STATE_GOTDOLLAR		1
#define STATE_GOTOBRACE		2
#define STATE_GOTCOLON		3
#define STATE_GOTCBRACE		4

#define _DOLLAR_	'$'
#define _OBRACE_	'{'
#define _CBRACE_	'}'
#define _COLON_		':'

/* ssievalutate:
 * This function is called whenever the server has detected a complete SSI
 * block in a file being served. 
 * The incoming buffer is used as input and output.  It contains the
 * complete SSI block string. 
 * This function parses that string and does application specific processing
 * on the data, and repopulates the buffer with up to 'sz' bytes of data
 * that will end up as part of the .html file being pumped to the client.
 * The string is formatted as:
 *
 *	SSI_PREFIX + COMMAND + ":" + COMMAND_ARGS + SSI_SUFFIX
 *
 * or...
 *
 *	<!--#CMD: CMDARGS-->"
 *
 */
int
ssievaluate(char *buf, int sz)
{
	int len;
	char *bp, *sfx;

	/* Sanity check... */
	if (strncmp(buf,SSI_PREFIX,SIZEOF_PREFIX) != 0) {
		return(0);
	}
	if ((sfx = strstr(buf,SSI_SUFFIX)) == 0) {
		return(0);
	}

	bp = buf+SIZEOF_PREFIX;

    if (strncasecmp(bp,"echo:",5) == 0) {
		/* Example:
		 *
		 * <!--#echo:PLATFORM=${PLATFORM}<br>IP=${IPADD}<br>GW=${GIPADD}<br>-->
		 *
		 * Converts to:
		 *
		 *		PLATFORM=SimpleCortex
		 *		IP=192.168.1.20
		 *		GW=192.168.1.1
		 */
		char *defval, *env, *val;
		int i, state;

		/* NOTE/WARNING: To allow this code to do the above conversion using
		 * one buffer we shift the original SSI string to the end of the buffer
		 * so that we can read from it while re-populating the beginning
		 * of the buffer with the converted string...
		 */
		len = sfx-buf;
		bp = buf+sz-(len+1);
		memcpy(bp,buf+SIZEOF_PREFIX,len);
		bp[len-SIZEOF_PREFIX] = 0;

		bp += 5;
		i = state = STATE_BEGIN;
		while(isspace(*bp)) bp++;
        while(*bp != 0) {
			if (state == STATE_BEGIN) {
				if (*bp == _DOLLAR_) {
					state = STATE_GOTDOLLAR;
				}
				else {
					buf[i++] = *bp;	
				}
				bp++;
			}
			else if (state == STATE_GOTDOLLAR) {
				if (*bp == _OBRACE_) {
					state = STATE_GOTOBRACE;
					env = bp+1;
				}
				else {
					buf[i++] = _DOLLAR_;
					buf[i++] = *bp;
					state = STATE_BEGIN;
				}
				bp++;
			}
			else if (state == STATE_GOTCOLON) {
				if (*bp == _CBRACE_) {
					*bp = 0;
					val = mon_getenv(env);
					if (val) {
						while(*val)
							buf[i++] = *val++;
					}
					else {
						while(*defval)
							buf[i++] = *defval++;
					}
					state = STATE_BEGIN;
				}
				bp++;
			}
			else if (state == STATE_GOTOBRACE) {
				if (*bp == _COLON_) {
					*bp = 0;
					defval = bp+1;
					state = STATE_GOTCOLON;
				}
				else if (*bp == _CBRACE_) {
					*bp = 0;
					val = mon_getenv(env);
					if (val) {
						while(*val)
							buf[i++] = *val++;
					}
					state = STATE_BEGIN;
				}
				bp++;
			}
		}
		len = i;
	}
#if 0
    else if (strncasecmp(bp,"CUSTOM_COMMAND_HERE:",NNN) == 0) {
	}
#endif
	else {
		len = 0;
	}
	return(len);
}

void *
fs_fileexists(char *name, char *fullname, int sz)
{
	TFILE *tfp;

	/* Look for the specified file in TFS.  If found, then
	 * return it; else return the nofile string above.
	 */
	if ((strlen(name) + sizeof(HTTP_FILENAME_PREFIX) + 1) >= sz) 
		return(0);

	mon_sprintf(fullname,"%s%s",HTTP_FILENAME_PREFIX,name);
	tfp = mon_tfsstat(fullname);
	return(tfp);
}

/* fs_open():
 * Open specified file and return file descriptor (fd >= 0) if successful;
 * else -1.  The incoming name is combined with HTTP_FILENAME_PREFIX
 */
int
fs_open(char *name, struct http_state *hs)
{
	int tfd;
	TFILE *tfp;
	char fullname[TFSNAMESIZE+1];

	HTTPDBG(("fs_open(hs=0x%x) %s\n",(long)hs,name));

	tfp = (TFILE *)fs_fileexists(name,fullname,sizeof(fullname));
	if (tfp == (TFILE *)0) {
		HTTPDBG(("fs_open fp fail\n"));
        hs->left = strlen(nofile);
        hs->file = nofile;
		return(-1);
	}

	/* If the file ends with .shtml, then we parse it for SSI... */
	if (strstr(fullname,".shtml")) {

		tfd = mon_tfsopen(fullname,TFS_RDONLY,0);
		if (tfd >= 0) {
			hs->fsf = (struct fs_file *)mon_malloc(sizeof(struct fs_file));
			HTTPDBG(("malloc@%d(%d) = 0x%x\n",__LINE__,sizeof(struct fs_file),(long)hs->fsf));
		}
		if ((tfd < 0) || (hs->fsf == 0)) {
			HTTPDBG(("fs_open fail %d 0x%x\n",tfd,hs->fsf));
			hs->left = strlen(nofile);
			hs->file = nofile;
			return(-1);
		}

		gotoSsiState(hs->fsf,SSISTATE_INPROGRESS);
		hs->fsf->fd = tfd;
		hs->fsf->chunksize = FSCHUNKSZ;
		hs->left = fs_nxtchunk(hs);
		hs->file = hs->fsf->chunk;
	}
	else {
		hs->fsf = 0;
		hs->left = TFS_SIZE(tfp);
		hs->file = TFS_BASE(tfp);
	}
	return(0);
}

static int
_fs_nxtchunk(struct http_state *hs)
{
	int tot, pidx, sidx;
	struct fs_file *fsfp = hs->fsf;

	if (hs->fsf == 0) {
		HTTPDBG(("fs_nxtchunk: SSI_OFF\n"));
		return(-1);
	}

	fsfp = hs->fsf;
	tot = mon_tfsread(fsfp->fd,fsfp->chunk,fsfp->chunksize);
	if (tot <= 0) {
#if HTTP_DEBUG
		if (tot != TFSERR_EOF)
			HTTPDBG(("fs_nxtchunk: read fail fd=%d\n",fsfp->fd));
#endif
		return(tot);
	}

top:
	if (fsfp->ssistate == SSISTATE_INPROGRESS) {
		pidx = ssiprefix(fsfp->chunk,tot);
		// If no portion of the prefix was found, return the
		// entire buffer...
		if (pidx == -1)
			return(tot);

		// If we found the entire prefix, change state...
		if (pidx < (tot-SIZEOF_PREFIX))
			gotoSsiState(fsfp,SSISTATE_GOTPREFIX);

		if (pidx == 0) {
			goto top;
		}

		// Otherwise, we need to adjust the position in the file
		// so that the next call to tfsread will begin at the
		// point where we detected the prefix...
        mon_tfsseek(fsfp->fd,-(tot-pidx),TFS_CURRENT);

		return(pidx);
	}
	else if (fsfp->ssistate == SSISTATE_GOTPREFIX) {
		// We can assume here that the first character read from the
		// file will be the first character of the prefix.
		pidx = ssiprefix(fsfp->chunk,tot);
		if (pidx != 0) {
			HTTPDBG(("SSI_PRE parsing error\n"));
			return(-1);
		}
		sidx = ssisuffix(fsfp->chunk,tot);
		if (sidx == -1) {
			HTTPDBG(("SSI_SFX parsing error\n"));
			return(-1);
		}

		// At this point we can assume that the entire SSI block is 
		// in the chunk we are working on, so now we pass the chunk
        mon_tfsseek(fsfp->fd,-(tot-sidx-SIZEOF_SUFFIX-1),TFS_CURRENT);

		tot = ssievaluate(fsfp->chunk, fsfp->chunksize);

		// Go back to in-progress...
		gotoSsiState(fsfp,SSISTATE_INPROGRESS);

		if (tot == 0) {
			goto top;
		}

		return(tot);
	}
	else {
		HTTPDBG(("fs_nxtchunk: SSI ILLEGAL\n"));
	}

	return(tot);
}

int
fs_nxtchunk(struct http_state *hs)
{
	int tot = _fs_nxtchunk(hs);

	HTTPDBG(("fs_nxtchunk(hs=%x): %d\n",(long)hs,tot));
	return(tot);
}
