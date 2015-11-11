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
#include <device.h>
#include "Monitor_Module_Handler.h"
#include "Communications_Handler.h"

uint8  Heat_Cmd_Counter;
uint32 CS_Always_OFF_Counter;
uint8  CS_Always_ON_Counter;
/*************************
* ISR: SAR2_IDs_DMA_Done *
**************************/
CY_ISR(SAR2_IDs_DMA_Done)
{
	/* Latching the DMA value into SAR1_SampleLatch */
	if (SAR2_IDs_DataWriteEnable  && IDsWriteEnable)
	{
		memcpy(SAR2_IDs_SampleLatch, SAR2_IDs_Sample, sizeof(SAR2_IDs_SampleLatch)); /* memcpy( dst, src, n bytes )*/
		/* Raise the DMADone flag */
		SAR2_IDs_DMADone_flag = 1;
	}
}

CY_ISR(Heat_Counter_Event)
{
	/* There was an activation pulse */
	// Increase the activation pulses counter
	CS_Always_OFF_Counter++;
	// Reset the detected pulses counter
	CS_Always_ON_Counter = 0;		
}

CY_ISR(Peak_Event)
{
	Peak_Event_Flag = 1;
}

CY_ISR(Zero_Event)
{
	Zero_Event_Flag = 1;
}



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
void Monitor_ISRs_Relocate(void)
{
	/* Start the Interrupt modules */
	ISR_SAR2_IDs_DMA_Start();
	Heat_Counter_isr_Start();
	P_isr_Start();
	Z_isr_Start();

	/* Give a name to the IDs_DMA interrupt vector */
	ISR_SAR2_IDs_DMA_SetVector(SAR2_IDs_DMA_Done);
	Heat_Counter_isr_SetVector(Heat_Counter_Event);
	P_isr_SetVector(Peak_Event);
	Z_isr_SetVector(Zero_Event);
}

/* [] END OF FILE */
