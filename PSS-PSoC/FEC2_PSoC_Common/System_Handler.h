/*****************************************************************************
* File Name: System_Handler.h
*
* Version: 02.02
*
* Description:
*   This file has the following tasks:
*   1. Allow software hardware interaction by means of Control and Status Register.
*   2. System timer generation.
*
* Note:
*
* Components Summary:
*   1. FreqDivT08MHz- divid the BUS_CLK to to generate CLK_8MHz.
*   2. FreqDivT01MHz- divid the BUS_CLK to to generate CLK_1MHz.
*   3. FreqDivT0100kHz- divid the BUS_CLK to to generate CLK_100kHz.
*   4. FreqDivT010kHz- divid the BUS_CLK to to generate CLK_10kHz.
*   5. PWM_Timer- Generates an interrupt every 1ms from wich 
*                 we generate 10ms and 1sec timers by means of software. 
*   6. System_Status_Reg- A status register to indecate the status of the system.
*   7. System_Control_Reg- A control register to control systems operation.
*
* Fnctions Summary:
*   1. void Init_Timer(void)- a function to initiate the PWM_Timr.
*   2. void TenthSecondDelay(uint8 count)- A delay function with 10ms steps.
*   3. void DSM_Sequencer_Enable(void)- Enables the DSM_Sequencer.
*   3. void DSM_Sequencer_Disable(void)- Disables the DSM_Sequencer.
*
********************************************************************************/

#include "device.h"

/***************************************
*    Defines
***************************************/

////////////////////////////////////////////////////////////////
//////////////////////////
// System Errors Hard Register //
//////////////////////////
////////////////////////////////////////////////////////////////
#define RELAY_OPEN_ALARM			0x01		
//#define XXXXXXXXXXXXXXXXXX		0x02
#define PSOC_OVER_TEMP_ALARM		0x04
#define PSOC_OVER_ABS_TEMP_ALARM	0x08
//#define XXXXXXXXXXXXXXXXXX		0x10
//#define XXXXXXXXXXXXXXXXXX		0x20
//#define XXXXXXXXXXXXXXXXXX		0x40
//#define XXXXXXXXXXXXXXXXXX		0x80


////////////////////////////////////////////////////////////
#define CLEAR_ALL_BITS				0x0000
#define SYS_CRG_INIT				0x80
////////////////////////////////////////////////////////////////
//////////////////////////
// Status Soft Register //
//////////////////////////
////////////////////////////////////////////////////////////////
#define PSOC_OFF_LINE_STS			0x0001	// OUTPUTS_DISABLE_STS_MASK member
#define NO_INIT_STS					0x0002  // OUTPUTS_DISABLE_STS_MASK member
#define NO_PBIT_EX_STS				0x0004	// OUTPUTS_DISABLE_STS_MASK member
#define UNCONFIG_SYS_STS			0x0008	// OUTPUTS_DISABLE_STS_MASK member

//#define SYS_CONFIG_MOD_STS		0x0010
#define SYSTEM_MDL_WARN_STS			0x0020
#define TEMP_MDL_WARN_STS			0x0040
#define ANALOG_MDL_WARN_STS			0x0080

#define MONITOR_MDL_WARN_STS		0x0100		
#define SAFETY_MDL_WARN_STS			0x0200
#define SYSTEM_MDL_ERR_STS			0x0400	// OUTPUTS_DISABLE_STS_MASK member
#define TEMP_MDL_ERR_STS			0x0800

#define ANALOG_MDL_ERR_STS			0x1000
#define MONITOR_MDL_ERR_STS			0x2000	// OUTPUTS_DISABLE_STS_MASK member
#define SAFETY_MDL_ERR_STS			0x4000	// OUTPUTS_DISABLE_STS_MASK member
//#define XXXXXXXXXX_STATUS			0x8000

#define SYSTEM_STATUS_INIT			0x000F
#define SYSTEM_STATUS_RECOVER		0x0008
//									  8421842184218421
//									0b0000110000001111
#define OUTPUTS_DISABLE_STS_MASK	0xCC0F
////////////////////////////////////////////////////////////////
/////////////////////////
// Error Soft Register //
/////////////////////////
////////////////////////////////////////////////////////////////
/* IBT DRYER ERRORS */
#define RELAY_OPEN_ERR				0x0001	// Safety and System Error mask member
//#define XXXXXXXXXXXXXX_ERR		0x0002	// Safety Error mask member
#define PSoC_RTD_OVER_TEMP_ERR		0x0004 	// System Error mask member
#define CABLES_ID_CONFLICT_ERR		0x0008	// System Error mask member

