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
#include "Digital_IO_Module_Handler.h"
#include "Analog_IO_Module_Handler.h"
#include "Monitor_Module_Handler.h"

/***************************************
*    Global Variables
***************************************/
const uint8 num2bit_8[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

uint32 Sys_Global_Timer;
uint8 FanSpeed;
uint8 FanEnable;
uint8 ValveEnable;
uint8 ValvePosContrl;
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
	/* Initiate System controll register */
	System_Control_Reg_Init();		
	/* Initializes the timing system */
	Init_Timer();
        FanSpeed = 0;
        FanEnable = 0;
        ValveEnable = 0;
        ValvePosContrl = 0;
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
		if(Count_100ms > 9)
		{
			Count_100ms = 0;
			Ex_100ms_Tasks();
		}		
/* 250ms Timer Counter */		
		Count_250ms++;
		if(Count_250ms > 24)
		{
			Count_250ms = 0;
			Ex_250ms_Tasks();
/* 1s Timer Counter */			
			Count_1s++;
			if(Count_1s > 3)
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
	Sys_Global_Timer++;
}

/**************************************/
/* Function Name: Ex_10ms_Tasks(void) */
/**************************************/
/**************************************/
void Ex_10ms_Tasks(void)
{	
  
}
/* Function Name: Ex_100ms_Tasks(void) */
void Ex_100ms_Tasks(void)
{
  DigitalInputs_Read();
}
/* Function Name: Ex_250ms_Tasks(void) */
void Ex_250ms_Tasks(void)
{
	GREEN_LED_TOGGLE;
}

/* Function Name: Ex_1s_Tasks(void) */
void Ex_1s_Tasks(void)
{
	RED_LED_TOGGLE;
        DOD_PWMOut1_Write(FanSpeed);
        DOD_PWMOut2_Write(FanEnable);
        DOD_HB_PWMOut1_Write(ValveEnable);
        AnalogOutput_Write((uint8)AnalogOut_SetCurrent(15/*ValvePosContrl*/),1);
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
    System_Control_Reg_Write( (System_Control_Reg_Read() | 0x04) );
}
void DSM_Sequencer_Disable(void)
{
    System_Control_Reg_Write( (System_Control_Reg_Read() & (~0x04)) );
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
    System_Control_Reg_Write( (System_Control_Reg_Read() | 0x08) );
}
void SAR1_soc_Disable()
{
    System_Control_Reg_Write( (System_Control_Reg_Read() & (~0x08)) );
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
    System_Control_Reg_Write( (System_Control_Reg_Read() | 0x10) );
}
void SAR2_soc_Disable()
{
    System_Control_Reg_Write( (System_Control_Reg_Read() & (~0x10)) );
}

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
	for(n=0;n<17;n++)
	{
		if(!Num)
		{
			return(n);	
		}
		Num >>= 1;
	}
	return(0xFF);
}
/* [] END OF FILE */
