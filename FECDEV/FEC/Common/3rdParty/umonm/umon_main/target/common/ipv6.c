/* ipv6.c:
 *	This code supports some basic IPV6 stuff.
 *
 *  The goal as of this writing (Aug 2013) is to meet or exceed the
 *	requirements specified by RFC 6434 "IPv6 Node Requirements".
 *	Some of the other RFCs (referenced in 6434) are...
 *
 *	2640: IPv6 Specification (updates in 5095 and 5722)
 *	2464: IPv6 Packets over Ethernet
 *	4443: ICMPv6
 *	4861: IPv6 Neighbor Discovery
 *	4862: IPv6 Stateless Address Autoconfiguration
 *	4291: IPv6 Addressing Architecture
 * 
 * Some good sites for information:
 *	http://www.zytrax.com/tech/protocols/ipv6.html
 *	http://computernetworkingnotes.com/ipv6-features-concepts-and-configurations/ipv6-neighbor-discovery.html
 *	http://astorinonetworks.com/2011/07/11/ipv6-solicited-node-addresses/
 *
 *	Status:
 *	As of 8/22/2013, DAD, NS, NA and ICMP-echo server are working.
 *	I believe RS is in place, but haven't been able to confirm that it
 *	works because we don't have a router that responds.  I did verify that
 *	my RS is the same as an RS out of a linux box bootup.
 *	DAD: duplicate address detection 
 *  NS:  neighbor solicit, NA: neighbor advertise, RS: Router solicit
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
#if INCLUDE_ETHERNET
#if INCLUDE_IPV6
#include "endian.h"
#include "genlib.h"
#include "ether.h"
#include "stddefs.h"
#include "cli.h"
#include "timer.h"


const uint8_t all_routers_mcast_addr[] = {
	0xff,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02
};

const uint8_t solicited_node_mcast_addr[] = {
	0xff,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0xff,0x00,0x00,0x00
};

const uint8_t allnodes_multicast_mac[] = {
	0x33,0x33,0x00,0x00,0x00,0x01
};

const uint8_t allrouters_multicast_mac[] = {
	0x33,0x33,0x00,0x00,0x00,0x02
};

uint8_t BinIpv6Addr[IPV6ADDRSZ];	/* Space for binary IPv6 address */

int SendNeighborAdvertise(struct ether_header *ehdr,uint16_t size);
int	SendEcho6Response(struct ether_header *ehdr,uint16_t size);

int
thisIsMyIPv6Addr(uint8_t *ipv6addr)
{
	/* If the incoming address matches our IPv6address return 1...
	 */
	if (memcmp((void *)BinIpv6Addr,(void *)ipv6addr,IPV6ADDRSZ) == 0)
		return(1);

	/* If the incoming address matches the solicited_node_mcast_address return 1...
	 */
	if (memcmp((void *)solicited_node_mcast_addr,(void *)ipv6addr,IPV6ADDRSZ-3) == 0) {
		if ((ipv6addr[13] == BinEnetAddr[3]) &&
			(ipv6addr[14] == BinEnetAddr[4]) &&
			(ipv6addr[15] == BinEnetAddr[5]))
			return(1);
	}
	/* Else return 0...
 	 */
	return(0);
}

/* MacToLinkLocalIPV6Addr():
 * This function returns the IPv6 "link-local" address for
 * this device.  The address is derived from the MAC address
 * and its scope is limited to the local LAN this device is on.
 *
 * The function prepends the link-local prefix padded with
 * zeros (fe80:0000:0000:0000) to an EUI64 derived from
 * this host's 48-bit Ethernet MAC address (EUI=Extended
 * Unique Id).
 *
 * Refer to sections 4 (Stateless Autoconfiguration) and
 * 5 (Link-local address) of RFC 2464 for more details on
 * this conversion.  
 * 
 */
#define ULBIT	0x02	// Universal/local bit

