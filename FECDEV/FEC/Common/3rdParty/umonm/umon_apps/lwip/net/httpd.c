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
 * Modified significantly to run as a uMon application:
 *	Ed Sutter <esutter@alcatel-lucent.com>
 */

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "httpd.h"
#include "monlib.h"

#define SUPPORT_UMONCMD_ACTION 0

char *http_respptr;
int  http_respsiz, http_respidx;

#ifndef CMDRESPBOT
char *cmdrespbot = "</pre>\n</body>\n</html>";
#endif

#if SUPPORT_UMONCMD_ACTION
static int
http_putchar(int c)
{
	char *mp;

	/* If http_respptr is -1, then we've aborted the response (probably
	 * because of a memory allocation failure) so just return.
	 */
	if (http_respptr == (char *)0xffffffff)
		return(0);

	/* If http_respptr is 0, then it is the start of a new response.
	 */
	if (http_respptr == 0) {
		int rc;
		char *name;
		TFILE finfo;
		static char namebuf[TFSNAMESIZE+1];

		name = "crtop.html";
		if ((strlen(name) + sizeof(HTTP_FILENAME_PREFIX) + 1) < sizeof(namebuf)) {
			mon_sprintf(namebuf,"%s%s",HTTP_FILENAME_PREFIX,name);
			name = namebuf;
		}
		rc = mon_tfsfstat(name,&finfo);
		if (rc < 0 ) 
			http_respsiz = 1024;
		else
			http_respsiz = finfo.filsize + 1024;

		http_respptr = mon_malloc(http_respsiz);
		HTTPDBG(("malloc@%d(%d) = 0x%x\n",__LINE__,http_respsiz,(long)http_respptr));
		if (http_respptr == 0) {
			http_respptr = (char *)0xffffffff;
			return(0);
		}
		if (rc >= 0) {
			int fd;
			fd = mon_tfsopen(name,TFS_RDONLY,0);
			if (fd < 0) {
				HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)http_respptr));
				mon_free(http_respptr);			
				http_respptr = (char *)0xffffffff;
				return(0);
			}
			else {
				mon_tfsread(fd,http_respptr,finfo.filsize);
				mon_tfsclose(fd,0);
				http_respidx = finfo.filsize;
			}
		}
	}
	else if (http_respidx >= http_respsiz - 32) {
		http_respsiz += 1024;
		mp = mon_realloc(http_respptr,http_respsiz);
		if (mp == 0) {
			HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)http_respptr));
			mon_free(http_respptr);
			http_respptr = (char *)0xffffffff;
			return(0);
		}
		http_respptr = mp;
	}
	http_respptr[http_respidx++] = (char)c;

	return(1);
}
#endif

/* httpapp_procnameval():
 *  This function is called by the server as it is processing any
 *  list of name/value pairs in a form reponse.  This is where the application
 *  puts all of the "actions" that must occur as a result of a client
 *  interacting with a form.  The name and value are incoming ascii and
 *  it is 100% application specific code that deals with these name/value
 *  pairs.
 */
static int
httpapp_procnameval(char *name, char *val)
{
	//mon_printf("procnameval: name= %s, val= %s\n",name,val);
#if SUPPORT_UMONCMD_ACTION
	if (strcmp(name,"umoncmd") == 0) {
		http_respptr = 0;
		mon_com(CHARFUNC_PUTCHAR,http_putchar,0,0);
		mon_docommand(val,0);
		if (http_respptr == 0)	// Just in case the message is empty
			http_putchar('\n');	// generate one character of response.
		mon_com(CHARFUNC_PUTCHAR,0,0,0);
	}
#endif
    return(0);
}

/* http_process_list():
 *	The input string from the html form has a few ugly details to deal with.
 *	The format of the string is:
 *		"name1=value1&name2=value2"
 *
 *		where "namex" is the name assigned in the form for a 
 *		particular piece of input.
 *		and "valuex" is what was entered by the user of the form.
 *
 *	The ampersand (&) is the delimiter between variable assignments.
 *	Any character can be written as a percent sign followed by a 2-digit
 *	hex value that represents the actual character.
 *	If a '+' sign is on the line, then it should be converted to a space.
 */
