/******************************************************************************
 *                     (C) LandaLabs 2011                                      *
 *******************************************************************************
 *
 *  PROJECT     : Dryer RIG
 *
 *  FILE        : Pwm.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\APPLIC
 *
 *  PURPOSE     : PWM task.
 *
 *  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.303
 *
 *  TARGET      : Phantom
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.08    Yossi Sarusi      1.1.0   First version
 *******************************************************************************
 Copyright and Proprietary Statment.

 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define MAX_NUM_OF_PWM_ELEMENTS  12                 
#define PWM_CLOCK    10								// 10 msec - tick clock   
#define PWM_DEFAULT_CYCLE       100    // 1 second

/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include "stm32f2xx_gpio.h"
#include "free_rtos_exp.h"
#include "ll_typedef.h"
#include "iwdg.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "..\BSP\bsp_srvcs.h"

#include "CfgDB.h"
#include "GnrlCfgDB.h"
#ifdef FEC_BOARD
#include "opto_out_cnfg.h"
#endif
#include  "opto_out.h"
//#include "Fec.h"
#include  "pwm.h"

#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif

//#ifdef __cplusplus
//extern "C"
//{
//#endif

/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */

STATUS Pwm_CreateElement(int ElementId, U32 DelayPeriod, U8 PercentDutyCycle, U32 TotalPeriod);

void Pwm_task(void * pvParameters);
/* --- PRIVATE_DATA --------------------------------------------------------- */

static xSemaphoreHandle PwmSemaphoreHand = NULL;
int PmwDutyCycleIn10ms;

//Pwm sampling data mutex
static bool PwmWaitSem()
{
    //LOG_ADD_SYS_LOG_MESSAGE(LOG_PWM_SEM_MODULE,0,0,0,NULL);
    if (PwmSemaphoreHand != NULL)
    {

        if (xSemaphoreTake( PwmSemaphoreHand, ( portTickType )100 ) == pdFALSE) // to avoid dead lock
        {
            //LOG_ADD_SYS_LOG_MESSAGE(LOG_PWM_SEM_MODULE,11,0,0,NULL);
            return false; //time out error
        }
        else
        {
            //LOG_ADD_SYS_LOG_MESSAGE(LOG_PWM_SEM_MODULE,1,0,0,NULL);
            return true;
        }
    }
    return false; //error
}

bool PwmGiveSem(bool FromIsr)
{

    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (PwmSemaphoreHand != NULL)
    {
        if (FromIsr)
        {
            //LOG_ADD_SYS_LOG_MESSAGE(LOG_PWM_SEM_MODULE,2,0,0,NULL);
            xSemaphoreGiveFromISR( PwmSemaphoreHand, &xHigherPriorityTaskWoken);
        }
        else
        {
            //LOG_ADD_SYS_LOG_MESSAGE(LOG_PWM_SEM_MODULE,3,0,0,NULL);
            xSemaphoreGive( PwmSemaphoreHand);
        }

        /* Switch tasks if necessary. */
        if (xHigherPriorityTaskWoken != pdFALSE)
        {
            portEND_SWITCHING_ISR( xHigherPriorityTaskWoken);
        }

        return true;
    }
    return false; //error
}

typedef struct
{
    BOOL Occupied;
    BOOL Enable;
    BOOL DualPower;
    int ID;
    U32 SetPulseValue;  //set the output pin when the CurrentCounter raech this value
    U32 ResetPulseValue;  //reset the output pin when the CurrentCounter raech this value
    U32 TotalPeriodValue; //reset the CurrentCounter when the CurrentCounter raech this value
    int CurrentCounter;  //Pwm current counter, icrement by one on each tick
//GPIO_TypeDef*	OutputPortId;    // output port of the PWM
//uint16_t		OutputPinId;     // output pint of the PWM
} PWM_ElementDescription;

static PWM_ElementDescription PwmData[MAX_NUM_OF_PWM_ELEMENTS];

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
xTaskHandle PwmTaskId;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */

void PWM_init(int TaskPriority, int StackSize)
{
    if (GlobalSubSysId == SUBSYS_BTC || GlobalSubSysId == SUBSYS_CONDITIONER)
        PmwDutyCycleIn10ms = 10; //100 ms  for cooling Fill pump
    else if (GlobalSubSysId ==SUBSYS_ITH)
        PmwDutyCycleIn10ms = 50; //100 ms for ITH heaters
    else
        PmwDutyCycleIn10ms = 100; //1 second

    vSemaphoreCreateBinary( PwmSemaphoreHand);

    for (int Element = 0; Element < MAX_NUM_OF_PWM_ELEMENTS; Element++)
        Pwm_CreateElement(Element, 0, 0, PmwDutyCycleIn10ms);

    STATUS RetValue = xTaskCreate( Pwm_task, ( signed char * ) "PWM", StackSize, NULL, TaskPriority, &PwmTaskId );
    if (RetValue != pdPASS)
    {
        // sys log message DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,88 /*PWM TASK IDTBD*/  ,0,-101);

    }

}

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
STATUS Pwm_CreateElement(int ElementId, U32 DelayPeriod, U8 PercentDutyCycle, U32 TotalPeriod)
{
    assert_param(ElementId<MAX_NUM_OF_PWM_ELEMENTS);
    assert_param(PercentDutyCycle<=100);

    if (PwmData[ElementId].Occupied)
        return LL_ERROR ;

    PwmData[ElementId].Occupied = true; //mark it as used
    PwmData[ElementId].Enable = false;
    PwmData[ElementId].DualPower=false;
    PwmData[ElementId].SetPulseValue = DelayPeriod;
    PwmData[ElementId].ResetPulseValue = DelayPeriod + (PercentDutyCycle * TotalPeriod) / 100;
    PwmData[ElementId].TotalPeriodValue = TotalPeriod;
    PwmData[ElementId].CurrentCounter = -1;
//	PwmData[ElementId].OutputPortId=DO_PortOf[ElementId];
//	PwmData[ElementId].OutputPinId=DO_PinOf[ElementId];

    return LL_OK ;
}

