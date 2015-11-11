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
 * email:	ed.sutter@alcatel-lucent.com
 * phone:	908-582-2351
 *
 * This code is made up of snippets of the Freescale SDK...
 *
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if INCLUDE_ETHERNET

#include "stddefs.h"
#include "ether.h"
#include "timer.h"
#include "endian.h"
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "hal/enet/fsl_enet_hal.h"
#include "fsl_clock_manager.h"

typedef enum _enet_phy_register
{
    kEnetPhyCR = 0, /*!< PHY control register */
    kEnetPhySR = 1, /*!< PHY status register*/
    kEnetPhyId1 = 2, /*!< PHY identification register 1*/
    kEnetPhyId2 = 3, /*!< PHY identification register 2*/
    kEnetPhyCt2 = 0x1f /*!< PHY control2 register*/
} enet_phy_register_t;

typedef enum _enet_phy_status
{
    kEnetPhyLinkStatus = 0x4,
    kEnetPhyAutoNegAble = 0x08,
    kEnetPhyAutoNegComplete = 0x20,
    kEnetPhySpeedDulpexMask = 0x1c
} enet_phy_status_t;

typedef enum _enet_phy_control
{
    kEnetPhyAutoNeg = 0x1000,
    kEnetPhySpeed = 0x2000,
    kEnetPhyLoop = 0x4000,
    kEnetPhyReset = 0x8000,
    kEnetPhy10HalfDuplex = 0x4,
    kEnetPhy100HalfDuplex = 0x8,
    kEnetPhy10FullDuplex = 0x14,
    kEnetPhy100FullDuplex = 0x18
} enet_phy_control_t;

#define ENETDEV	0
#define PHYADDR	0
#define SHOW_ENET_CONTROL 0
#define USE_ENHANCED_FRAME	1	// If this is cleared, use different bd struct
#define ENETTRACE(a) //printf a 
#define ENET_MII_CLOCK		(2500000L)
#define EnetMaxFrameSize	1518
#define EnetBufSize			1536	//(mod16)

#define RXBUFTOT	8
#define TXBUFTOT	4

static char lastlinkstate;
static unsigned long lastctrl;
static int phy_read(int phyReg, uint16_t *value);
static int phy_write(int phyReg, uint16_t value);

#if SHOW_ENET_CONTROL
static void showCtrl(uint16_t, char *);
static void showBd(enet_bd_struct_t *);
#endif


/* Buffer descriptors:
 */
static enet_bd_struct_t *rxbdtbl, *txbdtbl;
static uint8_t rxbdbuf[(sizeof(enet_bd_struct_t) * (RXBUFTOT+1))];
static uint8_t txbdbuf[(sizeof(enet_bd_struct_t) * (TXBUFTOT+1))];

/* Buffers:
 */
static uint8_t txbuf[TXBUFTOT][EnetBufSize], rxbuf[RXBUFTOT][EnetBufSize];
static uint8_t *txbuftbl[TXBUFTOT];
static int rxidx, txidx;

/*
 * enreset():
 *	Reset the PHY and MAC.
 */
void
enreset(void)
{
	struct elapsed_tmr tmr;

	/* Turn on ENET & DMA module clock gate */
	clock_manager_set_gate(kClockModuleENET, 0U, true);

	/* Reset ENET mac */
	enet_hal_reset_ethernet(ENETDEV);

	/* Wait for reset to complete... */
	startElapsedTimer(&tmr,1000);
	while(!enet_hal_is_reset_completed(ENETDEV)) {
		if(msecElapsed(&tmr)) {
			printf("Enet reset timeout\n");
			break;
		}
	}
}


/*
 * eninit():
 * These steps are meant to mimic the SDK function ENET_initialize()
 * which calls enet_mac_init().
 * Return 0 if successful; else -1.
 */

