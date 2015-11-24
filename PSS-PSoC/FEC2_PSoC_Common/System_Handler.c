/*****************************************************************************
 * File Name: System_Handler.c
 *
 * Version: 02.02
 *
 * Description:
 *   This file has the following tasks:
 *   1. Allow software hardware interaction by means of Control and Status Register.
 *   2. System timer generation.
 *
 *
 *****************************************************************************/

/***************************************
 *    Includes
 ***************************************/
#include <device.h>
#include "Temperature_Module_Handler.h"
#include "System_Handler.h"
#include "Communications_Handler.h"
#include "Digital_IO_Module_Handler.h"
#include "Analog_IO_Module_Handler.h"
#include "Monitor_Module_Handler.h"

/***************************************
 *    Global Variables
 ***************************************/
const uint8 num2bit_8[8] =
{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

const uint8 ErrorFlashSequenc[17][34] =
{
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //0
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //1
        { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //2
        { 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //3
        { 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //4
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //5
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //6
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //7
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //8
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //9
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //10
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //11
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //12
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //13
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, //14
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 }, //15
        { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0 } //16
};
uint8 ErrorFlashCode;

T_SystemState SystemState;
/* System Errors bit feilds Flags */
uint8 ErrRegBitActvnArry[16];
/* System Warnings bit feilds Flags */
uint8 WrnRegBitActvnArry[16];

uint32 Sys_Global_Timer;

/***************************************
 *    Function
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
void Init_System(void)
{
    /* Initiate the Sys_Global_Timer */
    Sys_Global_Timer = 0;
    /* Initiate the SPIS */
    Init_SPIS();
    /* Initiate System controll register */
    System_Control_Reg_Init();
    /* Initializes the timing system */
    Init_Timer();
    /* System Errors bit feilds Flags */
    System_Status_Reg_Init();
}

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
void System_Status_Reg_Init(void)
{
    /* Clear system Errors */
    memset(ErrRegBitActvnArry, 0, sizeof(ErrRegBitActvnArry));
    /* Clear system warnings  */
    memset(WrnRegBitActvnArry, 0, sizeof(WrnRegBitActvnArry));

    /* Set system warnings mask */
    SystemState.SystemWarningsMask = 0xFFFF;
    /* Set system Errors mask */
    SystemState.SystemErrorsMask = 0xFFFF;
    /* Clear Errors Flash */
    System_Status_Update();
    ErrorFlashCode = 0;
}

/*****************************************************************************
 * Function Name: TimerIsr()
 ******************************************************************************
 * Summary:
 *   An Interrupt Service Routine (ISR) for the pulse width modulator.  This
 *	function is not public.  The ISR provides basic timing for all background
 *	functions.  It is assumed that the ISR is called once per millisecond.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   None.  Timing event flags are set.
 *
 *****************************************************************************/
static CY_ISR(TimerIsr)
{
    /* counters for the longer time periods */
    static uint8 Count_10ms = 0;
    static uint8 Count_100ms = 0;
    static uint8 Count_250ms = 0;
    static uint8 Count_1s = 0;

    /* 1ms Timer */
    Ex_1ms_Tasks();
    /* 10ms Timer Counter */
    Count_10ms++;
    if (Count_10ms > 9)
    {
        Count_10ms = 0;
        Ex_10ms_Tasks();
        /* 100ms Timer Counter */
        Count_100ms++;
        if (Count_100ms > 9)
        {
            Count_100ms = 0;
            Ex_100ms_Tasks();
        }
        /* 250ms Timer Counter */
        Count_250ms++;
        if (Count_250ms > 24)
        {
            Count_250ms = 0;
            Ex_250ms_Tasks();
            /* 1s Timer Counter */
            Count_1s++;
            if (Count_1s > 3)
            {
                Count_1s = 0;
                Ex_1s_Tasks();
            }
        }
    }
}
/* end of TimerIsr() */

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
/* Function Name: Ex_1ms_Tasks(void) */
void Ex_1ms_Tasks(void)
{
    /******************************************/
    /*********   SPIS Delay Counter    ********/
    /******************************************/
    /**/if (!SPI_Delay_Timer) /**/
    /**/
    { /**/
        /**/SPIS_Comm_Enable(); /**/
        /**/} /**/
    /**/else if (SPI_Delay_Timer > 0) /**/
    /**/
    { /**/
        /**/SPI_Delay_Timer--; /**/
        /**/} /**/
    /******************************************/
    /******************************************/
    Sys_Global_Timer++;
}

/**************************************/
/* Function Name: Ex_10ms_Tasks(void) */
/**************************************/
/**************************************/
void Ex_10ms_Tasks(void)
{
    static uint8 StartSession = 0;
    /* Check if SPIS and SPIM session began */
    if (SPI_Session)
    {
        /* Update system Warnings*/
        if (!StartSession)
        {
            StartSession = 1;
            SystemState.SystemStatus &= (~PSOC_OFF_LINE_STS);
            System_Status_Update();
        }

        /* Check KeepAlive */
        KeepAliveViolationTimer++;
        if (KeepAliveViolationTimer > 200)
        {
            KeepAliveViolationTimer = 0;
            SPI_Session = 0;
            StartSession = 0;
            SPIS_Recover();
            /* Update system Warnings*/
            SystemState.SystemStatus |= PSOC_OFF_LINE_STS;
            System_Status_Update();
        }
    }

    /* Task by main function*/
    if (GET_BOARD_TYPE != E_PsocPrimaryFunction_Heater)
    {
        /* Pole the Digital inputs */
        DigitalInputs_Read();
    }
}

/* Function Name: Ex_100ms_Tasks(void) */
void Ex_100ms_Tasks(void)
{
    ExecuteOutputRamp();
    /* Task by main function*/
    /*
    switch (GET_BOARD_TYPE)
    {
    case IBT_DRYER:
    {

        break;
    }
    case CCS1:
    {

        break;
    }
    case CCS2:
    {

        break;
    }
    case CCS3:
    {

        break;
    }
    case CCS4:
    {

        break;
    }
    case VENTILATION1:
    {

        break;
    }
    case VENTILATION2:
    {

        break;
    }
    case COLD_WATER1:
    {

        break;
    }
    case COLD_WATER2:
    {

        break;
    }
    case COLD_WATER3:
    {

        break;
    }
#ifdef FEC2_PSoC_V02
        case HW_VALIDATION:
        {
            break;
        }
#endif
    case SOFT_MODE:
    {

        break;
    }
    default:
    {
        break;
    }
    }*/

}
/* Function Name: Ex_250ms_Tasks(void) */
void Ex_250ms_Tasks(void)
{
    /* Flash the Error register */
    Error_Message();
    /* Flash the green LED */
    if (SPI_Session)
    {
        GREEN_LED_Write(~GREEN_LED_Read());
    }
    else
    {
        GREEN_LED_Write(1);
    }
    /* Task by main function*/
    /*
    switch (GET_BOARD_TYPE)
    {
    case E_PsocPrimaryFunction_Heater:
    {

        break;
    }
    case CCS1:
    {

        break;
    }
    case CCS2:
    {

        break;
    }
    case CCS3:
    {

        break;
    }
    case CCS4:
    {

        break;
    }
    case VENTILATION1:
    {

        break;
    }
    case VENTILATION2:
    {

        break;
    }
    case COLD_WATER1:
    {

        break;
    }
    case COLD_WATER2:
    {

        break;
    }
    case COLD_WATER3:
    {

        break;
    }
#ifdef FEC2_PSoC_V02
        case HW_VALIDATION:
        {
            break;
        }
#endif
    case SOFT_MODE:
    {

        break;
    }
    default:
    {
        break;
    }
    }
    */
}

int checkForLiquidTimeout()
{
    int i;
    for (i = 0; i < NUM_OF_TEMP_SENS; i++)
    {
        if (No_Liquid_Timer[i] >= LiquidDetectorTimeOut[i])
            return TRUE;
    }
    return FALSE;
}

void incrementLiquidTimer()
{
    int i;
    for (i = 0; i < NUM_OF_TEMP_SENS; i++)
    {
        ++No_Liquid_Timer[i];
    }
}

/* Function Name: Ex_1s_Tasks(void) */
void Ex_1s_Tasks(void)
{
    if (SystemState.SystemStatus & NO_PBIT_EX_STS)
    {
        GREEN_LED_Write(~GREEN_LED_Read());
    }
    /* Task by main function*/
    switch (GET_BOARD_TYPE)
    {
    case E_PsocPrimaryFunction_Heater:
    {
        // if the PWM value is 0, zero the measured peak.
        if (!CurrentPwmValues[0])
        {
            if (!(SystemState.SystemErrors & SSR_ALWAYS_ON_ERR))
            {
                StoredCS4MMCU.Peak[0] = 0;
#ifdef FEC2_PSoC_V01
                StoredCS4MMCU.Peak[1] = 0;
#endif
            }
        }
        break;
    }
    case E_PsocPrimaryFunction_Applicator:
    {
        if (ApplicatorDripValveEnable)
        {
            incrementLiquidTimer();
            if (checkForLiquidTimeout())
            {
                SystemErrorReg_Write(LIQUID_DETECTOR_ERR_BIT_NUM, ENABLE, _16BIT1);
                ApplicatorDripValveEnable = FALSE;
                CurrentPwmValues[3] = FALSE;
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

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
void Init_Timer(void)
{
    isr_Timer_Start();
    /* set the interrupts ISR function to be the one in this file */
    isr_Timer_SetVector(TimerIsr);
} /* end of InitTiming() */

/***********************************************************************************
 * Function Name: void SystemErrorReg_Write(uint16 ErrBitCode, uint8 Op, uint16 Src)
 ************************************************************************************
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
void SystemErrorReg_Write(uint16 ErrBitCode, uint8 Op, uint16 Src)
{
    /* Check if update is required */
    if ((ErrRegBitActvnArry[ErrBitCode] & ((uint16) Src)) != Op)
    {
        if (Op)
            ErrRegBitActvnArry[ErrBitCode] |= Src;
        else
            ErrRegBitActvnArry[ErrBitCode] &= (~Src);

        System_Status_Update();
    }
}

/*****************************************************************************
 * Function Name: void SystemErrorReg_Write(uint16 ErrBitCode, uint8 Op, uint16 Src)
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
void SystemWarningReg_Write(uint16 WarningBitCode, uint8 Op, uint16 Src)
{
    /* Check if update is required */
    if ((WrnRegBitActvnArry[WarningBitCode] & ((uint16) Src)) != Op)
    {
        if (Op)
            WrnRegBitActvnArry[WarningBitCode] |= Src;
        else
            WrnRegBitActvnArry[WarningBitCode] &= (~Src);

        System_Status_Update();
    }
}

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
void System_Status_Update(void)
{
    /* Check status with respect to PSoC primary function */
    switch (GET_BOARD_TYPE)
    {
    case E_PsocPrimaryFunction_Heater:
    {
        IBT_Dryer_Error_Handler();
        IBT_Dryer_Warning_Handler();
        break;
    }
    case E_PsocPrimaryFunction_LiftPbOnError:
    case E_PsocPrimaryFunction_LiftPbOnErrorGen2:
    {
        CCS1_Error_Handler();
        CCS1_Warning_Handler();
        break;
    }
    case E_PsocPrimaryFunction_Applicator:
    {
        Applicator_Error_Handler();
        break;
    }
    case E_PsocPrimaryFunction_SoftDebug:
    {
        SoftMode_Error_Handler();
        SoftMode_Warning_Handler();
    }
    default:
    {
        /* Set the System Initiate bit status
         to make sure outputs are disabled */
        Undefined_Error_Handler();
        Undefined_Warning_Handler();
        break;
    }
    }
    /* update the system status register */
    SysStsBit_Handler();
}

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
void SysStsBit_Handler(void)
{
    // Update the System Status register
    // System module
    if (SystemState.SystemErrors & SYSTEM_MDL_ERR_MASK)
    {
        SystemState.SystemStatus |= SYSTEM_MDL_ERR_STS;
    }
    else
    {
        SystemState.SystemStatus &= (~SYSTEM_MDL_ERR_STS);
    }

    // Temperature module
    if (SystemState.SystemErrors & TEMP_MDL_ERR_MASK)
    {
        SystemState.SystemStatus |= TEMP_MDL_ERR_STS;
        Heat_Output_Disable();
    }
    else
    {
        SystemState.SystemStatus &= (~TEMP_MDL_ERR_STS);
        Heat_Output_Enable();
    }

    // Monitor module
    if (SystemState.SystemErrors & MONITOR_MDL_ERR_MASK)
    {
        SystemState.SystemStatus |= MONITOR_MDL_ERR_STS;
        Heat_Output_Disable();
    }
    else
    {
        SystemState.SystemStatus &= (~MONITOR_MDL_ERR_STS);
        Heat_Output_Enable();
    }

    // Safety module
    if (SystemState.SystemErrors & SAFETY_MDL_ERR_MASK)
    {
        SystemState.SystemStatus |= SAFETY_MDL_ERR_STS;
    }
    else
    {
        SystemState.SystemStatus &= (~SAFETY_MDL_ERR_STS);
    }

    // Temperature module
    if (SystemState.SystemWarnings & TEMP_MDL_WARN_MASK)
    {
        SystemState.SystemStatus |= TEMP_MDL_WARN_STS;
    }
    else
    {
        SystemState.SystemStatus &= (~TEMP_MDL_WARN_STS);
    }
    // Monitor module
    if (SystemState.SystemWarnings & MONITOR_MDL_WARN_MASK)
    {
        SystemState.SystemStatus |= MONITOR_MDL_WARN_STS;
    }
    else
    {
        SystemState.SystemStatus &= (~MONITOR_MDL_WARN_STS);
    }

    if (SystemState.SystemStatus & OUTPUTS_DISABLE_STS_MASK)
    {
        Outputs_SoftEn_OFF();
    }
    else
    {
        Outputs_SoftEn_ON();
    }
}

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
void SysWrnBit_Handler(void)
{

}

/*****************************************************************************
 * Function Name: Undefined_Error_Handler
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
void Undefined_Error_Handler(void)
{
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[SSR_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= SSR_OVER_TEMP_ERR;
        ErrorCode(SSR_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~SSR_OVER_TEMP_ERR);
        ErrorCode(SSR_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* MISING_CURRENT_SENS_ERR */
    if (ErrRegBitActvnArry[MISSING_CURRENT_SENS_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_CURRENT_SENS_ERR;
        //ErrorCode(MISSING_CURRENT_SENS_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_CURRENT_SENS_ERR);
        //ErrorCode(MISSING_CURRENT_SENS_ERR, DISABLE, FALSE);
    }

}

/*****************************************************************************
 * Function Name: Undefined_Warning_Handler
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
void Undefined_Warning_Handler(void)
{

}

/*****************************************************************************
 * Function Name: void IBT_Dryer_Error_Handler(void)
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
void IBT_Dryer_Error_Handler(void)
{
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* PSoC_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[PSoC_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= PSoC_RTD_OVER_TEMP_ERR;
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~PSoC_RTD_OVER_TEMP_ERR);
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* MISSING_RTD_ERR */
    if (ErrRegBitActvnArry[MISSING_RTD_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_RTD_ERR;
        ErrorCode(MISSING_RTD_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_RTD_ERR);
        ErrorCode(MISSING_RTD_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }

    /* MMCU_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[MMCU_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MMCU_RTD_OVER_TEMP_ERR;
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MMCU_RTD_OVER_TEMP_ERR);
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[SSR_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= SSR_OVER_TEMP_ERR;
        ErrorCode(SSR_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~SSR_OVER_TEMP_ERR);
        ErrorCode(SSR_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* MISING_CURRENT_SENS_ERR */
    if (ErrRegBitActvnArry[MISSING_CURRENT_SENS_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_CURRENT_SENS_ERR;
        ErrorCode(MISSING_CURRENT_SENS_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_CURRENT_SENS_ERR);
        ErrorCode(MISSING_CURRENT_SENS_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_CURRENT_ERR */
    if (ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
        ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
        ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
    }

    /* SSR_ALWAYS_ON_ERR */
    if (ErrRegBitActvnArry[SSR_ALWAYS_ON_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= SSR_ALWAYS_ON_ERR;
        ErrorCode(SSR_ALWAYS_ON_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~SSR_ALWAYS_ON_ERR);
        ErrorCode(SSR_ALWAYS_ON_ERR, DISABLE, FALSE);
    }
}

/*****************************************************************************
 * Function Name: void IBT_Dryer_Warning_Handler(void)
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
void IBT_Dryer_Warning_Handler(void)
{
    /* SSR_HIGH_TEMP_WARN */
    if (WrnRegBitActvnArry[SSR_HIGH_TEMP_WARN_BIT_NUM])
    {
        SystemState.SystemWarnings |= SSR_HIGH_TEMP_WARN;
    }
    else
    {
        SystemState.SystemWarnings &= (~SSR_HIGH_TEMP_WARN);
    }

    /* SSR_ALWAYS_OFF_WARN */
    if (WrnRegBitActvnArry[SSR_ALWAYS_OFF_WARN_BIT_NUM])
    {
        SystemState.SystemWarnings |= SSR_ALWAYS_OFF_WARN;
    }
    else
    {
        SystemState.SystemWarnings &= (~SSR_ALWAYS_OFF_WARN);
    }
}

/*****************************************************************************
 * Function Name: void CCS1_Error_Handler(void)
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
void CCS1_Error_Handler(void)
{
    /* SAFETY RELAY open */
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* PSoC_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[PSoC_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= PSoC_RTD_OVER_TEMP_ERR;
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~PSoC_RTD_OVER_TEMP_ERR);
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* MISSING_RTD_ERR */
    if (ErrRegBitActvnArry[MISSING_RTD_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_RTD_ERR;
        ErrorCode(MISSING_RTD_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_RTD_ERR);
        ErrorCode(MISSING_RTD_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }

    /* MMCU_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[MMCU_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MMCU_RTD_OVER_TEMP_ERR;
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MMCU_RTD_OVER_TEMP_ERR);
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_TEMP_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_TEMP_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_TEMP_ERR;
     ErrorCode(SSR_OVER_TEMP_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_TEMP_ERR);
     ErrorCode(SSR_OVER_TEMP_ERR, DISABLE, FALSE);
     }
     */

    /* MISING_CURRENT_SENS_ERR */
    /*
     if(ErrRegBitActvnArry[MISSING_CURRENT_SENS_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= MISSING_CURRENT_SENS_ERR;
     ErrorCode(MISSING_CURRENT_SENS_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~MISSING_CURRENT_SENS_ERR);
     ErrorCode(MISSING_CURRENT_SENS_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_OVER_CURRENT_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
     ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
     ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_ALWAYS_ON_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_ALWAYS_ON_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_ALWAYS_ON_ERR;
     ErrorCode(SSR_ALWAYS_ON_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_ALWAYS_ON_ERR);
     ErrorCode(SSR_ALWAYS_ON_ERR, DISABLE, FALSE);
     }
     */

    /* Lift bar protection activated */
    if (ErrRegBitActvnArry[PRINT_BAR_LIFT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= PRINT_BAR_LIFT_ERR;
        ErrorCode(PRINT_BAR_LIFT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~PRINT_BAR_LIFT_ERR);
        ErrorCode(PRINT_BAR_LIFT_ERR, DISABLE, FALSE);
    }
}

/*****************************************************************************
 * Function Name: CCS1_Warning_Handler(void)
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
void CCS1_Warning_Handler(void)
{

}

/*****************************************************************************
 * Function Name: void CCS2_Error_Handler(void)
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
void CCS2_Error_Handler(void)
{
    /* SAFETY RELAY open */
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* PSoC_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[PSoC_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= PSoC_RTD_OVER_TEMP_ERR;
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~PSoC_RTD_OVER_TEMP_ERR);
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* MISSING_RTD_ERR */
    if (ErrRegBitActvnArry[MISSING_RTD_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_RTD_ERR;
        ErrorCode(MISSING_RTD_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_RTD_ERR);
        ErrorCode(MISSING_RTD_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }

    /* MMCU_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[MMCU_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MMCU_RTD_OVER_TEMP_ERR;
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MMCU_RTD_OVER_TEMP_ERR);
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_TEMP_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_TEMP_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_TEMP_ERR;
     ErrorCode(SSR_OVER_TEMP_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_TEMP_ERR);
     ErrorCode(SSR_OVER_TEMP_ERR, DISABLE, FALSE);
     }
     */

    /* MISING_CURRENT_SENS_ERR */
    /*
     if(ErrRegBitActvnArry[MISSING_CURRENT_SENS_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= MISSING_CURRENT_SENS_ERR;
     ErrorCode(MISSING_CURRENT_SENS_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~MISSING_CURRENT_SENS_ERR);
     ErrorCode(MISSING_CURRENT_SENS_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_OVER_CURRENT_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
     ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
     ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_ALWAYS_ON_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_ALWAYS_ON_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_ALWAYS_ON_ERR;
     ErrorCode(SSR_ALWAYS_ON_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_ALWAYS_ON_ERR);
     ErrorCode(SSR_ALWAYS_ON_ERR, DISABLE, FALSE);
     }
     */
}

/*****************************************************************************
 * Function Name: CCS2_Warning_Handler(void)
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
void CCS2_Warning_Handler(void)
{

}

/*****************************************************************************
 * Function Name: void CCS3_Error_Handler(void)
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
void CCS3_Error_Handler(void)
{
    /* SAFETY RELAY open */
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* PSoC_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[PSoC_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= PSoC_RTD_OVER_TEMP_ERR;
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~PSoC_RTD_OVER_TEMP_ERR);
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* MISSING_RTD_ERR */
    if (ErrRegBitActvnArry[MISSING_RTD_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_RTD_ERR;
        ErrorCode(MISSING_RTD_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_RTD_ERR);
        ErrorCode(MISSING_RTD_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }

    /* MMCU_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[MMCU_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MMCU_RTD_OVER_TEMP_ERR;
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MMCU_RTD_OVER_TEMP_ERR);
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_TEMP_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_TEMP_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_TEMP_ERR;
     ErrorCode(SSR_OVER_TEMP_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_TEMP_ERR);
     ErrorCode(SSR_OVER_TEMP_ERR, DISABLE, FALSE);
     }
     */

    /* MISING_CURRENT_SENS_ERR */
    /*
     if(ErrRegBitActvnArry[MISSING_CURRENT_SENS_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= MISSING_CURRENT_SENS_ERR;
     ErrorCode(MISSING_CURRENT_SENS_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~MISSING_CURRENT_SENS_ERR);
     ErrorCode(MISSING_CURRENT_SENS_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_OVER_CURRENT_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
     ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
     ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_ALWAYS_ON_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_ALWAYS_ON_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_ALWAYS_ON_ERR;
     ErrorCode(SSR_ALWAYS_ON_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_ALWAYS_ON_ERR);
     ErrorCode(SSR_ALWAYS_ON_ERR, DISABLE, FALSE);
     }
     */
}

/*****************************************************************************
 * Function Name: CCS3_Warning_Handler(void)
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
void CCS3_Warning_Handler(void)
{
    if (ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
        ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
        ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
    }
}

/*****************************************************************************
 * Function Name: void CCS3_Error_Handler(void)
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
void CCS4_Error_Handler(void)
{
    /* SAFETY RELAY open */
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* PSoC_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[PSoC_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= PSoC_RTD_OVER_TEMP_ERR;
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~PSoC_RTD_OVER_TEMP_ERR);
        ErrorCode(PSoC_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* MISSING_RTD_ERR */
    if (ErrRegBitActvnArry[MISSING_RTD_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MISSING_RTD_ERR;
        ErrorCode(MISSING_RTD_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MISSING_RTD_ERR);
        ErrorCode(MISSING_RTD_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }

    /* MMCU_RTD_OVER_TEMP_ERR */
    if (ErrRegBitActvnArry[MMCU_RTD_OVER_TEMP_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= MMCU_RTD_OVER_TEMP_ERR;
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~MMCU_RTD_OVER_TEMP_ERR);
        ErrorCode(MMCU_RTD_OVER_TEMP_ERR, DISABLE, FALSE);
    }

    /* SSR_OVER_TEMP_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_TEMP_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_TEMP_ERR;
     ErrorCode(SSR_OVER_TEMP_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_TEMP_ERR);
     ErrorCode(SSR_OVER_TEMP_ERR, DISABLE, FALSE);
     }
     */

    /* MISING_CURRENT_SENS_ERR */
    /*
     if(ErrRegBitActvnArry[MISSING_CURRENT_SENS_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= MISSING_CURRENT_SENS_ERR;
     ErrorCode(MISSING_CURRENT_SENS_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~MISSING_CURRENT_SENS_ERR);
     ErrorCode(MISSING_CURRENT_SENS_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_OVER_CURRENT_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
     ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
     ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
     }
     */
    /* SSR_ALWAYS_ON_ERR */
    /*
     if(ErrRegBitActvnArry[SSR_ALWAYS_ON_ERR_BIT_NUM])
     {
     SystemState.SystemErrors |= SSR_ALWAYS_ON_ERR;
     ErrorCode(SSR_ALWAYS_ON_ERR, ENABLE, FALSE);
     }
     else
     {
     SystemState.SystemErrors &= (~SSR_ALWAYS_ON_ERR);
     ErrorCode(SSR_ALWAYS_ON_ERR, DISABLE, FALSE);
     }
     */
}

/*****************************************************************************
 * Function Name: CCS3_Warning_Handler(void)
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
void CCS4_Warning_Handler(void)
{
    if (ErrRegBitActvnArry[SSR_OVER_CURRENT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= SSR_OVER_CURRENT_ERR;
        ErrorCode(SSR_OVER_CURRENT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~SSR_OVER_CURRENT_ERR);
        ErrorCode(SSR_OVER_CURRENT_ERR, DISABLE, FALSE);
    }
}

/*****************************************************************************
 * Function Name: void Applicator_Error_Handler(void)
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
void Applicator_Error_Handler(void)
{
    /* Hard Latching Error */
    if (ErrRegBitActvnArry[LIQUID_DETECTOR_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= LIQUID_DETECTOR_ERR;
        //ApplicatorDripValveEnable = 0;
        CurrentPwmValues[3] = DOD_PWMOut2_Write(0);
    }
    else
    {
        SystemState.SystemErrors &= (~LIQUID_DETECTOR_ERR);
    }

}

/*****************************************************************************
 * Function Name: Undefined_Error_Handler
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
void SoftMode_Error_Handler(void)
{
    if (ErrRegBitActvnArry[RELAY_OPEN_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RELAY_OPEN_ERR;
        ErrorCode(RELAY_OPEN_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RELAY_OPEN_ERR);
        ErrorCode(RELAY_OPEN_ERR, DISABLE, FALSE);
    }

    /* CABLES_ID_CONFLICT_ERR */
    if (ErrRegBitActvnArry[CABLES_ID_CONFLICT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= CABLES_ID_CONFLICT_ERR;
        ErrorCode(CABLES_ID_CONFLICT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~CABLES_ID_CONFLICT_ERR);
        ErrorCode(CABLES_ID_CONFLICT_ERR, DISABLE, FALSE);
    }

    /* RTD_CAL_RES_FAULT_ERR */
    if (ErrRegBitActvnArry[RTD_CAL_RES_FAULT_ERR_BIT_NUM])
    {
        SystemState.SystemErrors |= RTD_CAL_RES_FAULT_ERR;
        ErrorCode(RTD_CAL_RES_FAULT_ERR, ENABLE, FALSE);
    }
    else
    {
        SystemState.SystemErrors &= (~RTD_CAL_RES_FAULT_ERR);
        ErrorCode(RTD_CAL_RES_FAULT_ERR, DISABLE, FALSE);
    }
}

/*****************************************************************************
 * Function Name: Undefined_Warning_Handler
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
void SoftMode_Warning_Handler(void)
{

}
/************************************************************************************************************************/
/************************************************************************************************************************/
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
void ErrorCode(uint16 ErrorBitField, uint8 en, uint8 Reset)
{
    static uint16 ErrorBits = 0;
    if (Reset)
    {
        ErrorBits = 0;
        ErrorFlashCode = 0;
    }
    /*
     else if(!SPI_Session)
     {
     ErrorBits = 0;
     ErrorFlashCode = 0;
     }
     */
    else
    {
        if (en)
        {
            ErrorBits |= ErrorBitField;
        }
        else
        {
            ErrorBits &= (~ErrorBitField);
        }

        if (ErrorBits & RELAY_OPEN_ERR)
        {
            ErrorFlashCode = 1;
        }
        /*
         else if (ErrorBits & XXX )
         {
         ErrorFlashCode = 2;
         }
         */
        else if (ErrorBits & PSoC_RTD_OVER_TEMP_ERR)
        {
            ErrorFlashCode = 3;
        }

        else if (ErrorBits & CABLES_ID_CONFLICT_ERR)
        {
            ErrorFlashCode = 4;
        }
        /*
         else if (ErrorBits & XXXX )
         {
         ErrorFlashCode = 5;
         }
         */
        else if (ErrorBits & MISSING_RTD_ERR)
        {
            ErrorFlashCode = 6;
        }

        else if (ErrorBits & RTD_CAL_RES_FAULT_ERR)
        {
            ErrorFlashCode = 7;
        }

        else if (ErrorBits & MMCU_RTD_OVER_TEMP_ERR)
        {
            ErrorFlashCode = 8;
        }

        else if (ErrorBits & SSR_OVER_TEMP_ERR)
        {
            ErrorFlashCode = 9;
        }

        else if (ErrorBits & MISSING_CURRENT_SENS_ERR)
        {
            ErrorFlashCode = 10;
        }

        else if (ErrorBits & SSR_OVER_CURRENT_ERR)
        {
            ErrorFlashCode = 11;
        }

        else if (ErrorBits & SSR_ALWAYS_ON_ERR)
        {
            ErrorFlashCode = 12;
        }
        /*
         else if (ErrorBits & XXX)
         {
         ErrorFlashCode = 13;
         }
         */
        else if (ErrorBits & PRINT_BAR_LIFT_ERR)
        {
            ErrorFlashCode = 14;
        }

        else if (ErrorBits & LIQUID_DETECTOR_ERR)
        {
            ErrorFlashCode = 15;
        }

        /*
         else if (ErrorBits & XXX)
         {
         ErrorFlashCode = 16;
         }
         */
        else
        {
            ErrorFlashCode = 0;
        }
    }
}
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
void Error_Message(void)
{
    static uint8 FlashSequenceBit = 0;
    static uint8 LoadSequenceFlag = 1;
    static uint8 localErrorFlashCode = 0;

    if (LoadSequenceFlag)
    {
        localErrorFlashCode = ErrorFlashCode;
        LoadSequenceFlag = 0;
    }

    RED_LED_Write(ErrorFlashSequenc[localErrorFlashCode][FlashSequenceBit++]);
    if (FlashSequenceBit == (2 * localErrorFlashCode + 2))
    {
        FlashSequenceBit = 0;
        LoadSequenceFlag = 1;
    }

}

/************************************************************************************************************************/
/************************************************************************************************************************/
/*****************************************************************************
 * Function Name: System_Control_Reg_Init()
 ******************************************************************************
 * Summary:
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 */
void System_Control_Reg_Init(void)
{
    System_Control_Reg_Write(0x00);
}

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
void Safety_Relay_TurnON(void)
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x01));
}
void Safety_Relay_TurnOFF(void)
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x01)));
}

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
void DSM_Sequencer_Enable(void)
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x04));
}
void DSM_Sequencer_Disable(void)
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x04)));
}

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
void SAR1_soc_Enable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x08));
}
void SAR1_soc_Disable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x08)));
}

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
void SAR2_soc_Enable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x10));
}
void SAR2_soc_Disable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x10)));
}

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
void SPIS_BC_Reset(void)
{
    uint8 i;
    /* Raise the System_Control_Reg SPIS_Reset bit */
    System_Control_Reg_Write((System_Control_Reg_Read() | (0x20)));
    /* provide delay */
    for (i = 0; i < 10; i++)
    {
    }
    /* Lower the System_Control_Reg SPIS_Reset bit */
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x20)));
}

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
void Outputs_SoftEn_ON(void)
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x02));
    /* Start the VDAC's */
    VDAC1_Start();
    VDAC2_Start();
}

