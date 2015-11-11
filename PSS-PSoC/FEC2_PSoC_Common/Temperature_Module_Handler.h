/*****************************************************************************
* File Name: Temperature_Module_Handler.h
*
* Version: 02.02
*
* Description:
*   The file is divided in 2:
*   1. PT100 Inputs which responsible for:
*      a. The MUXing of the voltages across 4 PT100's and 1 calibration resistor when 
*         current is flowing through them and when not. the "non-current" voltages are used 
*         for 1/f noise reduction. 
*      b. configures the DSM_DMA TD and handle the the signals upon DMA completion interrupt.
*      c. Conversion of measured voltage to temperature.
*   2. PWM outputs with controllable duty cycle.  
*
* Note:
*   the PWM outputs are hard ANDed with the Safet_Relay input.
*
* Components Summary:
*   1. PT100 Inputs:
*       a. IDAC_AMuxHw- MUXing of input currents to the PT100's and calibration resistor. 
*       b. DSM_AMuxHw- MUXing of input voltages from the PT100's and calibration resistor.
*       c. IDAC- Outputs 2mA and 0mA currents to the PT100's and calibration resistor.
*       d. DSM- Conversion of input voltages from the PT100's and calibration resistor.
*       e. DSM_Sequencer- handles the sequencing of the MUX's with EOC triggering and SOC delay.
*       f. DSM_DMA- configures the DSM_DMA TD. 
*       g. ISR_DSM_DMA- Generates an interrupt upon DSM_DMA transaction completion.
*       h. RTD- GConvertes the voltages across the PT100's to temperatures.
*
* Functions Summary:
*   1. void Init_Temperature_Module(void)- Initiates the temperature module.
*   2. PT100 Inputs:
*       a. void DSM_DMA_Config(void)- Configure the DMA TD.
*       b. void Calc_Temperature(void)- Calculates the temperatures by converting the measured 
*                                       voltages gatherd by the DMA.
*       c. float * Get_Temperature(void)- returns the pointer to the calculated temperatures.
*       d. void Init_PWM_Outputs(void).
*   3. PWM Outputs:
*       a. void PWM_Start(void)- Starts the PWMs modules.
*       b. void PWM_Stop(void)- Stops the PWMs modules.
*       c. void PWM_Write_Duty_Cycle(uint8 ModuleNumber , uint8 DtyCycl)- Writes the compare value in the "ModuleNumber" PWM module.
*       d. uint8 PWM_Read_Duty_Cycle(uint8 ModuleNumber)- Reads the compare value in the "ModuleNumber" PWM module.
*
********************************************************************************/
#include "device.h"

/***************************************
*    Defines
***************************************/
/*define for the number of samples to be transferred to memory on each switch press*/
#define NUM_OF_DSM_SAMPLES       	10
#define NUM_OF_TEMP_SENS          	4
#define	STATUS_BYTE					NUM_OF_TEMP_SENS
#define TEMPTR_MED_FILT_WIDTH		/*3*/ 5 /*6*//*7*//*9*//*25*/
#define FEEDFORWARD				  	256000 // IIR Feedforward;
// Fc = 0.5Hz
// Final Fc = Fs/(2*PI*IIR_FILTER_NUMERATOR)
//Numerator = round(512*PI*Fc/Fs)
// Nume = round(512*PI*0.5/100)
// 		= round(512*PI*0.005)
//      = round(2.56*PI)
//      = round(8.042)
//      = 8
// Final Fc = 100*Numerator/(2*PI*256)
//          = 800/(512*PI)
//			= 1.5625/PI
//			= 0.497
#define TEMP_FILT_NUME      		8
// Fc = 10Hz
// Final Fc = Fs/(2*PI*IIR_FILTER_NUMERATOR)
//Numerator = round(512*PI*Fc/Fs)
// Nume = round(512*PI*10/100)
// 		= round(512*PI*0.1)
//      = round(51.2*PI)
//      = round(160.14)
//      = 160
// Final Fc = 100*Numerator/(2*PI*256)
//          = 16000/(512*PI)
//			= 312.5/PI
//			= 9.95
#define LIQUID_DETECT_FILT_NUME     160
#define LIQUID_DETECTOR_THRESHOLD	100
#define PWM_4_DRIP_VALVE			90 // %
#define LIQUID_DRIP_MAX_TIME		120
#define NO_LIQUID_DRIP_TIME			60

