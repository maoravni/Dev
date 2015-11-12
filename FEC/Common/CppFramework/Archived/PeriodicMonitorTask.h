/*
 * PeriodicMonitorTask.h
 *
 *  Created on: 9 Dec 2012
 *      Author: maora
 */

#ifndef PERIODICMONITORTASK_H_
#define PERIODICMONITORTASK_H_

#include <AManagedTask.h>
#include "BaseZone.h"
#include "MessageStructs.h"
#include "ZoneManager.h"
#include "MiddlewareRegisterMonitor.h"
#include <CBinarySemaphore.h>

/*
 *
 */
class PeriodicMonitorTask: public AManagedTask
{
private:
    static PeriodicMonitorTask s_instance; //!< Instance of the master server singleton.

    ZoneManager* m_pZoneManager;
    MiddlewareRegisterMonitor* m_pRegisterMonitor;

    bool m_tmcServerReady; //!< true if all pointers required for the TMC server are available.

    int m_notificationPeriod;
    int m_notificationPeriodInCycles;
    int m_notificationCycleCounter;

    //TmcMessageHandler* m_pTmcMessageHandler;
    //TmcMessageStruct m_replyMessage;
    unsigned char m_boardAddress;

    /**
     * When activating a setpoint, this variable is set to true.
     * After all zones have changed state from "transition" to "ready"
     * seq ended message is sent to the OPC.
     */
    bool m_setPointWasActivated; //!< After activation of a setpoint,
    TmcMessageStruct m_setPointMessage;

    PeriodicMonitorTask();
public:
    virtual ~PeriodicMonitorTask();

    /**
     * return the instance of the monitor task
     */
    static PeriodicMonitorTask& getInstance();

    /**
     * Set the notification period the OPC will be notified.
     * @param period in ms.
     */
    void setNotificationPeriod(int period);

    void setZoneManager(ZoneManager* zoneManager);
    void setRegisterMonitor(MiddlewareRegisterMonitor* registerMonitor);

    void setPointWasActivated(TmcMessageStruct &message);

    /**
     * Configure the board address of this board for the periodic monitor task.
     * @param boardAddress
     */
    void setBoardAddress(char boardAddress);

    /**
     * In this function we simply check that the base zone array is configured.
     * @param pcName specifies the task name.
     * @param usStackDepth specifies the task stack depth.
     * @param uxPriority specifies the task priority.
     *
     * @return pdTRUE if success, pdFALSE otherwise. If the method return pdFALSE the task creation process
     * is stopped and no FreeRTOS resource are allocated.
     */
    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

    /**
     * Task control loop.
     */
    void run();

    //void sendReply(BaseZone* zone, U32 value);
    void sendSeqEnded(unsigned long messageId, unsigned long sn, char controllerId, char zoneId, int status);

    /**
     * Reset the task to its default state.
     */
    void reset();

    /**
     * Check that all data required for communicating with the OPC is available.
     */
    bool checkForTmcServerPointers();
    /**
     * Suspend the task.
     */
//    void suspend();
};

#endif /* PERIODICMONITORTASK_H_ */
