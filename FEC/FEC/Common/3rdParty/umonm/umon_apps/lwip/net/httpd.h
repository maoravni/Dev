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
 * Original author: Adam Dunkels <adam@sics.se>
 * This file has been modified extensively (from the original)
 * With contribution from Ed Sutter <ed.sutter@alcatel-lucent.com>
 *
 */
#ifndef __HTTPD_H__
#define __HTTPD_H__

#define HTTP_DEBUG		0
#define WEBSOC_DEBUG	0

#if HTTP_DEBUG
#define HTTPDBG(a) mon_printf a
#else
#define HTTPDBG(a) 
#endif

#if WEBSOC_DEBUG
#define WEBSOCDBG(a) mon_printf a
#else
#define WEBSOCDBG(a) 
#endif


#define SSI_PREFIX "<!--#"
#define SSI_SUFFIX "-->"

// For this case, sizes should not include the trailing null...
#define SIZEOF_PREFIX (sizeof(SSI_PREFIX)-1)
#define SIZEOF_SUFFIX (sizeof(SSI_SUFFIX)-1)

#define SSISTATE_OFF			0	// SSI parsing disabled.
#define SSISTATE_INPROGRESS		1	// SSI parsing in progress.
#define SSISTATE_GOTPREFIX		2	// SSI parsing found complete prefix.

#define FSCHUNKSZ	512

struct fs_file {
  char chunk[FSCHUNKSZ];
  int fd;
  int ssistate;
  int chunksize;
};

struct http_state {
  struct fs_file *fsf;
  char *file;
  int  left;
  char retries;
};

extern int http_verbose;
extern char *cmdrespbot;

extern void httpd_init(void);
extern char *http_getprefix(void);

extern void httpget_close(void);
extern int httpget_isactive(void);
extern int httpget_init(char *srvr, char *file, char *addr);

extern void websocd_init(void);
extern void websocd_poll(void);
extern int websocd_putchar(char c);

extern int fs_open(char *name, struct http_state *hs);
extern int fs_nxtchunk(struct http_state *hs);
extern void *fs_fileexists(char *name, char *fullname, int sz);


#define HTTP_FILENAME_PREFIX "web/"

#endif /* __HTTPD_H__ */
