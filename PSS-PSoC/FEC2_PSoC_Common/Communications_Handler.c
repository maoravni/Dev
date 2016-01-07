/*****************************************************************************
* File Name: Communications_Handler.c
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
*****************************************************************************/

/***************************************
*    Includes
***************************************/
#include <device.h>
#include <stdio.h>

#include "System_Handler.h"
#include "Communications_Handler.h"
#include "Temperature_Module_Handler.h"
#include "Analog_IO_Module_Handler.h"
#include "Digital_IO_Module_Handler.h"
#include "Monitor_Module_Handler.h"
#include "main.h"
#include "version.h"

/***************************************
*    Global Variables
***************************************/
int8 SPI_Delay_Timer;
/* Variable declarations for DMA_Tx_S */
uint8 S_TxChannel;
uint8 S_TxTD[2u];
uint8 Tx_IntrpCntrl;

/* Variable declarations for DMA_Rx_S */
uint8 S_RxChannel;
uint8 S_RxTD[1u];	

uint8 SPIS_ErrorsBitField;

uint8 KeepAliveViolationTimer;
uint8 SPI_Session;

uint8 TemperatureWriteEnable;
uint8 CurrentsWriteEnable;

uint8 CS_WriteEnable;
uint8 IDsWriteEnable;

T_Tx_Buffer s_txBuffer;
T_Rx_Buffer s_rxBuffer;
/************************************************************/
// Pointers for CRC calculations //
/************************************************************/
uint8 * p_rxBuffer = &s_rxBuffer.Start;

extern T_DOD_Configuration DOD_Configuration;

/* Because  Tx Massage Structure is:
"
typedef struct 
{
	uint8 			Status;
    uint8 			SerialNumber;
	T_MessageData 	Data;
	uint8 			crc;
} T_Tx_Buffer;
"
the pointer must start from &s_txBuffer.Status
But when sending the pointer to the function:
"
s_txBuffer.crc = crc_8ATM(p_txBuffer, TX_TD_SIZE);
"
we want txBuffer to be:
"
typedef struct 
{
	uint8 			SOH;
	uint8 			Status;
    uint8 			SerialNumber;
	T_MessageData 	Data;
} T_Tx_Buffer;
"
than * p_txBuffer should equel (&s_txBuffer.Status)-1 , * p_txBuffer = SOH; 
and the function:
"
crc_8ATM(p_txBuffer, TX_TD_SIZE)
" 
should work for us.
*/
uint8 * p_txBuffer = (&s_txBuffer.Status)-1;
/********************************************************************************/
uint8 crc_Errors_Counter;

extern T_RTD_Configuration RTD_Configuration;
uint8 tempRequiredRTDConfiguration;

const uint8 crcTable[256] = {	
								0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
								0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
								0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
								0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
								0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
								0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
								0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
								0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
								0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
								0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
								0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
								0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
								0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
								0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
								0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
								0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
							};
////////////////////////////////////
//      S P I S     P O R T       //
////////////////////////////////////

/* Receiving ISR */
CY_ISR(SPIS_Rx_ISR)
{
	//SPIS_RxFlag = 1;
	uint8 savedIntrStatus;	
    /* Disable global interrupts */	
	savedIntrStatus = CyEnterCriticalSection();
	
	////////////////////////////////////////////////
	/*PSOC_TX_Write(1);*/ SPIS_RxParser(); /*PSOC_TX_Write(0);*/
	////////////////////////////////////////////
    /* Enable global interrupts */	
	CyExitCriticalSection(savedIntrStatus);		
}

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
void Init_SPIS(void)
{
	uint8 savedIntrStatus;
	    /* Disable global interrupts */	
	savedIntrStatus = CyEnterCriticalSection();
//////////////////////////////////////////////////////////////////////////////
/*							Enter Critical Section							*/
//////////////////////////////////////////////////////////////////////////////
	/* initiate the s_rxBuffer and s_txBuffer */
	memset(&s_rxBuffer, 0, sizeof(s_rxBuffer));
	memset(&s_txBuffer, 0, sizeof(s_rxBuffer));	
	crc_Errors_Counter = 0;
	/* Initiate write enable Flags 
	to enable write to the stored data structures for MMCU */
	TemperatureWriteEnable = 1;
	CurrentsWriteEnable = 1;
	CS_WriteEnable = 1;
	IDsWriteEnable = 1;
	SPI_Delay_Timer = SPI_DELAY_DISABLE;
	/* initiate the SPIS_Rx interrupt */
	isr_SPIS_Rx_Start();
	isr_SPIS_Rx_SetVector(SPIS_Rx_ISR);
	
			
	/* Configuration and enabling of DMA's */
    Dma_S_Tx_Configuration();
	//Dma_Tx_CRC_Configuration();
    Dma_S_Rx_Configuration();	

	/* initiate SPIS */		
    SPIS_Start();	
    /* Disable the TX interrupt of SPIS */
	SPIS_TX_STATUS_MASK_REG &= (~SPIS_INT_ON_TX_NOT_FULL );
    /* Take a copy of SPIS_TX_STATUS_MASK_REG which will be used to disable the TX interrupt using DMA */
	Tx_IntrpCntrl = SPIS_TX_STATUS_MASK_REG;
    /* Enable the TX interrupt line to trigger the DMA when FIFO is empty*/
	SPIS_TX_STATUS_MASK_REG |= (SPIS_INT_ON_TX_NOT_FULL );

	/* Initiate the keepalive flag */
    KeepAliveViolationTimer = 0;
	/* Initiate the keepalive flag */
	SPI_Session = 0;
	/* load first Tx byte to SPIS rx/tx register */
    SPIS_WriteTxDataZero(SOH);			
	/* Enable the SPIS DMA's*/
    CyDmaChEnable(S_TxChannel, 1u);
    CyDmaChEnable(S_RxChannel, 1u);	
//////////////////////////////////////////////////////////////////////////////
	/* Enable global interrupts */
	CyExitCriticalSection(savedIntrStatus);
}

