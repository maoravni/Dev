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
/***************************************
*    Includes
***************************************/

#include "main.h"
#include "Safety_Module_Handler.h"
#include "System_Handler.h"

/****************************
* ISR: Safety_Relay_ISR *
*****************************/
CY_ISR(Safety_Relay_ISR)
{
	/* Turn OFF Safety Relay */
	Outputs_SoftEn_OFF();
	/* turn on error bit */
	SystemErrorReg_Write(RELAY_OPEN_ERR_BIT_NUM,ENABLE,_16BIT1);
}

/****************************
* ISR: LogicFunc_ISR *
*****************************/
CY_ISR(LogicFunc_ISR)
{
	/* turn on error bit */
	isr_LogicFunc_Stop();
	SystemErrorReg_Write(PRINT_BAR_LIFT_ERR_BIT_NUM,ENABLE,_16BIT1);
}

/****** d5*************************************************************************
* Function Name: void Safety_ISRs_Relocate(void)
********************************************************************************
* Summary:
*  
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Safety_ISRs_Relocate(void)
{
    /* Give a name to the Safety Relay interrupt vector */
    isr_Safety_Relay_SetVector(Safety_Relay_ISR);
	isr_Safety_Relay_SetPriority(5);
	/* Give a name to the Logic function interrupt vector */
	isr_LogicFunc_SetVector(LogicFunc_ISR);
	isr_LogicFunc_SetPriority(5);
}
/* [] END OF FILE */
