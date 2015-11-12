/* etherdev.c:
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
#include "stddefs.h"
#include "ether.h"
#include "timer.h"
#include "inc/hw_ethernet.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/ethernet.h"

extern unsigned long write_PHY (unsigned long PhyReg, unsigned short Value);
extern long read_PHY (unsigned long PhyReg);

/*
 * enreset():
 *	Reset the PHY and MAC.
 */
void
enreset(void)
{
}

/*
 * eninit():
 * Initialize the PHY and MAC.
 * These steps are based on the section titled "Programming Model"
 * in chapter 8 (Ethernet MAC) of the hardware reference manual.
 *
 * Return 0 if successful; else -1.
 */


static int
eninit(void)
{
	//
	// Enable and Reset the Ethernet Controller.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
	SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

	// Configure two GPIO pins as Ethernet status LEDs...
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);

	// Establish the MAC address:
	EthernetMACAddrSet(ETH_BASE,BinEnetAddr);

	// Enable auto-crc, padding and full-duplex:
	EthernetConfigSet(ETH_BASE,
		ETH_CFG_TX_DPLXEN|ETH_CFG_TX_CRCEN|ETH_CFG_TX_PADEN|ETH_CFG_RX_BADCRCDIS);

	EthernetEnable(ETH_BASE);

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
	unsigned long cfg = EthernetConfigGet(ETH_BASE);
	cfg &= ~ETH_CFG_RX_AMULEN;
	EthernetConfigSet(ETH_BASE,cfg);
}

void
enableMulticastReception(void)
{
	unsigned long cfg = EthernetConfigGet(ETH_BASE);
	cfg |= ETH_CFG_RX_AMULEN;
	EthernetConfigSet(ETH_BASE,cfg);
}

/* disablePromiscuousReception():
 * Provide the code that disables promiscuous reception.
 */
void
disablePromiscuousReception(void)
{
	unsigned long cfg = EthernetConfigGet(ETH_BASE);
	cfg &= ~ETH_CFG_RX_PRMSEN;
	EthernetConfigSet(ETH_BASE,cfg);
}

/* enablePromiscuousReception():
 * Provide the code that enables promiscuous reception.
 */
void
enablePromiscuousReception(void)
{
	unsigned long cfg = EthernetConfigGet(ETH_BASE);
	cfg |= ETH_CFG_RX_PRMSEN;
	EthernetConfigSet(ETH_BASE,cfg);
}

/* disableBroadcastReception():
 * Referring to the description of the MACRCTL register in
 * the datasheet, I don't think this device allows broadcast
 * reception to be disabled.
 */
void
disableBroadcastReception(void)
{
}

/* enableBroadcastReception():
 * Referring to the description of the MACRCTL register in
 * the datasheet, I think this device has broadcast reception
 * enabled by default (with no option to disable it).
 */
void
enableBroadcastReception(void)
{
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
	EthernetPacketPut(ETH_BASE, (unsigned char *)outpkt, (long)length);
	return(0);
}

/* DisableEtherdev():
 */
void
DisableEtherdev(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
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

int
polletherdev(void)
{
	int pktlen;
	pktlen = EthernetPacketGetNonBlocking(ETH_BASE , (unsigned char *)inpkt, sizeof(inpkt));
	if (pktlen == 0)
		return(0);

	processPACKET((struct ether_header *)inpkt,pktlen);
	return(1);
}

#endif
