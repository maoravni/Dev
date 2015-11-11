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
#define `$INSTANCE_NAME`_OVER_TEMP_MASK 	0x55 //0b01010101
#define	`$INSTANCE_NAME`_ABS_OVER_TEMP_MASK 0xAA //0b10101010
uint8 `$INSTANCE_NAME`_Check_Temperature( float Temperature, uint8 SensNo);

#endif
/* [] END OF FILE */
