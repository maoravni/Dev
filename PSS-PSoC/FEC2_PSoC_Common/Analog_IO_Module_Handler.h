/*****************************************************************************
* File Name: Analog_IO_Module_Handler.h
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
*
* Components Summary:
*   1. SAR1_AMux- MUXing the input voltages.
*   2. SAR1_Flwr- An input buffer.
*   3. SAR1- ADC_SAR.
*   4. VDAC1- controling the current in the 4-20mA output.
*   5. VDAC2- controling the current in the 4-20mA output.
*   6. VDAC1_Flwr- An Output buffer. 
*   7. VDAC2_Flwr- An Output buffer.
* Fnctions Summary:
********************************************************************************/

#include "device.h"

/***************************************
*    Defines
***************************************/

#define SAR1_RESOLUTION						4096 
#define SAR1_ADC_V2C						SAR1_RESOLUTION / VDD
#define SAR1_ADC_C2V						VDD / SAR2_RESOLUTION

/*define for the number of samples to be transferred to memory on each switch press*/
#define NUM_OF_ANALOG_INPUTS				2
#define NUM_OF_THERMISTORS_INPUTS			1
#define NUM_OF_BRIDGE_FEEDBACKS				2
#define NUM_OF_ANALOG_SENS					5//NUM_OF_ANALOG_INPUTS + NUM_OF_THERMISTORS_INPUTS + NUM_OF_BRIDGE_FEEDBACKS

#define THERMISTOR_INDEX					2
#define FET1_CURRENT_INDEX					3
#define SUPPLY_VOLTAGE_INDEX				4
#define SUPPLY_VOLTAGE_DIV_CONV				9.06
#define NUM_OF_SAR1_SAMPLES					20
#define NUM_OF_TOTAL_SAR1_SAMPLES			100//NUM_OF_ANALOG_SENS * NUM_OF_SAR1_SAMPLES

#define INPUTS_V2I_RESISTOR					100
#define BRIDGE_V2I_RESISTOR					100

#define NUM_OF_ANALOG_OUTPUTS				2

/* DMA Configuration for DMA */
#define SAR1_AnlgIn_DMA_BYTES_PER_BURST     2
#define SAR1_AnlgIn_DMA_REQUEST_PER_BURST   1
#define SAR1_AnlgIn_DMA_SRC_BASE            (CYDEV_PERIPH_BASE)
#define SAR1_AnlgIn_DMA_DST_BASE            (CYDEV_SRAM_BASE)
/* Calculation of (4-20)mA input IIR */
// Fc = 100Hz
// Fs = 10000Hz

// Numerator = round(512*PI*Fc/Fs)
// Final Fc = Fs*Numerator/(512*PI)

// Nume = round(512*PI*100/10000)
// 		= round(512*PI*0.01)
//      = round(5.12*PI)
//      = round(16.08)
//      = 16
// Final Fc = 10000*16/(512*PI)
//          = 160000/(512*PI)
//			= 312.5/PI
//			= 99.47 Hz
#define ANALOG_FILT_NUME      		8

/* Calculation of Board Supply voltage input IIR */
// Fc = 100Hz
// Fs = 10000Hz

// Numerator = round(512*PI*Fc/Fs)
// Final Fc = Fs*Numerator/(512*PI)

// Nume = round(512*PI*100/10000)
// 		= round(512*PI*0.01)
//      = round(5.12*PI)
//      = round(16.08)
//      = 16
// Final Fc = 10000*16/(512*PI)
//          = 160000/(512*PI)
//			= 312.5/PI
//			= 99.47 Hz
#define SUPPLY_VOLTAGE_FILT_NUME      16

///////////////////////////////////////////////////////////////
// Thermistor Calculation 
// Vs o---[Rt]----o----[R1]------o--------o
//	    						 |
//                               -
//                               R2      Vm
//                               -
//                               |
//                               o--------o
//                               |
//                              ---
//                               -
//////////////////////////////////////////////////////////////
#define THRMSTR_V_DIV_Vs					24 // ohm
#define THRMSTR_V_DIV_R1					10000 // ohm
#define THRMSTR_V_DIV_R2					2000 // ohm
#define THRMSTR_V_DIV_VsxR2					THRMSTR_V_DIV_R2 * THRMSTR_V_DIV_Vs
#define THRMSTR_V_DIV_R1pR2					THRMSTR_V_DIV_R1 + THRMSTR_V_DIV_R2

#define SSR_HIGH_TEMPERATURE				70
#define SSR_THRMSTR_OVER_TEMPERATURE		90

#define MINIMUM_CONTAINER_WEIGHT			400  // 4.88 mA

typedef struct
{
	uint16  AnalogInputs[NUM_OF_ANALOG_INPUTS];
	uint8	StatusBits;
} T_StoredAnalogIn4MMCU;

typedef struct
{
	float 	Currents[NUM_OF_BRIDGE_FEEDBACKS];
	uint8	Status;
} T_StoredBridgeCurrents4MMCU;

/***************************************
*    Extern Variables
***************************************/

/* A flag to indicate that the SAR1 DMA transfare is done */
extern uint8 SAR1_AnlgIn_DMADone_flag;
extern T_StoredAnalogIn4MMCU	StoredAnalogIn4MMCU;
extern T_StoredBridgeCurrents4MMCU	StoredBridgeCurrents4MMCU;

extern uint8 	SAR1DataWriteEnable;
extern uint16 	SAR1_Sample[NUM_OF_TOTAL_SAR1_SAMPLES];
extern uint16	SAR1_SampleLatch[NUM_OF_TOTAL_SAR1_SAMPLES];

extern uint16	MinAnalogInputLimit[NUM_OF_ANALOG_SENS];
extern uint16	MaxAnalogInputLimit[NUM_OF_ANALOG_SENS];

extern float AnalogInputScale[NUM_OF_ANALOG_SENS];
extern float AnalogInputCorrection[NUM_OF_ANALOG_SENS];
extern int16 ThermistorInput;

extern uint16 StoredAnalogOut[2];

/***************************************
*    Function Prototypes
***************************************/

/*******************************************************************************
* Function Name: Init_Analog_Module()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Init_Analog_Module(void);

/*******************************************************************************
* Function Name: void Analog_IO_ISRs_Relocate(void)
********************************************************************************
* Summary:
*  
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Analog_IO_ISRs_Relocate(void);

/*******************************************************************************
* Function Name: SAR1_AnlgIn_DMA_Config()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void SAR1_AnlgIn_DMA_Config(void);

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
void Calc_Currents(void);

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
void AnalogOutput_Write(uint8 DAC_Count , uint8 DACnum);

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
float Calc_Thermistor_Temperature(int16 Thermistor_Vcount);

//[] END OF FILE
