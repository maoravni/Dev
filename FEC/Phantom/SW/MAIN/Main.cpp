/*
 * Main.cpp
 *
 *  Created on: 1 бреб 2012
 *      Author: maora
 */

//#include "aux_term.h"
#ifndef WIN32
#include "bsp_srvcs.h"
#endif // WIN32
#include "CFreeRTOS.h"
#include "statusled.h"
//#include "auxterm.h"

//#include "TcpConnection.h"
#include "CTcpConnector.h"
#include "CLogger.h"
#include "TmcMasterServer.h"
#include "os_cnfg.h"
#include "Pwm.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "GnrlCfgDB.h"
#include "modbus_exp.h"
#include "cfg_db_if.h"
#include "Monitor.h"
#include "iwdg.h"
#include "rtc.h"
#include "logger.h"
//#include "modbusTask/modbusinit.h"
#include "aux_term.h"
#include "CfgDB.h"
#include "LogSys.h"
#include "netconf.h"//Igor 20121206
#include "stm32f2x7_eth_bsp.h"//Igor 20121206
#include "PeriodicMonitorTask.h"

#include "vcp_term.h"
#include "usbd_cdc_vcp.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "vcp_srvcs.h"

extern USB_OTG_CORE_HANDLE USB_OTG_dev;
long SysTickCounter;

//static CHelloWorld hello(1234);

//TBD should be moved to Chamber.c
static void TemperatureInit()
{
    TemperatureCfgInit();  //DB init
    while (!TemperatureCfgDbReady())
    {
    }; //wait for ready

    /*Due the fact that we need to start with Zone disable state and the PUMA devices keep the latest enable setting
     we need to force the PUMA into disable state*/
    U32 Data = FALSE;
    for (int Index = 0; Index < NUMBER_OF_TEMPERATURE_ZONES; Index++)
        CfgPutParam(0, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,Index), &Data,
                (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);

}

static void GenrlInit()
{
    GnrlCfgInit();  //DB init
    while (!GnrlCfgDbReady())
    {
    };

}

static void OthersInit()
{
    OthersCfgInit();  //DB init
    while (!OthersCfgDbReady())
    {
    };

}

void checkAllOutputs()
{
    for (int i = 0; i < NUMBER_OF_OPTO_OUTS; ++i)
    {
        dig_out_ctrl((OPTO_OUT_ID_ET) i, ACTIVE);
        vTaskDelay(50);
        dig_out_ctrl((OPTO_OUT_ID_ET) i, DEACTIVE);
    }
}
/*----------------------------------------------------------------------------
 * @brief  Init_task task
 * @param  pvParameters not used
 * @retval None
 *----------------------------------------------------------------------------*/
void Init_task(void * pvParameters)
{
    //checkAllOutputs();
    /*  PWM task */
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling PWM_Init");
    PWM_init(PWM_TASK_PRIORITY, PWM_TASK_STACK_SZ);
//init the DB

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling TemperatureInit");
    TemperatureInit();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling OthersInit");
    OthersInit();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling GenrlInit");
    GenrlInit();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling cfg_db_if_init");
    cfg_db_if_init();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling tcp_mb_init");
    tcp_mb_init();

    /*  Monitor task */
    //M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Calling Monitor_init");
    // Maor {10 Jan 2013}: Removed the monitor task since it's now called from the TmcServer monitor task:
    //Monitor_init(MONITOR_TASK_PRIORITY, MONITOR_TASK_STACK_SZ);
    // Maor {13 Jan 2013}: Start the C++ Monitor Task:
    PeriodicMonitorTask::getInstance();

    //turn off  the safetry alarm
    U32 Data = 0;
    CfgPutParam(INSTANCE_GNRL, ID_GNRL_MNTR_INT_CRITICAL_ALARM, &Data, (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);

    LOG_SYS_Init();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling iwdg_init");
    iwdg_init();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling RCC_ClearFlag");
    RCC_ClearFlag();/* Clear reset flags */

    // start the server task:
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Starting the TMC Master Server");

    //RTC Init
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling rtc_init");
    //rtc_init();

    //delete the init task
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Calling vTaskDelete");
    vTaskDelete(NULL);

}/* Init_task */

int main()
{
    /*!< At this stage the microcontroller clock setting is already configured to
     120 MHz, this is done through SystemInit() function which is called from
     startup file (startup_stm32f2xx.s) before to branch to application main.
     To reconfigure the default setting of SystemInit() function, refer to
     system_stm32f2xx.c file
     */
    prvSetupHardware();

    //ActiveSimulationMode(true);

    ETH_BSP_Config();/* configure ethernet (GPIOs, clocks, MAC, DMA) */

    // the status led task.
    //StatusLed::getInstance(); //the keep alive LED is handeling by the monitor task

    /* Initilaize the LwIP stack */
    LwIP_Init();

    //USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);

    TmcMasterServer::getInstance();
    // setup the logger
    static CLogger& loggerInstance = CLogger::getInstance();

    loggerInstance.enableOutputTcp(true);
    loggerInstance.enableLogFreeHeap(true);
    //loggerInstance.enableLogTasks(true);
    //loggerInstance.enableOutputStandardPrintf(true);
    //loggerInstance.enableOutputUart(true);
    //loggerInstance.enableOutputUsbVcp(true);
    //loggerInstance.enableOutputSum(true);

    //hello.create("Hello World", configMINIMAL_STACK_SIZE, 0);
    portBASE_TYPE res;

    //usb_term_init(AUX_TERM_QUEUE_SZ, USB_TASK_STACK_SZ, USB_TASK_PRIORITY);

//    res = aux_term_init(AUX_TERM_QUEUE_SZ, TERMINAL_TASK_STACK_SZ, TERMINAL_TASK_PRIORITY );/* Keep it first! */
//    if (!res)
//    {
//        while (1)
//        {
//        };
//    }

    // ModBus Init
    mb_node_1 = mb_master_node_create(MB_1);
    res = mb_master_node_init("mb_1_master", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN, mb_node_1);
    if (!res)
    {
        while (1)
        {
        };
    }

    change_ip_addr(172, 30, 30, 59);

    STATUS RetValue =
            xTaskCreate( Init_task, ( signed char * ) "Init", INIT_TASK_STACK_SZ, NULL, INIT_TASK_PRIORITY, NULL );
    if (RetValue != pdPASS)
    {
        while (1)
        {
        };
    }

    // start the FreeRTOS scheduler.
    CFreeRTOS::initHardwareForManagedTasks();
    CFreeRTOS::startScheduler();

    /* Should never get here as control is now taken by the scheduler */
    while (1)
        ;

}

extern "C"
{

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
{
    /* This function will get called if a task overflows its stack.   If the
     parameters are corrupt then inspect pxCurrentTCB to find which was the
     offending task. */

    (void) pxTask;
    (void) pcTaskName;

    for (;;)
        ;
}

void vApplicationIdleHook(void)
{
    if (TraceDisplayTraceLog())
    {
        LOG_PrintSysLog();
    }
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {}
}
#endif
}
