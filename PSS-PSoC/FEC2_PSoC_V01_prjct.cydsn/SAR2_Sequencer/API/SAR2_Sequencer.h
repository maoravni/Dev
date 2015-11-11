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

void `$INSTANCE_NAME`_Start();
void `$INSTANCE_NAME`_Stop();
void `$INSTANCE_NAME`_SOC_Delay_SetPeriod(uint8 Period);
void `$INSTANCE_NAME`_SOC_Delay_SetCompare(uint8 Compare);


#endif
//[] END OF FILE