//#define XXXXXXXXXXXXXX_ERR		0x0010	// System Error mask member
#define MISSING_RTD_ERR				0x0020	// Temperature Error mask member				
#define RTD_CAL_RES_FAULT_ERR		0x0040	// Temperature Error mask member
#define MMCU_RTD_OVER_TEMP_ERR		0x0080	// Temperature Error mask member

#define SSR_OVER_TEMP_ERR			0x0100	// Temperature Error mask member
#define MISSING_CURRENT_SENS_ERR	0x0200	// Monitor Error mask member
#define SSR_OVER_CURRENT_ERR		0x0400	// Monitor Error mask member
#define SSR_ALWAYS_ON_ERR			0x0800	// Monitor Error mask member

//#define MISSING_CURRENT_SENS_ERR	0x1000	// Error mask member
#define PRINT_BAR_LIFT_ERR			0x2000	// Error mask member
#define LIQUID_DETECTOR_ERR			0x4000	// Error mask member
//#define XXXXXXXXXXXXXX_ERR		0x8000	// Error mask member

#define RELAY_OPEN_ERR_BIT_NUM				0	
//#define XXXXXXXXXXXXXX_ERR_BIT_NUM		1
#define PSoC_RTD_OVER_TEMP_ERR_BIT_NUM		2 
#define CABLES_ID_CONFLICT_ERR_BIT_NUM		3

//#define XXXXXXXXXXXXXX_ERR_BIT_NUM		4
#define MISSING_RTD_ERR_BIT_NUM				5					
#define RTD_CAL_RES_FAULT_ERR_BIT_NUM		6
#define MMCU_RTD_OVER_TEMP_ERR_BIT_NUM		7

#define SSR_OVER_TEMP_ERR_BIT_NUM			8
#define MISSING_CURRENT_SENS_ERR_BIT_NUM	9
#define SSR_OVER_CURRENT_ERR_BIT_NUM		10
#define SSR_ALWAYS_ON_ERR_BIT_NUM			11

//#define XXXXXXXXXXXXXX_ERR_BIT_NUM		12
#define PRINT_BAR_LIFT_ERR_BIT_NUM			13
#define LIQUID_DETECTOR_ERR_BIT_NUM			14
//#define XXXXXXXXXXXXXX_ERR_BIT_NUM		15


/* Error bits should divided to modules subjects */

#define SAFETY_MDL_ERR_MASK			0x0001 //0b0000000000000001
#define SYSTEM_MDL_ERR_MASK			0x000E //0b0100000000001110
#define TEMP_MDL_ERR_MASK			0x03E0 //0b0000001111100000
//#define ANALOG_MDL_ERR_MASK		0x0xxx
//#define DIGITAL_MDL_ERR_MASK		0x0xxx									
#define MONITOR_MDL_ERR_MASK		0x0E00 //0b0100111000000000
//#define COMMS_MDL_ERR_MASK		0x0xxx
////////////////////////////////////////////////////////////////
///////////////////////////
// Warning Soft Register //
////////////////////////////////////////////////////////////////
//#define XXXXXXXXXXXXXX_WARN		0x0001
//#define XXXXXXXXXXXXXX_WARN		0x0002
//#define XXXXXXXXXXXXXX_WARN		0x0004
//#define XXXXXXXXXXXXXX_WARN		0x0008
//#define XXXXXXXXXXXXXX_WARN		0x0010
//#define XXXXXXXXXXXXXX_WARN		0x0020
//#define XXXXXXXXXXXXXX_WARN		0x0040
//#define XXXXXXXXXXXXXX_WARN		0x0080
#define SSR_HIGH_TEMP_WARN			0x0100
//#define XXXXXXXXXXXXXX_WARN		0x0200
//#define XXXXXXXXXXXXXX_WARN		0x0400
#define SSR_ALWAYS_OFF_WARN			0x0800
//#define XXXXXXXXXXXXXX_WARN		0x1000
//#define XXXXXXXXXXXXXX_WARN		0x2000
//#define XXXXXXXXXXXXXX_WARN		0x4000
//#define XXXXXXXXXXXXXX_WARN		0x8000

//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		0
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		1
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		2
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		3
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		4
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		5
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		6
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		7
#define SSR_HIGH_TEMP_WARN_BIT_NUM			8
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		9
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		10
#define SSR_ALWAYS_OFF_WARN_BIT_NUM			11
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		12
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		13
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		14
//#define XXXXXXXXXXXXXX_WARN_BIT_NUM		15

//#define SYSTEM_MDL_WARN_MASK		0x0003
#define TEMP_MDL_WARN_MASK			0x0100
//#define ANALOG_MDL_WARN_MASK		0x0xxx
#define MONITOR_MDL_WARN_MASK		0x0D00		
//#define SAFETY_MDL_WARN_MASK		0x0xxx

