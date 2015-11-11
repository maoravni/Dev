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
#include "Monitor.h"
#include "iwdg.h"
#include "rtc.h"
#include "can_tst.h"
#include "mod_bus_tst.h"
#include "e2_tst.h"
#include "opto_out_cnfg.h"
#include "opto_out.h"

#ifdef CPP_FRAMEWORK
  #include "logger.h"
#endif


//#ifdef __cplusplus
//extern "C"
//{
//#endif

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

	static int32_t AliveLedCounter=0;
	static int32_t GeneralDbMonitorCounter=0;
	static int32_t TemperatureDbMonitorCounter=0;
	static int32_t TemperDbMntrUpdateCycle;
#ifdef USE_IWDG        
	static int32_t iwdg_reload_count = 0;
#endif

	/*============================================================================*/
	/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
	/*============================================================================*/

uint32_t mon_stack_wm;
void monitor_function()
{
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
	   mon_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif  
			set_task_alive_bit(MONITOR_ALV_E);           

			//#ifndef CPP_FRAMEWORK
			if (++AliveLedCounter>ALIVE_LED_CYCLE)
			{
				AliveLedCounter=0;
				LedAliveToggle();
//                                if (GlobalSubSysId == SUBSYS_BTC)
//                                  dig_out_ctrl(OPTO_OUT1, TOGGLE);
//                                else
//                                  dig_out_ctrl(OPTO_OUT12, TOGGLE);
				//M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"Led update "); //
			}
			//#endif
			
                      
      if (++GeneralDbMonitorCounter>= GENERAL_DB_MONITER_UPDATE_CYCLE)
      {
        GeneralDbMonitorCounter=0;

        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"One second tick"); //for test only
        disp_free_heap_sz();

//#ifndef CPP_FRAMEWORK
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )   
       disp_free_heap_sz();
       disp_stacks_wm();
#endif        
//#endif CPP_FRAMEWORK
                              
        //update the monitor db values of the wall zones
        uint32_t Data=1;
        CfgPutParam(NULL,ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES,&Data,CfgDB_SET_TO_DB);
      }
      
      if (IsInAtpMode()){
        TemperDbMntrUpdateCycle = TEMPERATURE_DB_MONITER_UPDATE_CYCLE_IN_ATP;
      }else{
        TemperDbMntrUpdateCycle = TEMPERATURE_DB_MONITER_UPDATE_CYCLE;
       }
      
       if (TemperatureDbMonitorCounter++ > TemperDbMntrUpdateCycle)
       {
           TemperatureDbMonitorCounter=0;
           //update the monitor db values of the themperature zones
           uint32_t Data=1;
           CfgPutParam(NULL,ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES,&Data,CfgDB_SET_TO_DB);
           CfgPutParam(NULL,ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES,&Data,CfgDB_SET_TO_DB);
       }                     
               
       if (CfgRetGnrlInt(ID_GNRL_CMND_INT_START_CAN_TEST)==1)
       {
//         M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE, "CAN_TST_MNTR CALLED!",0,0,0,0,0);
         can_test_mntr();
       }
      
       if (CfgRetGnrlInt(ID_GNRL_CMND_INT_START_MODBUS_TEST)==1)
       {
         rtu_master_tst_mntr(SLAVE_ID, SLAVE_REG);//, 10);
       }
      
       if (CfgRetGnrlInt(ID_GNRL_CMND_INT_START_EEPROM_TEST)==1)
       {
         eeprom_tst();
       }      
#ifdef USE_IWDG

//			if(iwdg_reload_count++ > ALIVE_LED_CYCLE*4)
//			{
//
//				iwdg_reload_count = 0;
//
//				if( is_force_iwdg_requested()== TRUE )
//					reset_task_alive_bits();
//
//
//				if(is_all_task_alive() == TRUE)
//				{
//					reset_task_alive_bits();
//					IWDG_ReloadCounter();
//				}else{
//					while(1){};/* wait for IWD Reset */
//				}
//			}
#endif
}

//#ifdef __cplusplus
//}
//#endif

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
 #if 0                                                          
                              if (usbConnected && SysLogTimer>=10)
                              {
                                SysLogTimer=0;
                                PrintSyslogData();
                              }
                              else
                                SysLogTimer+=1;
#endif
#if 0
	void Monitor_task(void * pvParameters);

	xTaskHandle MonitorTaskId;

	void Monitor_init(int TaskPriority, int StackSize)
	{
		STATUS RetValue=xTaskCreate( Monitor_task, ( signed char * ) "Monitor", StackSize, NULL, TaskPriority, &MonitorTaskId );
		if (RetValue !=pdPASS)
		{
			// sys log message DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,88 /*PWM TASK IDTBD*/  ,0,-101);

		}
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
#endif

#if 0
extern int usbConnected;
static int SysLogTimer=0;
#endif
