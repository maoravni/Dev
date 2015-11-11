/*****************************************************************************
* File Name: Analog_IO_Module_Handler.h
*
* Version: 02.02
*
* Description:
*   This file has the following tasks:
*   1. handle the MUXing of the analog inputs to SAR1 which includes:
*       a. The voltage across a current to voltage converting resistor, 
           in order to measure a 4-20mA sensor output.
        b. The voltage across a current to voltage converting resistor,
           in order to measure the current flowing in a the digital output FET driver.
*   2. Providing the 4-20mA current output driver with a controling voltage. 
*
* Note:
*   Safety_feedback pin- TBD
*
* Components Summary:
*   1. SAR1_AMux- MUXing the input voltages.
*   2. SAR1_Flwr- An input buffer.
*   3. SAR1- ADC_SAR.
*   4. VDAC1- controling the current in the 4-20mA output.
*   5. VDAC2- controling the current in the 4-20mA output.
*   6. VDAC1_Flwr- An Output buffer. 
*   7. VDAC2_Flwr- An Output buffer.
* Fnctions Summary:
********************************************************************************/

#include "device.h"

/*******************************************************************************
* Function Name: void Init_Safety_Module(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Init_Safety_Module(void);

/*******************************************************************************
* Function Name: void Safety_ISRs_Relocate(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Safety_ISRs_Relocate(void);

//[] END OF FILE
