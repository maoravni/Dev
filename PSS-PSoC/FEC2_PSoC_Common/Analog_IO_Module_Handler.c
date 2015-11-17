/*****************************************************************************
* File Name: Analog_IO_Module_Handler.c
*
* Version: 02.02
*
* Description:
*   This file has the following tasks:
*   1. handle the MUXing of the analog inputs to SAR1 which includes:
*       a. The voltage across a current to voltage converting resistor, 
           in order to measure a 4-20mA sensor output.
        b. The voltage across a current to voltage converting resistor,
           in order to measure the current flowing in a the digital output FET driver.
*   2. Providing the 4-20mA current output driver with a controling voltage. 
*
* Note:
*   Safety_feedback pin- TBD
********************************************************************************/

/***************************************
*    Includes
***************************************/
#include "main.h"
#include "Analog_IO_Module_Handler.h"
#include "System_Handler.h"
#include "Monitor_Module_Handler.h"
#include "Communications_Handler.h"
#include "DSP.h"

/***************************************
*    Global Variables
***************************************/
/* Variable declarations for DMA . 
* These variables are defined as global variables to avoid "may be used before being set" warning 
* issued by the PSoC 5 compilers  MDK/RVDS.In this case these variables are automatically initialized to zero */

uint8 	SAR1_AnlgIn_DMADone_flag;
uint8 	SAR1DataWriteEnable;

uint16 	SAR1_Sample[NUM_OF_TOTAL_SAR1_SAMPLES]={0};
uint16	SAR1_SampleLatch[NUM_OF_TOTAL_SAR1_SAMPLES]={0};

uint16	MinAnalogInputLimit[NUM_OF_ANALOG_SENS];
uint16	MaxAnalogInputLimit[NUM_OF_ANALOG_SENS];

int16 ThermistorInput;
float  AnalogInputScale[NUM_OF_ANALOG_SENS];
float  AnalogInputCorrection[NUM_OF_ANALOG_SENS];

float 	SSR_Temperature;

T_StoredAnalogIn4MMCU	StoredAnalogIn4MMCU;
T_StoredBridgeCurrents4MMCU	StoredBridgeCurrents4MMCU;
uint16 StoredAnalogOut[2];


/*******************************************************************************
* Function Name: Init()
********************************************************************************/

void Init_Analog_Module(void)
{
	/* initiate VDAC1 */
	VDAC1_Start();
	VDAC1_Flwr_Start();
	VDAC1_SetValue(0u);
	/* initiate VDAC2 */
	VDAC2_Start();
	VDAC2_Flwr_Start();
	VDAC2_SetValue(0u);
	
	/* Start the SAR1 Follower amplifier */
	SAR1_Flwr_Start();	
	/* Start the SAR1 */
    SAR1_Start();	
    /* Disable the ADC ISR as it is not required */
    SAR1_IRQ_Disable(); 
    /* Start the DMA Interrupt module */
	ISR_SAR1_AnlgIn_DMA_Start(); 
    /* Relocate the ISRs of the Analog I\O Module */
    Analog_IO_ISRs_Relocate();	
    /* Initiate the DMA module */
    SAR1_AnlgIn_DMA_Config(); 
    /* Lower the  DMADone flag */
    SAR1_AnlgIn_DMADone_flag = 0;
	/* Start the SAR1 conversion */
	SAR1_soc_Enable();
	ThermistorInput = 0;
	/* Allow writting data into SAR1_SampleLatch */
	SAR1DataWriteEnable = 1;
    AnalogOutput_Write(0, 1);
    AnalogOutput_Write(0, 2);
}