#define CAL_REF_RESISTOR        	100//ohms
/* DMA Configuration for DMA */
#define DSM_DMA_BYTES_PER_BURST     2
#define DSM_DMA_REQUEST_PER_BURST   1
#define DSM_DMA_SRC_BASE            (CYDEV_PERIPH_BASE)
#define DSM_DMA_DST_BASE            (CYDEV_SRAM_BASE)

#define CH0_ERROR					0x01
#define CH0_WARNG					0x02
#define CH1_ERROR					0x04
#define CH1_WARNG					0x08
#define CH2_ERROR					0x10
#define CH2_WARNG					0x20
#define CH3_ERROR					0x40
#define CH3_WARNG					0x80

#define TEMPERATURE_OFFSET			0.2
#define MIN_SAMPLES_OFFSET			200
#define PRECISION_MULTIPLIER        100

typedef struct
{
	uint8 ChannelType;
	uint8 FilterType;
}T_RTD_Configuration;

typedef struct
{
    float temperature[NUM_OF_TEMP_SENS];
    uint8 statusBits;
} T_StoredTemps4MMCU;

/***************************************
*    Extern Variables
***************************************/
/* A flag to indicate that the DSM DMA transfer is done */

extern uint8 				DSM_DMA_Chan;
extern uint8 				CalcTempr_InitVar;
extern uint8				DSM_DMADone_flag;
extern uint8				RequiredRTDsBitfield;
extern uint8                ConnectedRTDsBitfield; // The first Enable is for calibrating resistor

extern uint8 				Ind2Bit_5[5];

extern uint8 				DSMDataWriteEnable;
extern int16 				DSM_Sample[NUM_OF_DSM_SAMPLES];
extern int16 				DSM_SampleLatch[NUM_OF_DSM_SAMPLES];
extern float 				TempUpLimit[NUM_OF_TEMP_SENS];

extern int8 No_Liquid_Timer[NUM_OF_TEMP_SENS];
extern uint8 LiquidDetectorTimeOut[NUM_OF_TEMP_SENS];

extern T_StoredTemps4MMCU	StoredTemps4MMCU;
extern T_RTD_Configuration  RTD_Configuration;

/***************************************
*    Function Prototypes
***************************************/


/*******************************************************************************
********************  P T 1 0 0    I N P T S   *********************************
********************************************************************************/