static int
eninit(void)
{
	int i;
	uint8_t *bdp, *bp;
	uint16_t phyreg;
	uint32_t freq;

	rxidx = txidx = 0;

	/* Disable all ENET mac interrupt and Clear all interrupt events*/
	enet_hal_config_interrupt(ENETDEV, kEnetAllInterrupt, false);
	enet_hal_clear_interrupt(ENETDEV, kEnetAllInterrupt);

	/* Program this station's physical address*/
	enet_hal_set_mac_address(ENETDEV, BinEnetAddr);

	/* Clear group and individual hash register*/
	enet_hal_set_group_hashtable(ENETDEV, 0, kEnetSpecialAddressInit);
	enet_hal_set_individual_hashtable(ENETDEV, 0, kEnetSpecialAddressInit);

	// equivalent of enet_mac_configure_controller (drivers/enet/src/fsl_enet_rtcs_adapter.c):
	enet_hal_config_rmii(ENETDEV,kEnetCfgRmii,kEnetCfgSpeed100M,kEnetCfgFullDuplex,0,0);
	enet_hal_set_rx_max_size(ENETDEV,EnetBufSize,EnetMaxFrameSize);
    enet_hal_config_promiscuous(ENETDEV, 0);
    enet_hal_enable_flowcontrol(ENETDEV, 0);
    enet_hal_enable_pauseforward(ENETDEV,0);
    enet_hal_enable_broadcastreject(ENETDEV,0);
    enet_hal_enable_padremove(ENETDEV, 0);
    enet_hal_enable_rxcrcforward(ENETDEV, 1);
    enet_hal_enable_payloadcheck(ENETDEV, 0);
    enet_hal_enable_sleep(ENETDEV, 0);
	
    enet_hal_clear_mib(ENETDEV, 1);

	/* NOTE: the function enet_mac_configure_fifo_accel() passes values to
	 * enet_hal_config_xx_fifo with some members not initialized.
 	 * Since accelleration is not used, I'm assuming it doesn't matter.
	 */
    BW_ENET_RSFL_RX_SECTION_FULL(ENETDEV,0);
    BW_ENET_RSEM_RX_SECTION_EMPTY(ENETDEV,0);
    BW_ENET_RAEM_RX_ALMOST_EMPTY(ENETDEV,4);
    BW_ENET_RAFL_RX_ALMOST_FULL(ENETDEV,4);

    BW_ENET_TFWR_STRFWD(ENETDEV, 0);
    BW_ENET_TSEM_TX_SECTION_EMPTY(ENETDEV,0);
    BW_ENET_TAEM_TX_ALMOST_EMPTY(ENETDEV,4);
    BW_ENET_TAFL_TX_ALMOST_FULL(ENETDEV,8);

	ENETTRACE(("bdsize = %ld\n",enet_hal_get_bd_size()));

	/* Initialize the buffer descriptor tables:
	 * (both the buffers and the descriptors have to be 16-byte aligned)
	 */
	memset(rxbdbuf,0,sizeof(rxbdbuf));
	bdp = rxbdbuf;
	while((long)bdp & 0xf)
		bdp++;
	rxbdtbl = (enet_bd_struct_t *)bdp;
	for(i=0;i<RXBUFTOT;i++) {
		bp = rxbuf[i];
		while((long)bp & 0xf)
			bp++;
		ENETTRACE(("&rxbdtbl[%d] = 0x%lx, buf=0x%lx\n",i,(long)&rxbdtbl[i],(long)bp));
		enet_hal_init_rxbds(&rxbdtbl[i], bp, i==RXBUFTOT-1);
#if SHOW_ENET_CONTROL
		showBd(&rxbdtbl[i]);
#endif
	}
	enet_hal_set_rxbd_address(ENETDEV,(uint32_t)rxbdtbl);

	memset(txbdbuf,0,sizeof(txbdbuf));
	bdp = txbdbuf;
	while((long)bdp & 0xf)
		bdp++;
	txbdtbl = (enet_bd_struct_t *)bdp;
	for(i=0;i<TXBUFTOT;i++) {
		bp = txbuf[i];
		while((long)bp & 0xf)
			bp++;
		txbuftbl[i] = bp;
		ENETTRACE(("&txbdtbl[%d] = 0x%lx, buf=0x%lx\n",i,(long)&txbdtbl[i],(long)bp));
		enet_hal_init_txbds(&txbdtbl[i], i==TXBUFTOT-1);
#if SHOW_ENET_CONTROL
		showBd(&txbdtbl[i]);
#endif
	}
	enet_hal_set_txbd_address(ENETDEV,(uint32_t)txbdtbl);

	/* Configure mii speed*/
    clock_manager_get_frequency(kSystemClock, &freq);
    enet_hal_config_mii(ENETDEV,(freq/(2*ENET_MII_CLOCK)+1),kEnetMdioHoldOneClkCycle,0);

	/* Enable Ethernet rx and tx interrupt*/
	enet_hal_config_interrupt(ENETDEV, (kEnetTxByteInterrupt | kEnetRxFrameInterrupt), 1);

	/* Enable Ethernet module*/
	enet_hal_config_ethernet(ENETDEV, USE_ENHANCED_FRAME, 1);

	/* Active Receive buffer descriptor must be done after module enable */
	enet_hal_active_rxbd(ENETDEV);

	/* Init phy: */
	if (phy_read(kEnetPhySR,&phyreg) == -1)
		return(-1);

	lastlinkstate = (((phyreg & kEnetPhyLinkStatus) == 0) ? 0:1);

	if (lastlinkstate == 0) {
		printf("Enet cable disconnected\n");
		return(0);
	}

	if ((phyreg & kEnetPhyAutoNegAble) && ((phyreg & kEnetPhyAutoNegComplete) == 0)) {
		struct elapsed_tmr tmr;

		if (phy_write(kEnetPhyCR,kEnetPhyAutoNeg) == -1)
			return(-1);

		startElapsedTimer(&tmr,3000);
		while(1) {
			if (phy_read(kEnetPhySR,&phyreg) == -1)
				return(-1);
			if (phyreg & kEnetPhyAutoNegComplete)
				break;
			if (msecElapsed(&tmr)) {
				printf("phy_init timeout\n");
				return(-1);
			}
		}
	}
	return(0);
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
}