//////////////////////////////////
// Board Configurations options //
//////////////////////////////////
typedef enum
{
        /// <remarks/>
        E_PsocPrimaryFunction_Undefined,

        /// <remarks/>
        E_PsocPrimaryFunction_HwValidation,

        /// <remarks/>
        E_PsocPrimaryFunction_SoftDebug,

        /// <remarks/>
        E_PsocPrimaryFunction_Heater,

        /// <remarks/>
        E_PsocPrimaryFunction_LiftPbOnError,

        /// <remarks/>
        E_PsocPrimaryFunction_Applicator,

        /// <remarks/>
        E_PsocPrimaryFunction_LiftPbOnErrorGen2,
        
        E_PsocPrimaryFunction_LiftPbOnErrorGen3,

    
    E_PsocPrimaryFunction_Size,
} E_PsocPrimaryFunction;
/*
#define	XXX					0x00
#define IBT_DRYER			0x01
#define CCS1				0x02
#define CCS2				0x03
#define CCS3				0x04
#define SOFT_MODE			0x05
#define VENTILATION1		0x06
#define VENTILATION2		0x07
#define COLD_WATER1			0x08
#define COLD_WATER2			0x09
#define COLD_WATER3			0x0A
#define HW_VALIDATION		0x0B
#define APPLICATOR			0x0C
#define CCS4				0x0D
*/

/**************************************
* Global Veriables
**************************************/

typedef struct
{
	uint16 SystemStatus;
	uint16 SystemWarnings;
	uint16 SystemErrors;
	uint16 SystemWarningsMask;
	uint16 SystemErrorsMask;
}T_SystemState;

/***************************************
*    Extern Variables
***************************************/
extern uint32 Sys_Global_Timer;

extern uint8 ErrorFlashCode;

extern T_SystemState	SystemState;
/* System Errors bit feilds Flags */
extern uint8 ErrRegBitActvnArry[16];
/* System Warnings bit feilds Flags */
extern uint8 WrnRegBitActvnArry[16];

/***************************************
*    Function Prototypes
***************************************/