void Outputs_SoftEn_OFF(void)
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x02)));

    CurrentPwmValues[0] = DOD_SlowPWM1_Write(0);
    // DOD_SlowPWM2 is a safety output !!!
    CurrentPwmValues[2] = DOD_PWMOut1_Write(0);
    CurrentPwmValues[3] = DOD_PWMOut2_Write(0);
    CurrentPwmValues[4] = DOD_HB_PWMOut1_Write(0);
    CurrentPwmValues[5] = DOD_HB_PWMOut2_Write(0);

    if (GET_BOARD_TYPE == E_PsocPrimaryFunction_LiftPbOnError)
    {
        /* Safety Output should be SET */
        CurrentPwmValues[1] = DOD_SlowPWM2_Write(100);
    }
    else
    {
        /* Safety Output should be RESET */
        CurrentPwmValues[1] = DOD_SlowPWM2_Write(0);
    }
}

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
void Heat_Output_Enable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x40));
}
void Heat_Output_Disable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x40)));
}

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
 *   noneSystem_Control_Reg
 *
 *****************************************************************************/
void PBIT_Enable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() | 0x80));
}
void PBIT_Disable()
{
    System_Control_Reg_Write((System_Control_Reg_Read() & (~0x80)));
}
/************************************************************************************************************************/
/************************************************************************************************************************/

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
 void D1_Enable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() | 0x01);
 }
 void D1_Disable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() & (~0x01) );
 }
 void D2_Enable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() | 0x02);
 }
 void D2_Disable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() & (~0x02));
 }
 void D3_Enable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() | 0x04);
 }
 void D3_Disable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() & (~0x04));
 }
 void D4_Enable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() | 0x08);
 }
 void D4_Disable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() & (~0x08));
 }
 void D5_Enable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() | 0x10);
 }
 void D5_Disable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() & (~0x10));
 }
 void D6_Enable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() | 0x20);
 }
 void D6_Disable()
 {
 DO_Control_Reg_Write(DO_Control_Reg_Read() & (~0x20));
 }
 */
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
uint8 bit2num16(uint8 Num)
{
    uint8 n;
    for (n = 0; n < 17; n++)
    {
        if (!Num)
        {
            return (n);
        }
        Num >>= 1;
    }
    return (0xFF);
}
/* [] END OF FILE */