void
MacToLinkLocalIPV6Addr(uint8_t *ipv6addr, uint8_t *mac)
{
	uint8_t *addr6 = ipv6addr;

	*addr6++ = 0xfe;
	*addr6++ = 0x80;
	*addr6++ = 0;
	*addr6++ = 0;
	*addr6++ = 0;
	*addr6++ = 0;
	*addr6++ = 0;
	*addr6++ = 0;

	*addr6 = *mac++;

	// compliment the universal/local bit:
	if (*addr6 & ULBIT)
		*addr6++ &= ~ULBIT;
	else
		*addr6++ |= ULBIT;

	*addr6++ = *mac++;
	*addr6++ = *mac++;
	*addr6++ = 0xff;
	*addr6++ = 0xfe;
	*addr6++ = *mac++;
	*addr6++ = *mac++;
	*addr6++ = *mac++;
}

int
processICMPv6(struct ether_header *ehdr,uint16_t size)
{
	struct	ipv6hdr *ipv6p;
	struct	icmpv6hdr *icmp6p;

	ipv6p = (struct ipv6hdr *)(ehdr+1);
	icmp6p = (struct icmpv6hdr *)(ipv6p+1);

	/* This code needs to be able to respond to an Echo request
	 * and a neighbor solicit.
	 */
	if (icmp6p->type == 135)			// Neighbor solicit
		SendNeighborAdvertise(ehdr,size);
	else if (icmp6p->type == 136) {		// Neighbor advertise
		if (thisIsMyIPv6Addr(ipv6p->daddr)) 
			printf("**********\n** WARNING: Duplicate address detected\n**********\n");
	}
	else if (icmp6p->type == 128)		// Echo request
		SendEcho6Response(ehdr,size);
	return(0);
}

/* processIPv6();
 *	Called by the fundamental ethernet driver code to
 *	process an incoming IPV6 packet.
 */
int
processIPv6(struct ether_header *ehdr,uint16_t size)
{
	struct	ipv6hdr *ipv6p;

	ipv6p = (struct ipv6hdr *)(ehdr+1);

	if (IPV6_VERSION(ecl(ipv6p->vtf)) != 6) {
		printf("  IPV6 pkt bad vrsn\n");
		return(0);
	}

	if (thisIsMyIPv6Addr(ipv6p->daddr) == 0)
		return(0);

	if (ipv6p->nxthdr == IP_ICMPV6) {
		processICMPv6(ehdr,size);
		return(0);
	}
	else if (ipv6p->nxthdr == IP_UDP) {
		struct Udphdr *uhdr = (struct Udphdr *)(ipv6p+1);

		if (ecs(uhdr->uh_dport) == IPPORT_MONCMD)
			processMONCMD(ehdr,size);
		return(0);
	}

	return(0);
}

