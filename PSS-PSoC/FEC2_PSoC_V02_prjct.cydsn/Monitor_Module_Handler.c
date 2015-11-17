/*****************************************************************************
* File Name: Analog_Outputs_Handler.h
*
* Version: 02.02
*
* Description:
*   This file has the following tasks:
*   1. handle the MUXing of the analog inputs to SAR2 which includes:
*       a. The voltage across a current to voltage converting resistor,
in order to measure the current in the SSR's.
b. The voltage across the connector ID resistors9
in order to identify which connector was connected to the FEC and PSoC.
*   2. Finding the peak value of the currents in the SSR's measured as described above.
*
* Note:
*   voltages coding of connector ID's- TBD.
*
* Note:
*
*****************************************************************************/

/***************************************
*    Includes
***************************************/

#include "main.h"
#include "Monitor_Module_Handler.h"
#include "Communications_Handler.h"
#include "System_Handler.h"
#include "Digital_IO_Module_Handler.h"
#include "DSP.h"

/***************************************
*    Global Variables
***************************************/
/* Variable declarations for DMA .
*  These variables are defined as global variables to avoid "may be used before being set" warning
*  issued by the PSoC 5 compilers  MDK/RVDS.In this case these variables are automatically initialized to zero */

uint8  MntrMdlInitVar;


uint8 	SAR2_IDs_DMADone_flag;
uint8   Peak_Event_Flag;
uint8   Zero_Event_Flag;

uint8 	SAR2_IDs_DataWriteEnable;

uint8  InitStateCycles;

uint8   CS_State;
uint8   CS_Required;
uint8 	CS_OverCurrent_EventCounter;
uint16	MinCSLimit[2];
uint16	MaxCSLimit[2];

uint16 Init_ZeroLevel;
int16  Init_ZeroLevel_Buffer[NUM_OF_CS_INIT_CYCLES];
uint16 ZeroLevel;
int16 PeaksResults[2];


int16 SAR2_IDs_Sample[NO_OF_TOTAL_SAR2_IDs_SAMPLES] = { 0 };
int16 SAR2_IDs_SampleLatch[NO_OF_TOTAL_SAR2_IDs_SAMPLES] = { 0 };

uint8 CableID_ViolationQualifier;

uint16 Bounds4ID[35] = {
	88, 232, 345, 459, 576, 691, 804, 917, 1028,
	1144, 1256, 1360, 1469, 1583, 1701, 1821,
	1942, 2064, 2186, 2306, 2424, 2540, 2653,
	2762, 2876, 2991, 3110, 3229, 3350, 3468,
	3580, 3697, 3833, 3969, 4096
};
uint32 FiltCountID[NUM_OF_IDs_SENS];
float Vsupply;

T_StoredCS4MMCU		StoredCS4MMCU;

T_StoredIDs4MMCU	StoredIDs4MMCU;

/*******************************************************************************
* Function Name: Init_Monitor_Module()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Init_Monitor_Module(void)
{
	InitStateCycles = 0;
	CS_Required = 0;
	Heat_Cmd_Counter = 0;
	
	CS_Always_OFF_Counter = 0;
	CS_Always_ON_Counter = 0;
	
	Peak_Event_Flag = 0;
	Zero_Event_Flag = 0;
	/* reset CS parameters */
	CS_State = CS_INITIAL_STATE;
	CS_OverCurrent_EventCounter = 0;
	
	ZeroLevel = 2048;
	memset(Init_ZeroLevel_Buffer,0,sizeof(Init_ZeroLevel_Buffer));
	
	memset(MinCSLimit,2048,sizeof(MinCSLimit));
	memset(MaxCSLimit,0,sizeof(MaxCSLimit));
	
	SAR2_Flwr_Start();
	/* Start the SAR2 */
	SAR2_Start();
	/* Disable the ADC ISR2 as it is not required */
	SAR2_IRQ_Disable();
	/* Relocate the ISRs of the Monitor Module */
	Monitor_ISRs_Relocate();
	/* Initiate the DMA module */
	SAR2_IDs_DMA_Config();
	/* Lower the  DMADone flag */
	SAR2_IDs_DMADone_flag = 0;
	/* Reset the SAR2 Cable ID's data buffer */
	memset(SAR2_IDs_Sample, 0, sizeof(SAR2_IDs_Sample));

	/* Reset the stored current loop data for the MMCU */
	memset(&StoredCS4MMCU, 0, sizeof(StoredCS4MMCU));
	/* Reset the stored Cable ID's data for the MMCU */
	memset(&StoredIDs4MMCU, 0, sizeof(StoredIDs4MMCU));
	/* Allow writting data into SAR2_CS_SampleLatch */
	SAR2_IDs_DataWriteEnable = 1;
	/* Cable ID Qualifier */
	CableID_ViolationQualifier = 0;

	/* Start the SAR2 conversion */
	SAR2_soc_Enable();
}

