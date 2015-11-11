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
#include "device.h"
#include "Monitor_Module_Handler.h"
#include "Communications_Handler.h"

uint8  Heat_Cmd_Counter;
uint32  CS_Always_OFF_Counter[2];
/*************************
* ISR: SAR2_IDs_DMA_Done *
**************************/
CY_ISR(SAR2_IDs_DMA_Done)
{
    /* Latching the DMA value into SAR1_SampleLatch */
	if (SAR2_IDs_DataWriteEnable  && IDsWriteEnable)
	{
		memcpy(SAR2_IDs_SampleLatch,SAR2_IDs_Sample,INT16_SIZE*NO_OF_TOTAL_SAR2_IDs_SAMPLES); /* memcpy( dst, src, n bytes )*/
		/* Raise the DMADone flag */
		SAR2_IDs_DMADone_flag = 1;
	}
}

CY_ISR(Heat_Counter_Event)
{
	Heat_Cmd_Counter++;
	CS_Always_OFF_Counter[0]++;
	CS_Always_OFF_Counter[1]++;
	
	CS_DAB_1_CS_ON_Counter = 0;
	CS_DAB_2_CS_ON_Counter = 0;
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
    /* Give a name to the IDs_DMA interrupt vector */
    ISR_SAR2_IDs_DMA_SetVector(SAR2_IDs_DMA_Done);
	ISR_SAR2_IDs_DMA_SetPriority(5);
	Heat_Counter_isr_SetVector(Heat_Counter_Event);
	Heat_Counter_isr_SetPriority(2);
}
	
/* [] END OF FILE */
