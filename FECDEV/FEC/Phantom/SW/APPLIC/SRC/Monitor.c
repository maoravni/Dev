/******************************************************************************
 *                     (C) LandaLabs 2011                                      *
 *******************************************************************************
 *
 *  PROJECT     : Dryer RIG
 *
 *  FILE        : monitior.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\APPLIC
 *
 *  PURPOSE     : Monitor/Status peridic task.
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

/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include "stm32f2xx_gpio.h"
#include "free_rtos_exp.h"
#include "ll_typedef.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "..\BSP\bsp_srvcs.h"

#include "CfgDB.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "leds.h"
#include "monitor.h"
#include "iwdg.h"
#include "rtc.h"

#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
void disp_free_heap_sz();
void disp_stacks_wm();
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
/* --- PRIVATE_DATA --------------------------------------------------------- */

static int32_t AliveLedCounter = 0;
static int32_t GeneralDbMonitorCounter = 0;
static int32_t TemperatureDbMonitorCounter = 0;
static int32_t iwdg_reload_count = 0;

static BOOL UpdateModbusDeviceStatusEnable = true; //default
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
// Maor {10 Jan 2013}: Uniting the two monitor tasks:
//void Monitor_task(void * pvParameters);
xTaskHandle MonitorTaskId;

void Monitor_init(int TaskPriority, int StackSize)
{
    STATUS RetValue =
            xTaskCreate( Monitor_task, ( signed char * ) "Monitor", StackSize, NULL, TaskPriority, &MonitorTaskId );
    if (RetValue != pdPASS)
    {
        // sys log message DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,88 /*PWM TASK IDTBD*/  ,0,-101);

    }
}

void EnableUpdateModbusDeviceStatusEnabe(BOOL Enable)
{
    UpdateModbusDeviceStatusEnable = Enable;
}

uint32_t mon_stack_wm;
/*----------------------------------------------------------------------------
 * @brief  Monitor task
 * @param  pvParameters not used
 * @retval None
 *----------------------------------------------------------------------------*/
void Monitor_task(void * pvParameters)
{

    while (1)
    {
        monitor_function();

        vTaskDelay(MONITOR_PERIODIC_MSEC);

        asm volatile("nop");
    }

}/* Monitor_task */

void monitor_function()
{

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
    mon_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif

    set_task_alive_bit(MONITOR_ALV_E);

//#ifndef CPP_FRAMEWORK
    if (++AliveLedCounter >= ALIVE_LED_CYCLE)
    {
        AliveLedCounter = 0;
        LedAliveToggle();
        //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_TRACE,"Led update "); //
    }
//#endif
    if (UpdateModbusDeviceStatusEnable)
    {
        if (++GeneralDbMonitorCounter >= GENERAL_DB_MONITER_UPDATE_CYCLE)
        {
            GeneralDbMonitorCounter = 0;

            M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_TRACE, "One second tick"); //for test only

            disp_free_heap_sz();

            //PrintVtaskList();

#ifndef CPP_FRAMEWORK
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
            disp_free_heap_sz();
            disp_stacks_wm();
#endif
#endif CPP_FRAMEWORK

            //update the monitor db values of the wall zones
            uint32_t Data = 1;
            CfgPutParam(NULL, ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES, &Data, CfgDB_SET_TO_DB);
        }

        if (!InSimulationMode())
        {  //skip real updateing the data in case in simulation mode
            if (++TemperatureDbMonitorCounter >= TEMPERATURE_DB_MONITER_UPDATE_CYCLE)
            {
                TemperatureDbMonitorCounter = 0;
                //update the monitor db values of the themperature zones
                uint32_t Data = 1;
                CfgPutParam(NULL, ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES, &Data, CfgDB_SET_TO_DB);
                CfgPutParam(NULL, ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES, &Data, CfgDB_SET_TO_DB);
            }
        }
    }

#ifdef USE_IWDG

    if(iwdg_reload_count++ >= ALIVE_LED_CYCLE*4)
    {

        iwdg_reload_count = 0;

        if( is_force_iwdg_requested()== TRUE )
        reset_task_alive_bits();

        if(is_all_task_alive() == TRUE)
        {
            reset_task_alive_bits();
            IWDG_ReloadCounter();
        }
        else
        {
            while(1)
            {};/* wait for IWD Reset */
        }
    }
#endif
}

#ifdef __cplusplus
}
#endif

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
