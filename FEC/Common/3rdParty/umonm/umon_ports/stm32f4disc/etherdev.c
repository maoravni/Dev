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
//#include "stddefs.h"
#include "ether.h"
#include "timer.h"
#include "stm32f4x7_eth.h"

extern int ETH_BSP_Config(void);

extern unsigned long write_PHY (unsigned long PhyReg, unsigned short Value);
extern long read_PHY (unsigned long PhyReg);

/* Ethernet Rx & Tx DMA Descriptors */
extern ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];

/* Ethernet Receive buffers  */
extern uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];

/* Ethernet Transmit buffers */
extern uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];

/* Global pointers to track current transmit and receive descriptors */
extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;

/* Global pointer for last received frame infos */
extern ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;


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
	int i, rc = ETH_BSP_Config();

	if (rc < 0)
		return(rc);

	ETH_MACAddressConfig(0, BinEnetAddr);

	/* Initialize Tx Descriptors list: Chain Mode */
	ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	/* Initialize Rx Descriptors list: Chain Mode  */
	ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	/* Enable Ethernet Rx interrrupt */
    for(i=0; i<ETH_RXBUFNB; i++)
      ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);

	ETH_Start();
	return(0);
}

int
EtherdevStartup(int verbose)
{
	enreset();
	return(eninit());
}

#define PAM	0x00000010	// Pass all multicast frames
#define BFD	0x00000020	// Broadcast frames disable
#define RA	0x80000000	// Receive all frames (promiscuous)

void
disableMulticastReception(void)
{
	ETH->MACFFR &= ~PAM;
}

void
enableMulticastReception(void)
{
	ETH->MACFFR |= PAM;
}

/* disablePromiscuousReception():
 * Provide the code that disables promiscuous reception.
 */
void
disablePromiscuousReception(void)
{
	ETH->MACFFR &= ~RA;
}

/* enablePromiscuousReception():
 * Provide the code that enables promiscuous reception.
 */
void
enablePromiscuousReception(void)
{
	ETH->MACFFR |= RA;
}

/* disableBroadcastReception():
 * Referring to the description of the MACRCTL register in
 * the datasheet, I don't think this device allows broadcast
 * reception to be disabled.
 */
void
disableBroadcastReception(void)
{
	ETH->MACFFR |=  BFD;
}

/* enableBroadcastReception():
 * Referring to the description of the MACRCTL register in
 * the datasheet, I think this device has broadcast reception
 * enabled by default (with no option to disable it).
 */
void
enableBroadcastReception(void)
{
	ETH->MACFFR &=  ~BFD;
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
static uint32_t outpkt[400];
static uint32_t inpkt[400];
 */
static unsigned char *nxtsndbfr;

unsigned char *
getXmitBuffer(void)
{
	nxtsndbfr = (unsigned char *)(DMATxDescToSet->Buffer1Addr);
	return(nxtsndbfr);
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
#if INCLUDE_ETHERVERBOSE
	if (EtherVerbose &  SHOW_OUTGOING)
		printPkt((struct ether_header *)nxtsndbfr,length,ETHER_OUTGOING);
#endif
	ETH_Prepare_Transmit_Descriptors(length);
	return(0);
}

/* DisableEtherdev():
 */
void
DisableEtherdev(void)
{
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
	FrameTypeDef frame;
	unsigned long i=0;
	__IO ETH_DMADESCTypeDef *DMARxNextDesc;

	/* Get received frame */
	frame = ETH_Get_Received_Frame_interrupt();

	/* check that frame has no error */
	if ((frame.descriptor->Status & ETH_DMARxDesc_ES) == (uint32_t)RESET) {
		if (frame.length > 0)
			processPACKET((struct ether_header *)frame.buffer,frame.length);
	}

	/* Release descriptors to DMA */
	/* Check if received frame with multiple DMA buffer segments */
	if (DMA_RX_FRAME_infos->Seg_Count > 1)
		DMARxNextDesc = DMA_RX_FRAME_infos->FS_Rx_Desc;
	else
		DMARxNextDesc = frame.descriptor;

	/* Set Own bit in Rx descriptors: gives the buffers back to DMA */
	for (i=0; i<DMA_RX_FRAME_infos->Seg_Count; i++) {
		DMARxNextDesc->Status = ETH_DMARxDesc_OWN;
		DMARxNextDesc = (ETH_DMADESCTypeDef *)(DMARxNextDesc->Buffer2NextDescAddr);
	}

	/* Clear Segment_Count */
	DMA_RX_FRAME_infos->Seg_Count =0;

	/* When Rx Buffer unavailable flag is set: clear it and resume reception */
	if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET) {
		/* Clear RBUS ETHERNET DMA flag */
		ETH->DMASR = ETH_DMASR_RBUS;

		/* Resume DMA reception */
		ETH->DMARPDR = 0;
	}
	return 1;
}

#endif
