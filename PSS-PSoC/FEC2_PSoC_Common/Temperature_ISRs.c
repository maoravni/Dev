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
 * Includes
 ***************************************/
#include "Temperature_Module_Handler.h"
#include "Communications_Handler.h"

/*********************
 * ISR: DSM_DMA_done *
 *********************/
CY_ISR(DSM_DMA_Done)
{
    /* check if StoredTemps4MMCU or DSM_SampleLatch is not read */
    if (DSMDataWriteEnable && TemperatureWriteEnable)
    {
        /* Latching the DMA value into DSM_SampleLatch */
        memcpy(DSM_SampleLatch, DSM_Sample, sizeof(DSM_Sample)); /* memcpy( dst, src, n bytes )*/
        /* Raise the DMADone flag */
        DSM_DMADone_flag = 1;
    }
}

/*******************************************************************************
* Function Name: void Temperature_ISRs_Relocate(void)
********************************************************************************
* Summary:
*  
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void  Temperature_ISRs_Relocate()
{
    /* Give a name to the DMA interrupt vector */
    ISR_DSM_DMA_SetVector(DSM_DMA_Done);
	ISR_DSM_DMA_SetPriority(2);
}

/* [] END OF FILE */