void Init_AllDma()
{
    Dma_S_Tx_Configuration();
    Dma_S_Rx_Configuration();
}
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
void Dma_S_Tx_Configuration()
{
    /* Init DMA, 1 byte bursts, each burst requires a request */ 
    S_TxChannel = DMA_TX_S_DmaInitialize(DMA_TX_S_BYTES_PER_BURST, DMA_TX_S_REQUEST_PER_BURST, HI16(DMA_TX_S_SRC_BASE),
            HI16(DMA_TX_S_DST_BASE) );
	
    S_TxTD[0u] = CyDmaTdAllocate();
    S_TxTD[1u] = CyDmaTdAllocate();	
    /* Configure this Td chain as follows:
    *  - The TD is looping on itself
    *  - Increment the source address, but not the destination address   
    */
    CyDmaTdSetConfiguration(S_TxTD[0u], TX_TD_SIZE, S_TxTD[1u], TD_INC_SRC_ADR);
  	CyDmaTdSetConfiguration(S_TxTD[1u], 1, S_TxTD[0u], DMA_TX_S__TD_TERMOUT_EN);
    /* From the memory to the SPIS */
    CyDmaTdSetAddress(S_TxTD[0u], LO16((uint32)&s_txBuffer), LO16((uint32)SPIS_TXDATA_PTR) );
	
    CyDmaTdSetAddress(S_TxTD[1u], LO16((uint32)&Tx_IntrpCntrl), LO16((uint32)&SPIS_TX_STATUS_MASK_REG) );
    /* Associate the TD with the channel */
    CyDmaChSetInitialTd(S_TxChannel, S_TxTD[0u]);	
}      

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
void Dma_S_Rx_Configuration()
{ 
    /* Init DMA, 1 byte bursts, each burst requires a request */ 
    S_RxChannel = DMA_RX_S_DmaInitialize(DMA_RX_S_BYTES_PER_BURST, DMA_RX_S_REQUEST_PER_BURST, HI16(DMA_RX_S_SRC_BASE),
            HI16(DMA_RX_S_DST_BASE) );

    S_RxTD[0u] = CyDmaTdAllocate();
    /* Configure this Td as follows:
    *  - The TD is looping on itself
    *  - Increment the destination address, but not the source address
    */
    CyDmaTdSetConfiguration(S_RxTD[0u], RX_TD_SIZE, /*S_RxTD[0u]*/CY_DMA_DISABLE_TD, DMA_RX_S__TD_TERMOUT_EN | TD_INC_DST_ADR);
    /* From the SPIS to the memory */
    CyDmaTdSetAddress(S_RxTD[0u], LO16((uint32)SPIS_RXDATA_PTR), LO16((uint32)&s_rxBuffer) );
    /* Associate the TD with the channel */
    CyDmaChSetInitialTd(S_RxChannel, S_RxTD[0u]);
}

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
void SPIS_Recover(void)
{
	uint8 savedIntrStatus;	
    /* Disable global interrupts */	
	savedIntrStatus = CyEnterCriticalSection();	
	/* stop the  SPIS and CRC's */
	SPIS_Stop();
	/* Force the DMA's to the begining of the TD chaines and disable them */
    CyDmaChSetRequest(S_TxChannel, CY_DMA_CPU_TERM_CHAIN );
    //CyDmaChSetRequest(DMA_Tx_CRC_Channel, CY_DMA_CPU_TERM_CHAIN );
    CyDmaChSetRequest(S_RxChannel, CY_DMA_CPU_TERM_CHAIN );
	/* Enable the SPIS DMA's*/
    CyDmaChEnable(S_TxChannel, 1u);
    //CyDmaChEnable(DMA_Tx_CRC_Channel, 1u);
    CyDmaChEnable(S_RxChannel, 1u);	
	/* Reset the SPIS bit counter */	
	/* start the  SPIS and CRC's */
	SPIS_Start();
	/* write first byte to the Tx register */
	SPIS_WriteTxDataZero(SOH);	
    /* Enable the TX interrupt line to trigger the DMA when FIFO is empty*/SPIS_TX_STATUS_MASK_REG |=
            (SPIS_INT_ON_TX_NOT_FULL );
	/* Enable global interrupts */
	CyExitCriticalSection(savedIntrStatus);		
}

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
void SPIS_Comm_Enable(void)
{
    uint8 enableInterrupts;
    /* Disable global interrupts */	
	enableInterrupts = CyEnterCriticalSection();
	
	/* Clear the Tx buffer */
	SPIS_ClearTxBuffer();
	/* Clear the Rx buffer */
	SPIS_ClearRxBuffer();
	/* Clear the FIFO's */
	SPIS_ClearFIFO();		
	/* Enable the SPIS DMA's*/
    CyDmaChEnable(S_RxChannel, 1u);	
	/* Enable the  SPIS and CRC's */
	SPIS_Enable();
	/* write first byte to the Tx register */
	SPIS_WriteTxDataZero(SOH);	
    /* Enable the TX interrupt line to trigger the DMA when FIFO is empty*/
	SPIS_TX_STATUS_MASK_REG |= (SPIS_INT_ON_TX_NOT_FULL );
	/* Disable the delay count */
	SPI_Delay_Timer = SPI_DELAY_DISABLE;
	// PSOC_RX_Write(0); 
	
	/* Enable global interrupts */
    CyExitCriticalSection(enableInterrupts);	
}

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
void SPIS_Comm_Disable(void)
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();
	/* Reset the bit counter of the SPI */
	SPIS_COUNTER_CONTROL_REG &= (uint8)~SPIS_CNTR_ENABLE;
	/* Disable Rx Tx Shist registers */
    SPIS_TX_STATUS_ACTL_REG &= (uint8)~SPIS_INT_ENABLE;
    SPIS_RX_STATUS_ACTL_REG &= (uint8)~SPIS_INT_ENABLE;
	/* Triger the 2-3ms delay timer */
	SPI_Delay_Timer = SPI_DELAY;
	
    CyExitCriticalSection(enableInterrupts);	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SPIS RxParser  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SPIS_RxParser(void)
{
	SPI_Session = 1;
	s_txBuffer.Status = E_Success;
	
	/* Calculate the CRC of the Rx message */
	/************************************************************/
	uint8 Message_CRC = crc_8ATM(p_rxBuffer,(RX_TD_SIZE-1));
	/************************************************************/
	
	if (s_rxBuffer.Start == SOH)
	{
        if (Message_CRC == s_rxBuffer.CRC)
		{			
			ExecuteSPISCommand();					
		}
		else
		{
			GREEN_LED_Write(0);
			s_txBuffer.Status |= E_InvalidCRC;
			crc_Errors_Counter++;
		}
	}
	else
	{ 
		GREEN_LED_Write(0);
        if (Message_CRC != s_rxBuffer.CRC)
		{
			s_txBuffer.Status |= E_InvalidCRC;
			crc_Errors_Counter++;
		}		
		s_txBuffer.Status |= E_InvalidSOH;				
	}
	
	/* Calculate the CRC of the Tx message */
	/************************************************************/
	* p_txBuffer = SOH;
	s_txBuffer.crc = crc_8ATM(p_txBuffer, TX_TD_SIZE);
	/************************************************************/
	/* Disable the SPIS module for 2-3ms */	
	SPIS_Comm_Disable(); /*PSOC_RX_Write(1);*/
}

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
uint8 crc_8ATM(uint8 const message[], int nBytes)
{
    uint8 data;
    uint8 remainder = 0;

    // Divide the message by the polynomial, a byte at a time.
	int byte;
    for(byte = 0; byte < nBytes; ++byte)
    {
		uint8 check = (8 * sizeof(uint8)) - 8 ;
        data = message[byte] ^ (remainder >> (check));
        remainder = crcTable[data] ^ (remainder << 8);
    }
     // The final remainder is the CRC.
    return (remainder);
}   /* crcFast() */