/*******************************************************************************
* Function Name:  DSM_DMA_Config
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
void SAR1_AnlgIn_DMA_Config()
{
	uint8 	SAR1_AnlgIn_DMA_Chan;
	uint8 	SAR1_AnlgIn_DMA_TD[1];
    // DMA Initialization - 2 byte per trigger.Each burst transfer needs a new request. 
	 // Set upper source and destination address. //	
    SAR1_AnlgIn_DMA_Chan = SAR1_AnlgIn_DMA_DmaInitialize(SAR1_AnlgIn_DMA_BYTES_PER_BURST, SAR1_AnlgIn_DMA_REQUEST_PER_BURST, 
                                 HI16(SAR1_AnlgIn_DMA_SRC_BASE), HI16(SAR1_AnlgIn_DMA_DST_BASE));
    // Allocate TD //
    SAR1_AnlgIn_DMA_TD[0] = CyDmaTdAllocate();    
    // Destination address is incremented after each transaction and Term out generated after 
     // the specified no. of bytes are transferred to memory
	 // Tranfer the specified number of samples and generate term out //
    CyDmaTdSetConfiguration(SAR1_AnlgIn_DMA_TD[0], (sizeof(SAR1_Sample)), SAR1_AnlgIn_DMA_TD[0], 
                            SAR1_AnlgIn_DMA__TD_TERMOUT_EN | TD_INC_DST_ADR); 
    
    // Set source and destination address //
    CyDmaTdSetAddress(SAR1_AnlgIn_DMA_TD[0], LO16((uint32)SAR1_SAR_WRK0_PTR), 
                            LO16((uint32)SAR1_Sample)); 
	
    // Set the intial TD for the channel //
    CyDmaChSetInitialTd(SAR1_AnlgIn_DMA_Chan, SAR1_AnlgIn_DMA_TD[0]);    
	// Enable the DMA channel //
	CyDmaChEnable(SAR1_AnlgIn_DMA_Chan, 1); 
}

/*******************************************************************************
* Function Name: Calc_Currents()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Calc_Currents(void)
{
    //static int32 AveSAR1_Sample[NUM_OF_ANALOG_SENS]={0};
	static int32 FiltSAR1_Sample[NUM_OF_ANALOG_SENS][2]={{5000},{5000}};
	static uint8 n=1;
	
	uint8 i,j;
	E_PsocPrimaryFunction BoardType = GET_BOARD_TYPE;
	//memset(AveSAR1_Sample,0,sizeof(AveSAR1_Sample));
	
	/* Disable the write to SAR1_SampleLatch */
	SAR1DataWriteEnable = 0;

	if(BoardType != E_PsocPrimaryFunction_Heater)
	{
		/* Calculate and store the Analog Inputs */
		for (i=0;i<NUM_OF_ANALOG_INPUTS;i++)
		{
			/* Start of Singel Pole IIR lowpasss filtering the AveSAR1_Sample */
			for(j=0;j<NUM_OF_SAR1_SAMPLES;j++)
			{
				FiltSAR1_Sample[i][n] = IIR_LPF_sp(SAR1_SampleLatch[i+j*NUM_OF_ANALOG_SENS]*10, &FiltSAR1_Sample[i][n-1], ANALOG_FILT_NUME);
			}
			StoredAnalogIn4MMCU.AnalogInputs[i] = (uint16) (FiltSAR1_Sample[i][n]);
	        /* End of SingelPole_IIR_LPF */			
		}
	}
	/* Is IBT_DRYER*/
	else
	{
		/* Calculate and store the Thermistor Temperature */
		for(j=0;j<NUM_OF_SAR1_SAMPLES;j++)
		{ 
			FiltSAR1_Sample[THERMISTOR_INDEX][n] = IIR_LPF_sp(SAR1_SampleLatch[THERMISTOR_INDEX+j*NUM_OF_ANALOG_SENS]
																, &FiltSAR1_Sample[THERMISTOR_INDEX][n-1], ANALOG_FILT_NUME);
		}
		ThermistorInput = (int16) FiltSAR1_Sample[THERMISTOR_INDEX][n];
		SSR_Temperature = Calc_Thermistor_Temperature(ThermistorInput);

	    /* End of SingelPole_IIR_LPF */			
		
		// Check for Over Temperature
		if(SSR_THRMSTR_OVER_TEMPERATURE < SSR_Temperature)
		{
			SystemErrorReg_Write(SSR_OVER_TEMP_ERR_BIT_NUM,ENABLE,_16BIT1);
			/* Update system Warrnings*/
			SystemWarningReg_Write(SSR_HIGH_TEMP_WARN_BIT_NUM,DISABLE,_16BIT1);
		}
		else if(SSR_HIGH_TEMPERATURE < SSR_Temperature )
		{		
			/* Update system Errors */
			SystemErrorReg_Write(SSR_OVER_TEMP_ERR_BIT_NUM,DISABLE,_16BIT1);
			/* Update system Warrnings*/
			SystemWarningReg_Write(SSR_HIGH_TEMP_WARN_BIT_NUM,ENABLE,_16BIT1);	
		}	
		else
		{					
			/* Update system Warrnings*/
			SystemWarningReg_Write(SSR_HIGH_TEMP_WARN_BIT_NUM,DISABLE,_16BIT1);
		}		
	}
	
	if(BoardType == E_PsocPrimaryFunction_HwValidation)
	{
		/* Calculate and store the Thermistor Temperature */
		for(j=0;j<NUM_OF_SAR1_SAMPLES;j++)
		{ 
			FiltSAR1_Sample[THERMISTOR_INDEX][n] = IIR_LPF_sp(SAR1_SampleLatch[THERMISTOR_INDEX+j*NUM_OF_ANALOG_SENS]
																, &FiltSAR1_Sample[THERMISTOR_INDEX][n-1], ANALOG_FILT_NUME);
		}
		ThermistorInput = (int16) FiltSAR1_Sample[THERMISTOR_INDEX][n];
		SSR_Temperature = Calc_Thermistor_Temperature(ThermistorInput);

	    /* End of SingelPole_IIR_LPF */			
	}
	

	#ifdef FEC2_PSoC_V02
		/* Calculate and store the Board Supply voltage Inputs */
		
		/* Start of Singel Pole IIR lowpasss filtering the AveSAR1_Sample */
		for(j=0;j<NUM_OF_SAR1_SAMPLES;j++)
		{
			FiltSAR1_Sample[SUPPLY_VOLTAGE_INDEX][n] = IIR_LPF_sp(SAR1_SampleLatch[SUPPLY_VOLTAGE_INDEX+j*NUM_OF_ANALOG_SENS]
																	,&FiltSAR1_Sample[SUPPLY_VOLTAGE_INDEX][n-1], SUPPLY_VOLTAGE_FILT_NUME); 				
		}
		/* End of SingelPole_IIR_LPF */	
		float Measured_Voltage_Supply = 5*((float)FiltSAR1_Sample[SUPPLY_VOLTAGE_INDEX][n])/4096;
		Vsupply = Measured_Voltage_Supply * SUPPLY_VOLTAGE_DIV_CONV;
	#endif
    		
	/* Enable the write to SAR1_SampleLatch */
	SAR1DataWriteEnable = 1;
}

