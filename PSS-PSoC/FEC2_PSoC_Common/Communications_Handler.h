/*****************************************************************************
* File Name: Communications_Handler.h
*
* Version: 02.02
*
* Description:
*   This file has the following tasks:
*   1. Handle the SPI between the PSoC(Slave) and the MMCU.
*   2. Handle the DPMI between the PSoC and the MMCU.
*   3. Handle the UART UI. 
*
* Note:
*   MMCU- Master Micro Controler Unit.
*   DPMI- Direct PSoC to MMCU Interface.
*
* Components Summary:
*   1. SAR2_AMux- MUXing the input voltages.
*   2. SAR2_Flwr- An input buffer.
*   3. SAR2- ADC_SAR.
*   4. PeakEvent_1- Outputs a pulse when a peak in 
                    the input voltage detected.
* Fnctions Summary:
*   1. void Init_UART(void)- Sets up the UART for operation and sends out a message.
*   2. void ExecuteCommand(char thisCommand)- Executes a command based on an input ASCII character. 
*	3. Dma_S_Tx_Configuration(void)
*	4. Dma_S_Rx_Configuration(void)
*
********************************************************************************/

#include "device.h"
#include "ICDMessageStructs.h"


/***************************************
*    Defines
***************************************/
#define TX_TD_SIZE   31u
#define RX_TD_SIZE   32u

/* DMA Configuration for DMA_TX_S */
#define DMA_TX_S_BYTES_PER_BURST       (1u)
#define DMA_TX_S_REQUEST_PER_BURST     (1u)
#define DMA_TX_S_SRC_BASE              (CYDEV_SRAM_BASE)
#define DMA_TX_S_DST_BASE              (CYDEV_PERIPH_BASE)

/* DMA Configuration for DMA_RX_S */
#define DMA_RX_S_BYTES_PER_BURST       (1u)
#define DMA_RX_S_REQUEST_PER_BURST     (1u)
#define DMA_RX_S_SRC_BASE              (CYDEV_PERIPH_BASE)
#define DMA_RX_S_DST_BASE              (CYDEV_SRAM_BASE)

/* UART Definitions */
#define		SPI_DELAY			2
#define		SPI_DELAY_DISABLE	(-1)
#define 	NUMBER_OF_PT100     4

/* SPIS Definitions */
#define		SPI_RX_BUFFER_SIZE		0xFF
#define		RX_TEST_SIZE		0x8
#define		NO_DATA_PACKET_SIZE	5u // {SOH}{CMD/STATUS}{LNG}{Checksum}{ETB}

#define 	SOH					0x01

/***************************************
*    Extern Variables
***************************************/
extern uint8 S_RxChannel;
extern uint8 S_TxChannel;
extern int8  SPI_Delay_Timer;
extern uint8 TemperatureWriteEnable;
extern uint8 CS_WriteEnable;
extern uint8 IDsWriteEnable;
extern uint8 CurrentsWriteEnable;
/* Make Sure that PSoC receives messages every 2 seconds */
extern uint8 KeepAliveViolationTimer;
extern uint8 SPI_Session;

extern uint8 ApplicatorDripValveEnable;
/***************************************
*    Function Prototypes
***************************************/

/*******************************************************************************
* Function Name: Init_SPIS()
********************************************************************************
* Summary:
*   Sets up the SPIS for operation and sends out a message to MMCU.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void Init_SPIS(void);

/*******************************************************************************
* Function Name: SPIS_Recover()
********************************************************************************
* Summary:
*   Sets up the SPIS for operation and sends out a message to MMCU.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void SPIS_Recover(void);

/*******************************************************************************
* Function Name: SPIS_Comm_Enable()
********************************************************************************
* Summary:
*   Sets up the SPIS for operation and sends out a message to MMCU.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void SPIS_Comm_Enable(void);

/*******************************************************************************
* Function Name: SPIS_Comm_Disable()
********************************************************************************
* Summary:
*   Sets up the SPIS for operation and sends out a message to MMCU.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void SPIS_Comm_Disable(void);

/*******************************************************************************
* Function Name: Dma_M_Tx_Configuration
********************************************************************************
* Summary:
*  Configures the DMA transfer for TX direction
*   
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Dma_S_Tx_Configuration(void);


/*******************************************************************************
* Function Name: Dma_S_Rx_Configuration
********************************************************************************
* Summary:
*  Configures the DMA transfer for RX direction
*   
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Dma_S_Rx_Configuration(void);

/*******************************************************************************
* Function Name: ()
********************************************************************************
* Summary:
*   Sets up the SPIS for operation and sends out a message to MMCU.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void SPIS_RxParser(void);

/*******************************************************************************
* Function Name: uint8 crc_8ATM(uint8 const message[], int nBytes)
********************************************************************************
* Summary:
*   clculates the CRC of a message data array.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 crc_8ATM(uint8 const message[], int nBytes);

/*******************************************************************************
* Function Name: ExecuteSpiCommand()
********************************************************************************
* Summary:
*   Sets up the SPIS for operation and sends out a message to MMCU.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void ExecuteSPISCommand(void);

/*******************************************************************************
* Function Name: Init_UART()
********************************************************************************
* Summary:
*   Sets up the UART for operation and sends out a message.
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void Init_UART(void);

/*******************************************************************************
* Function Name: ExecuteCommand()
********************************************************************************
* Summary:
*   Executes a command based on an input byte.  The input byte is expected to
*   be an ASCII character.
*
* Parameters:
*   thisCommand: the command byte
*
* Return:
*   none
*******************************************************************************/
void ExecuteCommand(char thisCommand);

/*******************************************************************************
* Function Name: Dma_S_Tx_Configuration()
********************************************************************************
* Summary:
*   
*
* Parameters:
*
* Return:
*  
* Notes:
*
*******************************************************************************/
void Dma_S_Rx_Configuration(void);

/*******************************************************************************
* Function Name: Dma_S_Rx_Configuration()
********************************************************************************
* Summary:
*   
*
* Parameters:
*
* Return:
*  
* Notes:
*
*******************************************************************************/
void Dma_S_Rx_Configuration(void);


/* [] END OF FILE */



/* A code to generate an CRC code for al posiable byte per polynomial */

/*
#define GP  0x107   // x^8 + x^2 + x + 1 
#define DI  0x07


static unsigned char crc8_table[256];     // 8-bit table 
static int made_table=0;

static void init_crc8()
  ///
{
	int i,j;
	unsigned char crc;

	if (!made_table) {
	 for (i=0; i<256; i++) {
	   crc = i;
	   for (j=0; j<8; j++)
	     crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
	   crc8_table[i] = crc & 0xFF;
	   // printf("table[%d] = %d (0x%X)\n", i, crc, crc); 
	 }
	 made_table=1;
	}
}
*/