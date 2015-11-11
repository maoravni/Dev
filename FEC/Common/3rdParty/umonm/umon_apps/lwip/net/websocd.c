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
 * Modified significantly to run in a uMon application for interfacing
 * with HTML5's WebSockets API.  For some good intro on this check out
 * these sites:
 *
 *	http://www.developerfusion.com/article/143158/an-introduction-to-websockets/
 *	http://www.tutorialspoint.com/html5/html5_websocket.htm
 *	http://tools.ietf.org/html/rfc6455
 *	http://www.html5rocks.com/en/tutorials/websockets/basics/
 *
 *	Ed Sutter <ed.sutter@alcatel-lucent.com>
 */

#include <ctype.h>
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "base64.h"
#include "crypto.h"
#include "monlib.h"
#include "timer.h"

extern void console_prompt(void);

#define FINAL_FRAGMENT(a)		((a[0] & 0x80) == 0x80 ? 1 : 0)
#define GET_OPCODE(a)			(a[0] & 0xf)
#define PAYLOAD_MASKED(a)		((a[1] & 0x80) == 0x80 ? 1 : 0)
#define OPCODE_CONTINUE			0
#define OPCODE_PAYLOAD_TEXT		1
#define OPCODE_PAYLOAD_BINARY	2
#define OPCODE_CLOSE_CONN		8
#define OPCODE_PING				9
#define OPCODE_PONG				10

#define WSAPI_MAGIC		"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

#define WSAPI_KEYPREFIX	"Sec-WebSocket-Key:"

#define WSAPI_ACCEPTPREFIX  \
  "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: "

#define WSAPI_ACCEPTSUFFIX  \
  "\r\n\r\n"

int		wspidx = 0;
char	wspbuf[1024];
struct	tcp_pcb *wspcb;
struct	http_state *wshs;
struct	elapsed_tmr wstmr;
int		timedout, msg_sent;

/* ws_tmrstart():
 * Init (or re-init) the inactivity timer...
 */
void
ws_tmrstart(void)
{
	wstmr.start = 1000*60*5;	// 5 minutes
	timedout = 0;
	mon_timer(TIMER_START,&wstmr);
}

/* ws_tmrtimeout():
 * Return 1 if inactivity timeout has occured;
 * else 0. 
 */
int
ws_tmrtimeout(void)
{
	if ((timedout == 0) && mon_timer(TIMER_ELAPSED,&wstmr)) {
		timedout = 1;
		return(1);
	}
	return(0);
}

void
websoc_status(char *msg)
{
	mon_printf("Websoc srvr: <%s>\n",msg);
}

void
freehs(struct http_state *hs)
{
	mem_free(hs);
	if (hs == wshs)
		wshs = 0;
	else
		mon_printf("WEBSOCERR: wshs != hs\n");
}

static void
ws_connerr(void *arg, err_t err)
{
	LWIP_UNUSED_ARG(err);
	struct http_state *hs = arg;

	WEBSOCDBG(("ws_connerr hs=0x%x err=%d\n",(long)hs,err));
	freehs(hs);
}

static void
ws_closeconn(struct tcp_pcb *pcb, struct http_state *hs, int fromline)
{
	WEBSOCDBG(("ws_closeconn@%d hs=0x%x\n",fromline,(long)hs));
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	freehs(hs);
	tcp_close(pcb);
	wspcb = 0;
}

static void
ws_send_data(struct tcp_pcb *pcb, struct http_state *hs, int fromline)
{
	err_t err;
	u16_t len;

	/* We cannot send more data than space available in the send
     * buffer.
	 */     
	if (tcp_sndbuf(pcb) < hs->left) {
		len = tcp_sndbuf(pcb);
	}
	else {
		len = hs->left;
	}

	do {
		err = tcp_write(pcb, hs->file, len, 0);
		if (err == ERR_MEM) {
			len /= 2;
		}
	} while (err == ERR_MEM && len > 1);  
  
	if (err == ERR_OK) {
		WEBSOCDBG(("ws_send_data(hs=0x%x) @%d len: %d\n",
			(long)hs,fromline,len));
		hs->file += len;
		hs->left -= len;
	} else {
		mon_printf("ws_send_data: error %d len %d %d\n",
			err, len, tcp_sndbuf(pcb));
	}
}