STATUS Pwm_Enable(int ElementId, BOOL Enable)
{
    assert_param(ElementId<MAX_NUM_OF_PWM_ELEMENTS);

    PwmWaitSem();

    PwmData[ElementId].Enable = Enable;

    if (!Enable)
    {
        dig_out_ctrl(ElementId, DEACTIVE); //turn it off
        if (PwmData[ElementId].DualPower)
            dig_out_ctrl(ElementId+1, DEACTIVE); ////turn its successor off
    }
    //PwmData[ElementId].OutputPortId->BSRRL = PwmData[ElementId].OutputPinId;  //turn it off

    PwmGiveSem(false);

    return LL_OK ;
}

BOOL IsPwm_Enable(int ElementId)
{
    return PwmData[ElementId].Enable;
}

STATUS Pwm_DualPower(int ElementId, BOOL Enable)
{
    assert_param(ElementId<MAX_NUM_OF_PWM_ELEMENTS);

    PwmWaitSem();

    PwmData[ElementId].DualPower = Enable;

    PwmGiveSem(false);

    return LL_OK ;
}

STATUS Pwm_SetDutyCycle(int ElementId, U8 PercentDutyCycle)
{
    assert_param(ElementId<MAX_NUM_OF_PWM_ELEMENTS);
    assert_param(PercentDutyCycle<=100);

    PwmWaitSem();

    if (PwmData[ElementId].Enable)
        PwmData[ElementId].ResetPulseValue = PwmData[ElementId].SetPulseValue
                + (PercentDutyCycle * PwmData[ElementId].TotalPeriodValue) / 100;

    PwmGiveSem(false);

    return LL_OK ;
}

/*

 if (Data==0)
 SSR_PortOf[Index]->BSRRL = SSR_PinOf[Index];  //turn it off
 else
 SSR_PortOf[Index]->BSRRH = SSR_PinOf[Index];  //turn it on
 */
uint32_t pwm_stack_wm;
/*----------------------------------------------------------------------------
 * @brief  Pwm_task
 * @param  pvParameters not used
 * @retval None
 *----------------------------------------------------------------------------*/
void Pwm_task(void * pvParameters)
{

    while (1)
    {

        PwmWaitSem();

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
        pwm_stack_wm = uxTaskGetStackHighWaterMark(PwmTaskId);
#endif
        set_task_alive_bit(PWM_ALV_E);
        static PWM_ElementDescription * PwmElementPtr;

        /* TERMINAL TEST */
        //static   int8_t     msg_buf[255];
        static uint32_t counter = 0;
        static uint32_t counter_10ms = 0;

        if (counter_10ms++ == PmwDutyCycleIn10ms)
        // if ((ElementNum==0) && (PwmElementPtr->CurrentCounter==10))
        {
            counter += 1;
            counter_10ms = 0;

        }

        for (int ElementNum = 0; ElementNum < MAX_NUM_OF_PWM_ELEMENTS; ElementNum++)
        {
            PwmElementPtr = &PwmData[ElementNum];

            if (PwmElementPtr->Enable == FALSE)
                continue;

            PwmElementPtr->CurrentCounter += 1; //update Pwm counter

            if ((PwmElementPtr->ResetPulseValue == 0)
                    || ((PwmElementPtr->ResetPulseValue <= 90)
                            && (PwmElementPtr->CurrentCounter >= PwmElementPtr->ResetPulseValue)))
            {
                dig_out_ctrl(ElementNum, DEACTIVE); //turn it off
                if (PwmElementPtr->DualPower)
                            dig_out_ctrl(ElementNum+1, DEACTIVE); //turn its successor off
            }
            //(PwmElementPtr->OutputPortId)->BSRRL = PwmElementPtr->OutputPinId;
            else if ((PwmElementPtr->ResetPulseValue > 90)
                    || ((PwmElementPtr->ResetPulseValue != 0)
                            && (PwmElementPtr->CurrentCounter == PwmElementPtr->SetPulseValue)))
            {
                dig_out_ctrl(ElementNum, ACTIVE); //turn it on
                if (PwmElementPtr->DualPower)
                    dig_out_ctrl(ElementNum+1, ACTIVE); //turn its successor on
            }
            //(PwmElementPtr->OutputPortId)->BSRRH = PwmElementPtr->OutputPinId;  //turn it on

            if (PwmElementPtr->CurrentCounter >= PwmElementPtr->TotalPeriodValue)
            {
                PwmElementPtr->CurrentCounter = -1;  //reset couter
                //(PwmElementPtr->OutputPortId)->BSRRL = PwmElementPtr->OutputPinId;  //turn it off
            }

        }

        PwmGiveSem(false);

        vTaskDelay(PWM_CLOCK);

        asm volatile("nop");

    }/* while(1) */

}/* Monitor_task */

//#ifdef __cplusplus
//}
//#endif

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
