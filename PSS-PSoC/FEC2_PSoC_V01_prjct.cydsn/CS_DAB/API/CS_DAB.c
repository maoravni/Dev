/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include<device.h>

/*******************/
/* Global variable */
/*******************/
uint8 `$INSTANCE_NAME`_initVar = 1;
uint8 `$INSTANCE_NAME`_CS_ON_Counter;

uint8 `$INSTANCE_NAME`_Main_DMA_Chan;
uint8 `$INSTANCE_NAME`_Peak_DMA_Chan;
uint8 `$INSTANCE_NAME`_Zero_DMA_Chan;
uint8 `$INSTANCE_NAME`_Store_Peak_DMA_Chan;
uint8 `$INSTANCE_NAME`_Store_Zero_DMA_Chan;

int16 `$INSTANCE_NAME`_Peak_Data_Buffer[`$INSTANCE_NAME`_SIZE_OF_PEAKS_DATA_BUFFER];
int16 `$INSTANCE_NAME`_Zero_Data_Buffer[`$INSTANCE_NAME`_SIZE_OF_ZERO_DATA_BUFFER];

int16 `$INSTANCE_NAME`_Peak_Data[`$INSTANCE_NAME`_SIZE_OF_PEAKS_DATA_BUFFER];
int16 `$INSTANCE_NAME`_Zero_Data[`$INSTANCE_NAME`_SIZE_OF_ZERO_DATA_BUFFER];

uint8 `$INSTANCE_NAME`_P_Data_Ready      = 0;
uint8 `$INSTANCE_NAME`_Z_Data_Ready      = 0;

CY_ISR(`$INSTANCE_NAME`_P_ISR_Done)
{
	`$INSTANCE_NAME`_P_Data_Ready = 1;
	`$INSTANCE_NAME`_CS_ON_Counter++;
}

CY_ISR(`$INSTANCE_NAME`_Z_ISR_Done)
{
	`$INSTANCE_NAME`_Z_Data_Ready = 1;
}

void `$INSTANCE_NAME`_Start()
{
	if(`$INSTANCE_NAME`_initVar)
	{
		`$INSTANCE_NAME`_P_Data_Ready = 0;
		`$INSTANCE_NAME`_Z_Data_Ready = 0;
		`$INSTANCE_NAME`_IsCurrent_WriteCounter(`$INSTANCE_NAME`_ZERO_LEVEL);
		`$INSTANCE_NAME`_IsCurrent_WriteCompare1(`$INSTANCE_NAME`_POS_DETECT_THRESHOLD);
		`$INSTANCE_NAME`_IsCurrent_WriteCompare2(`$INSTANCE_NAME`_NEG_DETECT_THRESHOLD);
		/* Start the "IsCurrent" PWM module */
	    `$INSTANCE_NAME`_IsCurrent_Start();
	    /* Start the NextMap Interrupt module */
		`$INSTANCE_NAME`_P_isr_Start();
		`$INSTANCE_NAME`_Z_isr_Start();
		/* Set the priority of that ISR */
		`$INSTANCE_NAME`_P_isr_SetPriority(1);
		`$INSTANCE_NAME`_Z_isr_SetPriority(2);
	    /* Give a name to the interrupt vector */
		`$INSTANCE_NAME`_P_isr_SetVector(`$INSTANCE_NAME`_P_ISR_Done);
	    `$INSTANCE_NAME`_Z_isr_SetVector(`$INSTANCE_NAME`_Z_ISR_Done);
		/* initialize the DMA's */
		`$INSTANCE_NAME`_DMAs_Config();
		`$INSTANCE_NAME`_initVar = 0;
	}
}

void `$INSTANCE_NAME`_Stop()
{   
	if(!`$INSTANCE_NAME`_initVar)
	{
	    `$INSTANCE_NAME`_IsCurrent_Stop();
		/* Will Disable al the DMA's Network for the CS_DAB */
		CyDmaChDisable(`$INSTANCE_NAME`_Main_DMA_Chan);
	}
}