static err_t
websoc_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(len);
	struct http_state *hs = arg;

	hs->retries = 0;
  
	if (hs->left > 0) {    
		ws_send_data(pcb, hs,__LINE__);
	} else {
		hs->file = NULL;
		hs->left = 0;
		hs->retries = 0;
	}
	return ERR_OK;
}

/* websoc_get_key_from_header():
 * Referring to 1.3 of RFC 6455 "Opening Handshake", this
 * function parses the incoming header looking for
 * the websocket key. 
 */
static err_t
websoc_get_key_from_header(char *header, char *key, int max)
{
	char *wskey;
	char *keyprefix = WSAPI_KEYPREFIX;

	wskey = strstr(header,keyprefix);
	if (wskey == (char *)0)
		return(ERR_VAL);

	wskey += strlen(keyprefix);
	while(isspace(*wskey)) wskey++;

	while((*wskey != '\r') && (*wskey != '\n'))
		*key++ = *wskey++;

	return(ERR_OK);
}


int
websoc_build_accept_from_key(char *key, char *b64out, int max)
{
	int b64len;
	SHA1_CTX sha1;
	char *magic = WSAPI_MAGIC;
	base64_encodestate b64state;
	unsigned char buf[128], digest[20];

	if ((strlen(magic) + strlen(key)) > sizeof(buf))
      return(-1);

	strcpy(buf,key);
	strcat(buf,magic);
	memset((void *)digest,0,(size_t)sizeof(digest));
	SHA1_Init(&sha1);
	SHA1_Update(&sha1,buf,strlen(buf));
	SHA1_Final(digest,&sha1);

#if WEBSOC_DEBUG
    { int i;
	mon_printf("SHA1 hash: ");
	for(i=0;i<20;i++)
		mon_printf("%02x",digest[i]);
	mon_printf("\n");
    }
#endif

	base64_init_encodestate(&b64state);
	b64len = base64_encode_block((const char *)digest, 20, b64out, &b64state);
	b64len += base64_encode_blockend(b64out+b64len, &b64state);

#if WEBSOC_DEBUG
    { int i;
	mon_printf("B64 out:   ");
	for(i=0;i<b64len;i++)
		mon_printf("%c",b64out[i]);
	mon_printf("\n");
    }
#endif
    return(b64len);
}

int
websoc_get_payload_length(unsigned char *data)
{
	int len;

	len = data[1] & 0x7f;
	if (len <= 125)
		return(len);

	if (len == 126) {
		len = (((int)(data[2]) << 8) | data[3]);
		return(len);
	}
	else {
		/* Theoretically this is an 8-byte field, but we only
		 * process the lower 4 bytes...
		 */
		len = (((int)(data[6]) << 24) | ((int)(data[7]) << 16) |
			((int)(data[8]) << 8) | ((int)(data[9])));
		return(len);
	}
	return(-1);
}

/* websoc_extract_text_payload():
 * This function takes an incoming text payload frame and parses
 * it based on the rules outlined in section 5.2 of RFC6455.
 * The 'payload' parameter is the incoming frame and 'msg'
 * is the buffer (size=msgmax) into which the unmasked text
 * is to be placed.
 * The return value is either negaive (error) or the length
 * of the unmasked text.
 */
int
websoc_extract_text_payload(unsigned char *payload, char *msg, int msgmax)
{
	int i, plen, len;
	unsigned char *payload_data;

	plen = websoc_get_payload_length(payload);
	if (plen > msgmax)
		len = msgmax;
	else
		len = plen;

	if (plen <= 125)
		payload_data = payload+2;
	else if (plen == 126)
		payload_data = payload+4;
	else
		payload_data = payload+10;

	if (PAYLOAD_MASKED(payload)) {
		unsigned char *mask = payload_data; 
		payload_data += 4;

		mon_printf("TEXT rcvd (%d bytes):\n",len);
		for(i=0;i<len;i++,msg++) {
			*msg = (char)(payload_data[i] ^ mask[i%4]);
			mon_putchar(*msg);
		}
		mon_putchar('\n');
	}
	else {
		mon_printf("YIKES_X!!!!\n");
		return(-1);
	}
	return(len);
}