/*****************************************************************************
* Function Name: Init_Syatem(()
******************************************************************************
* Summary:
*   Initializes the system
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Init_System(void);

/*****************************************************************************
* Function Name: System_Control_Reg_Init()
******************************************************************************
* Summary:
*   Enables the DSM_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void System_Control_Reg_Init(void);

/*****************************************************************************
* Function Name: System_Status_Reg_Init()
******************************************************************************
* Summary:
*   Enables the DSM_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void System_Status_Reg_Init(void);

/*****************************************************************************
* Function Name: Init_Timer()
******************************************************************************
* Summary:
*   Initializes the timing system, particularly the interrupt component and
*	the ISR.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Init_Timer(void);

/*****************************************************************************
* Function Name: Ex_1ms_Tasks(void)
* Function Name: Ex_10ms_Tasks(void)
* Function Name: Ex_100ms_Tasks(void)
* Function Name: Ex_250ms_Tasks(void)
* Function Name: Ex_1s_Tasks(void)
******************************************************************************
* Summary:
*   Initializes the timing system, particularly the interrupt component and
*	the ISR.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Ex_1ms_Tasks(void);
void Ex_10ms_Tasks(void);
void Ex_100ms_Tasks(void);
void Ex_250ms_Tasks(void);
void Ex_1s_Tasks(void);

/***********************************************************************************
* Function Name: void SystemErrorReg_Write(uint16 ErrBitCode, uint8 Op, uint16 Src)
************************************************************************************
* Summary:
*   
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SystemErrorReg_Write(uint16 ErrBitCode, uint8 Op, uint16 Src);

/*****************************************************************************
* Function Name: void SystemErrorReg_Write(uint16 ErrBitCode, uint8 Op, uint16 Src)
******************************************************************************
* Summary:
*   
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SystemWarningReg_Write(uint16 WarningBitCode, uint8 Op, uint16 Src);

/*****************************************************************************
* Function Name: void System_Status_Update(void)
******************************************************************************
* Summary:
*   Initializes the system
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void System_Status_Update(void);
/*****************************************************************************
* Function Name: Error_Message()
******************************************************************************
* Summary:
*   Error_Message
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Error_Message(void);

/*****************************************************************************
* Function Name: void ErrorCode(uint16 ErrorBitFeild,uint8 en, uint8 Reset)
******************************************************************************
* Summary:
*   Error_Message
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void ErrorCode(uint16 ErrorBitField, uint8 en, uint8 Reset);

/*****************************************************************************
* Function Name: void SysStsBit_Handler(void)
******************************************************************************
* Summary:
*   Disables the SAR1_Sequencer by writing 0 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SysStsBit_Handler(void);

/*****************************************************************************
* Function Name: void SysErrBit_Handler(uint8 * ErrRegBitActvnArry)
******************************************************************************
* Summary:
*   Disables the SAR1_Sequencer by writing 0 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SysErrBit_Handler(void);

void Undefined_Error_Handler(void);
void Undefined_Warning_Handler(void);

void IBT_Dryer_Error_Handler(void);
void IBT_Dryer_Warning_Handler(void);

void CCS1_Error_Handler(void);
void CCS1_Warning_Handler(void);

void CCS2_Error_Handler(void);
void CCS2_Warning_Handler(void);

void CCS3_Error_Handler(void);
void CCS3_Warning_Handler(void);

void CCS3_Error_Handler(void);
void CCS3_Warning_Handler(void);

void CCS4_Error_Handler(void);
void CCS4_Warning_Handler(void);

void COLD_WATER1_Error_Handler(void);
void COLD_WATER1_Warning_Handler(void);

void COLD_WATER2_Error_Handler(void);
void COLD_WATER2_Warning_Handler(void);

void COLD_WATER3_Error_Handler(void);
void COLD_WATER3_Warning_Handler(void);

void SoftMode_Error_Handler(void);
void SoftMode_Warning_Handler(void);

void Applicator_Error_Handler(void);
void Applicator_Warning_Handler(void);
/*****************************************************************************
* Function Name: void SysWrnBit_Handler(void)
******************************************************************************
* Summary:
*   Disables the SAR1_Sequencer by writing 0 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SysWrnBit_Handler(void);

/*****************************************************************************
* Function Name: Safety_Relay_TurnON()
* Function Name: Safety_Relay_TurnOFF()
******************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Safety_Relay_TurnON(void);
void Safety_Relay_TurnOFF(void);

/*****************************************************************************
* Function Name: OutPuts_SoftEn_ON()
* Function Name: OutPuts_SoftEn_OFF()
******************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Outputs_SoftEn_ON(void);
void Outputs_SoftEn_OFF(void);

/*****************************************************************************
* Function Name: DSM_Sequencer_Enable()
* Function Name: DSM_Sequencer_Disable()
******************************************************************************
* Summary:
*   Enables the DSM_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void DSM_Sequencer_Enable(void);
void DSM_Sequencer_Disable(void);

/*****************************************************************************
* Function Name: SAR1_Sequencer_Enable()
* Function Name: SAR1_Sequencer_Disable()
******************************************************************************
* Summary:
*   Enables the SAR1_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SAR1_soc_Enable();
void SAR1_soc_Disable();

/*****************************************************************************
* Function Name: SAR2_Sequencer_Enable()
* Function Name: SAR2_Sequencer_Disable()
******************************************************************************
* Summary:
*   Enables the SAR1_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SAR2_soc_Enable();
void SAR2_soc_Disable();

/*****************************************************************************
* Function Name: SPIS_BC_Reset()
******************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void SPIS_BC_Reset(void);

/*****************************************************************************
* Function Name: Heat_Output_Enable()
* Function Name: Heat_Output_Disable()
******************************************************************************
* Summary:
*   Enables the SAR1_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void Heat_Output_Enable();
void Heat_Output_Disable();

/*****************************************************************************
* Function Name: PBIT_Enable()
* Function Name: PBIT_Disable()
******************************************************************************
* Summary:
*   Enables the SAR1_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
void PBIT_Enable();
void PBIT_Disable();
/*****************************************************************************/

/*****************************************************************************
* Function Name: Dx_Enable()
* Function Name: Dx_Disable()
******************************************************************************
* Summary:
*   Enables the SAR1_Sequencer by writing 1 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
/*
void D1_Enable();
void D1_Disable();

void D2_Enable();
void D2_Disable();

void D3_Enable();
void D3_Disable();

void D4_Enable();
void D4_Disable();

void D5_Enable();
void D5_Disable();

void D6_Enable();
void D6_Disable();
*/
/*****************************************************************************/

/*****************************************************************************/


/*****************************************************************************
* Function Name: uint8 bit2num16(uint8 Num)
******************************************************************************
* Summary:
*   Disables the SAR1_Sequencer by writing 0 to bit0 of System Control Reg.
*
* Parameters:
*   none
*
* Return:
*   none
*
*****************************************************************************/
uint8 bit2num16(uint8 Num);
int checkForLiquidTimeout();

/* [] END OF FILE */