int
SendRouterSolicitation(uint8_t *srcmac)
{
	uint8_t		*rsvd, *opts;
	uint16_t	*csp;
	uint32_t	csum;
	int			i, icmplen, pktsiz;
	struct ipv6hdr *ip6p;
	struct ether_header *te;
	struct icmpv6hdr *icmp6p;

	/* Retrieve an ethernet buffer from the driver and populate the */
	/* link layer portion of the outgoing packet: */
	te = (struct ether_header *) getXmitBuffer();
	memcpy((char *)&te->ether_shost,(char *)BinEnetAddr,6);
	memcpy((char *)&te->ether_dhost,(char *)allrouters_multicast_mac,6);
	te->ether_type = ecs(ETHERTYPE_IPV6);

	/* Move to the IPv6 portion of the packet and populate it appropriately: */
	ip6p = (struct ipv6hdr *) (te + 1);
	memcpy((void *)ip6p->daddr,(void *)all_routers_mcast_addr,IPV6ADDRSZ);
	MacToLinkLocalIPV6Addr(ip6p->saddr,(uint8_t *)srcmac);
	ip6p->hoplmt = 255;
	ip6p->nxthdr = IP_ICMPV6;
	ip6p->vtf = ecl(0x60000000);

	/* Build the ICMPv6 message:
	 */
	icmp6p = (struct icmpv6hdr *)(ip6p+1);
	icmp6p->type = 133;		// Type: Router solicitation
	icmp6p->code = 0;
	icmp6p->cksum = 0;
	rsvd = (uint8_t *)(icmp6p+1);
	*rsvd++ = 0;
	*rsvd++ = 0;
	*rsvd++ = 0;
	*rsvd++ = 0;
	icmplen = sizeof(struct icmpv6hdr) + 4;

	/* Append the source-link-layer option:
	 */
	opts = rsvd;
	*opts++ = 1;	// Type=1: Source Link-layer address
	*opts++ = 1;	// Length = 1 octet
	memcpy((char *)opts,(char *)srcmac,6);
	icmplen += 8;
	
	/* After building the packet, we populate the payload length...
	 */
	ip6p->plen = ecs(icmplen);

	/* compute checksum of icmp message and a pseudo header which
	 * consists of the source and destination addresses, pkt length
	 * and nxthdr (=58) portions of the IPV6 header.
	 */
	// Start with pseudo header:
	csum = icmplen;						// pktlen
	csum += IP_ICMPV6;					// nxthdr
	csp = (uint16_t *)&ip6p->saddr;		// saddr & daddr
	for (i=0;i<IPV6ADDRSZ;i++,csp++)
		csum += ecs(*csp);

	// Follow with the ICMPv6 header:
	csp = (uint16_t *) icmp6p;
	for (i=0;i<icmplen;i+=2,csp++)
		csum += ecs(*csp);

	csum = (csum & 0xffff) + (csum >> 16);
	csum = ~csum;

	icmp6p->cksum = ecs(csum);

	pktsiz = (sizeof(struct ether_header) + sizeof(struct ipv6hdr) + icmplen);
	sendBuffer(pktsiz);

	return(pktsiz);
}

int
SendNeighborSolicitation(uint8_t *neighbor)
{
	uint8_t		*rsvd;
	uint16_t	*csp;
	uint32_t	csum;
	int			i, icmplen, pktsiz;
	struct ipv6hdr *ip6p;
	struct ether_header *te;
	struct icmpv6hdr *icmp6p;

	/* Retrieve an ethernet buffer from the driver and populate the */
	/* link layer portion of the outgoing packet: */
	te = (struct ether_header *) getXmitBuffer();
	memcpy((char *)&te->ether_shost,(char *)BinEnetAddr,6);

	/* Based on what I've read (see IP Multicast in Wikipedia), the MAC
	 * level IPv6 Multicast address is derived by the four low-order
	 * octets OR'ed with 33:33:00:00:00:00; however, I've only gotten a
	 * response (from a neighbor linux box) if I use MAC broadcast
	 * (ff:ff:ff:ff:ff:ff) or  33:33:00:00:00:01.
	 */
#if 1
	memcpy((char *)&te->ether_dhost,(char *)allnodes_multicast_mac,6);
#else
	te->ether_dhost.ether_addr_octet[0] = 0x33;
	te->ether_dhost.ether_addr_octet[1] = 0x33;
	te->ether_dhost.ether_addr_octet[2] = neighbor[12];
	te->ether_dhost.ether_addr_octet[3] = neighbor[13];
	te->ether_dhost.ether_addr_octet[4] = neighbor[14];
	te->ether_dhost.ether_addr_octet[5] = neighbor[15];
#endif
	te->ether_type = ecs(ETHERTYPE_IPV6);

	/* Move to the IPv6 portion of the packet and populate it appropriately: */
	ip6p = (struct ipv6hdr *) (te + 1);
	memcpy((void *)ip6p->daddr,(void *)solicited_node_mcast_addr,IPV6ADDRSZ);
	ip6p->daddr[13] = neighbor[13];
	ip6p->daddr[14] = neighbor[14];
	ip6p->daddr[15] = neighbor[15];
	memset((void *)ip6p->saddr,0,IPV6ADDRSZ);
	ip6p->hoplmt = 255;
	ip6p->nxthdr = IP_ICMPV6;
	ip6p->vtf = ecl(0x60000000);

	/* Build the ICMPv6 message:
	 */
	icmp6p = (struct icmpv6hdr *)(ip6p+1);
	icmp6p->type = 135;		// Type: Neighbor solicitation
	icmp6p->code = 0;
	icmp6p->cksum = 0;
	rsvd = (uint8_t *)(icmp6p+1);
	*rsvd++ = 0;
	*rsvd++ = 0;
	*rsvd++ = 0;
	*rsvd++ = 0;

	memcpy((void *)rsvd,(void *)neighbor,IPV6ADDRSZ);

	icmplen = sizeof(struct icmpv6hdr) + 4 + IPV6ADDRSZ;

	/* After building the packet, we populate the payload length...
	 */
	ip6p->plen = ecs(icmplen);

	/* compute checksum of icmp message and a pseudo header which
	 * consists of the source and destination addresses, pkt length
	 * and nxthdr (=58) portions of the IPV6 header.
	 */
	// Start with pseudo header:
	csum = icmplen;						// pktlen
	csum += IP_ICMPV6;					// nxthdr
	csp = (uint16_t *)&ip6p->saddr;		// saddr & daddr
	for (i=0;i<IPV6ADDRSZ;i++,csp++)
		csum += ecs(*csp);

	// Follow with the ICMPv6 header:
	csp = (uint16_t *) icmp6p;
	for (i=0;i<icmplen;i+=2,csp++)
		csum += ecs(*csp);

	csum = (csum & 0xffff) + (csum >> 16);
	csum = ~csum;

	icmp6p->cksum = ecs(csum);

	pktsiz = (sizeof(struct ether_header) + sizeof(struct ipv6hdr) + icmplen);
	sendBuffer(pktsiz);

	return(pktsiz);
}