/*******************************************************************************
* Function Name:  SAR2_IDs_DMA_Config()
********************************************************************************
*
* Summary:
*    Code generated by the DMA Wizard.
*
*   DMA_Config() performs following functions:
*  1: Initializes the DMA channel
*  2: Allocates TraCH0_ERRORnsfer Descriptors
*  3: Configures the TD.
*  4: Sets source and destination address for this TD.
*  5: Initializes the TD.
*  6: Enables the DMA channel.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SAR2_IDs_DMA_Config()
{
	uint8 	SAR2_IDs_DMA_Chan;
	uint8 	SAR2_IDs_DMA_TD[1];
	// DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	// Set upper source and destination address. //	
	SAR2_IDs_DMA_Chan = SAR2_IDs_DMA_DmaInitialize(SAR2_IDs_DMA_BYTES_PER_BURST, SAR2_IDs_DMA_REQUEST_PER_BURST,
		HI16(SAR2_IDs_DMA_SRC_BASE), HI16(SAR2_IDs_DMA_DST_BASE));
	// Allocate TD //
	SAR2_IDs_DMA_TD[0] = CyDmaTdAllocate();
	// Destination address is incremented after each transaction and Term out generated after 
	// the specified no. of bytes are transferred to memory
	// Tranfer the specified number of samples and generate term out //
	CyDmaTdSetConfiguration(SAR2_IDs_DMA_TD[0], (sizeof(SAR2_IDs_Sample)), SAR2_IDs_DMA_TD[0],
		SAR2_IDs_DMA__TD_TERMOUT_EN | TD_INC_DST_ADR);

	// Set source and destination address //
	CyDmaTdSetAddress(SAR2_IDs_DMA_TD[0], LO16((uint32)SAR2_SAR_WRK0_PTR),
		LO16((uint32)SAR2_IDs_Sample));

	// Set the intial TD for the channel //
	CyDmaChSetInitialTd(SAR2_IDs_DMA_Chan, SAR2_IDs_DMA_TD[0]);
	// Enable the DMA channel //
	CyDmaChEnable(SAR2_IDs_DMA_Chan, 1);
}

/*******************************************************************************
* Function Name: void Current_Sensors_Analysis(void);
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Current_Sensors_Analysis(void)
{
	if(CS_State == CS_CONNECT_STATE)
	{
		/* There was a current pulse detection */
		// Increase the detected pulses counter
		CS_Always_ON_Counter++;
		// Reset the activation pulses counter
		CS_Always_OFF_Counter = 0;
		
		Find_Current_Peaks(CSAB_1_Peak_Data, PeaksResults, CSAB_1_SIZE_OF_PEAKS_DATA_BUFFER, AVE_WIDTH, ZeroLevel);

		if(PeaksResults[0] > fabs(PeaksResults[1]))
		{
			StoredCS4MMCU.Peak[0] = PeaksResults[0];
		}
		else
		{
			StoredCS4MMCU.Peak[0] = fabs(PeaksResults[1]);
		}
		if(StoredCS4MMCU.Peak[0] > MaxCSLimit[0])
		{
			/* Over current Error */
			if(CS_OverCurrent_EventCounter++ > CS_OVERCURRENT_EVENTS)
			{
				SystemErrorReg_Write(SSR_OVER_CURRENT_ERR_BIT_NUM,ENABLE,_16BIT1);
			}
			else
			{
				CS_OverCurrent_EventCounter = 0;	
			}
		}
	}
	else if(GET_BOARD_TYPE == E_PsocPrimaryFunction_HwValidation)
	{
		StoredCS4MMCU.Peak[0] = mean(CSAB_1_Peak_Data, CSAB_1_SIZE_OF_PEAKS_DATA_BUFFER);
	}
}

