/* arpstuff.c:
 *	This code supports some basic arp/rarp stuff.
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
#if INCLUDE_ETHERNET
#include "endian.h"
#include "genlib.h"
#include "ether.h"
#include "stddefs.h"
#include "cli.h"
#include "timer.h"

static int	ArpStore(uchar *,uchar *);
static int	IpIsOnThisNet(uchar *);

static unsigned long probeIP;
static char probeAbort;

/* Constants used by Dynamic IPV4 Local-Link Address setup...
 * (see RFC 3927)
 */
#define PROBE_WAIT			1000		/* (msec) */
#define PROBE_NUM			3
#define PROBE_MIN			1000		/* (msec) */
#define PROBE_MAX			2000		/* (msec) */
#define ANNOUNCE_WAIT		2000		/* (msec) */
#define ANNOUNCE_NUM 		2
#define ANNOUNCE_INTERVAL 	2000		/* (msec) */
#define MAX_CONFLICTS		10
#define RATE_LIMIT_INTERVAL	60000		/* (msec) */
#define DEFEND_INTERVAL 	10000		/* (msec) */

/* arpcache[]:
 *	Used to store the most recent set of IP-to-MAC address correlations.
 */
struct arpcache {
	uchar	ip[4];
	uchar	ether[6];
} ArpCache[SIZEOFARPCACHE];

/* ArpIdx & ArpTot:
 *	Used for keeping track of current arp cache content.
 */
int ArpIdx, ArpTot;

/* ArpStore():
 *	Called with binary ip and ethernet addresses.
 *	It will store that set away in the cache.
 */
int
ArpStore(uchar *ip,uchar *ether)
{
	if (EtherFromCache(ip))
		return(0);
	if (ArpIdx >= SIZEOFARPCACHE)
		ArpIdx=0;
	memcpy((char *)ArpCache[ArpIdx].ip,(char *)ip,4);
	memcpy((char *)ArpCache[ArpIdx].ether,(char *)ether,6);
	ArpIdx++;
	ArpTot++;
	return(0);
}

/* EtherFromCache():
 *	Called with a binary (4-byte) ip address.  If a match is found
 *	in the cache, return a pointer to that ethernet address; else
 *	return NULL.
 */
uchar *
EtherFromCache(uchar *ip)
{
	int	i;

	for(i=0;i<SIZEOFARPCACHE;i++) {
		if (!memcmp((char *)ArpCache[i].ip, (char *)ip,4))
			return(ArpCache[i].ether);
	}
	return(0);
}

/* SendArpResp():
 *	Called in response to an ARP REQUEST.  The incoming ethernet
 *	header pointer points to the memory area that contains the incoming
 *	ethernet packet that this function is called in response to.
 *	In other words, it is used to fill the majority of the response
 *	packet fields.
 */
int
SendArpResp(struct ether_header *re)
{
	struct ether_header *te;
	struct arphdr *ta, *ra;

	te = (struct ether_header *) getXmitBuffer();
	memcpy((char *)&(te->ether_shost), (char *)BinEnetAddr,6);
	memcpy((char *)&(te->ether_dhost),(char *)&(re->ether_shost),6);
	te->ether_type = ecs(ETHERTYPE_ARP);

	ta = (struct arphdr *) (te + 1);
	ra = (struct arphdr *) (re + 1);
	ta->hardware = ra->hardware;
	ta->protocol = ra->protocol;
	ta->hlen = ra->hlen;
	ta->plen = ra->plen;
	ta->operation = ARP_RESPONSE;
	memcpy((char *)ta->senderha, (char *)BinEnetAddr,6);
	memcpy((char *)ta->senderia, (char *)ra->targetia,4);
	memcpy((char *)ta->targetha, (char *)ra->senderha,6);
	memcpy((char *)ta->targetia, (char *)ra->senderia,4);
	self_ecs(ta->hardware);
	self_ecs(ta->protocol);
	self_ecs(ta->operation);
	sendBuffer(ARPSIZE);
	return(0);
}

/* SendArpRequest():
 * If 'probe' is non-zero, then the request is an arp-probe.
 * Refer to RFC3927 for more on that.
 */