/* websoc_create_text_frame():
 * This function takes the incoming text and converts it to a websocket
 * frame based on section 5.2 of RFC6455.  To avoid using a second buffer
 * to copy this into, it assumes the incoming buffer is writeable and is
 * at least 4 bytes larger than the null-terminated text.
 */
int
websoc_create_text_frame(char *text, int frmmax)
{
	int flen, plen;
	int toidx, frmidx;

	plen = strlen(text);
	if ((plen + 4) >= frmmax)
		plen = frmmax - 4;

	if (plen <= 125) {
		flen = plen+2;
		toidx = plen+1;
		frmidx = plen-1;
		while(frmidx >= 0)
			text[toidx--] = text[frmidx--];
		text[0] = 0x80 | OPCODE_PAYLOAD_TEXT; 
		text[1] = plen;
	}
	else {
		flen = plen+4;
		toidx = plen+3;
		frmidx = plen-1;
		while(frmidx >= 0)
			text[toidx--] = text[frmidx--];
		text[0] = 0x80 | OPCODE_PAYLOAD_TEXT; 
		text[1] = 126;
		text[2] = (plen & 0xff00) >> 8;
		text[3] = (plen & 0xff);
	}
	return(flen);
}

static err_t
websoc_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	int b64len, tlen;
	unsigned char *data;
	static char accept[512];
	char *b64out, wskey[64];
	char *accept_prefix = WSAPI_ACCEPTPREFIX;
	char *accept_suffix = WSAPI_ACCEPTSUFFIX;
	struct http_state *hs = arg;

	if (err == ERR_OK && p != NULL) {
		/* Inform TCP that we have taken the data. */
		tcp_recved(pcb, p->tot_len);
    
		data = p->payload;
      
		if (strncmp(data, "GET ", 4) == 0) {
#if WEBSOC_DEBUG
			{ int i;
			mon_printf("WEBSOC_RCV:\n");
			for(i=0;i<p->tot_len;i++)
				mon_putchar(data[i]);
			}
#endif
			memset((void *)wskey,0,sizeof(wskey));
			if (websoc_get_key_from_header((char *)data,wskey,sizeof(wskey)) != ERR_OK) {
				mon_printf("YIKES_1!!!!\n");
				pbuf_free(p);
				ws_closeconn(pcb, hs, __LINE__);
				return(ERR_OK);
			}

			strcpy(accept,accept_prefix);
			b64out = accept + strlen(accept_prefix);
			b64len = websoc_build_accept_from_key(wskey,b64out,
				sizeof(accept) - sizeof(WSAPI_ACCEPTPREFIX));

			if (b64len < 0) {
				mon_printf("YIKES_2!!!!\n");
				pbuf_free(p);
				ws_closeconn(pcb, hs, __LINE__);
				return(ERR_OK);
			}

			strcat(accept,accept_suffix);
			WEBSOCDBG(("WEBSOC_RESP:\n%s\n",accept));
			hs->file = accept;
			hs->left = strlen(accept);

			pbuf_free(p);
			ws_send_data(pcb, hs,__LINE__);

			/* Tell TCP that we wish be to informed of data that has been
			 * successfully sent by a call to the websoc_sent() function.
			 */
			tcp_sent(pcb, websoc_sent);
			return(ERR_OK);
		}
		else {
			/* Parse the incoming data based on RFC6455 section 5.2
			 * "Base Framing Protocol"...
			 */
#if WEBSOC_DEBUG
			mon_printf("WEBSOCD DATA...\n");
			mon_printmem((char *)data,p->tot_len,1);
#endif
			if (!FINAL_FRAGMENT(data)) {
				/* not ready for fragmentation at the moment... */
				mon_printf("YIKES_3!!!!\n");
				pbuf_free(p);
				ws_closeconn(pcb, hs, __LINE__);
				return(ERR_OK);
			}

			switch(GET_OPCODE(data)) {
				case OPCODE_PAYLOAD_TEXT:
					tlen = websoc_extract_text_payload(data,accept,sizeof(accept));
					accept[tlen] = 0;
					pbuf_free(p);
					if (strncmp(accept,"WSCMD: ",7) == 0) {
						ws_tmrstart();
						WEBSOCDBG(("doing command: %s\n",accept+7));
						mon_com(CHARFUNC_PUTCHAR,(void *)websocd_putchar,0,0);
						mon_docommand(accept+7,1);
						console_prompt();
						mon_com(CHARFUNC_PUTCHAR,0,0,0);
					}
					else {
						strcpy(accept,"server-to-client response");
						hs->left = websoc_create_text_frame(accept, sizeof(accept));
						hs->file = accept;
						ws_send_data(pcb, hs,__LINE__);
						tcp_sent(pcb, websoc_sent);
					}
					return(ERR_OK);
				case OPCODE_CLOSE_CONN:
					websoc_status("client requested close");
					pbuf_free(p);
					ws_closeconn(pcb, hs, __LINE__);
					return(ERR_OK);
				case OPCODE_CONTINUE:
				case OPCODE_PAYLOAD_BINARY:
				case OPCODE_PING:
				case OPCODE_PONG:
				default:
					mon_printf("Websock Opcode: %d\n",GET_OPCODE(data));
					pbuf_free(p);
					ws_closeconn(pcb, hs, __LINE__);
					return(ERR_OK);
			}
		}
	}

	if (err == ERR_OK && p == NULL) {
		// We get here if the other end shuts down
		websoc_status("close detected");
		ws_closeconn(pcb, hs, __LINE__);
	}
	return ERR_OK;
}