/*******************************************************************************
* Function Name: void Current_Sensors_GetRef(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Current_Sensors_GetRef(void)
{
	switch(CS_State)
	{
		case CS_INITIAL_STATE:
		{
			if(CS_Required & 0x01)
			{
				if(InitStateCycles < NUM_OF_CS_INIT_CYCLES)
				{
					Init_ZeroLevel_Buffer[InitStateCycles] = mean(CSAB_1_Zero_Data, CSAB_1_SIZE_OF_ZERO_DATA_BUFFER);
					InitStateCycles++;
				}
				else
				{
					Init_ZeroLevel = mean(Init_ZeroLevel_Buffer, NUM_OF_CS_INIT_CYCLES);
					/* Check if initial zero level is in good range */
					if(Init_ZeroLevel > (2048 - 2*CS_DETECT_LEVEL))
					{
						/* The initial reference zero level is accepted */
						CSAB_1_Calibrate(Init_ZeroLevel, CS_DETECT_LEVEL);
						CS_State = CS_CONNECT_STATE;

						SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM,DISABLE,_16BIT2);
					}
					else // not in range 
					{
						SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM,ENABLE,_16BIT2);	
					}				
				}
				StoredCS4MMCU.Peak[0] = 0;
			}
			else if(GET_BOARD_TYPE == E_PsocPrimaryFunction_HwValidation)
			{
				StoredCS4MMCU.Peak[1] = mean(CSAB_1_Zero_Data, CSAB_1_SIZE_OF_ZERO_DATA_BUFFER);	
			}
			break;
		}
		
		case CS_CONNECT_STATE:
		{
			ZeroLevel = mean(CSAB_1_Zero_Data, CSAB_1_SIZE_OF_ZERO_DATA_BUFFER);
			StoredCS4MMCU.Peak[1] = ZeroLevel;
			/* Check Zero level for disconnection */
			if( ZeroLevel > Init_ZeroLevel + CS_DETECT_LEVEL ||
				ZeroLevel < Init_ZeroLevel - CS_DETECT_LEVEL )
			{
				SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM,ENABLE,_16BIT1);
				CS_State = CS_DISCONNECT_STATE;
			}
			else
			{
				CSAB_1_Calibrate(ZeroLevel, CS_DETECT_LEVEL);
				if(CS_Always_ON_Counter > CS_NUM_OF_FALSE_CURRENTS)
				{
					/* Current Always ON */
                    Safety_Relay_TurnON();
					SystemErrorReg_Write(SSR_ALWAYS_ON_ERR_BIT_NUM,ENABLE,_16BIT1);
				}
				if(CS_Always_OFF_Counter > CS_NUM_OF_VAIN_ACTIV)
				{
					/* Current Always OFF */
					SystemWarningReg_Write(SSR_ALWAYS_OFF_WARN_BIT_NUM,ENABLE,_16BIT1);
				}
				else
				{
					/* Disable the current always off warning */
					SystemWarningReg_Write(SSR_ALWAYS_OFF_WARN_BIT_NUM,DISABLE,_16BIT1);
				}
			}
			if(!CurrentPwmValues[0])
			{
				StoredCS4MMCU.Peak[0] = 0;
			}
			break;
		}	
		case CS_DISCONNECT_STATE:
		{
			ZeroLevel = mean(CSAB_1_Zero_Data, CSAB_1_SIZE_OF_ZERO_DATA_BUFFER);
			/* Check Zero level for re-connection */
			if( ZeroLevel < Init_ZeroLevel + CS_DETECT_LEVEL &&
				ZeroLevel > Init_ZeroLevel - CS_DETECT_LEVEL )
			{	
				CS_State = CS_CONNECT_STATE;
				SystemErrorReg_Write(MISSING_CURRENT_SENS_ERR_BIT_NUM,DISABLE,_16BIT1);
			}
			StoredCS4MMCU.Peak[0] = 0;
			break;
		}
		default:
		{
			CS_State = CS_INITIAL_STATE;
			InitStateCycles = 0;
			break;
		}
	}
	//StoredCS4MMCU.Peak[0] = ZeroLevel;
}
/****************************************************************************************************
* Function Name: void Find_Current_Peaks(int16 * x, int16 * y, uint8 N, uint8 aw, int16 Zero_Level)
*****************************************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Find_Current_Peaks(int16 * x, int16 * y, uint8 N, uint8 aw, int16 Zero_Level)
{
	int32	Sum;
	int32 	Pos_Peak;
	int32 	Neg_Peak;
	uint8	i;
	int8	j;
	/* Input protection */
	if (aw<1)
	{
		aw = 1;
	}
	if (N<1)
	{
		N = 1;
	}
	/* reseting peaks */
	Pos_Peak = aw*Zero_Level;
	Neg_Peak = aw*Zero_Level;

	for (i = aw; i<N; i++)
	{
		Sum = 0;

		for (j = i; j>i - aw; j--)
		{
			Sum += (int32)x[j];
		}

		if (Sum > Pos_Peak)
		{
			Pos_Peak = Sum;
		}

		if (Sum < Neg_Peak)
		{
			Neg_Peak = Sum;
		}
	}
	y[0] = (int16)(Pos_Peak / aw - Zero_Level);
	y[1] = (int16)((int16)(Neg_Peak / aw) - Zero_Level);
}

