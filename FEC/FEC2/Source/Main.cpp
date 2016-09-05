/*
 * Main.cpp
 *
 *  Created on: 3 Jun 2013
 *      Author: maora
 */

#include <CFreeRTOS.h>
#include <logger.h>

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#define __root 
#else
#include <stm32f2xx.h>
#endif

#include <board_setup.h>
#include <netconf.h>
#include <dg_in_ctrl.h>
#include <continousA2D.h>
#include <Tasks/TestTask.h>
#include <PscSubsystem.h>
#include <PscServer/PscMasterServer.h>
#include <sntp.h>
#include <PscServer/BoardStateMachine.h>
#include <StatusLed.h>
#include <Peripherals/PSoC/PsocHandler.h>
#include <Peripherals/PSoC/PsocManager.h>
#include <Peripherals/Mi3Sensor.h>
#include <crc_ccitt.h>
#include <flash_if.h>
#include <version.h>
#include <opto_out.h>
//#include <trcUser.h>

#include <glcd.h>
#include <fonts/font5x7.h>

#define mainCHECK_TASK_PRIORITY             ( tskIDLE_PRIORITY + 3 )

/**
 @startuml
 participant "OPC" as opc
 participant "Leakage\nControl" as leakageControl
 participant "HTX\nInverter" as htxInverter
 participant "Fill\nInverter" as fillInverter

 participant "Drain\nControl" as drainControl

 == Calculate Tank Initial Level ==
 opc -> leakageControl: calcTankInitialLevel
 leakageControl -> opc: Sequence Ended
 leakageControl -> opc: tankInitialLevel value

 == Find DeltaHTX ==
 opc -> htxInverter: activateInverter at <Parameter>
 htxInverter -> opc: Sequence Ended
 opc -> leakageControl: calcDeltaHTX
 leakageControl -> opc: Sequence Ended
 leakageControl -> opc: DeltaHTX value

 == Find DeltaTub ==
 opc -> fillInverter: activateInverter at <Parameter>
 opc -> drainControl: activateControl at <Parameter>
 fillInverter -> opc: Sequence Ended
 drainControl -> opc: Sequence Ended
 opc -> leakageControl: calcDeltaTub
 leakageControl -> opc: Sequence Ended
 leakageControl -> opc: DeltaTub value
 opc -> htxInverter: Stop
 opc -> fillInverter: Stop
 opc -> drainControl: Stop
 @enduml
 */
/**
 @startuml
 participant "Update\nScheduler\nTask" as ust
 participant "Temperature\nPeripheral" as tempPeriph
 participant "Temperature\nSensor" as tempSens
 participant "Temperature\nElement" as tempElem
 participant "Output\nElement" as outElem
 participant PID

 activate PWM
 == Update Device Phase ==
 ust -> tempPeriph: readInputs
 activate ust
 activate tempPeriph
 tempPeriph -> tempSens : readA2D
 activate tempSens
 tempPeriph <-- tempSens : readA2D
 deactivate tempSens
 tempPeriph -> tempElem: updateValue
 activate tempElem
 tempElem -> ust: updatePending
 deactivate tempElem
 tempPeriph --> ust: nextInterval
 deactivate tempPeriph

 == Control Logic Phase ==
 ust -> ust: check\nPending\nUpdates
 'activate elemRep
 ust -> PID: execute
 activate PID
 PID -> tempElem: readValue
 activate tempElem
 tempElem --> PID : readValue
 deactivate tempElem
 PID -> PID : calculate
 PID -> outElem : updateValue
 activate outElem
 outElem -> ust: updatePending
 deactivate outElem
 deactivate PID
 ust -> ust: checkPending
 ust -> PWM: execute
 PWM -> outElem: readValue
 activate outElem
 PWM <-- outElem: readValue
 deactivate outElem
 PWM -> PWM: updateDutyCycle
 ust -> ust: checkPending
 deactivate ust
 @enduml
 @startuml
 participant OPC as opc
 participant PSCn as fec
 == Startup Phase==
 opc --> fec : TCP Connection Established
 note left: OPC State "StartingUp"
 fec -> opc : FecStatusNotification
 fec -> opc : FecErrorNotification
 note right: State "On"
 fec -> opc : FecWarningNotification
 opc -> fec : GetVersion
 fec --> opc : GetVersion
 note left
 OPC performs fimware
 and protocol validation
 end note
 opc -> fec : GetDefinedPeripherals
 fec --> opc : GetDefinedPeripherals
 note left
 FEC returns all defined
 peripherals.
 OPC checks that FEC
 resources are sufficient
 for configuration
 end note
 @enduml
 @startuml
 participant OPC as opc
 participant PSCn as fec
 == FEC Init ==
 opc -> fec : SetLoggingLevel
 fec --> opc : Ack
 opc -> fec : SetBoardConfiguration
 fec --> opc : Ack
 opc -> fec : BoardInit
 fec --> opc : Ack
 fec -> opc : FecStatusNotification
 note right: State changed to "Init"
 group PSoC Configuration
 opc -> fec : SetPsocAllowedSlaveCableMask
 note right: Defines which cable ID's\nthe slavecan handle.
 fec --> opc : Ack
 opc -> fec : SetPsocPrimaryFunction
 note right: Defines specific logic\nfunction of the PSoC.
 fec --> opc : Ack
 end
 group Peripheral Configuration
 opc -> fec : SetPeripheralConfig
 fec --> opc : Ack
 end
 group Device Configuration
 opc -> fec : SetDeviceConfig
 fec --> opc : Ack
 end
 group Protection Configuration
 opc -> fec : SetControlConfig
 fec --> opc : Ack
 fec --> opc : StatusNotification
 note right: Protection Control State "Ready"
 end
 group Control Configuration
 opc -> fec : SetControlConfig
 fec --> opc : Ack
 opc -> fec : SetControlParameters
 fec --> opc : Ack
 note right: Control State "On"
 end
 opc -> fec : BoardInitDone
 opc <-- fec : Ack
 note right
 FEC State changed to "Ready"
 end note
 opc <- fec : FecStatusNotification
 group Monitoring
 opc -> fec : EnableMonitoring
 fec --> opc : Ack
 loop StatusNotification
 fec -> opc : StatusNotification
 end
 end
 note left: OPC State changed to "On"
 @enduml
 @startuml
 participant OPC as opc
 participant PSCn as fec
 == OPC Init ==
 group Control Initialization
 opc -> fec : InitControal
 fec --> opc : Ack
 opc -> fec : SetControlParameters
 fec --> opc : Ack
 end
 == OPC Move2Ready ==
 opc -> fec : ActivateControl
 note left: Depending on subsystem
 opc <-- fec : Ack
 opc <-- fec : SeqEnded
 @enduml

 */
