/*****************************************************************************
* File Name: AMonitor_Module_Handler.h
*
* Version: 02.02
*
* Description:
*
*
* Note:
*   Safety_feedback pin- TBD
*
* Components Summary:
*   1. SAR2_AMux- MUXing the input voltages.
*   2. SAR2_Flwr- An input buffer.
*   3. SAR2- ADC_SAR.
* Fnctions Summary:
********************************************************************************/

#include <device.h>
#include "main.h"
/***************************************
*    Defines
***************************************/
/*define for the number of samples to be transferred to memory on each switch press*/

#define CS_INITIAL_STATE					0x00
#define CS_CONNECT_STATE					0x01
#define CS_DISCONNECT_STATE					0x02

#define NUM_OF_CS_INIT_CYCLES				2

#define SAR2_RESOLUTION						4096 
#define SAR2_ADC_V2C						SAR2_RESOLUTION / VDD
#define SAR2_ADC_C2V						VDD / SAR2_RESOLUTION

#define NUM_OF_CS_SENS						2
#define NUM_OF_SAR2_CS_SAMPLES				100
#define NO_OF_TOTAL_SAR2_CS_SAMPLES			NUM_OF_CS_SENS * NUM_OF_SAR2_CS_SAMPLES

#define PEAK_MA_WIDTH						20

#define CS_DETECT_LEVEL						205

#define AVE_WIDTH							5
#define CS_MED_FILT_WIDTH					5


#define NUM_OF_IDs_SENS						3
#define NUM_OF_SAR2_IDs_SAMPLES				50	
#define NO_OF_TOTAL_SAR2_IDs_SAMPLES	    NUM_OF_IDs_SENS * NUM_OF_SAR2_IDs_SAMPLES 	


///////////////////////////////////////////////////////////////
// Cable ID Calculation 
// Vs o---[Rid]----o----[R1]------o--------o
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
#define ID_V_DIV_R1					38300 // ohm
#define ID_V_DIV_R2					10000 // ohm
#define ID_V_DIV_R1pR2				ID_V_DIV_R1 + ID_V_DIV_R2
#define IC_COMON_FACTOR				( ID_V_DIV_R2 * SAR2_ADC_V2C ) / 2	
///////////////////////////////////////////////////////////////////

#define CABLE_ID_MISSMATCH_QUALIFIER	100

#define SAR2_IDs_DMA_BYTES_PER_BURST	2
#define SAR2_IDs_DMA_REQUEST_PER_BURST	1
#define SAR2_IDs_DMA_SRC_BASE			(CYDEV_PERIPH_BASE)
#define SAR2_IDs_DMA_DST_BASE			(CYDEV_SRAM_BASE)
		


typedef struct
{
	int16 	Peak[NUM_OF_CS_SENS];
	uint8   HeatCntrlPulseCount;
	uint8	StatusBits;
} T_StoredCS4MMCU;

typedef struct
{
	uint8 	ID[NUM_OF_IDs_SENS];
	uint8	StatusBits;
} T_StoredIDs4MMCU;

/***************************************
*    Extern Variables
***************************************/

extern uint16 Init_ZeroLevel;

extern float Vsupply;
/* A flag to indicate that the SAR2 Current loop DMA transfare is done */
extern uint8 SAR2_CS_DMADone_flag;
/* A flag to indicate that the SAR2 connectors ID DMA transfare is done */
extern uint8 SAR2_IDs_DMADone_flag;
/* A flag to indicate that the SAR2 Thermistor DMA transfare is done */
extern uint8 SAR2_Thrmstr_DMADone_flag;

extern uint8  CS_State;
extern uint8  CS_Required;
extern uint8  Peak_Event_Flag;
extern uint8  Zero_Event_Flag;

extern uint8  Heat_Cmd_Counter;
extern uint32 CS_Always_OFF_Counter;
extern uint8  CS_Always_ON_Counter;

extern T_StoredCS4MMCU				StoredCS4MMCU;
extern T_StoredIDs4MMCU				StoredIDs4MMCU;

extern uint8 	SAR2_CS_DataWriteEnable;
extern uint8 	CABLES_ID_CONFLICT_ERR_Source;
extern int16 	SAR2_CS_Sample[NO_OF_TOTAL_SAR2_CS_SAMPLES];
extern int16	SAR2_CS_SampleLatch[NO_OF_TOTAL_SAR2_CS_SAMPLES];

extern uint8 	SAR2_IDs_DataWriteEnable;
extern int16 	SAR2_IDs_Sample[NO_OF_TOTAL_SAR2_IDs_SAMPLES];
extern int16	SAR2_IDs_SampleLatch[NO_OF_TOTAL_SAR2_IDs_SAMPLES];

extern uint16   MinCSLimit[2];
extern uint16   MaxCSLimit[2];
/***************************************
*    Function Prototypes
***************************************/

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
void Init_Monitor_Module(void);

/*******************************************************************************
* Function Name: void Monitor_ISRs_Relocate(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Monitor_ISRs_Relocate(void);


/*******************************************************************************
* Function Name: SAR2_IDs_DMA_Config()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void SAR2_IDs_DMA_Config(void);

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
void Current_Sensors_Analysis(void);

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
void Current_Sensors_GetRef(void);

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
void Find_Current_Peaks(int16 * x, int16 * y, uint8 N, uint8 aw, int16 Zero_Level);

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
uint8 Read_Counter(uint8 Prev_Count, uint8 Count);

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
void CS_Check_Current(void);

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
uint8 Read_HCP_Counter(void);

/*******************************************************************************
* Function Name: void Calc_Cable_IDs(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Calc_Cable_IDs(void);

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
void Cable_Count_To_ID(void);



//[] END OF FILE


