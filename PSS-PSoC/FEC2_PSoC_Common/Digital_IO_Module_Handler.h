/*****************************************************************************
* File Name: Digital_IO_Module_Handler.h
*
* Version: 02.02
*
* Description:
*   File to handle the Digital Inputs and outputs.
*
* Notes:
*
* Components Summary:
*   1. An input for the "Safety_Relay"
*   2. 11 more inputs
*   3. 2 indicating LED outputs.
* Fnctions Summary:
*   1. 
* Note:
*
********************************************************************************/

#include "device.h"

/***************************************
*    Defines
***************************************/

#define NUM_OF_PWM_OUTS 6


typedef struct
{
    uint16 DI1:1;
	uint16 DI2:1;
	uint16 DI3:1;
	uint16 DI4:1;
	uint16 DI5:1;
	uint16 DI6:1;
	
	uint16 GI1:1;
	uint16 GI2:1;
	uint16 GI3:1;
	uint16 GI4:1;
	uint16 GI5:1;
	uint16 GI6:1;
	
	uint16 Spare:4;
}T_StoredDigiIn4MMCU;

typedef union
{
	T_StoredDigiIn4MMCU Struct;
	uint16 				Byte;
}U_StoredDigiIn4MMCU;

/***************************************
*    Extern Variables
***************************************/
extern U_StoredDigiIn4MMCU			StoredDigiIn4MMCU;
extern uint8 						CurrentPwmValues[NUM_OF_PWM_OUTS];

/***************************************
*    Function Prototypes
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
void Init_Digital_Module(void);

/*******************************************************************************
* Function Name: PWMout_WriteCompare2All(uint8 * dcs)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void PWMout_WriteCompare2All(uint8 * dcs);

/*******************************************************************************
* Function Name: void DigitalInputs_Read(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void DigitalInputs_Read(void);

uint8 DigitalOutputsForceWrite(uint8 channelIndex, uint8 outputValue);
uint8 DigitalOutputsWrite(uint8 channelIndex, uint8 outputValue);
void ConfigureSoftStartParameters(uint8 channelIndex, uint8 riseStepSize, uint8 fallStepSize, uint8 riseTickCount, uint8 fallTickCount);

void ExecuteOutputRamp();


//[] END OF FILE