int
SendNeighborAdvertise(struct ether_header *ehdr,uint16_t size)
{
	uint8_t		*rsvd, *opts;
	uint16_t	*csp;
	uint32_t	csum;
	int			i, icmplen, pktsiz;
	struct ether_header *te;
	struct icmpv6hdr *icmp6p, *icmp6ptmp;
	struct ipv6hdr *ip6p, *ip6ptmp;

	ip6ptmp = (struct ipv6hdr *)(ehdr+1);
	icmp6ptmp = (struct icmpv6hdr *)(ip6ptmp+1);
	
	/* Retrieve an ethernet buffer from the driver and populate the */
	/* link layer portion of the outgoing packet: */
	te = (struct ether_header *) getXmitBuffer();
	memcpy((char *)&te->ether_shost,(char *)BinEnetAddr,6);
	memcpy((char *)&te->ether_dhost,(char *)&ehdr->ether_shost,6);
	te->ether_type = ecs(ETHERTYPE_IPV6);

	/* Move to the IPv6 portion of the packet and populate it appropriately: */
	ip6p = (struct ipv6hdr *) (te + 1);
	memcpy((void *)ip6p->daddr,(void *)ip6ptmp->saddr,IPV6ADDRSZ);
	memcpy((void *)ip6p->saddr,(void *)ip6ptmp->daddr,IPV6ADDRSZ);
	ip6p->hoplmt = 255;
	ip6p->nxthdr = IP_ICMPV6;
	ip6p->vtf = ecl(0x60000000);

	/* Build the ICMPv6 message:
	 */
	icmp6p = (struct icmpv6hdr *)(ip6p+1);
	icmp6p->type = 136;		// Type: Neighbor advertisement
	icmp6p->code = 0;
	icmp6p->cksum = 0;
	rsvd = (uint8_t *)(icmp6p+1);
	*rsvd++ = 0x60;	// R=0|S=1|O=1
	*rsvd++ = 0;
	*rsvd++ = 0;
	*rsvd++ = 0;
	memcpy((void *)rsvd,(void *)(icmp6ptmp+1)+4,IPV6ADDRSZ);
	icmplen = sizeof(struct icmpv6hdr) + 4 + IPV6ADDRSZ;

	/* Append the source-link-layer option:
	 */
	opts = rsvd+IPV6ADDRSZ;
	*opts++ = 2;	// Type=2: Target Link-layer address
	*opts++ = 1;	// Length = 1 octet
	memcpy((char *)opts,(char *)BinEnetAddr,6);
	icmplen += 8;

	/* After building the packet, we populate the payload length...
	 */
	ip6p->plen = ecs(icmplen);

	/* compute checksum of icmp message and a pseudo header which
	 * consists of the source and destination addresses, pkt length
	 * and nxthdr (=58) portions of the IPV6 header.
	 */
	// Start with pseudo header:
	csum = icmplen;						// pktlen
	csum += IP_ICMPV6;					// nxthdr
	csp = (uint16_t *)&ip6p->saddr;		// saddr & daddr
	for (i=0;i<IPV6ADDRSZ;i++,csp++)
		csum += ecs(*csp);

	// Follow with the ICMPv6 header:
	csp = (uint16_t *) icmp6p;
	for (i=0;i<icmplen;i+=2,csp++)
		csum += ecs(*csp);

	csum = (csum & 0xffff) + (csum >> 16);
	csum = ~csum;

	icmp6p->cksum = ecs(csum);

	pktsiz = (sizeof(struct ether_header) + sizeof(struct ipv6hdr) + icmplen);
	sendBuffer(pktsiz);
	return(pktsiz);
}