/*******************************************************************************
* Function Name: void ExecuteSPISCommand()
********************************************************************************
* Summary:
*   
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void ExecuteSPISCommand()
{
	/* Lower the Keep A live flag */
	KeepAliveViolationTimer = 0;
	s_txBuffer.SerialNumber = s_rxBuffer.SerialNumber;
	
	switch (s_rxBuffer.Command)
	{
/* 
		1. General Messages 
********************************************************************/
		
		//////////////////////
		// 1.1 EmptyCommand //
		//////////////////////
		case E_EmptyCommand:
		{
			Ex_EmptyCommand();
			break;
		}
		////////////////////////
		// 1.2 GetPSoCversion //
		////////////////////////
		case E_GetPSoCversion:
		{
			Ex_GetPSoCversion();
			break;
		}
		///////////////////
		// 1.3 KeepAlive //
		///////////////////
		case E_KeepAlive:
		{
			Ex_KeepAlive();
			break;
		}	
		/////////////////////
		// 1.4  GetCableID //
		/////////////////////
		case E_GetCableID:
		{
        	Ex_GetCableID();
			break;
		}		
		///////////////////
		// 1.5 GetStatus //
		///////////////////
		case E_GetStatus:
		{
			Ex_GetStatus();
			break;
		}
		///////////////////////////
		// 1.6 StartBoardConfig  //
		///////////////////////////
		case E_StartBoardConfig:
		{
			Ex_StartBoardConfig();
			break;
		}
		/////////////////////////
		// 1.7 EndBoardConfig  //
		/////////////////////////
		case E_EndBoardConfig:
		{
			Ex_EndBoardConfig();
			break;
		}			
		///////////////////////
		// 1.8 SetErrorsMask //
		///////////////////////
		case E_SetErrorsMask:
		{
			Ex_SetErrorsMask();
			break;
		}		
		/////////////////////////
		// 1.9 SetWarningsMask //
		/////////////////////////
		case E_SetWarningsMask:
		{
			Ex_SetWarningsMask();
			break;
		}
		////////////////////
		// 1.10 GetErrors //
		////////////////////
		case E_GetErrors:
		{
			Ex_GetErrors();
			break;
		}
		//////////////////////
		// 1.11 GetWarnings //
		//////////////////////
		case E_GetWarnings:
		{
			Ex_GetWarnings();
			break;
		}
/********************************************************************/
		
/* 
		2. Configuration Messages 
********************************************************************/			
		/////////////////////////////////
		// 2.1 ConfigTemperatureSensor //
		/////////////////////////////////
		case E_ConfigTemperatureSensor:
		{
			Ex_ConfigTemperatureSensor();
			break;
		}
		///////////////////////////
		// 2.2 ConfigCurrentSensor //
		///////////////////////////
		case E_ConfigCurrentSensor:
		{
			Ex_ConfigCurrentSensor();
			break;
		}
		/////////////////////////
		// 2.3 ConfigDigitalIO //
		/////////////////////////
		case E_ConfigDigitalIO:
		{
			Ex_ConfigDigitalIO();
			break;
		}	
		////////////////////////
		// 2.4 ConfigAnalogIO //
		////////////////////////
		case E_ConfigAnalogIO:
		{
			Ex_ConfigAnalogIO();
			break;
		}
		/////////////////////////
		// 2.5 ConfigChannelType //
		/////////////////////////		
		case E_ConfigChannelType:
		{
			Ex_ConfigChannelType();
			break;
		}
/********************************************************************/
		
/* 
		3. I/O Operation Messages 
********************************************************************/		
		//////////////////////////
		// 3.1 ReadSingleDevice //
		//////////////////////////
		case E_ReadSingleDevice:
		{
			Ex_ReadSingleDevice();
			break;
		}
		/////////////////////////////
		//  3.2 WriteSingleDevice  //
		/////////////////////////////
		case E_WriteSingleDevice:
		{
			Ex_WriteSingleDevice();
			break;
		}
		///////////////////////////////
		//  3.3 WriteMultipleDevice  //
		///////////////////////////////
		case E_WriteMultipleDevice:
		{
			Ex_WriteMultipleDevice();
			break;
		}		
		///////////////////////////
		//  3.4 GetDeviceErrors  //
		//////////////////////////
		case E_GetDeviceErrors:
		{
			Ex_GetDeviceErrors();
			break;
		}	
		/////////////////////////////
		//  3.5 GetDeviceWarnings  //
		/////////////////////////////
		case E_GetDeviceWarnings:
		{
			Ex_GetDeviceWarnings();
			break;
		}	
		//////////////////////////////////
		//  3.6 ReadTemperatureSensors  //
		//////////////////////////////////
		case E_ReadTemperatureSensors:
		{
			Ex_ReadTemperatureSensors();
			break;
		}	
		////////////////////
		//  3.7 ReadPWMs  //
		////////////////////
		case E_ReadPWMs:
		{
			Ex_ReadPWMs();
			break;
		}	
		////////////////////////////
		//  3.8 ReadAnalogInputs  //
		////////////////////////////
		case E_ReadAnalogInputs:
		{
			Ex_ReadAnalogInputs();
			break;
		}	
		///////////////////////////////
		//  3.9 ReadBridgeCurrents  //
		///////////////////////////////
		case E_ReadBridgeCurrents:
		{
			Ex_ReadBridgeCurrents();
			break;
		}
		////////////////////////////
		//  3.10 ReadCurrentSensor //
		////////////////////////////
		case E_ReadCurrentSensor:
		{
			Ex_ReadCurrentSensor();
			break;
		}
		//////////////////////////////
		//  3.11 ReadDigitalInputs  //
		//////////////////////////////
		case E_ReadDigitalInputs:
		{
			Ex_ReadDigitalInputs();
			break;
		}			

        ////////////////////////////////////////////////
        //  3.12 Read Temperature, Pwm and DI sensors //
        ////////////////////////////////////////////////
	    case E_ReadTemperaturePwmDISensors:
	    {
	        Ex_ReadTemperaturePwmDISensors();
	        break;
	    }
		//////////////////////////////////
		//  3.13 ReadTemperaturePacket  //
		//////////////////////////////////
		case E_ReadTemperaturePacket:
		{
			Ex_ReadTemperaturePacket();
			break;
		}
		///////////////////////////////////////
		//  3.14 ReadAnalogsFeedbacksErrors  //
		///////////////////////////////////////		
		case E_ReadAnalogsFeedbacksErrors:
		{
		    Ex_ReadAnalogsFeedbacksErrors();
		    break;
		}
/********************************************************************/
		
/* 
		4. System Operation Messages 
********************************************************************/		
			
		//////////////////////////
		//  4.1 ResetBoardToON  //
		//////////////////////////
		case E_ResetBoardToON:
		{
			Ex_ResetBoardToON();
			break;
		}		
		//////////////////////////////////
		//  4.2 StartPSoCErrorRecovery  //
		//////////////////////////////////
		case E_StartPSoCErrorRecovery:
		{
			Ex_StartPSoCErrorRecovery();
			break;
		}
		/////////////////////
		//  4.3 ResetPSoC  //
		/////////////////////
		case E_ResetPSoC:
		{
			Ex_ResetPSoC();
			break;
		}		
		///////////////////////////
		//  4.4 StartBootloader  //
		///////////////////////////
		case E_StartBootloader:
		{
			Ex_StartBootloader();
			break;
		}
/********************************************************************/
		
		default:
		{
			/* command not supported by ICD */
			s_txBuffer.Status |= E_InvalidCommand;
			break;
		}
	}	
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*   
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/

/* 
		1. General Messages 
********************************************************************/
		
