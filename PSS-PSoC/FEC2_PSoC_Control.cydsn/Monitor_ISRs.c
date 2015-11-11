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

/*************************
* ISR: SAR2_IDs_DMA_Done *
**************************/
CY_ISR(SAR2_IDs_DMA_Done)
{
	/* Latching the DMA value into SAR1_SampleLatch */
	if (SAR2_IDs_DataWriteEnable)
	{
		memcpy(SAR2_IDs_SampleLatch, SAR2_IDs_Sample, sizeof(SAR2_IDs_SampleLatch)); /* memcpy( dst, src, n bytes )*/
		/* Raise the DMADone flag */
		SAR2_IDs_DMADone_flag = 1;
	}
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
	P_isr_Start();
	Z_isr_Start();

	/* Give a name to the IDs_DMA interrupt vector */
	ISR_SAR2_IDs_DMA_SetVector(SAR2_IDs_DMA_Done);
	P_isr_SetVector(Peak_Event);
	Z_isr_SetVector(Zero_Event);
}

/* [] END OF FILE */