static err_t
websoc_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	if (wshs != 0) {
		mon_printf("websoc_accept: WSHS in use\n");
		return ERR_USE;
	}
  
	/* Allocate memory for the structure that holds the state of the
	 * connection.
	 */
	wshs = (struct http_state *)mem_malloc(sizeof(struct http_state));

	if (wshs == NULL) {
		mon_printf("websoc_accept: Out of memory\n");
		return ERR_MEM;
	}
  
	/* Initialize the structure. */
	wshs->file = NULL;
	wshs->left = 0;
	wshs->retries = 0;
  
	/* Tell TCP that this is the structure we wish to be passed for our
	 * callbacks.
	 */
	tcp_arg(pcb, wshs);

	/* Tell TCP that we wish to be informed of incoming data by a call
	 * to the websoc_recv() function.
	 */
	tcp_recv(pcb, websoc_recv);
	tcp_err(pcb, ws_connerr);

	wspcb = pcb;
	ws_tmrstart();
	websoc_status("connection established");
	return ERR_OK;
}

void
websocd_init(void)
{
	struct tcp_pcb *pcb;

	wshs = 0;
	msg_sent = 1;
	wspcb = (struct tcp_pcb *)0;
	pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 50080);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb, websoc_accept);
}

static err_t
websoc_async_msg_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(len);
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(pcb);

	msg_sent = 1;
	return ERR_OK;
}

static err_t
websoc_close_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(len);
	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(pcb);

	ws_closeconn(wspcb, wshs, __LINE__);
	return ERR_OK;
}

int
websocd_putchar(char c)
{
	if (wspidx < sizeof(wspbuf)-2) {
		wspbuf[wspidx++] = c;
		if (wspidx >= sizeof(wspbuf))
			wspidx = 0;
	}
	return(REMOTE_PUTCHAR_INSERTT);
}

void
websocd_poll(void)
{
	if ((wspcb) && ws_tmrtimeout()) {
		websoc_status("inactivity timeout");
		wspbuf[0] = 0x80 | OPCODE_CLOSE_CONN;
		wspbuf[1] = 0;
		wshs->file = wspbuf;
		wshs->left = 2;
		ws_send_data(wspcb, wshs,__LINE__);
		tcp_sent(wspcb, websoc_close_sent);
		return;
	}

	if ((msg_sent == 0) || (wspcb == 0))
		return;

	if (wspidx > 0) {
		wspbuf[wspidx] = 0;
		wshs->left = websoc_create_text_frame(wspbuf, sizeof(wspbuf));
		wshs->file = wspbuf;
		ws_send_data(wspcb, wshs,__LINE__);
		msg_sent = 0;
		tcp_sent(wspcb, websoc_async_msg_sent);
		wspidx = 0;
	}
}