#if !defined(WIN32) && !defined(__GNUC__)
extern "C" __root unsigned int __checksum;
extern "C" __root unsigned int __checksum_begin;
extern "C" __root unsigned int __checksum_end;
extern "C" unsigned short slow_crc16(unsigned short sum, unsigned char *p, unsigned int len);
#endif

xTaskHandle g_initTaskHandle;

void vInitTask(void *pvParameters)
{

#ifndef WIN32
    /* Initilaize the LwIP stack */
    LwIP_Init();
#endif

#ifndef WIN32
    sntp_server_address = 0;
#endif

    // setup the logger
    static CLogger& loggerInstance = CLogger::getInstance();
    //loggerInstance.enableOutputTcp(true);
    //loggerInstance.enableLogFreeHeap(true);
    //    loggerInstance.enableLogTasks(true);
    //#ifdef DEBUG
    loggerInstance.enableOutputStandardPrintf(true);
    //#endif
    //loggerInstance.enableOutputUart(true);
    //loggerInstance.enableLongFormat(false);
    //loggerInstance.enableOutputUsbVcp(true);
    //loggerInstance.enableOutputSum(true);
    //   loggerInstance.enableOutputUart(true);

    // start A2D
    a2d_init_hw();

    I2C1_init(M_I2C_IR_SENSOR_BAUD_RATE);
    I2C2_init(M_I2C_IR_SENSOR_BAUD_RATE);
    //    I2C1_DMA_IRQ_init();

    Psc_SubsystemCheckInit();

    Psc_SetIpAddress();

    loggerInstance.setAllTaskMask(M_LOGGER_LEVEL_DEBUG);

    //loggerInstance.enableOutputUdp(true);

    glcd_init();

    char message[26];
    glcd_font(Font5x7, 5, 7, 32, 127, STANG);
    glcd_clear_buffer();
    glcd_draw_string_xy(0, 0, "Landa FEC3");
    sprintf(message, "Cable ID: %d", Psc_GetCableId());
    glcd_draw_string_xy(0, 8, message);
    sprintf(message, "IP: %d.%d.%d.%d", 172, 30, 30, 100+Psc_GetCableId());
    glcd_draw_string_xy(0, 16, message);
//    glcd_print_debug_buffer();
    glcd_write();


#ifndef WIN32
    lwip_igmp_start();
#endif

    //    led_light((LED_ID_ET)0);
    //    vTaskDelay(100);
    //    led_light((LED_ID_ET)1);
    //    vTaskDelay(100);
    //    led_light((LED_ID_ET)2);
    //    vTaskDelay(100);
    //    led_light((LED_ID_ET)3);
    //    vTaskDelay(100);
    //    led_light((LED_ID_ET)4);
    //    vTaskDelay(100);
    //    led_light((LED_ID_ET)5);
    //    vTaskDelay(100);
    //    led_dark((LED_ID_ET)0);
    //    vTaskDelay(100);
    //    led_dark((LED_ID_ET)1);
    //    vTaskDelay(100);
    //    led_dark((LED_ID_ET)2);
    //    vTaskDelay(100);
    //    led_dark((LED_ID_ET)3);
    //    vTaskDelay(100);
    //    led_dark((LED_ID_ET)4);
    //    vTaskDelay(100);
    //    led_dark((LED_ID_ET)5);
    //
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)0, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)1, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)2, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)3, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)4, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)5, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)6, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)7, (OPTO_OUT_CTRL_VAL_ET)1);
    //    vTaskDelay(100);
    //
    //    dig_out_ctrl((OPTO_OUT_ID_ET)0, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)1, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)2, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)3, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)4, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)5, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)6, (OPTO_OUT_CTRL_VAL_ET)0);
    //    vTaskDelay(100);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)7, (OPTO_OUT_CTRL_VAL_ET)0);
    //    a2d_start(A2D_TASK_STACK_SZ, A2D_TASK_PRIORITY );

    // start the telnet server task
    //    TelnetServer::getInstance();

    //    RCC_ClearFlag();/* Clear reset flags *//

    //    PsocManager::getInstance().create("psocManager", DEFAULT_THREAD_STACKSIZE+100, 3);

    //    PsocManager::getInstance().initAllPsocs();
    StatusLed::getInstance();

    // start the ICD server task
    PscMessageHandler::getInstance();
    PscMasterServer::getInstance();

