/*
 * Main.cpp
 *
 *  Created on: 3 Jun 2013
 *      Author: maora
 */

#include <CFreeRTOS.h>
#include <logger.h>
#include <stm32f2xx.h>
#include <board_setup.h>
#include <netconf.h>
#include <dg_in_ctrl.h>
#include <GnrlCfgDB.h>
#include <TelnetServer/TelnetServer.h>
#include <continousA2D.h>
#include <Tasks/TestTask.h>
#include <PscSubsystem.h>
#include <PscServer/PscMasterServer.h>
#include <sntp.h>
#include <PscServer/BoardStateMachine.h>
#include <StatusLed.h>

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

int main()
{
    /*!< At this stage the microcontroller clock setting is already configured to
     120 MHz, this is done throuhgh SystemInit() function which is called from
     startup file (startup_stm32f2xx.s) before to branch to application main.
     To reconfigure the default setting of SystemInit() function, refer to
     system_stm32f2xx.c file
     */
    prvSetupHardware();

    ETH_BSP_Config();/* configure ethernet (GPIOs, clocks, MAC, DMA) */

    /* Initilaize the LwIP stack */
    LwIP_Init();

    sntp_server_address = 0;

// setup the logger
    static CLogger& loggerInstance = CLogger::getInstance();
    //loggerInstance.enableOutputTcp(true);
//    CLogger::getInstance().enableOutputUdp(true);
    //loggerInstance.enableLogFreeHeap(true);
//    loggerInstance.enableLogTasks(true);
    loggerInstance.enableOutputStandardPrintf(true);
    //loggerInstance.enableOutputUart(true);
    //loggerInstance.enableLongFormat(false);
    //loggerInstance.enableOutputUsbVcp(true);
    //loggerInstance.enableOutputSum(true);
    //   loggerInstance.enableOutputUart(true);

    Psc_SetIpAddress();

    loggerInstance.setAllTaskMask(M_LOGGER_LEVEL_DEBUG);

    lwip_igmp_start();

    // start A2D
    a2d_init_hw();
//    a2d_start(A2D_TASK_STACK_SZ, A2D_TASK_PRIORITY );

    // start the telnet server task
    TelnetServer::getInstance();

    // start the ICD server task
    PscMasterServer::getInstance();

//    RCC_ClearFlag();/* Clear reset flags *//

//    TestTask *testTask = new TestTask();
//    portBASE_TYPE res = testTask->create("testTask", DEFAULT_THREAD_STACKSIZE, 3);

    StatusLed::getInstance();

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

void vApplicationMallocFailedHook( void )
{
    /* This function will get called if a malloc failed.   If the
     parameters are corrupt then inspect pxCurrentTCB to find which was the
     offending task. */


    for (;;)
    ;
}

void vApplicationIdleHook(void)
{
//    led_dark(DIP_LED);
//    led_light(DIP_LED);
    taskYIELD();
}

}
