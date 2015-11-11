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
/*********************************************************/
/* '$INSTANCE_NAME'_Start():
 * Enables the SOC_Delay
 * Disable the DSM_Seq_Reg
 */
void `$INSTANCE_NAME`_Start()
{    
    `$INSTANCE_NAME`_SOC_Delay_Start();
}

/*********************************************************/
/* '$INSTANCE_NAME'_SOC_Delay_Period_Write(uint8 Period):
 * Write a Period value to the SOC_Delay PWM module
 */
void `$INSTANCE_NAME`_SOC_Delay_SetPeriod(uint8 Period)
{    
    /* Write a Period value to the SOC_Delay PWM module*/
    `$INSTANCE_NAME`_SOC_Delay_WritePeriod(Period); 
}

/*********************************************************/
/* '$INSTANCE_NAME'_SOC_Delay_Compare_Write(uint8 Compare):
 * Write a Compare value to the SOC_Delay PWM module
 */
void `$INSTANCE_NAME`_SOC_Delay_SetCompare(uint8 Compare)
{    
    /* Write a Period value to the SOC_Delay PWM module*/
    `$INSTANCE_NAME`_SOC_Delay_WriteCompare(Compare); 
}

/*********************************************************/
/* '$INSTANCE_NAME'_Stop():
 * Disables the SOC_Delay
 * Disable the DSM_Seq_Reg
 */
void `$INSTANCE_NAME`_Stop()
{
    `$INSTANCE_NAME`_SOC_Delay_Stop();
}
/* [] END OF FILE */