/****************************************************************************************************
* Function Name: uint8 Read_Counter(uint8 Prev_Count, uint8 Count);
*****************************************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
uint8 Read_Counter(uint8 Prev_Count, uint8 Count)
{
	if (Prev_Count > Count)
	{
		Count = 0xFF - Prev_Count + Count;
		return(Count);
	}
	return(Count - Prev_Count);
}

/****************************************************************************************************
* Function Name: void CS_Check_Current(void);
*****************************************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void CS_Check_Current(void)
{


}
/****************************************************************************************************
* Function Name: uint8 Read_HCP_Counter(void)
*****************************************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
uint8 Read_HCP_Counter(void)
{
	uint8 Count = Heat_Cmd_Counter;
	Heat_Cmd_Counter = 0;
	return(Count);
}
/*******************************************************************************
* Function Name: Calc_Current_Loop()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Calc_Cable_IDs(void)
{
	static int16 IDs_Data[NUM_OF_IDs_SENS][NUM_OF_SAR2_IDs_SAMPLES];

	uint8 i, j;

	/* Disable the write to SAR2_CS_SampleLatch */
	SAR2_IDs_DataWriteEnable = 0;
	/*  */
	for (j = 0; j<NUM_OF_IDs_SENS; j++)
	{
		for (i = 0; i<NUM_OF_SAR2_IDs_SAMPLES; i++)
		{
			IDs_Data[j][i] = (SAR2_IDs_SampleLatch[j + (NUM_OF_IDs_SENS*i)]);
		}
	}
	/* Enable the write to SAR2_CS_SampleLatch */
	SAR2_IDs_DataWriteEnable = 1;

	/* Calculate the mean value of the samples, convert to volts and store in StoredIDs4MMCU */
	for (i = 0; i<NUM_OF_IDs_SENS; i++)
	{
		FiltCountID[i] = (uint32)(mean(&(IDs_Data[i][0]), NUM_OF_SAR2_IDs_SAMPLES));
		if (FiltCountID[i] > 4095)
		{
			FiltCountID[i] = 4095;
		}

	}
	Cable_Count_To_ID();

	if ((StoredIDs4MMCU.ID[0] == StoredIDs4MMCU.ID[1])
		&& (StoredIDs4MMCU.ID[1] == StoredIDs4MMCU.ID[2])
		&& (StoredIDs4MMCU.ID[2]))

	{
		if (!StoredIDs4MMCU.ID[0])
		{
			if (CableID_ViolationQualifier > CABLE_ID_MISSMATCH_QUALIFIER)
			{
				SystemErrorReg_Write(CABLES_ID_CONFLICT_ERR_BIT_NUM, ENABLE, _16BIT2);
				CableID_ViolationQualifier = 0;
			}
			CableID_ViolationQualifier++;
		}
		else
		{
			// CableID OK 
			SystemErrorReg_Write(CABLES_ID_CONFLICT_ERR_BIT_NUM, DISABLE, _16BIT2);
			CableID_ViolationQualifier = 0;
		}
	}
	else
	{
		CableID_ViolationQualifier++;
		if (CableID_ViolationQualifier > CABLE_ID_MISSMATCH_QUALIFIER)
		{
			// CableID Violation 
			if(GET_BOARD_TYPE == E_PsocPrimaryFunction_HwValidation)
			{
				SystemErrorReg_Write(CABLES_ID_CONFLICT_ERR_BIT_NUM, DISABLE, _16BIT2);
			}
			else
			{
				SystemErrorReg_Write(CABLES_ID_CONFLICT_ERR_BIT_NUM, ENABLE, _16BIT2);
			}
			CableID_ViolationQualifier = 0;
		}
	}
}

/*******************************************************************************
* Function Name: void Cable_Count_To_ID(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Cable_Count_To_ID(void)
{
	uint8 k, i;
	float VsRatio = 1;
	if (Vsupply != 0)
		VsRatio = 24 / Vsupply;

	for (i = 0; i<NUM_OF_IDs_SENS; i++)
	{
		for (k = 0; k<35; k++)
		{
			if ((uint16)(VsRatio * FiltCountID[i]) <= Bounds4ID[k])
			{
				StoredIDs4MMCU.ID[i] = k;
				break;
			}
		}
	}
}

/* [] END OF FILE */