//////////////////////
// 1.1 EmptyCommand //
//////////////////////
void Ex_EmptyCommand(void)
{
	memcpy(&s_txBuffer.Data, &s_rxBuffer.Data, sizeof(T_MessageData));
}
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////
// 1.2 GetPSoCversion //
////////////////////////
void Ex_GetPSoCversion(void)
{
	s_txBuffer.Data.DataBytes_uint16[0] = M_FEC_FIRMWARE_VERSION_MAJOR;
	s_txBuffer.Data.DataBytes_uint16[1] = M_FEC_FIRMWARE_VERSION_MINOR;
	s_txBuffer.Data.DataBytes_uint16[2] = M_FEC_FIRMWARE_VERSION_BUILD;
	s_txBuffer.Data.DataBytes_uint16[3] = M_FEC_FIRMWARE_VERSION_REVISION;
	
	s_txBuffer.Data.DataBytes_uint16[4] = M_ICD_VERSION_MAJOR;
	s_txBuffer.Data.DataBytes_uint16[5] = M_ICD_VERSION_MINOR;
	s_txBuffer.Data.DataBytes_uint16[6] = M_ICD_VERSION_BUILD;
	s_txBuffer.Data.DataBytes_uint16[7] = M_ICD_VERSION_REVISION;
}
/////////////////////////////////////////////////////////////////////////////////////

///////////////////
// 1.3 KeepAlive //
///////////////////
void Ex_KeepAlive(void)
{
	// Nothing to do.
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////
// 1.4  GetCableID //
/////////////////////
void Ex_GetCableID(void)
{
	#ifdef FEC2_PSoC_V01
		Vsupply = s_rxBuffer.Data.DataBytes_float[0];
	#endif
	Cable_Count_To_ID();
	
	/* Disable write to StoredIDs4MMCU */
	IDsWriteEnable = 0;
	memcpy(	s_txBuffer.Data.GetCableIDReply.CableIDs,
			StoredIDs4MMCU.ID,
			sizeof(StoredIDs4MMCU.ID)
	);	 
	s_txBuffer.Data.GetCableIDReply.StatusBits = StoredIDs4MMCU.StatusBits;
	/* Enable write to StoredIDs4MMCU */
	IDsWriteEnable = 1;
}
//////////////////////////////////////////////////////////////////////////////////

///////////////////
// 1.5 GetStatus //
///////////////////
void Ex_GetStatus()
{
	s_txBuffer.Data.DataBytes_uint16[0] = SystemState.SystemStatus;
}
//////////////////////////////////////////////////////////////////////////////////

///////////////////////////
// 1.6 StartBoardConfig  //
///////////////////////////
void Ex_StartBoardConfig()
{
	isr_LogicFunc_Disable();
	/* Start the DSM conversion */
	//DSM_Sequencer_Disable();
	/* Stop the SAR1 and conversion */
//	#ifdef FEC2_PSoC_V01
//		SAR1_soc_Disable();
//	#endif
	//SAR2_soc_Disable();
	/* Disable the Current sensors */
	//CSAB_1_Stop();
	/* Update system Warrnings*/
	SystemState.SystemStatus |= UNCONFIG_SYS_STS;
	System_Status_Update();
    /*Initiate RequiredRTDsBitfield */
	RequiredRTDsBitfield = 0x01;
	tempRequiredRTDConfiguration = 0x01;
	ConnectedRTDsBitfield = 0x00;
	CS_Required = 0;
    SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM, DISABLE, _16BIT1);
    SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM, DISABLE, _16BIT2);
	// Maor: Don't reset the variable, as this will cause the filters to reset
	// and issue an incorrect error:
	//CalcTempr_InitVar = 0;
}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////
// 1.7 EndBoardConfig  //
/////////////////////////
void Ex_EndBoardConfig()
{
    RequiredRTDsBitfield = tempRequiredRTDConfiguration;

    /* Start the DSM conversion */
    DSM_Sequencer_Enable();
	switch(GET_BOARD_TYPE)
	{
		case E_PsocPrimaryFunction_Heater:
		{
			/* Start the current sensors */
			#ifdef FEC2_PSoC_V01
						CS_DAB_1_Start();
						CS_DAB_2_Start();
			#endif
			#ifdef FEC2_PSoC_V02
						CSAB_1_Start();
			#endif
						// for the ITH, we need to have the SPWM option. So if the output is not configured to one of the
						// options, reconfigure it to random PWM.
						if (!(DOD_Configuration.SlowPWM1 == 0 || DOD_Configuration.SlowPWM1 == 1))
						    DOD_OutConfig(0, 0);
			break;
		}
		case E_PsocPrimaryFunction_LiftPbOnError:
		case E_PsocPrimaryFunction_LiftPbOnErrorGen2:
		{
//			/* Start the DSM conversion */
//			DSM_Sequencer_Enable();
			/* Start SAR1 for analog current inputs */
			#ifdef FEC2_PSoC_V01
				SAR1_soc_Enable();
			#endif
			break;
		}
	#ifdef FEC2_PSoC_V02
		case E_PsocPrimaryFunction_HwValidation:
		{
			/* Start the DSM conversion */
//			DSM_Sequencer_Enable();
			/* Start SAR1 for analog current inputs */
			//SAR1_soc_Enable();
			/* Enable the Current Sensor module */
			CSAB_1_Start();
			/* Configure the System Automatically */
			/* Configure the Temperature Module */
			memset(TempUpLimit,400,sizeof(TempUpLimit));
			RequiredRTDsBitfield = 0xFF;
			/* Configure the Monitor Module */
			memset(MaxCSLimit,2048,sizeof(MaxCSLimit));
			/* Configure the Digital Outputs */
			uint8 DigIng;
			for(DigIng=0;DigIng<6;DigIng++)
			{
				/* Digital Outputs in ON/OFF Mode */
				DOD_OutConfig(DigIng,0x03);
			}
			/* Enable the Heat Output */
			Heat_Output_Enable();
			SystemState.SystemStatus = 0;
			break;	
		}
	#endif	
		case E_PsocPrimaryFunction_SoftDebug:
		{
			/* Start the DSM conversion */
//			DSM_Sequencer_Enable();

			/* Start SAR1 for analog current inputs */
			#ifdef FEC2_PSoC_V01
				SAR1_soc_Enable();
			#endif
			
			/* Anable the current sensors */
			#ifdef FEC2_PSoC_V01
				if(CS_Required & 0x01)
				{
					CS_DAB_1_Start();
				}
				if(CS_Required & 0x02)
				{
					CS_DAB_2_Start();
				}
			#endif
			#ifdef FEC2_PSoC_V02
				if(CS_Required)
				{
					CSAB_1_Start();	
				}
			#endif
			break;
		}
		case E_PsocPrimaryFunction_Applicator:
		{
			/* Config DO_4 as PWM output */
			//DOD_OutConfig(/*Channel*/0x03, /*PWM Configuration*/0x02);
			//StoredPwmValues[3] = DOD_PWMOut2_Write(0);
			
			/* Define RTD 1 as required */
			RequiredRTDsBitfield = 0x03;
			/* Start the DSM conversion */
//			DSM_Sequencer_Enable();
			/* Start SAR1 for analog current inputs */
			#ifdef FEC2_PSoC_V01
				SAR1_soc_Enable();
			#endif
		}
		default:
		{
			break;	
		}
	}

	/* Update system Status */
	SystemState.SystemStatus &= (~UNCONFIG_SYS_STS);
	System_Status_Update();
	isr_LogicFunc_Enable();

    // clean all current errors.
    //Ex_StartPSoCErrorRecovery();

	// lower the safety output, which outputs the dry contact.
    Safety_Relay_TurnOFF();
}
//////////////////////////////////////////////////////////////////////////////////

