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
#include <otsm.h>

E_SensorMissingPriority g_sensorMissingPriorityArray[4] =
{ E_SensorMissingPriority_Low, E_SensorMissingPriority_Low, E_SensorMissingPriority_Low, E_SensorMissingPriority_Low };

uint8 OTSM_Check_Temperature(float Temperature, int SensNo)
{
    static uint8 OTSM_QualiCntr[4] =
    { 50, 50, 50, 50 };
    static uint8 OTSM_SoftControlReg = 0x00;
    static uint8 OTSM_Status = 0x00;

    switch(g_sensorMissingPriorityArray[SensNo])
    {
    case E_SensorMissingPriority_High:
    case E_SensorMissingPriority_Med:
        /* Stop Heat Alarm 1 */
        if (Temperature >= 370)
        {
            OTSM_SoftControlReg = OTSM_SoftControlReg | (0x01);
        }
        else
        {
            OTSM_SoftControlReg = OTSM_SoftControlReg & ~(0x01);
        }

        /* Safety Relay Alarm 1 */
        if (Temperature >= 420)
        {
            if (!OTSM_QualiCntr[SensNo])
            {
                OTSM_SoftControlReg = OTSM_SoftControlReg | (0x02);
                //OTSM_QualiCntr[0] = 50;
            }
            OTSM_QualiCntr[SensNo]--;
        }
        else
        {
            OTSM_QualiCntr[SensNo] = 50;
            OTSM_SoftControlReg = OTSM_SoftControlReg & ~(0x02);
        }
        break;
    case E_SensorMissingPriority_Low:
        break;
    }

    return OTSM_SoftControlReg;

//    switch(SensNo)
//	{
//		/* PT100 1 */
//		case 0:
//		{
//			/* Stop Heat Alarm 1 */
//			if (Temperature >= 370)
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg | (0x01);
//			}
//			else
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x01);
//			}
//
//			/* Safety Relay Alarm 1 */
//			if (Temperature >= 420)
//			{
//				if (!OTSM_QualiCntr[0])
//				{
//					OTSM_SoftControlReg = OTSM_SoftControlReg | (0x02);
//					//OTSM_QualiCntr[0] = 50;
//				}
//				OTSM_QualiCntr[0]--;
//			}
//			else
//			{
//				OTSM_QualiCntr[0] = 50;
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x02);
//			}
//
//			break;
//		}
//
//		/* PT100 2 */
//		case 1:
//		{
//			/* Stop Heat Alarm 2 */
//			if (Temperature >= 370)
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg | (0x04);
//			}
//			else
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x04);
//			}
//
//			/* Safety Relay Alarm 2 */
//			if (Temperature >= 420)
//			{
//				if (!OTSM_QualiCntr[1])
//				{
//					OTSM_SoftControlReg = OTSM_SoftControlReg | (0x08);
//					//OTSM_QualiCntr[1] = 50;
//				}
//				OTSM_QualiCntr[1]--;
//			}
//			else
//			{
//				OTSM_QualiCntr[1] = 50;
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x08);
//			}
//			break;
//		}
//
//		/* PT100 3 */
//		case 2:
//		{
//			/* Stop Heat Alarm 3 */
//			if (Temperature >= 370)
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg | (0x10);
//			}
//			else
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x10);
//			}
//			/* Safety Relay Alarm 3 */
//			if (Temperature >= 420)
//			{
//				if (!OTSM_QualiCntr[2])
//				{
//					OTSM_SoftControlReg = OTSM_SoftControlReg | (0x20);
//					//OTSM_QualiCntr[2] = 50;
//				}
//				OTSM_QualiCntr[2]--;
//			}
//			else
//			{
//				OTSM_QualiCntr[2] = 50;
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x20);
//			}
//			break;
//		}
//
//		/* PT100 4 */
//		case 3:
//		{
//			/* Stop Heat Alarm 4 */
//			if (Temperature >= 370)
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg | (0x40);
//			}
//			else
//			{
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x40);
//			}
//
//			/* Safety Relay Alarm 4 */
//			if (Temperature >= 420)
//			{
//				if (!OTSM_QualiCntr[3])
//				{
//					OTSM_SoftControlReg = OTSM_SoftControlReg | (0x80);
//					//OTSM_QualiCntr[3] = 50;
//				}
//				OTSM_QualiCntr[3]--;
//			}
//			else
//			{
//				OTSM_QualiCntr[3] = 50;
//				OTSM_SoftControlReg = OTSM_SoftControlReg & (~0x80);
//			}
//			break;
//		}
//		default:
//		{
//
//			break;
//		}
//	}
//    // Check if there is over temperature
//    if (OTSM_SoftControlReg & OTSM_OVER_TEMP_MASK)
//    {
//        OTSM_Status = OTSM_Status | (0x01);
//    }
//    else
//    {
//        OTSM_Status = OTSM_Status & (~0x01);
//    }
//    // Check if there is absolute over temperature
//    if (OTSM_SoftControlReg & OTSM_ABS_OVER_TEMP_MASK)
//    {
//        OTSM_Status = OTSM_Status | (0x02);
//    }
//    else
//    {
//        OTSM_Status = OTSM_Status & (~0x02);
//    }
//    return (OTSM_Status);
}

void OTSM_SetSensorPriority(int SensNo, E_SensorMissingPriority priority)
{
    g_sensorMissingPriorityArray[SensNo] = priority;
}

E_SensorMissingPriority OTSM_GetSensorPriority(int SensNo)
{
    return g_sensorMissingPriorityArray[SensNo];
}

void OTSM_ResetSensorPriority()
{
    OTSM_SetSensorPriority(0, E_SensorMissingPriority_Low);
    OTSM_SetSensorPriority(1, E_SensorMissingPriority_Low);
    OTSM_SetSensorPriority(2, E_SensorMissingPriority_Low);
    OTSM_SetSensorPriority(3, E_SensorMissingPriority_Low);
}

/* [] END OF FILE */