/*******************************************************************************
* Function Name: Init_Temp_Sense()
********************************************************************************
* Summary:
*   InitAnalogOutputs will start:
*   IDAC.
*   IDAC_AMux.
*   DSM.
*   DSM_AMux.
*  
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Init_Temperature_Module(void);

/*******************************************************************************
* Function Name:  DSM_DMA_Config()
********************************************************************************
*
* Summary:
*    Code generated by the DMA Wizard.
*
*   DSM_DMA_Config() performs following functions:
*  1: Initializes the DSM_DMA channel
*  2: Allocates Transfer Descriptors
*  3: Configures the TD.
*  4: Sets source and destination address for this TD.
*  5: Initializes the TD.
*  6: Enables the DSM_DMA channel.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DSM_DMA_Config(void);

/*******************************************************************************
* Function Name: void Temperature_ISRs_Relocate(void)
********************************************************************************
* Summary:
*  
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Temperature_ISRs_Relocate(void);

/*******************************************************************************
* Function Name:  Calc_Temperature
********************************************************************************
*
* Summary:
*   Calc_Temperature will calculate the temperatures by the measured voltages across the PT100's.
*   After a cycle of IDAC_AMuxHw and DSM_AMuxHw (refer to TopDesign\Temp Sense) DMADone_flag will be raised and this function will be called.
*   in principle each channel is measured twice: 
*       - once with 0 excitation current in order to measure the noise in the channel. 
*       - once with excitation current in order to measure the signal + noise in the channel.
*   
*   *Note: the voltages are in terms of ADC counts.

*   1. DSM_Sample[NUM_OF_DSM_SAMPLES] holding the measured voltages:
*       DSM_Sample[0]- is the Voltage measured on the Calibration resistor when the excitation current is 0mA.
*       DSM_Sample[1]- is the Voltage measured on the Calibration resistor when the excitation current is 2mA.
*       DSM_Sample[2]- is the Voltage measured on the First PT100 when the excitation current is 0mA.
*       DSM_Sample[3]- is the Voltage measured on the First PT100 when the excitation current is 2mA.
*       DSM_Sample[4]- is the Voltage measured on the Second PT100 when the excitation current is 0mA.
*       DSM_Sample[5]- is the Voltage measured on the Second PT100 when the excitation current is 2mA.
*       DSM_Sample[6]- is the Voltage measured on the Third PT100 when the excitation current is 0mA.
*       DSM_Sample[7]- is the Voltage measured on the Third PT100 when the excitation current is 2mA.
*       DSM_Sample[8]- is the Voltage measured on the Fourth PT100 when the excitation current is 0mA.
*       DSM_Sample[9]- is the Voltage measured on the Fourth PT100 when the excitation current is 2mA.
*       
*   2. NoOffset_DSM_Sample[NUM_OF_TEMP_SENS+1] is the 1/f noise free signal:
*       NoOffset_DSM_Sample[0] = DSM_Sample[1] -DSM_Sample[0]  
*       NoOffset_DSM_Sample[1] = DSM_Sample[3] -DSM_Sample[2]
*       NoOffset_DSM_Sample[2] = DSM_Sample[5] -DSM_Sample[4]
*       NoOffset_DSM_Sample[3] = DSM_Sample[7] -DSM_Sample[6]
*       NoOffset_DSM_Sample[4] = DSM_Sample[9] -DSM_Sample[8]
*
*   3. FiltNoOffset_DSM_Sample[NUM_OF_TEMP_SENS+1] is the filtered NoOffset_DSM_Sample[NUM_OF_TEMP_SENS+1]. Here we apply a single pole IIR Filter.
*      the cut-of frequency is:
*                              fs      f_DSM                1
*                       fc = ------ =  ----- * ------------------------------- ~ 5Hz
*                            2*pi*a    2*2*5   2*pi*(2^IIR_FILTER_COEFF_SHIFT)
*
*   4. StoredTemps4MMCU[NUM_OF_TEMP_SENS] is the measured temperatures holding array.
*       StoredTemps4MMCU[0] = RTD_GetTemperature ( 1000*( FiltNoOffset_DSM_Sample[1] * CAL_REF_RESISTOR ) / FiltNoOffset_DSM_Sample[0] )
*       StoredTemps4MMCU[1] = RTD_GetTemperature ( 1000*( FiltNoOffset_DSM_Sample[2] * CAL_REF_RESISTOR ) / FiltNoOffset_DSM_Sample[0] )
*       StoredTemps4MMCU[2] = RTD_GetTemperature ( 1000*( FiltNoOffset_DSM_Sample[3] * CAL_REF_RESISTOR ) / FiltNoOffset_DSM_Sample[0] )
*       StoredTemps4MMCU[3] = RTD_GetTemperature ( 1000*( FiltNoOffset_DSM_Sample[4] * CAL_REF_RESISTOR ) / FiltNoOffset_DSM_Sample[0] )
*
* Parameters:
*   none.  
*
* Return:
*   none. 
*
*******************************************************************************/
void Calc_Temperature(void);


//[] END OF FILE