///////////////////////
// 1.8 SetErrorsMask //
///////////////////////
void Ex_SetErrorsMask(void)
{
	SystemState.SystemErrorsMask = s_rxBuffer.Data.DataBytes_uint16[0];
}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////
// 1.9 SetWarningsMask //
/////////////////////////
void Ex_SetWarningsMask(void)
{
	SystemState.SystemWarnings = s_rxBuffer.Data.DataBytes_uint16[0];
}
//////////////////////////////////////////////////////////////////////////////////

////////////////////
// 1.10 GetErrors //
////////////////////
void Ex_GetErrors(void)
{
	s_txBuffer.Data.DataBytes_uint16[0] = SystemState.SystemErrors;
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////
// 1.11 GetWarnings //
//////////////////////
void Ex_GetWarnings(void)
{
	s_txBuffer.Data.DataBytes_uint16[0] = SystemState.SystemWarnings;
}
//////////////////////////////////////////////////////////////////////////////////

/* 
		2. Configuration Messages 
********************************************************************/

/////////////////////////////////
// 2.1 ConfigTemperatureSensor //
/////////////////////////////////
void Ex_ConfigTemperatureSensor(void)
{
	uint8 i = s_rxBuffer.Data.ConfigTemperatureSensor.ChannelIndex;
	
    switch (s_rxBuffer.Data.ConfigTemperatureSensor.SensorType)
	{
		case E_PT100: 
		{
			RTD_Configuration.ChannelType = E_PT100;
			break;
		}
		case E_PT500: 
		{
			RTD_Configuration.ChannelType = E_PT500;
			break;
		}
		case E_PT1000: 
		{
			RTD_Configuration.ChannelType = E_PT1000;
			break;
		}
		case E_Liquid_Detector: 
		{
			RTD_Configuration.ChannelType = E_Liquid_Detector;
			break;
		}
		default: 
		{
			s_txBuffer.Status |= E_InvalidType;
			break;
		}
	}
    switch (s_rxBuffer.Data.ConfigTemperatureSensor.FilterType)
	{
		case E_SinglePoleIIR: 
		{
			RTD_Configuration.FilterType = E_SinglePoleIIR; 
			break;
		}
		case E_MovingAverage: 
		{
			RTD_Configuration.FilterType = E_MovingAverage;
			break;
		}
		case E_GroupAverage: 
		{
			RTD_Configuration.FilterType = E_GroupAverage;
			break;
		}
		default: 
		{
			s_txBuffer.Status |= E_InvalidType;
			break;
		}				
	}
	if(i < 4)
	{
		if(RTD_Configuration.ChannelType == E_Liquid_Detector)
		{
			LiquidDetectorTimeOut[i] = (int8) s_rxBuffer.Data.ConfigTemperatureSensor.Gain_LiquidDetectTimer;
			if(LiquidDetectorTimeOut[i] < 2)
			{
				LiquidDetectorTimeOut[i] = 2;
				s_txBuffer.Status |= E_InvalidValue;
			}
			if(LiquidDetectorTimeOut[i] > LIQUID_DRIP_MAX_TIME)
			{
				LiquidDetectorTimeOut[i] = LIQUID_DRIP_MAX_TIME;
				s_txBuffer.Status |= E_InvalidValue;
			}
		}
		else
		{
			TempUpLimit[i] = s_rxBuffer.Data.ConfigTemperatureSensor.TempUpLimit;
		}
		/* Enable the temperature sensor channel */
//		RequiredRTDsBitfield |= Ind2Bit_5[ (i) + 1 ];
        tempRequiredRTDConfiguration |= Ind2Bit_5[ (i) + 1 ];
		/* Enable the Heat Output */
		Heat_Output_Enable();
	}
	else
	{
		s_txBuffer.Status |= E_InvalidDeviceIndex;	
	}
}
//////////////////////////////////////////////////////////////////////////////////


///////////////////////////
// 2.2 ConfigCurrentSensor //
///////////////////////////
void Ex_ConfigCurrentSensor(void)
{	
	switch(s_rxBuffer.Data.ConfigCurrentSensor.ChannelIndex)
	{
		case 0:
		{
			MinCSLimit[0] = s_rxBuffer.Data.ConfigCurrentSensor.CurrentLowerLimit;
			MaxCSLimit[0] = s_rxBuffer.Data.ConfigCurrentSensor.CurrentUpperLimit;
			CS_Required |= 0x01;
			SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM, DISABLE, _16BIT2);
			break;	
		}
		case 1:
		{
			MinCSLimit[1] = s_rxBuffer.Data.ConfigCurrentSensor.CurrentLowerLimit;
			MaxCSLimit[1] = s_rxBuffer.Data.ConfigCurrentSensor.CurrentUpperLimit;
			CS_Required |= 0x02;
			SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM, DISABLE, _16BIT2);			
			break;		
		}
		default:
		{
			s_txBuffer.Status |= E_InvalidDeviceIndex;	
			break;	
		}		
	}

}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////
// 2.3 ConfigDigitalIO //
/////////////////////////
void Ex_ConfigDigitalIO(void)
{
    // This function configures the actual hardware/firmware behavior.
	switch(DOD_OutConfig(s_rxBuffer.Data.ConfigDigitalIO.ChannelIndex, s_rxBuffer.Data.ConfigDigitalIO.ChannelType))
	{
		case 1:
		{
			s_txBuffer.Status |= E_InvalidDeviceIndex;
			break;	
		}
		case 2:
		{
			s_txBuffer.Status |= E_InvalidDevice;
			break;	
		}		
	}
	
	// we now need to configure the SW soft start:
	ConfigureSoftStartParameters(s_rxBuffer.Data.ConfigDigitalIO.ChannelIndex,
	        s_rxBuffer.Data.ConfigDigitalIO.RiseStepSize,
	        s_rxBuffer.Data.ConfigDigitalIO.FallStepSize,
	        s_rxBuffer.Data.ConfigDigitalIO.RiseStepTickCount,
	        s_rxBuffer.Data.ConfigDigitalIO.FallStepTickCount);
}
//////////////////////////////////////////////////////////////////////////////////

