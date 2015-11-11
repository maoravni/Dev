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

#include "Analog_IO_Module_Handler.h"
#include "Communications_Handler.h"


/****************************
* ISR: SAR1_AnlgIn_DMA_Done *
*****************************/
CY_ISR(SAR1_AnlgIn_DMA_Done)
{
	if (SAR1DataWriteEnable  && CurrentsWriteEnable)
	{
		/* Latching the DMA value into SAR1_SampleLatch */
		memcpy(SAR1_SampleLatch,SAR1_Sample,sizeof(SAR1_Sample)); /* memcpy( dst, src, n bytes )*/
		/* Raise the DMADone flag */
		SAR1_AnlgIn_DMADone_flag = 1;
	}
}

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
void Analog_IO_ISRs_Relocate(void)
{
    /* Give a name to the DMA interrupt vector */
    ISR_SAR1_AnlgIn_DMA_SetVector(SAR1_AnlgIn_DMA_Done);
	ISR_SAR1_AnlgIn_DMA_SetPriority(1);
	
}
/* [] END OF FILE */