void `$INSTANCE_NAME`_Calibrate(uint16 `$INSTANCE_NAME`_Zero_Level, uint16 `$INSTANCE_NAME`_Detect_Thrshld)
{
	`$INSTANCE_NAME`_IsCurrent_Stop();
	`$INSTANCE_NAME`_IsCurrent_WriteCounter(`$INSTANCE_NAME`_Zero_Level);
	`$INSTANCE_NAME`_IsCurrent_WriteCompare1(`$INSTANCE_NAME`_Zero_Level + `$INSTANCE_NAME`_Detect_Thrshld);
	`$INSTANCE_NAME`_IsCurrent_WriteCompare2(`$INSTANCE_NAME`_Zero_Level - `$INSTANCE_NAME`_Detect_Thrshld);
	`$INSTANCE_NAME`_IsCurrent_Start();
}

void `$INSTANCE_NAME`_DMAs_Config()
{
/* Main_DMA_Configuration 
***********************************************************************************************************************************************/
	uint8 `$INSTANCE_NAME`_Main_DMA_TD[1];	
	/* DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	Set upper source and destination address. */	
	`$INSTANCE_NAME`_Main_DMA_Chan = 
		`$INSTANCE_NAME`_Main_DMA_DmaInitialize(`$INSTANCE_NAME`_Main_DMA_BYTES_PER_BURST, `$INSTANCE_NAME`_Main_DMA_REQUEST_PER_BURST, 
	    HI16(`$INSTANCE_NAME`_Main_DMA_SRC_BASE), HI16(`$INSTANCE_NAME`_Main_DMA_DST_BASE));
	/* Allocate TD */	
	`$INSTANCE_NAME`_Main_DMA_TD[0] = CyDmaTdAllocate();
    /* Term out generated after the specified no. of bytes are transferred to memory.
	   Tranfer the specified number of samples and generate term out */	
	CyDmaTdSetConfiguration(`$INSTANCE_NAME`_Main_DMA_TD[0], 2, `$INSTANCE_NAME`_Main_DMA_TD[0],`$INSTANCE_NAME`_Main_DMA__TD_TERMOUT_EN);
	/* Set source and destination address */
	CyDmaTdSetAddress(	`$INSTANCE_NAME`_Main_DMA_TD[0], 
						LO16((uint32)SAR2_SAR_WRK0_PTR), 
						LO16((uint32)`$INSTANCE_NAME`_IsCurrent_COUNTER_LSB_PTR));
	/* Set the channel priority */
	CyDmaChPriority(`$INSTANCE_NAME`_Main_DMA_Chan, 1);
	/* Set the intial TD for the channel */
	CyDmaChSetInitialTd(`$INSTANCE_NAME`_Main_DMA_Chan, `$INSTANCE_NAME`_Main_DMA_TD[0]);
	/* Set Channel Priorty */
	CyDmaChPriority(`$INSTANCE_NAME`_Main_DMA_Chan, 1);
	/* Enable the DMA channel */
	CyDmaChEnable(`$INSTANCE_NAME`_Main_DMA_Chan, 1);
/***********************************************************************************************************************************************/
	
/* Peak_DMA_Configuration 
***********************************************************************************************************************************************/
	uint8 `$INSTANCE_NAME`_Peak_DMA_TD[1];	
	/* DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	Set upper source and destination address. */	
	`$INSTANCE_NAME`_Peak_DMA_Chan = 
		`$INSTANCE_NAME`_Peak_DMA_DmaInitialize(`$INSTANCE_NAME`_Peak_DMA_BYTES_PER_BURST, `$INSTANCE_NAME`_Peak_DMA_REQUEST_PER_BURST, 
	    HI16(`$INSTANCE_NAME`_Peak_DMA_SRC_BASE), HI16(`$INSTANCE_NAME`_Peak_DMA_DST_BASE));
	/* Allocate TD */	
	`$INSTANCE_NAME`_Peak_DMA_TD[0] = CyDmaTdAllocate();
    /* Term out generated after the specified no. of bytes are transferred to memory.
	   Tranfer the specified number of samples and generate term out */	
	CyDmaTdSetConfiguration(`$INSTANCE_NAME`_Peak_DMA_TD[0], sizeof(`$INSTANCE_NAME`_Peak_Data_Buffer), 
							`$INSTANCE_NAME`_Peak_DMA_TD[0],`$INSTANCE_NAME`_Peak_DMA__TD_TERMOUT_EN | TD_INC_DST_ADR);
	/* Set source and destination address */
	CyDmaTdSetAddress(`$INSTANCE_NAME`_Peak_DMA_TD[0], LO16((uint32)`$INSTANCE_NAME`_IsCurrent_COUNTER_LSB_PTR),
					  LO16((uint32)`$INSTANCE_NAME`_Peak_Data_Buffer));
	/* Set the channel priority */
	CyDmaChPriority(`$INSTANCE_NAME`_Peak_DMA_Chan, 1);
	/* Set the intial TD for the channel */
	CyDmaChSetInitialTd(`$INSTANCE_NAME`_Peak_DMA_Chan, `$INSTANCE_NAME`_Peak_DMA_TD[0]);
	/* Set Channel Priorty */
	CyDmaChPriority(`$INSTANCE_NAME`_Peak_DMA_Chan, 1);
	/* Enable the DMA channel */
	CyDmaChEnable(`$INSTANCE_NAME`_Peak_DMA_Chan, 1);
/***********************************************************************************************************************************************/
	
/* Zero_DMA_Configuration 
***********************************************************************************************************************************************/
	uint8 `$INSTANCE_NAME`_Zero_DMA_TD[1];	
	/* DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	Set upper source and destination address. */	
	`$INSTANCE_NAME`_Zero_DMA_Chan = 
		`$INSTANCE_NAME`_Zero_DMA_DmaInitialize(`$INSTANCE_NAME`_Zero_DMA_BYTES_PER_BURST, `$INSTANCE_NAME`_Zero_DMA_REQUEST_PER_BURST, 
	    HI16(`$INSTANCE_NAME`_Zero_DMA_SRC_BASE), HI16(`$INSTANCE_NAME`_Zero_DMA_DST_BASE));
	/* Allocate TD */	
	`$INSTANCE_NAME`_Zero_DMA_TD[0] = CyDmaTdAllocate();
    /* Term out generated after the specified no. of bytes are transferred to memory.
	   Tranfer the specified number of samples and generate term out */	
	CyDmaTdSetConfiguration(`$INSTANCE_NAME`_Zero_DMA_TD[0], sizeof(`$INSTANCE_NAME`_Zero_Data_Buffer),
							`$INSTANCE_NAME`_Zero_DMA_TD[0],
							`$INSTANCE_NAME`_Zero_DMA__TD_TERMOUT_EN | TD_INC_DST_ADR);
	/* Set source and destination address */
	CyDmaTdSetAddress(`$INSTANCE_NAME`_Zero_DMA_TD[0], LO16((uint32)`$INSTANCE_NAME`_IsCurrent_COUNTER_LSB_PTR),
					  LO16((uint32)`$INSTANCE_NAME`_Zero_Data_Buffer));
	/* Set the channel priority */
	CyDmaChPriority(`$INSTANCE_NAME`_Zero_DMA_Chan, 1);
	/* Set the intial TD for the channel */
	CyDmaChSetInitialTd(`$INSTANCE_NAME`_Zero_DMA_Chan, `$INSTANCE_NAME`_Zero_DMA_TD[0]);
	/* Set Channel Priorty */
	CyDmaChPriority(`$INSTANCE_NAME`_Zero_DMA_Chan, 2);
	/* Enable the DMA channel */
	CyDmaChEnable(`$INSTANCE_NAME`_Zero_DMA_Chan, 1);
/***********************************************************************************************************************************************/
	
/* Store_Peak_DMA_Configuration 
***********************************************************************************************************************************************/
	uint8 `$INSTANCE_NAME`_Store_Peak_DMA_TD[1];	
	/* DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	Set upper source and destination address. */	
	`$INSTANCE_NAME`_Store_Peak_DMA_Chan = 
		`$INSTANCE_NAME`_Store_Peak_DMA_DmaInitialize(`$INSTANCE_NAME`_Peak_DMA_BYTES_PER_BURST, 
		`$INSTANCE_NAME`_Store_Peak_DMA_REQUEST_PER_BURST, HI16(`$INSTANCE_NAME`_Store_Peak_DMA_SRC_BASE), 
		 HI16(`$INSTANCE_NAME`_Peak_DMA_DST_BASE));
	/* Allocate TD */	
	`$INSTANCE_NAME`_Store_Peak_DMA_TD[0] = CyDmaTdAllocate();
    /* Term out generated after the specified no. of bytes are transferred to memory.
	   Transfer the specified number of samples and generate term out */	
	CyDmaTdSetConfiguration(`$INSTANCE_NAME`_Store_Peak_DMA_TD[0], sizeof(`$INSTANCE_NAME`_Peak_Data_Buffer), 
							`$INSTANCE_NAME`_Store_Peak_DMA_TD[0],
							`$INSTANCE_NAME`_Store_Peak_DMA__TD_TERMOUT_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR);
	/* Set source and destination address */
	CyDmaTdSetAddress(`$INSTANCE_NAME`_Store_Peak_DMA_TD[0], LO16((uint32)`$INSTANCE_NAME`_Peak_Data_Buffer),
					  LO16((uint32)`$INSTANCE_NAME`_Peak_Data));
	/* Set the channel priority */
	CyDmaChPriority(`$INSTANCE_NAME`_Store_Peak_DMA_Chan, 1);
	/* Set the initial TD for the channel */
	CyDmaChSetInitialTd(`$INSTANCE_NAME`_Store_Peak_DMA_Chan, `$INSTANCE_NAME`_Store_Peak_DMA_TD[0]);
	/* Set Channel Priorty */
	CyDmaChPriority(`$INSTANCE_NAME`_Store_Peak_DMA_Chan, 2);
	/* Enable the DMA channel */
	CyDmaChEnable(`$INSTANCE_NAME`_Store_Peak_DMA_Chan, 1);
/***********************************************************************************************************************************************/
/* Store_Zero_DMA_Configuration 
***********************************************************************************************************************************************/
	uint8 `$INSTANCE_NAME`_Store_Zero_DMA_TD[1];	
	/* DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	Set upper source and destination address. */	
	`$INSTANCE_NAME`_Store_Zero_DMA_Chan = 
		`$INSTANCE_NAME`_Store_Zero_DMA_DmaInitialize(`$INSTANCE_NAME`_Zero_DMA_BYTES_PER_BURST, 
		`$INSTANCE_NAME`_Store_Zero_DMA_REQUEST_PER_BURST, HI16(`$INSTANCE_NAME`_Store_Zero_DMA_SRC_BASE), 
		 HI16(`$INSTANCE_NAME`_Zero_DMA_DST_BASE));
	/* Allocate TD */	
	`$INSTANCE_NAME`_Store_Zero_DMA_TD[0] = CyDmaTdAllocate();
    /* Term out generated after the specified no. of bytes are transferred to memory.
	   Transfer the specified number of samples and generate term out */	
	CyDmaTdSetConfiguration(`$INSTANCE_NAME`_Store_Zero_DMA_TD[0], sizeof(`$INSTANCE_NAME`_Zero_Data_Buffer), 
							`$INSTANCE_NAME`_Store_Zero_DMA_TD[0],
							`$INSTANCE_NAME`_Store_Zero_DMA__TD_TERMOUT_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR);
	/* Set source and destination address */
	CyDmaTdSetAddress(`$INSTANCE_NAME`_Store_Zero_DMA_TD[0], LO16((uint32)`$INSTANCE_NAME`_Zero_Data_Buffer),
					  LO16((uint32)`$INSTANCE_NAME`_Zero_Data));
	/* Set the channel priority */
	CyDmaChPriority(`$INSTANCE_NAME`_Store_Zero_DMA_Chan, 1);
	/* Set the initial TD for the channel */
	CyDmaChSetInitialTd(`$INSTANCE_NAME`_Store_Zero_DMA_Chan, `$INSTANCE_NAME`_Store_Zero_DMA_TD[0]);
	/* Set Channel Priorty */
	CyDmaChPriority(`$INSTANCE_NAME`_Store_Zero_DMA_Chan, 2);
	/* Enable the DMA channel */
	CyDmaChEnable(`$INSTANCE_NAME`_Store_Zero_DMA_Chan, 1);
/***********************************************************************************************************************************************/	
}

void `$INSTANCE_NAME`_Peak_DMA_Enable()
{
	/* Enable the DMA channel */
	CyDmaChEnable(`$INSTANCE_NAME`_Store_Peak_DMA_Chan, 1);	
}


/* [] END OF FILE */