static int
http_process_list(char *nvlist, int length)
{
	char	*ipin, *ipout, *eq, *amp, *cp, *namevallist, hval[3];

	http_respptr = (char *)0;

    /* We can't assume that the incoming list is NULL terminated, so
     * to allow us to parse as a standard 'C' string, copy the data
     * to a buffer and null terminate it...
     */
    namevallist = mon_malloc(length + 1);
	HTTPDBG(("malloc@%d(%d) = 0x%x\n",__LINE__,length+1,(long)namevallist));
	if (namevallist == (char *)0) {
      mon_printf("malloc failed\n");
      return(-1);
    }

    memcpy(namevallist,nvlist,length);
    namevallist[length] = 0;

	/* The incoming list may have other HTTPish stuff prepended to it, so
	 * strip it off by searching for the first '=' sign, then backing up to
	 * either the beginning, or the first instance of a CR or LF...
	 */
	cp = strchr(namevallist,'=');
	if (cp) {
		while ((*cp != '\n') && (*cp != '\r') && (cp != namevallist))
			cp--;
		if ((*cp == '\r') || (*cp == '\n'))
			namevallist = cp+1;
		else
			namevallist = cp;
	}

	/* Terminate the incoming name list when a CR or LF is found... */
	cp = strchr(namevallist,'=');
	if (cp) {
		while(*cp) {
			if ((*cp == '\r') || (*cp == '\n')) {
				*cp = 0;
				break;
			}
			cp++;
		}
	}

	ipin = ipout = namevallist;
	
	/* First convert + and %xx... */
	while(*ipin) {
		if (*ipin == '+')
			*ipout = ' ';
		else if (*ipin == '%') {
			hval[0] = ipin[1];
			hval[1] = ipin[2];
			hval[2] = 0;
			ipin += 2;
			*ipout = (unsigned char)strtol(hval,(char **)0,16);
		}
		else
			*ipout = *ipin;
		ipin++;
		ipout++;
	}
	*ipout = 0;

	/* Now parse for the name=var& strings... */
	ipin = namevallist;
	eq = strchr(namevallist,'=');
	if (!eq) {
		HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)namevallist));
        mon_free(namevallist);
		return(-1);
    }
	amp = strchr(namevallist,'&');
	while(eq) {
		*eq = 0;
		if (amp)
			*amp = 0;
		httpapp_procnameval(ipin,eq+1);
		if (!amp)
			break;
		ipin = amp+1;
		eq = strchr(ipin,'=');
		amp = strchr(ipin,'&');
	}
	HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)namevallist));
    mon_free(namevallist);
	return(0);
}

static void
http_connerr(void *arg, err_t err)
{
	LWIP_UNUSED_ARG(err);

	HTTPDBG(("http_connerr hs=0x%x\n",(long)arg));
	if (arg) {
		struct http_state *hs = (struct http_state *)arg;

		if (hs->fsf) {
			if ((hs->fsf->fd >= 0))
				mon_tfsclose(hs->fsf->fd,0);

			HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)hs->fsf));
			mon_free((void *)hs->fsf);
		}

		HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)hs));
		mon_free((void *)hs);
	}
}

static void
http_closeconn(struct tcp_pcb *pcb, struct http_state *hs)
{
	HTTPDBG(("http_closeconn hs=0x%x\n",(long)hs));
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);

	if (hs->fsf) {
		if ((hs->fsf->fd >= 0))
			mon_tfsclose(hs->fsf->fd,0);

		HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)hs->fsf));
		mon_free((void *)hs->fsf);
	}

	HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)hs));
	mon_free((void *)hs);
	tcp_close(pcb);
}

static void
http_send_data(struct tcp_pcb *pcb, struct http_state *hs, int fromline)
{
	err_t err;
	u16_t len;

	/* We cannot send more data than space available in the send buffer. */     
	len = tcp_sndbuf(pcb);
	if (len >= hs->left)
		len = hs->left;

	do {
		err = tcp_write(pcb, hs->file, len, 0);
		if (err == ERR_MEM) {
			len /= 2;
		}
	} while (err == ERR_MEM && len > 1);  
  
	if (err == ERR_OK) {
		HTTPDBG(("http_send_data(hs=0x%x) @%d len: %d\n",
			(long)hs,fromline,len));
		hs->file += len;
		hs->left -= len;
	}
	else {
		mon_printf("http_send_data(hs=0x%x) @%d err: %d\n",
			(long)hs,fromline,err);
	}
}

