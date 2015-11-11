/* etherdev.c:
 * Hooks needed between uuMon and BF537's ethernet MAC.
 *
 * General notice:
 * This code is part of a boot-monitor package developed as a generic base
 * platform for embedded system designs.  As such, it is likely to be
 * distributed to various projects beyond the control of the original
 * author.  Please notify the author of any enhancements made or bugs found
 * so that all may benefit from the changes.  In addition, notification back
 * to the author will allow the new user to pick up changes that may have
 * been made by other users after this version of the code was distributed.
 *
 * Author:	Ed Sutter
 * email:	esutter@lucent.com
 * phone:	908-582-2351
 */

#include "config.h"

#if INCLUDE_ETHERNET

#include "genlib.h"
#include "stddef.h"
#include "ether.h"
#include "timer.h"
#include "lpc_types.h"
#include "lpc17xx_emac.h"

extern unsigned long write_PHY (unsigned long PhyReg, unsigned short Value);
extern long read_PHY (unsigned long PhyReg);

/*
 * enreset():
 *	Reset the PHY and MAC.
 */
void
enreset(void)
{
	EMAC_DeInit();
}

/*
 * eninit():
 * Initialize the PHY and MAC.
 * These steps are based on the section titled "Programming Model"
 * in chapter 8 (Ethernet MAC) of the hardware reference manual.
 *
 * Return 0 if successful; else -1.
 */

EMAC_CFG_Type emaccfg;

static int
eninit(void)
{
	emaccfg.Mode = EMAC_MODE_AUTO;
	emaccfg.pbEMAC_Addr = BinEnetAddr;
	if (EMAC_Init(&emaccfg) == ERROR)
		return(-1);
	return (0);
}

int
EtherdevStartup(int verbose)
{
	enreset();
	return(eninit());
}

void
disableMulticastReception(void)
{
	EMAC_SetFilterMode(EMAC_RFC_MCAST_HASH_EN, DISABLE);
}

void
enableMulticastReception(void)
{
	EMAC_SetFilterMode(EMAC_RFC_MCAST_HASH_EN, ENABLE);
}

/* disablePromiscuousReception():
 * Provide the code that disables promiscuous reception.
 */
void
disablePromiscuousReception(void)
{
	EMAC_SetFilterMode(EMAC_RFC_PERFECT_EN, ENABLE);
}

/* enablePromiscuousReception():
 * Provide the code that enables promiscuous reception.
 */
void
enablePromiscuousReception(void)
{
	EMAC_SetFilterMode(EMAC_RFC_PERFECT_EN, DISABLE);
}

/* disableBroadcastReception():
 * Provide the code that disables broadcast reception.
 */
void
disableBroadcastReception(void)
{
	EMAC_SetFilterMode(EMAC_RFC_BCAST_EN, DISABLE);
}

/* enableBroadcastReception():
 * Provide the code that enables broadcast reception.
 */
void
enableBroadcastReception(void)
{
	EMAC_SetFilterMode(EMAC_RFC_BCAST_EN, ENABLE);
}

/* 
 * enselftest():
 *	Run a self test of the ethernet device(s).  This can be stubbed
 *	with a return(1).
 *	Return 1 if success; else -1 if failure.
 */
int
enselftest(int verbose)
{
	printf("Self-test not yet available for this driver.\n");
	return(1);
}

/* ShowEtherdevStats():
 * This function is used to display device-specific stats (error counts
 * usually).
 */
void
ShowEtherdevStats(void)
{
}

/* getXmitBuffer():
 * Return a pointer to the buffer that is to be used for transmission of
 * the next packet.  The monitor will never need more than one packet at
 * a time; hence, we just keep giving out the same pointer.
 * Note that for the BF537, the first two bytes of the buffer are used
 * by the DMA engine for the length of the packet; so this function actually
 * hands the requestor a pointer incremented by two from the base of tx_data.
 */
static uint32_t outpkt[400];
static uint32_t inpkt[400];

unsigned char *
getXmitBuffer(void)
{
	return((unsigned char *)outpkt);
}

/* sendBuffer():
 * Send out the packet assumed to be built in the buffer returned by the
 * previous call to getXmitBuffer() above.
 *
 * This driver uses register based DMA for ethernet transmit simply
 * because uMon only needs one transmit buffer; hence, there's really
 * no need for the added complexity of the descriptor model.
 * Note that DMA_2 is for ethernet transmit.
 */
int
sendBuffer(int length)
{
	EMAC_PACKETBUF_Type pkt;

	pkt.ulDataLen = length;
	pkt.pbDataBuf = outpkt;
	EMAC_WritePacketBuffer(&pkt);
	EMAC_UpdateTxProduceIndex();
	return(0);
}

/* DisableEtherdev():
 */
void
DisableEtherdev(void)
{
	enreset();
}

/* extGetIpAdd():
 * If there was some external mechanism (other than just using the
 * IPADD shell variable established in the monrc file) for retrieval of
 * the board's IP address, then do it here...
 */
char *
extGetIpAdd(void)
{
	return((char *)0);
}

/* extGetEtherAdd():
 * If there was some external mechanism (other than just using the
 * ETHERADD shell variable established in the monrc file) for retrieval of
 * the board's MAC address, then do it here...
 */
char *
extGetEtherAdd(void)
{
	return((char *)0);
}

/*
 * polletherdev():
 * Called continuously by the monitor (ethernet.c) to determine if there
 * is any incoming ethernet packets.
 *
 * NOTES:
 * 1. This function must be reentrant, because there are a few cases in
 *    processPACKET() where pollethernet() may be called.
 * 2. It should only process one packet per call.  This is important
 *    because if allowed to stay here to flush all available packets,
 *    it may starve the rest of the system (especially in cases of heavy
 *    network traffic).
 * 3. There are cases in the monitor's execution that may cause the
 *    polling polletherdev() to cease for several seconds.  Depending on
 *    network traffic, this may cause the input buffering mechanism on
 *    the ethernet device to overflow.  A robust polletherdev() function
 *    should support this gracefully (i.e. when the error is detected,
 *    attempt to pass all queued packets to processPACKET(), then do what
 *    is necessary to clear the error).
 */

int
polletherdev(void)
{
	int pktlen;
	EMAC_PACKETBUF_Type pkt;

	EMAC_LinkStatus();

	if (EMAC_CheckReceiveIndex() == FALSE)
		return(0);

	pktlen = EMAC_GetReceiveDataSize();
	pkt.ulDataLen = pktlen;
	pkt.pbDataBuf = inpkt;
	EMAC_ReadPacketBuffer(&pkt);
	
	processPACKET((struct ether_header *)inpkt,pktlen);
	EMAC_UpdateRxConsumeIndex();

	return(1);
}

#endif