//    TestTask *testTask = new TestTask();
//    portBASE_TYPE res = testTask->create("testTask", DEFAULT_THREAD_STACKSIZE + 2000, 3);

    //    uiTraceStart();.

    //    vTaskDelete(g_initTaskHandle);
    // for some reason this causes an assert failure in the logger
    vTaskSuspend(g_initTaskHandle);
}

extern "C" HeapRegion_t xHeapRegions[];

#ifdef WIN32
int optionCount;
char **optionArray;
#endif

int main(int ac, char* av[])
{
#ifdef WIN32
    optionCount = ac;
    optionArray = av;
#endif

    /*!< At this stage the microcontroller clock setting is already configured to
     120 MHz, this is done throuhgh SystemInit() function which is called from
     startup file (startup_stm32f2xx.s) before 0to branch to application main.\
	 To reconfigure the default setting of SystemInit() function, refer to .
     system_stm32f2xx.c file
     */

#ifndef WIN32
    vPortDefineHeapRegions(xHeapRegions);
#endif

#ifndef __GNUC__
#ifndef WIN32
    printf("%x\n", __checksum);
#endif
#endif
    printf("%d.%d.%d.%d\n", M_FEC_FIRMWARE_VERSION_MAJOR, M_FEC_FIRMWARE_VERSION_MINOR, M_FEC_FIRMWARE_VERSION_BUILD,
            M_FEC_FIRMWARE_VERSION_REVISION);

    //
    ////    uint16_t sum = 0;
    ////    sum = slow_crc16(sum, (unsigned char*)&__checksum_begin, (unsigned int)&__checksum_end-(unsigned int)&__checksum_begin + 1);
    ////    sum = slow_crc16(sum, (unsigned char*)&zero, 2);
    //
    //    uint16_t sum = 0;
    //    uint32_t sumLen = (uint32_t)&__checksum_end - (uint32_t)&__checksum_begin;
    //    sum = crc_ccitt(sum, (unsigned char*)&__checksum_begin, sumLen - 1);
    //
    if (!CheckIfApplicationPresent())
    {
        FLASH_If_Init();
        MarkApplicationAsPresent();
        FLASH_If_Deinit();
        resetBoard(0);
    }

#ifdef WIN32
    int iResult;
    WSADATA wsaData;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
#endif

    prvSetupHardware();

    Safety_out_ctrl(DEACTIVE);

#ifndef WIN32
    ETH_BSP_Config();/* configure ethernet (GPIOs, clocks, MAC, DMA) */
#endif

    //    dig_out_ctrl((OPTO_OUT_ID_ET)0, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)1, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)2, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)3, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)4, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)5, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)6, (OPTO_OUT_CTRL_VAL_ET)1);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)7, (OPTO_OUT_CTRL_VAL_ET)1);
    //
    //    dig_out_ctrl((OPTO_OUT_ID_ET)0, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)1, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)2, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)3, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)4, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)5, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)6, (OPTO_OUT_CTRL_VAL_ET)0);
    //    dig_out_ctrl((OPTO_OUT_ID_ET)7, (OPTO_OUT_CTRL_VAL_ET)0);

    //    /* Initilaize the LwIP stack */
    //    LwIP_Init();

    (void) xTaskCreate(vInitTask, (portCHAR *) "Init Task", 600, NULL, mainCHECK_TASK_PRIORITY, &g_initTaskHandle);

    //    uiTraceStart();
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

void vApplicationMallocFailedHook(void)
{
    /* This function will get called if a malloc failed.   If the
     parameters are corrupt then inspect pxCurrentTCB to find which was the
     offending task. */

    for (;;)
        ;
}

extern PsocHandler* psocHandler;

void vApplicationIdleHook(void)
{
    StatusLed::m_idleWasExecuted = true;
    led_light(CPU_ACTIVITY_LED8);

}

}
