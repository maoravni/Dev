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


uint8 `$INSTANCE_NAME`_Check_Temperature( float Temperature, uint8 SensNo)
{
	static uint8 `$INSTANCE_NAME`_QualiCntr[4] = {`$Qualifier_Counter_1`,`$Qualifier_Counter_2`,`$Qualifier_Counter_3`,`$Qualifier_Counter_4`};
	static uint8 `$INSTANCE_NAME`_SoftControlReg = 0x00;
	static uint8 `$INSTANCE_NAME`_Status = 0x00;
	
	switch(SensNo)
	{
		/* PT100 1 */
		case 0:
		{
			/* Stop Heat Alarm 1 */
			if (Temperature >= `$Stop_Heat_Limit_1`)
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x01);
			}
			else
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x01);
			}
			
			/* Safety Relay Alarm 1 */
			if (Temperature >= `$Safety_Relay_Limit_1`)
			{
				if (!`$INSTANCE_NAME`_QualiCntr[0])
				{
					`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x02);
					//`$INSTANCE_NAME`_QualiCntr[0] = `$Qualifier_Counter_1`;
				}
				`$INSTANCE_NAME`_QualiCntr[0]--;
			}
			else
			{
				`$INSTANCE_NAME`_QualiCntr[0] = `$Qualifier_Counter_1`;
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x02);
			}			

			break;
		}
		
		/* PT100 2 */
		case 1:
		{
			/* Stop Heat Alarm 2 */
			if (Temperature >= `$Stop_Heat_Limit_2`)
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x04);
			}
			else
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x04);
			}
			
			/* Safety Relay Alarm 2 */
			if (Temperature >= `$Safety_Relay_Limit_2`)
			{
				if (!`$INSTANCE_NAME`_QualiCntr[1])
				{
					`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x08);
					//`$INSTANCE_NAME`_QualiCntr[1] = `$Qualifier_Counter_2`;
				}
				`$INSTANCE_NAME`_QualiCntr[1]--;
			}
			else
			{
				`$INSTANCE_NAME`_QualiCntr[1] = `$Qualifier_Counter_2`;
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x08);
			}			
			break;
		}
		
		/* PT100 3 */
		case 2:
		{
			/* Stop Heat Alarm 3 */
			if (Temperature >= `$Stop_Heat_Limit_3`)
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x10);
			}
			else
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x10);
			}
			/* Safety Relay Alarm 3 */
			if (Temperature >= `$Safety_Relay_Limit_3`)
			{
				if (!`$INSTANCE_NAME`_QualiCntr[2])
				{
					`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x20);
					//`$INSTANCE_NAME`_QualiCntr[2] = `$Qualifier_Counter_3`;
				}
				`$INSTANCE_NAME`_QualiCntr[2]--;
			}
			else
			{
				`$INSTANCE_NAME`_QualiCntr[2] = `$Qualifier_Counter_3`;
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x20);
			}			
			break;
		}
		
		/* PT100 4 */
		case 3:
		{
			/* Stop Heat Alarm 4 */
			if (Temperature >= `$Stop_Heat_Limit_4`)
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x40);
			}
			else
			{
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x40);
			}
			
			/* Safety Relay Alarm 4 */
			if (Temperature >= `$Safety_Relay_Limit_4`)
			{
				if (!`$INSTANCE_NAME`_QualiCntr[3])
				{				
					`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg | (0x80);
					//`$INSTANCE_NAME`_QualiCntr[3] = `$Qualifier_Counter_4`;
				}
				`$INSTANCE_NAME`_QualiCntr[3]--;
			}
			else
			{
				`$INSTANCE_NAME`_QualiCntr[3] = `$Qualifier_Counter_4`;
				`$INSTANCE_NAME`_SoftControlReg = `$INSTANCE_NAME`_SoftControlReg & (~0x80);
			}			
			break;
		}
		default:
		{
		
			break;
		}
	}
	// Check if there is over temperature
	if(`$INSTANCE_NAME`_SoftControlReg & `$INSTANCE_NAME`_OVER_TEMP_MASK)
	{
		`$INSTANCE_NAME`_Status = `$INSTANCE_NAME`_Status | (0x01);	
	}
	else
	{
		`$INSTANCE_NAME`_Status = `$INSTANCE_NAME`_Status & (~0x01);		
	}
	// Check if there is absolute over temperature
	if(`$INSTANCE_NAME`_SoftControlReg & `$INSTANCE_NAME`_ABS_OVER_TEMP_MASK)
	{
		`$INSTANCE_NAME`_Status = `$INSTANCE_NAME`_Status | (0x02);	
	}
	else
	{
		`$INSTANCE_NAME`_Status = `$INSTANCE_NAME`_Status & (~0x02);		
	}
	return(`$INSTANCE_NAME`_Status);
}

/* [] END OF FILE */
