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
#define GREEN_LED_TOGGLE 	GREEN_LED_Write(~GREEN_LED_Read());
#define RED_LED_TOGGLE 		RED_LED_Write(~RED_LED_Read());
/***************************************
*    Extern Variables
***************************************/
extern uint32 Sys_Global_Timer;

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
void Ex_1ms_Tasks(void);

/**************************************/
/* Function Name: Ex_10ms_Tasks(void) */
/**************************************/
/**************************************/
void Ex_10ms_Tasks(void);
/* Function Name: Ex_100ms_Tasks(void) */
void Ex_100ms_Tasks(void);
/* Function Name: Ex_250ms_Tasks(void) */
void Ex_250ms_Tasks(void);

/* Function Name: Ex_1s_Tasks(void) */
void Ex_1s_Tasks(void);


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
void System_Control_Reg_Init(void);

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
/* [] END OF FILE */