int
SendEcho6Response(struct ether_header *ehdr,uint16_t size)
{
	uint8_t		*idsno_out, *idsno_in;
	uint16_t	*csp;
	uint32_t	csum, tot;
	int			i, icmplen, pktsiz;
	struct ether_header *te;
	struct icmpv6hdr *icmp6p, *icmp6ptmp;
	struct ipv6hdr *ip6p, *ip6ptmp;

	ip6ptmp = (struct ipv6hdr *)(ehdr+1);
	icmp6ptmp = (struct icmpv6hdr *)(ip6ptmp+1);
	
	/* Retrieve an ethernet buffer from the driver and populate the */
	/* link layer portion of the outgoing packet: */
	te = (struct ether_header *) getXmitBuffer();
	memcpy((char *)&te->ether_shost,(char *)BinEnetAddr,6);
	memcpy((char *)&te->ether_dhost,(char *)&ehdr->ether_shost,6);
	te->ether_type = ecs(ETHERTYPE_IPV6);

	/* Move to the IPv6 portion of the packet and populate it appropriately: */
	ip6p = (struct ipv6hdr *) (te + 1);
	memcpy((void *)ip6p->daddr,(void *)ip6ptmp->saddr,IPV6ADDRSZ);
	memcpy((void *)ip6p->saddr,(void *)ip6ptmp->daddr,IPV6ADDRSZ);
	ip6p->hoplmt = 255;
	ip6p->nxthdr = IP_ICMPV6;
	ip6p->vtf = ecl(0x60000000);

	/* Build the ICMPv6 message:
	 */
	icmp6p = (struct icmpv6hdr *)(ip6p+1);
	icmp6p->type = 129;		// Type:  Echo reply
	icmp6p->code = 0;
	icmp6p->cksum = 0;

	/* Transfer id/seqno and data from incoming echo request to
	 * the outgoing echo reply...
	 */
	idsno_out = (uint8_t *)(icmp6p+1);
	idsno_in = (uint8_t *)(icmp6ptmp+1);
	tot = size - (idsno_in - (uint8_t *)ehdr); 
	for(i=0;i<tot;i++)
		*idsno_out++ = *idsno_in++;

	icmplen = sizeof (struct icmpv6hdr) + tot;

	/* After building the packet, we populate the payload length...
	 */
	ip6p->plen = ecs(icmplen);

	/* compute checksum of icmp message and a pseudo header which
	 * consists of the source and destination addresses, pkt length
	 * and nxthdr (=58) portions of the IPV6 header.
	 */
	// Start with pseudo header:
	csum = icmplen;						// pktlen
	csum += IP_ICMPV6;					// nxthdr
	csp = (uint16_t *)&ip6p->saddr;		// saddr & daddr
	for (i=0;i<IPV6ADDRSZ;i++,csp++)
		csum += ecs(*csp);

	// Follow with the ICMPv6 header:
	csp = (uint16_t *) icmp6p;
	for (i=0;i<icmplen;i+=2,csp++)
		csum += ecs(*csp);

	csum = (csum & 0xffff) + (csum >> 16);
	csum = ~csum;

	icmp6p->cksum = ecs(csum);

	pktsiz = (sizeof(struct ether_header) + sizeof(struct ipv6hdr) + icmplen);
	sendBuffer(pktsiz);
	return(pktsiz);
}