static int
SendArpRequest(uchar *ip, int probe)
{
	struct ether_header *te;
	struct arphdr *ta;

	/* Populate the ethernet header: */
	te = (struct ether_header *) getXmitBuffer();
	memcpy((char *)&(te->ether_shost), (char *)BinEnetAddr,6);
	memcpy((char *)&(te->ether_dhost), (char *)BroadcastAddr,6);
	te->ether_type = ecs(ETHERTYPE_ARP);

	/* Populate the arp header: */
	ta = (struct arphdr *) (te + 1);
	ta->hardware = ecs(1);		/* 1 for ethernet */
	ta->protocol = ecs(ETHERTYPE_IPV4);
	ta->hlen = 6;		/* Length of hdware (ethernet) address */
	ta->plen = 4;		/* Length of protocol (ip) address */
	ta->operation = ecs(ARP_REQUEST);
	memcpy((char *)ta->senderha, (char *)BinEnetAddr,6);
	if (probe) {
		memset((char *)ta->senderia,0,4);
		memset((char *)ta->targetha,0,6);
	}
	else {
		memcpy((char *)ta->senderia, (char *)BinIpAddr,4);
		memcpy((char *)ta->targetha, (char *)BroadcastAddr,6);
	}
	memcpy((char *)ta->targetia, (char *)ip,4);
	sendBuffer(ARPSIZE);
	return(0);
}

/* GetBinNetMask():
 *  Return a subnet mask in binary form.
 *	Since this function needs a netmask, if NETMASK is not set, then
 *	it uses the default based on the upper 3 bits of the IP address
 *	(refer to TCP/IP Illustrated Volume 1 Pg8 for details).
 */
void
GetBinNetMask(uchar *binnetmask)
{
	char	*nm;

	nm = getenv("NETMASK");
	if (!nm) {
		memset((char *)binnetmask,0xff,4);
		if ((BinIpAddr[0] & 0xe0) == 0xc0) {		/* Class C */
			binnetmask[3] = 0;
		}
		else if ((BinIpAddr[0] & 0xc0) == 0x80) {	/* Class B */
			binnetmask[3] = 0;
			binnetmask[2] = 0;
		}
		else if ((BinIpAddr[0] & 0x80) == 0x00) {	/* Class A */
			binnetmask[3] = 0;
			binnetmask[2] = 0;
			binnetmask[1] = 0;
		}
	}
	else
		IpToBin(nm,binnetmask);
}

/* IpIsOnThisNet():
 *	Return 1 if the incoming ip is on this subnet; else 0.
 *	For each bit in the netmask that is set to 1...
 *	If the corresponding bits in the incoming IP and this board's IP
 *	do not match, return 0; else return 1.
 */
int
IpIsOnThisNet(uchar *ip)
{
	int		i;
	uchar	binnetmask[8];

	GetBinNetMask(binnetmask);

	for(i=0;i<4;i++) {
		if ((ip[i] & binnetmask[i]) != (BinIpAddr[i] & binnetmask[i])) {
			return(0);
		}
	}
	return(1);
}

/* ArpEther():
 *	Retrieve the ethernet address that corresponds to the incoming IP 
 *	address.  First check the local ArpCache[], then if not found issue
 *	an ARP request... If the incoming IP is on this net, then issue the
 *	request for the MAC address of that IP; otherwise, issue the request
 *	for this net's GATEWAY.
 */
uchar *
ArpEther(uchar *binip, uchar *ecpy, int proxyarp)
{
	char	*gip;
	struct	elapsed_tmr tmr;
	uchar	gbinip[8], *Ip, *ep;
	int		timeoutsecs, retry;

	if (!EtherIsActive) {
		printf("Ethernet disabled\n");
		return(0);
	}

	/* First check local cache. If found, return with pointer to MAC. */
	ep = EtherFromCache(binip);
	if (ep) {
		if (ecpy) {
			memcpy((char *)ecpy, (char *)ep,6);
			ep = ecpy;
		}
		return(ep);
	}

	retry = 0;
	RetransmitDelay(DELAY_INIT_ARP);
	while(1) {
		/* If IP is not on this net, then get the GATEWAY IP address. */
		if (!proxyarp && !IpIsOnThisNet(binip)) {
			gip = getenv("GIPADD");
			if (gip) {
				IpToBin(gip,gbinip);
				if (!IpIsOnThisNet(gbinip)) {
					printf("GIPADD/IPADD subnet confusion.\n");
					return(0);
				}
				ep = EtherFromCache(gbinip);
				if (ep) {
					if (ecpy) {
						memcpy((char *)ecpy, (char *)ep,6);
						ep = ecpy;
					}
					return(ep);
				}
				SendArpRequest(gbinip,0);
				Ip = gbinip;
			}
			else {
				SendArpRequest(binip,0);
				Ip = binip;
			}
		}
		else {
			SendArpRequest(binip,0);
			Ip = binip;
		}
		if (retry) {
			printf("  ARP Retry #%d (%d.%d.%d.%d)\n",retry,
				binip[0],binip[1],binip[2],binip[3]);
		}

		/* Now that the request has been issued, wait for a while to see if
		 * the ARP cache is loaded with the result of the request.
		 */
		timeoutsecs = RetransmitDelay(DELAY_OR_TIMEOUT_RETURN);
		if (timeoutsecs == RETRANSMISSION_TIMEOUT)
			break;
		startElapsedTimer(&tmr,timeoutsecs*1000);
		while(!msecElapsed(&tmr)) {
			ep = EtherFromCache(Ip);
			if (ep)
				break;

			pollethernet();
		}
		if (ep) {
			if (ecpy) {
				memcpy((char *)ecpy, (char *)ep,6);
				ep = ecpy;
			}
			return(ep);
		}
		RetransmitDelay(DELAY_INCREMENT);
		retry++;
	}
#if INCLUDE_ETHERVERBOSE
	if ((EtherVerbose & SHOW_ARP) && (retry))
		printf("  ARP giving up\n");
#endif
	return(0);
}

