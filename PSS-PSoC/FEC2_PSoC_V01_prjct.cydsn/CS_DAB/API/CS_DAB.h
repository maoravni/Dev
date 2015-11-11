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

#define `$INSTANCE_NAME`_SIZE_OF_PEAKS_DATA_BUFFER	100
#define `$INSTANCE_NAME`_SIZE_OF_ZERO_DATA_BUFFER	100

#define `$INSTANCE_NAME`_ZERO_LEVEL				2048
#define `$INSTANCE_NAME`_DETECT_THRESHOLD		330
		
#define `$INSTANCE_NAME`_POS_DETECT_THRESHOLD	(`$INSTANCE_NAME`_ZERO_LEVEL + `$INSTANCE_NAME`_DETECT_THRESHOLD)
#define `$INSTANCE_NAME`_NEG_DETECT_THRESHOLD	(`$INSTANCE_NAME`_ZERO_LEVEL - `$INSTANCE_NAME`_DETECT_THRESHOLD)

#define `$INSTANCE_NAME`_PULSE_COUNTER_COMPARE		7
	
/* Main_DMA Configuration for CS1_Detect_DMA */
#define `$INSTANCE_NAME`_Main_DMA_BYTES_PER_BURST 2
#define `$INSTANCE_NAME`_Main_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_Main_DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define `$INSTANCE_NAME`_Main_DMA_DST_BASE (CYDEV_PERIPH_BASE)
	
/* Peak_DMA Configuration for CS1_Detect_DMA */
#define `$INSTANCE_NAME`_Peak_DMA_BYTES_PER_BURST 2
#define `$INSTANCE_NAME`_Peak_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_Peak_DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define `$INSTANCE_NAME`_Peak_DMA_DST_BASE (CYDEV_SRAM_BASE)
	
/* Zero_DMA Configuration for CS1_Detect_DMA */	
#define `$INSTANCE_NAME`_Zero_DMA_BYTES_PER_BURST 2
#define `$INSTANCE_NAME`_Zero_DMA_REQUEST_PER_BURST 1
#define `$INSTANCE_NAME`_Zero_DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define `$INSTANCE_NAME`_Zero_DMA_DST_BASE (CYDEV_SRAM_BASE)	

/* Store_Peak_DMA Configuration for CS1_Detect_DMA */
#define `$INSTANCE_NAME`_Store_Peak_DMA_REQUEST_PER_BURST 0
#define `$INSTANCE_NAME`_Store_Peak_DMA_SRC_BASE (CYDEV_SRAM_BASE)	

/* Store_Zero_DMA Configuration for CS1_Detect_DMA */
#define `$INSTANCE_NAME`_Store_Zero_DMA_REQUEST_PER_BURST 0
#define `$INSTANCE_NAME`_Store_Zero_DMA_SRC_BASE (CYDEV_SRAM_BASE)
	
extern int16 `$INSTANCE_NAME`_Peak_Data[`$INSTANCE_NAME`_SIZE_OF_PEAKS_DATA_BUFFER];
extern int16 `$INSTANCE_NAME`_Zero_Data[`$INSTANCE_NAME`_SIZE_OF_ZERO_DATA_BUFFER];

extern uint8 `$INSTANCE_NAME`_P_Data_Ready;
extern uint8 `$INSTANCE_NAME`_Z_Data_Ready;

extern uint8 `$INSTANCE_NAME`_CS_ON_Counter;


void `$INSTANCE_NAME`_Start();
void `$INSTANCE_NAME`_Stop();

void `$INSTANCE_NAME`_DMAs_Config();
void `$INSTANCE_NAME`_Peak_DMA_Enable();
void `$INSTANCE_NAME`_Zero_DMA_Enable();

void `$INSTANCE_NAME`_Calibrate(uint16 `$INSTANCE_NAME`_Zero_Level, uint16 `$INSTANCE_NAME`_Detect_Thrshld);

#endif
/* [] END OF FILE */