char *CliHelp_IPV6[] = {
	"IPV6 support",
	"-[f:m:t:] {rsm | nsm | dad}",
#if INCLUDE_VERBOSEHELP
	"Options...",
	" -f {flow label} ",
	" -m {mac address} ",	// shuttle test: ipv6 -m 00:30:1b:44:05:e3 nsm
	" -t {traffic class} ",
	"Commands...",
	" rsm   send router solicitation message",
#endif
	0,
};

/* CliCmd_IPV6():
 * The ndp command implements the "Router Solicitation" message as
 * outlined by RFC4861....
 * Hosts send Router Solicitations in order to prompt routers to
 * generate Router Advertisements quickly.
 */
int
CliCmd_IPV6(int argc,char *argv[])
{
	int			opt;
	uint8_t		srcmac[6];
	uint8_t		addr6buf[48];	// IPv6 addr bin or ASCII storage

	// Default source MAC is this board's MAC...
	memcpy((void *)srcmac,(void *)BinEnetAddr,6);

	while ((opt=getopt(argc,argv,"m:s:")) != -1) {
		switch(opt) {
		case 'm':
			if (strcmp(optarg,"on") == 0) {
				IPv6MulticastOnly = 0;
				enableMulticastReception();
			}
			else if (strcmp(optarg,"only") == 0) {
				IPv6MulticastOnly = 1;
				enableMulticastReception();
			}
			else if (strcmp(optarg,"off") == 0) {
				IPv6MulticastOnly = 0;
				disableMulticastReception();
			}
			else
				return(CMD_PARAM_ERROR);
			break;
		case 's':	// Override default src mac address (for test). 
			if (EtherToBin(optarg,srcmac) < 0)
				return(CMD_PARAM_ERROR);
			break;
		default:
			return(CMD_PARAM_ERROR);
		}
	}

	if (argc == optind) {
		printf("My IPv6 Link-Local addr: %s\n",Ipv6ToString(BinIpv6Addr,(char *)addr6buf));
		return(CMD_SUCCESS);
	}

	if (strcmp(argv[optind],"rsm") == 0) {
		SendRouterSolicitation(srcmac);
	}
	else if (strcmp(argv[optind],"nsm") == 0) {
		if ((argc == optind + 2)  &&
			(Ipv6ToBin(argv[optind+1],addr6buf) == 0))
			SendNeighborSolicitation(addr6buf);
		else
			return(CMD_PARAM_ERROR);
	}
	else if (strcmp(argv[optind],"dad") == 0) {
		/* DAD is a special case of NSM which simply uses this nodes'
		 * address as the neighbor (to make sure no one answers).
		 */
		SendNeighborSolicitation(BinIpv6Addr);
	}
	else
		return(CMD_PARAM_ERROR);

	return(CMD_SUCCESS);
}


#endif	/* INCLUDE_IPV6 */
#endif	/* INCLUDE_ETHERNET */
