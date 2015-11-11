/*****************************************************************************
* File Name: Analog_IO_Module_Handler.c
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
********************************************************************************/

/***************************************
*    Includes
***************************************/
#include "main.h"
#include "Safety_Module_Handler.h"
#include "System_Handler.h"
#include "Communications_Handler.h"
#include "DSP.h"

/***************************************
*    Global Variables
***************************************/
/* Variable declarations for DMA . 
* These variables are defined as global variables to avoid "may be used before being set" warning 
* issued by the PSoC 5 compilers  MDK/RVDS.In this case these variables are automatically initialized to zero */




/*******************************************************************************
* Function Name: void Init_Safety_Module(void)
********************************************************************************/

void Init_Safety_Module(void)
{
	/* Start the Safety_Relay Interrupt module */
	isr_Safety_Relay_Start();
	/* Start the LogicFunc Interrupt module */
	isr_LogicFunc_Start();
    /* Relocate the ISRs of the Safety Module */
    void Safety_ISRs_Relocate(void);	
}

/* [] END OF FILE */



















