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
#include <cytypes.h>

#if !defined (`$INSTANCE_NAME`_H `$H`)
#define `$INSTANCE_NAME`_H `$H`

#define `$INSTANCE_NAME`_SIZE_OF_PEAKS_DATA_BUFFER		100
#define `$INSTANCE_NAME`_SIZE_OF_ZERO_DATA_BUFFER		20

#define `$INSTANCE_NAME`_INITIAL_ZERO_LEVEL				2048
		
#define `$INSTANCE_NAME`_INITIAL_POS_DETECT_THRESHOLD	4096
#define `$INSTANCE_NAME`_INITIAL_NEG_DETECT_THRESHOLD	0
	
/* Main_DMA Configuration */
#define `$INSTANCE_NAME`_Main_DMA_BYTES_PER_BURST 2
#define `$INSTANCE_NAME`_Main_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_Main_DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define `$INSTANCE_NAME`_Main_DMA_DST_BASE (CYDEV_PERIPH_BASE)
	
/* Peak_DMA Configuration */
#define `$INSTANCE_NAME`_Peak_DMA_BYTES_PER_BURST 2
#define `$INSTANCE_NAME`_Peak_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_Peak_DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define `$INSTANCE_NAME`_Peak_DMA_DST_BASE (CYDEV_SRAM_BASE)
	
/* Zero_DMA */	
#define `$INSTANCE_NAME`_Zero_DMA_BYTES_PER_BURST 2
#define `$INSTANCE_NAME`_Zero_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_Zero_DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define `$INSTANCE_NAME`_Zero_DMA_DST_BASE (CYDEV_SRAM_BASE)	

/* Store_Peak_DMA */
#define `$INSTANCE_NAME`_Store_Peak_DMA_REQUEST_PER_BURST 0
#define `$INSTANCE_NAME`_Store_Peak_DMA_SRC_BASE (CYDEV_SRAM_BASE)	

/* Store_Zero_DMA */
#define `$INSTANCE_NAME`_Store_Zero_DMA_REQUEST_PER_BURST 0
#define `$INSTANCE_NAME`_Store_Zero_DMA_SRC_BASE (CYDEV_SRAM_BASE)

/* Data for External use */
extern int16 `$INSTANCE_NAME`_Peak_Data[`$INSTANCE_NAME`_SIZE_OF_PEAKS_DATA_BUFFER];
extern int16 `$INSTANCE_NAME`_Zero_Data[`$INSTANCE_NAME`_SIZE_OF_ZERO_DATA_BUFFER];

void `$INSTANCE_NAME`_Start();

void `$INSTANCE_NAME`_Stop();

void `$INSTANCE_NAME`_DMAs_Config();

void `$INSTANCE_NAME`_Reset();

void `$INSTANCE_NAME`_Calibrate(uint16 `$INSTANCE_NAME`_Zero_Level, uint16 `$INSTANCE_NAME`_Detect_Thrshld);

#endif
/* [] END OF FILE */