////////////////////////
// 2.4 ConfigAnalogIO //
////////////////////////
void Ex_ConfigAnalogIO(void)
{
	switch(s_rxBuffer.Data.ConfigAnalogIO.ChannelIndex)
	{
		case 0:
		{
			MinAnalogInputLimit[0]	= s_rxBuffer.Data.ConfigAnalogIO.CurrentLowerLimit;
			MaxAnalogInputLimit[0] = s_rxBuffer.Data.ConfigAnalogIO.CurrentUpperLimit;
			break;	
		}
		case 1:
		{
			MinAnalogInputLimit[1]	= s_rxBuffer.Data.ConfigAnalogIO.CurrentLowerLimit;
			MaxAnalogInputLimit[1] = s_rxBuffer.Data.ConfigAnalogIO.CurrentUpperLimit;
			break;	
		}
		default:
		{
			s_txBuffer.Status |= E_InvalidDeviceIndex;
			break;	
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////

///////////////////////////
// 2.5 ConfigChannelType //
///////////////////////////
void Ex_ConfigChannelType(void)
{
    if (s_rxBuffer.Data.ConfigPrimaryFunction.PrimaryFunction >= E_PsocPrimaryFunction_Size)
    	s_txBuffer.Status |= E_InvalidValue;
    else
	    Board_Type_Sel_Reg_Write(s_rxBuffer.Data.ConfigPrimaryFunction.PrimaryFunction);
}
//////////////////////////////////////////////////////////////////////////////////////////

/* 
		3. I/O Operation Messages 
********************************************************************/		
//////////////////////////
// 3.1 ReadSingleDevice //
//////////////////////////
void Ex_ReadSingleDevice(void)
{
	switch (s_rxBuffer.Data.ReadSingleDevice.Device)	
	{
		case E_TemperatureSensor:
		{
        	if (s_rxBuffer.Data.ReadSingleDevice.ChannelIndex > (NUM_OF_TEMP_SENS - 1))
			{
				s_txBuffer.Status |= E_InvalidDeviceIndex;
			}
			else
			{
				/* Disable write to StoredTemps4MMCU */
				TemperatureWriteEnable = 0;
				/* load the s_txBuffer with temperature data */
	            s_txBuffer.Data.ReadSingleDeviceReply.Value.TemperatureSensor = StoredTemps4MMCU.temperature[s_rxBuffer.Data.ReadSingleDevice.ChannelIndex];
	            s_txBuffer.Data.ReadSingleDeviceReply.SensorsState = StoredTemps4MMCU.statusBits;
				/* Enable write to StoredTemps4MMCU */
				TemperatureWriteEnable = 1;
			}
			break;
		}
		
		case E_AnalogInput:
		{
        	if (s_rxBuffer.Data.ReadSingleDevice.ChannelIndex > (NUM_OF_ANALOG_INPUTS - 1))
			{
				s_txBuffer.Status |= E_InvalidDeviceIndex;
			}
			else
			{
				/* Disable write to StoredCurrents4MMCU */
				CurrentsWriteEnable = 0;
				/* load the s_txBuffer with temperature data */
            	s_txBuffer.Data.ReadSingleDeviceReply.Value.AnalogInput[0] = StoredAnalogIn4MMCU.AnalogInputs[s_rxBuffer.Data.ReadSingleDevice.ChannelIndex];
            	s_txBuffer.Data.ReadSingleDeviceReply.SensorsState = StoredAnalogIn4MMCU.StatusBits;
				/* Enable write to StoredTemps4MMCU */
				CurrentsWriteEnable = 1;
			}
			break;
		}	
		case E_ThermistorInput:
		{
        	if (s_rxBuffer.Data.ReadSingleDevice.ChannelIndex > 0)
			{
				s_txBuffer.Status |= E_InvalidDeviceIndex;
			}
			else
			{
				/* load the s_txBuffer with temperature data */

				/* Enable write to StoredTemps4MMCU */
			}
			break;
		}	
		
		case E_CurrentSensor:
		{
        	if (s_rxBuffer.Data.ReadSingleDevice.ChannelIndex > (NUM_OF_CS_SENS - 1))
			{
				s_txBuffer.Status |= E_InvalidDeviceIndex;
			}
			else
			{
				/* Disable write to StoredCS4MMCU */
				CS_WriteEnable = 0;
				/* load the s_txBuffer with temperature data */
            	s_txBuffer.Data.ReadSingleDeviceReply.Value.CurrentSensor[0] = StoredCS4MMCU.Peak[s_rxBuffer.Data.ReadSingleDevice.ChannelIndex];
            	s_txBuffer.Data.ReadSingleDeviceReply.SensorsState = StoredCS4MMCU.StatusBits;
				/* Enable write to StoredCS4MMCU */
				CS_WriteEnable = 1;
			}
			break;
		}
		
		case E_CableIDs:
		{
        	if (s_rxBuffer.Data.ReadSingleDevice.ChannelIndex > (NUM_OF_IDs_SENS - 1))
			{
				s_txBuffer.Status |= E_InvalidDeviceIndex;
			}
			else
			{
				/* Disable write to StoredIDs4MMCU */
				IDsWriteEnable = 0;
				/* load the s_txBuffer with temperature data */
            	s_txBuffer.Data.ReadSingleDeviceReply.Value.CableID[0] = StoredIDs4MMCU.ID[s_rxBuffer.Data.ReadSingleDevice.ChannelIndex];
				/* Sensor state */
            	s_txBuffer.Data.ReadSingleDeviceReply.SensorsState = StoredIDs4MMCU.StatusBits;
				/* Enable write to StoredIDs4MMCU */
				IDsWriteEnable = 1;
			}
			break;			
		}
		
		case E_DigitalInputs:
		{
        	if (s_rxBuffer.Data.ReadSingleDevice.ChannelIndex > 0)
			{
				s_txBuffer.Status |= E_InvalidDeviceIndex;
			}
			else
			{
				/* load the s_txBuffer with temperature data */
            	s_txBuffer.Data.ReadSingleDeviceReply.Value.DigitalInputs[0] = StoredDigiIn4MMCU.Byte;
				/* Sensor state */
            	s_txBuffer.Data.ReadSingleDeviceReply.SensorsState = 0;
			}		
			break;
		}
    default:
		{
			s_txBuffer.Status |= E_InvalidDevice;
			break;
		}				
	}	
}
///////////////////////////////////////////////////////////////////////////////////

/////////////////////////////
//  3.2 WriteSingleDevice  //
/////////////////////////////
void Ex_WriteSingleDevice(void)
{
	
	switch (s_rxBuffer.Data.WriteSingleDevice.Device)
	{
		case E_AnalogOutput:
		{
			switch (s_rxBuffer.Data.WriteSingleDevice.ChannelIndex)
			{
				case 0:
				{
                    AnalogOutput_Write(s_rxBuffer.Data.WriteSingleDevice.Value, 1);
//					VDAC1_SetValue(s_rxBuffer.Data.WriteSingleDevice.Value);
					break;
				}
				case 1:
				{
                    AnalogOutput_Write(s_rxBuffer.Data.WriteSingleDevice.Value, 2);
//					VDAC2_SetValue(s_rxBuffer.Data.WriteSingleDevice.Value);
					break;
				}
				default:
				{
					s_txBuffer.Status |= E_InvalidDeviceIndex;
					break;
				}				
			}
			break;
		}
		case E_DigitalOutputs:
		    DigitalOutputsWrite(s_rxBuffer.Data.WriteSingleDevice.ChannelIndex, s_rxBuffer.Data.WriteSingleDevice.Value);
		default:
		{
			s_txBuffer.Status |= E_InvalidDevice;
			break;
		}				
	}
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////
//  3.3 WriteMultipleDevice  //
///////////////////////////////
void Ex_WriteMultipleDevice(void)
{
	if(SystemState.SystemStatus & OUTPUTS_DISABLE_STS_MASK)
	{	
	    int i;
		#ifdef FEC2_PSoC_V01
			memset(CS_Always_OFF_Counter,0,sizeof(CS_Always_OFF_Counter));
		#endif
		#ifdef FEC2_PSoC_V02
			CS_Always_OFF_Counter = 0;
		#endif
		AnalogOutput_Write(0, 1);
		AnalogOutput_Write(0, 2);

		for (i = 0; i < NUM_OF_PWM_OUTS; ++i)
		{
		    DigitalOutputsForceWrite(i, 0);
		    DigitalOutputsWrite(i, 0);
		}
//		CurrentPwmValues[0] = DOD_SlowPWM1_Write(0);
//		CurrentPwmValues[1] = DOD_SlowPWM2_Write(0);
//		CurrentPwmValues[2] = DOD_PWMOut1_Write(0);
//		CurrentPwmValues[3] = DOD_PWMOut2_Write(0);
//		CurrentPwmValues[4] = DOD_HB_PWMOut1_Write(0);
//		CurrentPwmValues[5] = DOD_HB_PWMOut2_Write(0);
	}
	else
	{
		uint8 WriteTo = s_rxBuffer.Data.WriteMultipleDevice.WriteBitfield;
		
		if( WriteTo & E_AnalogInput1)
		{
	    	AnalogOutput_Write(s_rxBuffer.Data.WriteMultipleDevice.AnalogOutputs[0], 1);
		}
		if( WriteTo & E_AnalogInput2 )
		{
			AnalogOutput_Write(s_rxBuffer.Data.WriteMultipleDevice.AnalogOutputs[1], 2);
		}
		if( WriteTo & E_SlowPWM_Output1)
		{
		    DigitalOutputsWrite(0, s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[0]);
//			if(SystemState.SystemErrors & TEMP_MDL_ERR_MASK)
//			{
//				CurrentPwmValues[0] = DOD_SlowPWM1_Write(0);
//				#ifdef FEC2_PSoC_V01
//					memset(CS_Always_OFF_Counter,0,sizeof(CS_Always_OFF_Counter));
//				#endif
//				#ifdef FEC2_PSoC_V02
//					CS_Always_OFF_Counter = 0;
//				#endif
//			}
//			else
//			{
//				CurrentPwmValues[0] = DOD_SlowPWM1_Write(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[0]);
//				if(!CurrentPwmValues[0])
//				{
//					#ifdef FEC2_PSoC_V01
//						memset(CS_Always_OFF_Counter,0,sizeof(CS_Always_OFF_Counter));
//					#endif
//					#ifdef FEC2_PSoC_V02
//						CS_Always_OFF_Counter = 0;
//					#endif
//				}
//			}
		}
		if( WriteTo & E_SlowPWM_Output2 )
		{
		    DigitalOutputsWrite(1, s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[1]);
//			CurrentPwmValues[1] = DOD_SlowPWM2_Write(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[1]);
		}
		if( WriteTo & E_PWM_Output1 )
		{
            DigitalOutputsWrite(2, s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[2]);
//			CurrentPwmValues[2] = DOD_PWMOut1_Write(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[2]);
		}
		if( WriteTo & E_PWM_Output2 )
		{	
            DigitalOutputsWrite(3, s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[3]);
//			if(GET_BOARD_TYPE == APPLICATOR)
//			{
//              ApplicatorEnable(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[3]);
//			}
//			else
//			{
//				CurrentPwmValues[3] = DOD_PWMOut2_Write(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[3]);
//			}
		}
		if( WriteTo & E_HB_PWM_Output1 )
		{
            DigitalOutputsWrite(4, s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[4]);
//			CurrentPwmValues[4] = DOD_HB_PWMOut1_Write(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[4]);
		}
		if( WriteTo & E_HB_PWM_Output2 )
		{
            DigitalOutputsWrite(5, s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[5]);
//			CurrentPwmValues[5] = DOD_HB_PWMOut2_Write(s_rxBuffer.Data.WriteMultipleDevice.DigitalOutput[5]);
		}		
	}

}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////
//  3.4 GetDeviceErrors  //
///////////////////////////
void Ex_GetDeviceErrors(void)
{

}
///////////////////////////////////////////////////////////////////////////////////

/////////////////////////////
//  3.5 GetDeviceWarnings  //
/////////////////////////////
void Ex_GetDeviceWarnings(void)
{
	
}

//////////////////////////////////
//  3.6 ReadTemperatureSensors  //
//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Ex_ReadTemperatureSensors(void)
{
	/* Disable write to StoredTemps4MMCU */
	TemperatureWriteEnable = 0;
	/* load the s_txBuffer with temperature data */
    memcpy(s_txBuffer.Data.ReadTemperatureSensors.temperatureSensor,StoredTemps4MMCU.temperature,sizeof(StoredTemps4MMCU.temperature));
    s_txBuffer.Data.ReadTemperatureSensors.StatusBits = StoredTemps4MMCU.statusBits;
	/* Enable write to StoredTemps4MMCU */
	TemperatureWriteEnable = 1;	
	
}
///////////////////////////////////////////////////////////////////////////////////

////////////////////
//  3.7 ReadPWMs  //
////////////////////

void Ex_ReadPWMs(void)
{
	memcpy(s_txBuffer.Data.ReadPWMs.OutputChannel,CurrentPwmValues,sizeof(CurrentPwmValues));
	s_txBuffer.Data.ReadPWMs.StatusBits = 0;
}


///////////////////////////////////////////////////////////////////////////////////

////////////////////////////
//  3.8 ReadAnalogInputs  //
////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void Ex_ReadAnalogInputs(void)
{
	/* Disable write to StoredTemps4MMCU */
	CurrentsWriteEnable = 0;
	/* load the s_txBuffer with the data */
	memcpy(s_txBuffer.Data.ReadAnalogInputs.AnalogInputs,StoredAnalogIn4MMCU.AnalogInputs,sizeof(StoredAnalogIn4MMCU.AnalogInputs));
    s_txBuffer.Data.ReadAnalogInputs.StatusBits = StoredAnalogIn4MMCU.StatusBits;
	s_txBuffer.Data.ReadAnalogInputs.ThermistorInput = ThermistorInput;
	/* Enable write to StoredTemps4MMCU */
	CurrentsWriteEnable = 1;		
	
}
///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////
//  3.9 ReadBridgeCurrents  //
//////////////////////////////
void Ex_ReadBridgeCurrents(void)
{

}


////////////////////////////////////////////////////////////////////////////////////////////
void Ex_ReadCurrentSensor(void)
{
	memcpy(	s_txBuffer.Data.ReadCurrentSensor.PeakCurrent,
			StoredCS4MMCU.Peak,
			sizeof(StoredCS4MMCU.Peak)
	);	
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.HeatCntrlPulseCount = Read_HCP_Counter();
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.CurrentSensorStatusBits = StoredCS4MMCU.StatusBits;
}

////////////////////////////////////////////////////////////////////////////////////////////
void Ex_ReadDigitalInputs(void)
{
	s_txBuffer.Data.ReadDigitalInputs.DigitalInputsData.DigitalInputsWord = StoredDigiIn4MMCU.Byte;
	s_txBuffer.Data.ReadDigitalInputs.StatusBits = 0;
}
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Ex_ReadTemperaturePwmDISensors(void)
{
	#ifdef FEC2_PSoC_V01
		Vsupply = s_rxBuffer.Data.DataBytes_float[0];
	#endif
    /* Disable write to StoredTemps4MMCU */
    TemperatureWriteEnable = 0;
    /* load the s_txBuffer with temperature data */
	memcpy(	s_txBuffer.Data.ReadTemperaturePwmDISensors.temperatureSensors,
			StoredTemps4MMCU.temperature,
			sizeof(StoredTemps4MMCU.temperature)
	);	s_txBuffer.Data.ReadTemperaturePwmDISensors.temperatureStatusBits = StoredTemps4MMCU.statusBits; 

    /* Enable write to StoredTemps4MMCU */
    TemperatureWriteEnable = 1;
	// read pwms
	memcpy(	s_txBuffer.Data.ReadTemperaturePwmDISensors.pwmOutputs,
			CurrentPwmValues,
			sizeof(CurrentPwmValues)
	);	s_txBuffer.Data.ReadTemperaturePwmDISensors.pwmStatusBits = 0; 
	// Read digital inputs
	s_txBuffer.Data.ReadTemperaturePwmDISensors.DigitalInputBits = StoredDigiIn4MMCU.Byte;
    // system status
    s_txBuffer.Data.ReadTemperaturePwmDISensors.systemStatus = SystemState.SystemStatus;
}

////////////////////////////////////////////////////////////////////////////////////////////
void Ex_ReadTemperaturePacket(void)
{
	#ifdef FEC2_PSoC_V01
		Vsupply = s_rxBuffer.Data.DataBytes_float[0];
	#endif
	/* Disable write to StoredIDs4MMCU */
	IDsWriteEnable = 0;	
	/* Disable write to StoredCS4MMCU */
	CS_WriteEnable = 0;
	/* Disable write to StoredTemps4MMCU */
	TemperatureWriteEnable = 0;
	/* Read Temperature of the first PT100 */
	s_txBuffer.Data.ReadTemperaturePacket.temperatureSensors[0] = StoredTemps4MMCU.temperature[0];
	s_txBuffer.Data.ReadTemperaturePacket.temperatureSensors[1] = StoredTemps4MMCU.temperature[1];
	s_txBuffer.Data.ReadTemperaturePacket.temperatureSensors[2] = StoredTemps4MMCU.temperature[3];
	
	s_txBuffer.Data.ReadTemperaturePacket.PeakCurrent[0] = StoredCS4MMCU.Peak[0];
	s_txBuffer.Data.ReadTemperaturePacket.PeakCurrent[1] = StoredCS4MMCU.Peak[1];
	
	s_txBuffer.Data.ReadTemperaturePacket.CS_Counter = Read_HCP_Counter();
	
	s_txBuffer.Data.ReadTemperaturePacket.SystemStatus = SystemState.SystemStatus;
	s_txBuffer.Data.ReadTemperaturePacket.SystemWarnings = SystemState.SystemWarnings;
	s_txBuffer.Data.ReadTemperaturePacket.SystemErrors = SystemState.SystemErrors;
	
	s_txBuffer.Data.ReadTemperaturePacket.SlowPWM1 = CurrentPwmValues[0];
	
	s_txBuffer.Data.ReadTemperaturePacket.InputCableID1 = StoredIDs4MMCU.ID[0];
	
	/* Enable write to StoredIDs4MMCU */
	IDsWriteEnable = 1;	
	/* Enable write to StoredIDs4MMCU */
	CS_WriteEnable = 1;
	/* Enable write to StoredTemps4MMCU */
	TemperatureWriteEnable = 1;		
}

//////////////////////////////////////////////////////////////////////////////////////////////
void Ex_ReadAnalogsFeedbacksErrors(void)
{
	#ifdef FEC2_PSoC_V01
		Vsupply = s_rxBuffer.Data.DataBytes_float[0];
	#endif
	memcpy(	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.analogIn,
			StoredAnalogIn4MMCU.AnalogInputs,
			sizeof(StoredAnalogIn4MMCU.AnalogInputs)
	);	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.analogInStatusBits = StoredAnalogIn4MMCU.StatusBits;
		
	memcpy(	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.PeakCurrent,
			StoredCS4MMCU.Peak,
			sizeof(StoredCS4MMCU.Peak)
	);	
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.HeatCntrlPulseCount = Read_HCP_Counter();
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.CurrentSensorStatusBits = StoredCS4MMCU.StatusBits;	
	
    s_txBuffer.Data.ReadAnalogsFeedbacksErrors.SystemWarnings = SystemState.SystemWarnings;
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.SystemStatus = SystemState.SystemStatus;
    s_txBuffer.Data.ReadAnalogsFeedbacksErrors.SystemErrors = SystemState.SystemErrors;
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.analogOut[0] = StoredAnalogOut[0];
	s_txBuffer.Data.ReadAnalogsFeedbacksErrors.analogOut[1] = StoredAnalogOut[1];
	//s_txBuffer.Data.ReadAnalogsFeedbacksErrors.crcErrorsCount = crc_Errors_Counter;
}
//////////////////////////////////////////////////////////////////////////////////////////
void Ex_ResetBoardToON(void)
{
	/* initiate all system */
	SystemState.SystemStatus |= (NO_INIT_STS | UNCONFIG_SYS_STS);
	Init();
		
}
/////////////////////////////////////////////////////////////////////////////////////////

void Ex_StartPSoCErrorRecovery(void)
{
	/* Clear the System Initiate bit status*/
//	SystemState.SystemStatus = 0x08;
	/* Update system Errors */
	memset(ErrRegBitActvnArry,0,sizeof(ErrRegBitActvnArry));
	/* Update system warnings  */
	memset(WrnRegBitActvnArry,0,sizeof(WrnRegBitActvnArry));
	//CSAB_1_Reset();			
	CS_State = CS_INITIAL_STATE;
	/* Clear the Current Always ON error */
	SystemErrorReg_Write(SSR_ALWAYS_ON_ERR_BIT_NUM, DISABLE, _16BIT1);

	System_Status_Update();
	PBIT_Enable();
	ApplicatorEnable(FALSE);

	#ifdef FEC2_PSoC_V01
		CS_DAB_1_CS_ON_Counter = 0;
		CS_DAB_2_CS_ON_Counter = 0;	
		memset(CS_Always_OFF_Counter,0,sizeof(CS_Always_OFF_Counter));
	#endif
	#ifdef FEC2_PSoC_V02
		CSAB_1_Start();
		CS_Always_OFF_Counter = 0;
		CS_Always_ON_Counter = 0;
	#endif
	isr_LogicFunc_Start();
	
	/*clear ErrorFlashCode */
	ErrorCode(0,0,1);		

    Safety_Relay_TurnOFF();

}

///////////////////////////////////////////////////////////////////////////////////
void Ex_ResetPSoC(void)
{

}

///////////////////////////////////////////////////////////////////////////////////
void Ex_StartBootloader(void)
{
    Bootloadable_Load();
}

///////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////
//      U A R T     P O R T       //
////////////////////////////////////

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
/*
void Init_UART(void)
{	
    UART_Start();
    // delay for the UART to initialize 
    CyDelay(1LU); // msec 
    UART_PutString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    UART_PutString("*********************************\r\n");
    UART_PutString("Welcome to The PSoC Interface\r\n");
    UART_PutString("*********************************\r\n");
	
    UART_PutString("*********************************\r\n");
    UART_PutString("Top Menue\r\n");
    UART_PutString("*********************************\r\n");
	UART_PutString("    'M' - Top Menue\r");
    UART_PutString("    'T' - Temperature Handler\r");
    UART_PutString("    'D' - Digital Outputs Handler\r");
	UART_PutString("    'S' - SPI Com. Veiw\r");
    UART_PutString("    'C' - Current Loop\r");					
    UART_PutString("    'N' - New Page\r");                    
    UART_PutString("\n");
    UART_PutString("*********************************\r\n");    
}
*/

/* [] END OF FILE */
