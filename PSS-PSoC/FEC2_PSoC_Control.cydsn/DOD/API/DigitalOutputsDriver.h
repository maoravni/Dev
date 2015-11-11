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
	
#define `$INSTANCE_NAME`_MAX_PULSE_DENSITY 	(float) (0xFFFF / 100)

typedef struct
{
	uint8 SlowPWM1:2;
	uint8 SlowPWM2:1;
	uint8 PWM1:1;
	uint8 PWM2:1;
	uint8 HB_PWM1:1;
	uint8 HB_PWM2:1;
}T_`$INSTANCE_NAME`_Configuration;
	
void `$INSTANCE_NAME`_Start(void);

uint8 `$INSTANCE_NAME`_OutConfig(uint8 Channel, uint8 Config);

uint8 `$INSTANCE_NAME`_SlowPWM1_Write(uint8 dc);
uint8 `$INSTANCE_NAME`_SlowPWM2_Write(uint8 dc);
uint8 `$INSTANCE_NAME`_PWMOut1_Write(uint8 dc);
uint8 `$INSTANCE_NAME`_PWMOut2_Write(uint8 dc);
uint8 `$INSTANCE_NAME`_HB_PWMOut1_Write(uint8 dc);
uint8 `$INSTANCE_NAME`_HB_PWMOut2_Write(uint8 dc);

#endif
/* [] END OF FILE */
