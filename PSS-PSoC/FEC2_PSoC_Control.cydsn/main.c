/* ======================================== *
* Copyright YOUR COMPANY, THE YEAR
* All Rights Reserved
Hardware version 1.8.01
* ========================================
*/
/***************************************
*    Includes
***************************************/

#include <device.h>
#include <stdio.h>

#include "main.h"
#include "Temperature_Module_Handler.h"
#include "System_Handler.h"
#include "Analog_IO_Module_Handler.h"
#include "Monitor_Module_Handler.h"
#include "Digital_IO_Module_Handler.h"
#include "DSP.h"

/***************************************
*    Global Variables
***************************************/

/**********************************************************
*    Function Prototypes
**********************************************************/
void PwrUp();
void PBIT();

/**********************************************************/
int main()
{
    GREEN_LED_Write(1);
    RED_LED_Write(1);
    PwrUp();
    GREEN_LED_Write(1);
    RED_LED_Write(0);
    Init();
    GREEN_LED_Write(0);
    RED_LED_Write(1);
    GREEN_LED_Write(0);
    RED_LED_Write(0);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	/* FOREVER LOOP */
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	for (;;)
	{
		//1. Check if Temperatures ADC is done		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		/* if the DSM_DMA Done */
		if (DSM_DMADone_flag)
		{
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_5_DAC_VALUE);
			#endif	
			/* Call a function to recive the pointer of the update temperature array */
			Calc_Temperature();
			/* Clear interrupt flag */
			DSM_DMADone_flag = 0;

			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_0_DAC_VALUE);
			#endif
		}

		//2. Check if Analog inputs SAR1 is done		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
		/* if the SAR1_AnlgIn_DMA Done */
		if (SAR1_AnlgIn_DMADone_flag)
		{
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_6_DAC_VALUE);
			#endif	

			/* Call a function to recive the pointer of the update temperature array */
			Calc_Currents();
			/* Clear interrupt flag */
			SAR1_AnlgIn_DMADone_flag = 0;

			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_0_DAC_VALUE);
			#endif	
		}

		//3. Check if Corrent lopps inputs SAR2 is done			
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* CS_DAB_1_P_Data_Ready */
		if (Peak_Event_Flag)
		{
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_1_DAC_VALUE);
			#endif				
				
			Current_Sensors_Analysis();
			Peak_Event_Flag = 0;
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_0_DAC_VALUE);
			#endif	
		}
		
		if (Zero_Event_Flag)
		{
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_1_DAC_VALUE);
			#endif				

			Current_Sensors_GetRef();
			Zero_Event_Flag = 0 ;
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_0_DAC_VALUE);
			#endif	
		}
		
		//4. Check if Cable ID's inputs SAR2 is done		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		      
		/* if the SAR2_IDs_DMADone_flag */
		if (SAR2_IDs_DMADone_flag)
		{
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_4_DAC_VALUE);
			#endif
			/* */
			Calc_Cable_IDs();
			/* Clear interrupt flag */
			SAR2_IDs_DMADone_flag = 0;
			#ifdef TASKS_DEBUG
			Task_Indctr_VDAC_SetValue(TASK_0_DAC_VALUE);
			#endif	
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Check if software is runing.  
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		CyWdtClear();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	}
}
void PwrUp()
{
	Vsupply = 23.3;
	/* Initialize the System_Control_Reg */
	System_Control_Reg_Write(DISABLE);
	/* Start watchdog */
	CyWdtStart(CYWDT_128_TICKS, CYWDT_LPMODE_NOCHANGE); // 256-384 ms	
}

void Init(void)
{
	uint8 savedIntrStatus;
	/* Disable global interrupts */
	savedIntrStatus = CyEnterCriticalSection();
	CyGlobalIntDisable;
	/* Initiate the System */
	Init_System();
	/* Initiate the Temperature sensing module */
	Init_Temperature_Module();
	/* Initiate the Analog sensing module */
	Init_Analog_Module();
	/* Initiate the monitor module */
	Init_Monitor_Module();
	/* Initiate the Digital module */
	Init_Digital_Module();
	/* Enable global interrupts */
	CyExitCriticalSection(savedIntrStatus);
	CyGlobalIntEnable;
	/* Start the DSM conversion */
	DSM_Sequencer_Enable();
	/* Start the SAR1 and SAR2 conversion */
	SAR1_soc_Enable();
	SAR2_soc_Enable();
	/* Indication for which task is executed */
	Task_Indctr_VDAC_Start();
}

void Check_Preemption()
{
	//1. Check if Temperatures ADC is done		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	/* if the DSM_DMA Done */
	if (DSM_DMADone_flag)
	{
#ifdef TASKS_DEBUG
		Task_Indctr_VDAC_SetValue(TASK_5_DAC_VALUE);
#endif

		/* Call a function to recive the pointer of the update temperature array */
		Calc_Temperature();
		/* Clear interrupt flag */
		DSM_DMADone_flag = 0;

#ifdef TASKS_DEBUG
		Task_Indctr_VDAC_SetValue(TASK_1_DAC_VALUE);
#endif
	}

	//2. Check if Analog inputs SAR1 is done		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
	/* if the SAR1_AnlgIn_DMA Done */
	if (SAR1_AnlgIn_DMADone_flag)
	{
#ifdef TASKS_DEBUG
		Task_Indctr_VDAC_SetValue(TASK_6_DAC_VALUE);
#endif	

		/* Call a function to recive the pointer of the update temperature array */
		Calc_Currents();
		/* Clear interrupt flag */
		SAR1_AnlgIn_DMADone_flag = 0;

#ifdef TASKS_DEBUG
		Task_Indctr_VDAC_SetValue(TASK_1_DAC_VALUE);
#endif	
	}

	//3. Check if Corrent lopps inputs SAR2 is done			
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		      


	//4. Check if Cable ID's inputs SAR2 is done		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		      


	//5. Check if Cable ID's inputs SAR2 is done		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		      

}
/* [] END OF FILE */