/* processRARP();
 *	Called by the fundamental ethernet driver code to process a RARP
 *	request.
 */
int
processRARP(struct ether_header *ehdr,ushort size)
{
	struct	arphdr *arpp;

	arpp = (struct arphdr *)(ehdr+1);
	self_ecs(arpp->hardware);
	self_ecs(arpp->protocol);
	self_ecs(arpp->operation);

	switch(arpp->operation) {
	case RARP_RESPONSE:
		if (!memcmp((char *)arpp->targetha, (char *)BinEnetAddr,6)) {
#if INCLUDE_ETHERVERBOSE
			if (EtherVerbose & SHOW_ARP) {
				printf("  RARP Response from %d.%d.%d.%d\n",
				    arpp->senderia[0],arpp->senderia[1],
				    arpp->senderia[2],arpp->senderia[3]);
				printf("  MY IP: from %d.%d.%d.%d\n",
				    arpp->targetia[0],arpp->targetia[1],
				    arpp->targetia[2],arpp->targetia[3]);
			}
#endif
			memcpy((char *)BinIpAddr, (char *)arpp->targetia,6);
		}
		break;
	case RARP_REQUEST:
		break;
	default:
		printf("  Invalid RARP operation: 0x%x\n",arpp->operation);
		return(-1);
	}
	return(0);
}

/* processARP();
 *	Called by the fundamental ethernet driver code to process a ARP
 *	request.
 */
char *
processARP(struct ether_header *ehdr,ushort size)
{
	struct	arphdr *arpp;

	arpp = (struct arphdr *)(ehdr+1);
	self_ecs(arpp->hardware);
	self_ecs(arpp->protocol);
	self_ecs(arpp->operation);

	/* If the sender IP address is all zeroes, then assume this is
	 * an arp probe.  If we are currently doing a probe, and the
	 * address we are probing matches the target IP address of this
	 * probe, then set a flag that will abort the current probe.
	 */
	if ((arpp->senderia[0] == 0) && (arpp->senderia[1] == 0) &&
		(arpp->senderia[2] == 0) && (arpp->senderia[3] == 0)) {
			if (memcmp((char *)arpp->targetia,(char *)&probeIP,4) == 0)
				probeAbort = 1;
	}

	switch(arpp->operation) {
	case ARP_REQUEST:
		if (!memcmp((char *)arpp->targetia, (char *)BinIpAddr,4)) {
			ArpStore(arpp->senderia,arpp->senderha);
			SendArpResp(ehdr);
		}
		break;
	case ARP_RESPONSE:
		if (!memcmp((char *)arpp->targetia, (char *)BinIpAddr,4)) {
			if (!memcmp((char *)arpp->targetia,(char *)arpp->senderia,4))
				printf("WARNING: IP %s may be in use on network\n",IPadd);

			ArpStore(arpp->senderia,arpp->senderha);
		}
		break;
	default:
		printf("  Invalid ARP operation: 0x%x\n",arpp->operation);
		printPkt(ehdr,(int)size,ETHER_INCOMING);
		return((char *)0);
	}
	return((char *)(arpp + 1));
}

/* sendGratuitousARP():
 * As defined in RFC 3220...
 *
 *  An ARP packet sent by a node in order to spontaneously
 *  cause other nodes to update an entry in their ARP cache.
 *
 * Issue a "gratuitous ARP" (RFC 3220) for two reasons...
 * - make sure no other host is already using this IP address.
 * - cause other devices on cable to update their arp cache.
 */
void
sendGratuitousArp(void)
{
	SendArpRequest(BinIpAddr,0);
}

#endif
