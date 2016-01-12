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

#if !defined (OTSM_H )
#define OTSM_H

typedef enum
{
    /**
     * High sensor priority means that we check temperature limits and also close outputs when the sensor is missing.
     * Med sensor priority means that we check temperature limits, but don't close outputs when the sensor is missing.
     * Low sensor priority means that we don't check temperature limits and don't close outputs when the sensor is missing.
     */
    E_SensorMissingPriority_High,
    E_SensorMissingPriority_Med,
    E_SensorMissingPriority_Low,
} E_SensorMissingPriority;

#define OTSM_OVER_TEMP_MASK 	0x55 //0b01010101
#define	OTSM_ABS_OVER_TEMP_MASK 0xAA //0b10101010

uint8 OTSM_Check_Temperature( float Temperature, int SensNo);
void OTSM_ResetSensorPriority();
void OTSM_SetSensorPriority(int SensNo, E_SensorMissingPriority priority);
E_SensorMissingPriority OTSM_GetSensorPriority(int SensNo);

#endif
/* [] END OF FILE */
