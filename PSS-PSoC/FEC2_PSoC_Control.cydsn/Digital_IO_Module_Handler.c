/*****************************************************************************
* File Name: Digital_IO_Module_Handler.h
*
* Version: 02.02
*
* Description:
*   File to handle the Digital Inputs and outputs.
*
* Note:
********************************************************************************/

/***************************************
*    Includes
***************************************/
#include "main.h"
#include "Digital_IO_Module_Handler.h"
#include "ICDMessageStructs.h"

/***************************************
*    Global Variables
***************************************/
uint8 						StoredPwmValues[NUM_OF_PWM_OUTS];
U_StoredDigiIn4MMCU			StoredDigiIn4MMCU;


/***************************************
*    Function 
***************************************/

/*******************************************************************************
* Function Name: Init_Digital_Module()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Init_Digital_Module(void)
{
	DOD_Start();
	memset(StoredPwmValues,0,sizeof(StoredPwmValues));
	StoredDigiIn4MMCU.Byte = 0;
        DOD_OutConfig(2, 2);
        DOD_OutConfig(3, 3);
        DOD_PWMOut1_Write(0);
        DOD_PWMOut2_Write(0);
}




/*******************************************************************************
* Function Name: void DigitalInputs_Read()
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void DigitalInputs_Read()
{
	StoredDigiIn4MMCU.Struct.DI1 = Digital_input_1_Read();
	StoredDigiIn4MMCU.Struct.DI2 = Digital_input_2_Read();
	StoredDigiIn4MMCU.Struct.DI3 = Digital_input_3_Read();
	StoredDigiIn4MMCU.Struct.DI4 = Digital_input_4_Read();
	StoredDigiIn4MMCU.Struct.DI5 = Digital_input_5_Read();
	StoredDigiIn4MMCU.Struct.DI6 = Digital_input_6_Read();

	StoredDigiIn4MMCU.Struct.GI1 = Global_input_1_Read();
	StoredDigiIn4MMCU.Struct.GI2 = Global_input_2_Read();
	StoredDigiIn4MMCU.Struct.GI3 = Global_input_3_Read();
	StoredDigiIn4MMCU.Struct.GI4 = Global_input_4_Read();
	StoredDigiIn4MMCU.Struct.GI5 = Global_input_5_Read();
	StoredDigiIn4MMCU.Struct.GI6 = Global_input_6_Read();	
}

/* [] END OF FILE */