/*******************************************************************************
* Function Name: void AnalogOutput_Write(uint8 DAC_Count , uint8 DACnum)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void AnalogOutput_Write(uint8 DAC_Count , uint8 DACnum)
{
	if (DAC_Count < 0) // 4mA
	{
		DAC_Count = 0;
	}
	if (DAC_Count > 250) //20mA
	{
		DAC_Count = 250;	
	}
	if (DACnum==1)
	{
        StoredAnalogOut[0] = DAC_Count;
		VDAC1_SetValue(DAC_Count);	
	}
	else if (DACnum==2)
	{
        StoredAnalogOut[1] = DAC_Count;
		VDAC2_SetValue(DAC_Count);	
	}	
}

/*******************************************************************************
* Function Name: float Calc_Thermistor_Temperature(int16 Thermistor_Vcount)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
float Calc_Thermistor_Temperature(int16 Thermistor_Vcount)
{
	uint32 resT;
	float Vm;

	Vm = SAR2_CountsTo_Volts((int16) Thermistor_Vcount);
	resT = ((float) THRMSTR_V_DIV_VsxR2/Vm - THRMSTR_V_DIV_R1pR2) ;
	
	return(Thermistor_GetTemperature ( (uint32) resT )/100);
}

/* [] END OF FILE */


















