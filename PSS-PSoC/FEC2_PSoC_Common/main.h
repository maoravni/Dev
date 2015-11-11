/*****************************************************************************
* File Name: main.h
*
* Version: 1.1
*
* Description:
*   Header for main.
*
* Note:  Testing the Team explorer 
********************************************************************************/

#if !defined(main_H)
#define main_H
#endif

/***************************************
*    Defines
***************************************/


//#define TASKS_DEBUG
#define REAL_HEATER
//#define CURRENT_LOOPS_DEBUG

#define 	VDD					5					
#define     SAFETY_ALARM        0x01

#define 	ZERO				0

#define     HIGH        		1
#define     LOW        			0

#define 	TRUE				HIGH
#define 	FALSE				LOW

#define		ENABLE				HIGH
#define		DISABLE				LOW

#define 	ON					TRUE
#define 	OFF					FALSE

#define 	GET_BOARD_TYPE		Board_Type_Sel_Reg_Read()
#define     _16BIT1				0x0001
#define     _16BIT2				0x0002
#define     _16BIT3				0x0004
#define     _16BIT4				0x0008
#define     _16BIT5				0x0010
#define     _16BIT6				0x0020
#define     _16BIT7				0x0040
#define     _16BIT8				0x0080

#define     _16BIT9				0x0100
#define     _16BIT10			0x0200
#define     _16BIT11			0x0400
#define     _16BIT12			0x0800
#define     _16BIT13			0x1000
#define     _16BIT14			0x2000
#define     _16BIT15			0x4000
#define     _16BIT16			0x8000



#define TASK_0_DAC_VALUE		0x00
#define TASK_1_DAC_VALUE		0x1F
#define TASK_2_DAC_VALUE		0x3E
#define TASK_3_DAC_VALUE		0x5D
#define TASK_4_DAC_VALUE		0x7C
#define TASK_5_DAC_VALUE		0x9B
#define TASK_6_DAC_VALUE		0xBA
#define TASK_7_DAC_VALUE		0xD9
#define TASK_8_DAC_VALUE		0xF8

//#define 	TEMPS_DEBUG
/***************************************
*    Function Prototypes
***************************************/

/*******************************************************************************
* Function Name: InitSystem()
********************************************************************************
* Summary:
*   InitSystem will Initiate any subsystem in turn.
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void InitSystem(void);

/*******************************************************************************
* Function Name: void Check_Preemption(void)
********************************************************************************
* Summary:
*   InitSystem will Initiate any subsystem in turn.
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Check_Preemption(void);
//[] END OF FILE
