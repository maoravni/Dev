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
#include <sntp.h>
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
//static CHelloWorld hello(1234);

//#include "usbd_cdc_vcp.h"
//#include "usbd_cdc_core.h"
//#include "usbd_usr.h"
//#include "usb_conf.h"
//#include "usbd_desc.h"
//#include "vcp_srvcs.h"
//#include "..\..\..\Common\VCP_TERM\vcp_term.h"

#include "Calibration.h"
#include "dg_in_ctrl.h"

#include "can_tst.h"
#include "CBtcDef.h"
#include "Hw_pwm.h"
#include <math.h>
#include <iostream>
#include <os_cnfg.h>

//extern unsigned short __checksum;

long SysTickCounter;
extern "C" SUBSYS_ID GlobalSubSysId;

//#include "test_board.h"
//#define TEST_FEC
//extern USB_OTG_CORE_HANDLE USB_OTG_dev;
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

//    GlobalSubSysId = (SUBSYS_ID) CfgRetGnrlInt(ID_GNRL_MNTR_INT_SUBSYS_ID); //just update the global value for future use
}

static void OthersInit()
{
    OthersCfgInit();  //DB init
    while (!OthersCfgDbReady())
    {
    };

}

/*----------------------------------------------------------------------------
 * @brief  Init_task task
 * @param  pvParameters not used
 * @retval None
 *----------------------------------------------------------------------------*/
void Init_task(void * pvParameters)
{

//init the DB

    // must be called before TemperatureInit and OthersInit
    GlobalSubSysId = (SUBSYS_ID) Get_SSID_dig();

    CalibrationInit(); // read calibration data

    GenrlInit();

    TemperatureInit();

    OthersInit();

    //vTaskDelay(1000);

    //Set the IP address according to the SubSystem type and node ID
    SetIpAddr(Get_SSID_dig(), nod_id_get());
    lwip_igmp_start();

    if (!IsInAtpMode())
    { //during ATP eliminate the use of the PWM
        /*  PWM task */
        PWM_init(PWM_TASK_PRIORITY, PWM_TASK_STACK_SZ);
    }

    cfg_db_if_init();

    //start the Modbus server task
    tcp_mb_init();

    // start the landa server task:
    TmcMasterServer::getInstance();

    /*  Monitor task */

    // Monitor_init(MONITOR_TASK_PRIORITY, MONITOR_TASK_STACK_SZ);
    PeriodicMonitorTask::getInstance();

    //turn off  the safetry alarm
    U32 Data = 1;
    CfgPutParam(INSTANCE_GNRL, ID_GNRL_MNTR_INT_CRITICAL_ALARM, &Data, (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);

    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_DEBUG, "Calling iwdg_init");
    iwdg_init();

    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_DEBUG, "Calling RCC_ClearFlag");
    RCC_ClearFlag();/* Clear reset flags */

    //RTC Init
    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_DEBUG, "Calling rtc_init");
    rtc_init();

    sntp_init();

#ifdef _ATP_VERSION_    
    CanTestTimerCreate();
#endif    


    //printf("calibration done\n");
    LOG_SYS_Init();

    if (GlobalSubSysId == SUBSYS_BTC || GlobalSubSysId == SUBSYS_CONDITIONER)
    {
        //hw_pwm_channel_init (FILL_PUMP_DIGITAL_OUTPUT);
        hw_pwm_init(0, 0, 3000); //3000 hz = 333us
    }

    led_light(LABL_RESET);

    //CLogger::getInstance().setAllTaskMask(M_LOGGER_LEVEL_OFF);

    //delete the init task
    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_DEBUG, "Calling vTaskDelete");
    vTaskDelete(NULL);

}/* Init_task */

int main()
{

    /*!< At this stage the microcontroller clock setting is already configured to
     120 MHz, this is done throuhgh SystemInit() function which is called from
     startup file (startup_stm32f2xx.s) before to branch to application main.
     To reconfigure the default setting of SystemInit() function, refer to
     system_stm32f2xx.c file
     */
    prvSetupHardware();

    //ActiveSimulationMode(true);

    ETH_BSP_Config();/* configure ethernet (GPIOs, clocks, MAC, DMA) */

#if 0
    // test the sys log
    LOG_SYS_Init();
#endif

    /* Initilaize the LwIP stack */
    LwIP_Init();

    // start A2D
    a2d_init_hw();
    a2d_start(A2D_TASK_STACK_SZ, A2D_TASK_PRIORITY );

    // setup the logger

    static CLogger& loggerInstance = CLogger::getInstance();
    //loggerInstance.enableOutputTcp(true);
    loggerInstance.enableOutputUdp(true);
    loggerInstance.enableLogFreeHeap(true);
    //loggerInstance.enableLogTasks(true);
    //loggerInstance.enableOutputStandardPrintf(true);
    //loggerInstance.enableOutputUart(true);
    loggerInstance.enableLongFormat(false);
    //loggerInstance.enableOutputUsbVcp(true);
    //loggerInstance.enableOutputSum(true);
    //   loggerInstance.enableOutputUart(true);

    portBASE_TYPE res;

    /*****    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);    *****/

#if _OLD_LOGGER_  
    /*****   usb_term_init(AUX_TERM_QUEUE_SZ, USB_TASK_STACK_SZ, USB_TASK_PRIORITY); *****/
    res = aux_term_init(AUX_TERM_QUEUE_SZ, TERMINAL_TASK_STACK_SZ, TERMINAL_TASK_PRIORITY);/* Keep it first! */
    if (!res)
    {
        while (1)
        {
        };
    }
#endif

    // ModBus Init
    mb_node_1 = mb_master_node_create(MB_1);
    res = mb_master_node_init("mb_1_master", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN, mb_node_1);
    if (!res)
    {
        while (1)
        {
        };
    }

//    CalibrationInit(); // read calibration data

#if _CALIBRATION_TEST_
    int Data=-10;
    //just fill the temperature calibration with data
    for (int Index=0;Index< NUMBER_OF_TEMPERATURE_ZONES;Index++)
    {
        Data+=1;
        CfgPutParam(0, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,Index), &Data,
                (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
    }

    Data=0;
    //just fill the AO calibration with data
    for (int Index=0;Index< NUM_OF_ANALOG_OUT_MA;Index++)
    {
        Data+=1;
        CfgPutParam(0, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_IN_MA_CALIB_n,Index), &Data,
                (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
    }

#endif    

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
        StartSysLog();
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
    //extern c