void
enableMulticastReception(void)
{
}

/* disablePromiscuousReception():
 * Provide the code that disables promiscuous reception.
 */
void
disablePromiscuousReception(void)
{
	ENETTRACE(("Promiscuous=0\n"));
    enet_hal_config_promiscuous(ENETDEV, 0);
}

/* enablePromiscuousReception():
 * Provide the code that enables promiscuous reception.
 */
void
enablePromiscuousReception(void)
{
	ENETTRACE(("Promiscuous=1\n"));
    enet_hal_config_promiscuous(ENETDEV, 1);
}

/* disableBroadcastReception():
 * Referring to the description of the MACRCTL register in
 * the datasheet, I don't think this device allows broadcast
 * reception to be disabled.
 */
void
disableBroadcastReception(void)
{
	ENETTRACE(("BroadcastRcv=0\n"));
    enet_hal_enable_broadcastreject(ENETDEV,1);
}

/* enableBroadcastReception():
 * Referring to the description of the MACRCTL register in
 * the datasheet, I think this device has broadcast reception
 * enabled by default (with no option to disable it).
 */
void
enableBroadcastReception(void)
{
	ENETTRACE(("BroadcastRcv=1\n"));
    enet_hal_enable_broadcastreject(ENETDEV,0);
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
static unsigned char *nxtsndbfr;

unsigned char *
getXmitBuffer(void)
{
	nxtsndbfr = txbuftbl[txidx];
	//printf("getXmitBuffer(%d): 0x%lx\n",txidx,(long)nxtsndbfr);
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
	if (length < 64)
		length = 64;

#if INCLUDE_ETHERVERBOSE
	if (EtherVerbose &  SHOW_OUTGOING)
		printPkt((struct ether_header *)nxtsndbfr,length,ETHER_OUTGOING);
#endif

	enet_hal_update_txbds(&txbdtbl[txidx],nxtsndbfr,length,0);
	enet_hal_active_txbd(ENETDEV);

	if (++txidx >= TXBUFTOT)
		txidx = 0;

	EtherXFRAMECnt++;
	return(0);
}

/* DisableEtherdev():
 */
void
DisableEtherdev(void)
{
	enreset();
	clock_manager_set_gate(kClockModuleENET, 0U, false);
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
linkpoll(void)
{
	char linkstate;
	uint16_t stat;

	if (phy_read(kEnetPhySR,&stat) == -1)
		return(0);

	linkstate = (stat & kEnetPhyLinkStatus ? 1:0);

	if (linkstate != lastlinkstate) {
		if (linkstate) {
			// Link transitioned from down to up, so re-init...
			enreset();
			eninit();
		}
		printf("Link %s\n",linkstate ? "up":"dwn");
		lastlinkstate = linkstate;
	}
	return(lastlinkstate);
}

#if SHOW_ENET_CONTROL
static void
showBd(enet_bd_struct_t *bdp)
{
	printf("bd@0x%lx:\n",(long)bdp);
	printf(" length:  %d\n",htons((uint16_t)(bdp->length)));
	printf(" control: x%04x\n",enet_hal_get_rxbd_control(bdp));
	showCtrl(enet_hal_get_rxbd_control(bdp),"");
	printf(" buffer:  x%08lx\n",(long)enet_hal_get_bd_buffer(bdp));
	printf(" ctrlex0: x%04x\n",htons((uint16_t)(bdp->controlExtend0)));
	printf(" ctrlex1: x%04x\n",htons((uint16_t)(bdp->controlExtend1)));
	printf(" pldcsum: x%04x\n",htons((uint16_t)(bdp->payloadCheckSum)));
	printf(" hdrlen:  %d\n",bdp->headerLength);
	printf(" prot:    %d\n",bdp->protocalTyte);
}

static void
showCtrl(uint16_t ctrl,char *msg)
{
	printf("CTRL (%s): ",msg);
	if (ctrl & kEnetRxBdBroadCast)
		printf("kEnetRxBdBroadCast, ");
	if (ctrl & kEnetRxBdMultiCast)
		printf("kEnetRxBdMultiCast ");
	if (ctrl & kEnetRxBdLengthViolation)
		printf("kEnetRxBdLengthViolation ");
	if (ctrl & kEnetRxBdNoOctet)
		printf("kEnetRxBdNoOctet ");
	if (ctrl & kEnetRxBdCrc)
		printf("kEnetRxBdCrc ");
	if (ctrl & kEnetRxBdOverRun)
		printf("kEnetRxBdOverRun ");
	if (ctrl & kEnetRxBdTrunc)
		printf("kEnetRxBdTrunc ");
	if (ctrl & kEnetRxBdEmpty)
		printf("kEnetRxBdEmpty ");
	if (ctrl & kEnetRxBdRxSoftOwner1)
		printf("kEnetRxBdRxSoftOwner1 ");
	if (ctrl & kEnetRxBdWrap)
		printf("kEnetRxBdWrap ");
	if (ctrl & kEnetRxBdRxSoftOwner2)
		printf("kEnetRxBdRxSoftOwner2 ");
	if (ctrl & kEnetRxBdLast)
		printf("kEnetRxBdLast ");
	if (ctrl & kEnetRxBdMiss)
		printf("kEnetRxBdMiss ");
	printf("\n");
}
#endif

int
polletherdev(void)
{
	int pktcnt;
	uint16_t ctrl;
	enet_bd_struct_t *rbd;

	pktcnt = 0;

	if (linkpoll()) {
		rbd = &rxbdtbl[rxidx];
		if (((long)rbd & 0xf) != 0) {
			printf("misaligned bd!\n");
			return(0);
		}
		ctrl = enet_hal_get_rxbd_control(rbd);
		if (ctrl != lastctrl) {
#if SHOW_ENET_CONTROL
			showCtrl(ctrl,"1");
#endif
			lastctrl = ctrl;
		}
		if ((ctrl & kEnetRxBdEmpty) == 0) {
			if (++rxidx >= RXBUFTOT)
				rxidx = 0;
	
			processPACKET((struct ether_header *)enet_hal_get_bd_buffer(rbd),enet_hal_get_bd_length(rbd));
			enet_hal_update_rxbds(rbd,0,0);
			enet_hal_active_rxbd(ENETDEV);
//			*(vuint32_t *)0x400c0004 = *(vuint32_t *)0x400c0004;
//			*(vuint32_t *)0x400c0010 = 0x01000000;
			pktcnt++;
		}
	}
	return (pktcnt);
}

int
phy_read(int phyReg, uint16_t *value)
{
	struct elapsed_tmr tmr;

	if (!enet_hal_is_mii_enabled(ENETDEV)) {
		printf("phy_read: mii not enabled\n");
		return(-1);
	}
	/* Clear the MII interrupt event*/
	enet_hal_clear_interrupt(ENETDEV, kEnetMiiInterrupt);

	/* Read command operation*/
	enet_hal_set_mii_command(ENETDEV, PHYADDR, phyReg, kEnetReadValidFrame, 0);

	/* Poll for MII complete*/
	startElapsedTimer(&tmr,1000);
	while(!enet_hal_get_interrupt_status(ENETDEV, kEnetMiiInterrupt)) {
		if(msecElapsed(&tmr)) {
			printf("phy_read timeout\n");
			return(-1);
		}
	}
	
	/* Get data from mii register*/
    *value = enet_hal_get_mii_data(ENETDEV);

	/* Clear MII interrupt event*/
	enet_hal_clear_interrupt(ENETDEV, kEnetMiiInterrupt);
	return(0);
}

int
phy_write(int phyReg, uint16_t value)
{
	struct elapsed_tmr tmr;

    /* Check if the mii is enabled*/
    if (!enet_hal_is_mii_enabled(ENETDEV)) {
		printf("phy_write: mii not enabled\n");
		return(-1);
    }

    /* Clear the MII interrupt event*/
    enet_hal_clear_interrupt(ENETDEV, kEnetMiiInterrupt);

    /* Read command operation*/
    enet_hal_set_mii_command(ENETDEV, PHYADDR, phyReg, kEnetWriteValidFrame, value);

	/* Poll for MII complete*/
	startElapsedTimer(&tmr,1000);
	while(!enet_hal_get_interrupt_status(ENETDEV, kEnetMiiInterrupt)) {
		if(msecElapsed(&tmr)) {
			printf("phy_write timeout\n");
			return(-1);
		}
	}

    /* Clear MII intrrupt event*/
    enet_hal_clear_interrupt(ENETDEV, kEnetMiiInterrupt);
    return(0);
}

#endif