static err_t
http_poll(void *arg, struct tcp_pcb *pcb)
{
	struct http_state *hs = arg;

	if (hs == NULL) {
		HTTPDBG(("http_poll NULL abort\n"));
		tcp_abort(pcb);
		return ERR_ABRT;
	} else {
		hs->retries++;
		if (hs->retries == 4) {
			HTTPDBG(("http_poll RETRY abort\n"));
			tcp_abort(pcb);
			return ERR_ABRT;
		}
		http_send_data(pcb, hs,__LINE__);
	}
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/

static err_t
http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(len);
	struct http_state *hs = arg;

	hs->retries = 0;
  
	if (hs->left > 0) {    
		http_send_data(pcb, hs,__LINE__);
	}
	else {
		hs->left = fs_nxtchunk(hs);
		hs->file = hs->fsf->chunk;

		if (hs->left < 0) {
			http_closeconn(pcb, hs);
			if (http_respptr) {
				HTTPDBG(("free@%d(0x%x)\n",__LINE__,(long)http_respptr));
				mon_free(http_respptr);
				http_respptr = 0;
				http_respidx = 0;
			}
		}
	}
	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
static err_t
http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	int i;
	char *data, *fname;
	struct http_state *hs;
	char fullname[TFSNAMESIZE+1];

	hs = arg;

	if (err == ERR_OK && p != NULL) {
		/* Inform TCP that we have taken the data. */
		tcp_recved(pcb, p->tot_len);
    
		if (hs->file == NULL) {
			data = p->payload;
      
#if HTTP_DEBUG
			mon_printf("HTTPD_RCV:\n");
			for(i=0;i<p->tot_len;i++)
				mon_putchar(data[i]);
			mon_putchar('\n');
#endif

			/* test for "GET /FILENAME... "*/
			if (strncmp(data, "GET ", 4) == 0) {
				int tot = p->tot_len-4;
				char *cp = data+4;

				/* Look for the end of the filename in the GET request: */
				for(i = 0; i < tot; i++,cp++) {
					if ((*cp == '?') || (*cp == ' ') || (*cp == '\r') || (*cp == '\n')) {
						*cp = 0;
						break;
					}
				}

				if ((data[4] == '/') && (data[5] == 0)) {
					fname = "index.shtml";
					if (!fs_fileexists(fname,fullname,sizeof(fullname)))
						fname = "index.html";
				}
				else {
					fname = data+5;
				}
	
				fs_open(fname,hs);
	
				pbuf_free(p);
				http_send_data(pcb, hs, __LINE__);
				tcp_sent(pcb, http_sent);
	
			}
			else if (strncmp(data,"POST ",5) == 0) {
				char *cp = data+p->tot_len;
				char gotequal = 0;
	
				for(cp = data+p->tot_len;cp>data;cp--) {
					if (*cp == '=') {
						gotequal = 1;
						continue;
					}
					if (gotequal && ((*cp == 0x0d) || (*cp == 0x0a))) {
						break;
					}
				}
				if (cp == data) {
					pbuf_free(p);
					http_closeconn(pcb, hs);
				}
				else {
					cp++;
					http_process_list(cp,p->tot_len - (cp-data));
					if ((http_respptr != 0) && (http_respptr != (char *)0xffffffff)) {
						strcpy(http_respptr+http_respidx,cmdrespbot);
						http_respidx += strlen(cmdrespbot);
						hs->file = http_respptr;
						hs->left = http_respidx;
						pbuf_free(p);
						http_send_data(pcb, hs,__LINE__);
						tcp_sent(pcb, http_sent);
					}
				}
				//pbuf_free(p);
			}
			else {
				mon_printf("HTTP NOT POST OR GET...\n");
				mon_printmem(data,p->tot_len,1);
				pbuf_free(p);
				http_closeconn(pcb, hs);
			}
		}
		else {
			pbuf_free(p);
		}
	}

	if (err == ERR_OK && p == NULL) {
		http_closeconn(pcb, hs);
	}
	return ERR_OK;
}

static err_t
http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	struct http_state *hs;
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	HTTPDBG(("http_accept\n"));
//	tcp_setprio(pcb, TCP_PRIO_MIN);
  
	/* Allocate memory for the structure that holds the state of the
	 * connection.
	 */
	hs = (struct http_state *)mon_malloc(sizeof(struct http_state));
	HTTPDBG(("malloc@%d(%d) = 0x%x\n",__LINE__,sizeof(struct http_state),(long)hs));

	if (hs == NULL) {
		mon_printf("http_accept: Out of memory1\n");
		return ERR_MEM;
	}
#if 0
	hs->fsf = (struct fs_file *)mon_malloc(sizeof(struct fs_file));
	HTTPDBG(("malloc@%d(%d) = 0x%x\n",__LINE__,sizeof(struct fs_file),(long)hs->fsf));

	if (hs->fsf == NULL) {
		mon_printf("http_accept: Out of memory2\n");
		return ERR_MEM;
	}
#else
	hs->fsf = 0;
#endif
  
	/* Initialize the structure. */
	hs->file = NULL;
	hs->left = 0;
	hs->retries = 0;
  
	/* Set up the callback functions used for this connection. */
	tcp_arg(pcb, hs);
	tcp_recv(pcb, http_recv);
	tcp_err(pcb,  http_connerr);
	tcp_poll(pcb, http_poll, 1);

	return ERR_OK;
}

void
httpd_init(void)
{
	struct tcp_pcb *pcb;

	HTTPDBG(("http_init\n"));
	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 80);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb, http_accept);
}
/*-----------------------------------------------------------------------------------*/

