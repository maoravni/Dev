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

void `$INSTANCE_NAME`_Init(void);
void `$INSTANCE_NAME`_SetMode(uint8 Mode);
void `$INSTANCE_NAME`_Write_DC(uint8 DC);
uint8 `$INSTANCE_NAME`_Read_DC(void);
#endif
/* [] END OF FILE */
