/*
 * UpdateSchedulerTask.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef UPDATESCHEDULERTASK_H_
#define UPDATESCHEDULERTASK_H_

#include <AManagedTask.h>
#include <Peripherals/PeripheralBase.h>
#include <Elements/ElementBase.h>
#include <Controls/ControlBase.h>
#include <list>
#include <CBinarySemaphore.h>

#define M_KEEP_ALIVE_TIMEOUT_PSSID 0xfffe

enum E_NodeType
{
    E_NodeType_PeripheralNode, E_NodeType_TimeoutNode, E_NodeType_ProtectionChecker
};

//#define M_PROFILE_SCHEDULER_JITTER

// TODO: Move the queue nodes to different files.
class QueueNode
{
public:
    QueueNode* m_next;
protected:
    portTickType m_nextWakeupTime;
    uint16_t m_subType;
#ifdef M_PROFILE_SCHEDULER_JITTER
    portTickType m_previousWakeupTime;
#endif

public:

    QueueNode() :
            m_next(NULL), m_nextWakeupTime(0), m_subType(0)
#ifdef M_PROFILE_SCHEDULER_JITTER
    , m_previousWakeupTime(0)
#endif
    {
    }

    virtual ~QueueNode()
    {
    }

    bool operator<(const QueueNode& node)
    {
        return (m_nextWakeupTime < node.m_nextWakeupTime);
    }

    bool operator>=(const QueueNode& node)
    {
        return (m_nextWakeupTime >= node.m_nextWakeupTime);
    }

    portTickType getNextWakeupTime() const
    {
        return m_nextWakeupTime;
    }

    void setNextWakeupTime(portTickType nextWakeupTime)
    {
        m_nextWakeupTime = nextWakeupTime;
//#ifdef M_PROFILE_SCHEDULER_JITTER
//        printf("Update Jitter: %d\n", (xTaskGetTickCount() - m_previousWakeupTime) - getUpdateInterval());
//        m_previousWakeupTime = xTaskGetTickCount();
//#endif
    }

    virtual E_NodeType getNodeType() = 0;
    virtual void execute() = 0;
    virtual uint16_t getPssId() = 0;
    virtual portTickType getUpdateInterval() = 0;
    virtual void print() = 0;

    uint16_t getSubType() const
    {
        return m_subType;
    }

    void setSubType(uint16_t subType)
    {
        m_subType = subType;
    }
};

class PeripheralQueueNode: public QueueNode
{
private:
    PeripheralBase* m_peripheralBase;

public:

    PeripheralQueueNode() :
            m_peripheralBase(NULL)
    {
    }

    void setDevice(PeripheralBase* device)
    {
        m_peripheralBase = device;
    }

    E_NodeType getNodeType()
    {
        return E_NodeType_PeripheralNode;
    }

    virtual void execute()
    {
#ifdef M_PROFILE_SCHEDULER_JITTER
        portTickType currentTicks = xTaskGetTickCount();
        if (currentTicks != getNextWakeupTime())
        printf("Wakeup time vs. Current Tick Count: %d\n", getNextWakeupTime() - currentTicks);
#endif
        m_peripheralBase->execute();
    }

    virtual uint16_t getPssId()
    {
        return m_peripheralBase->getPssId();
    }

    virtual portTickType getUpdateInterval()
    {
        return m_peripheralBase->getUpdateInterval();
    }
    virtual void print()
    {
        printf("Wakeup: %d, PSS ID %d\n", m_nextWakeupTime, m_peripheralBase->getPssId());
    }
};

class TimeoutQueueNode: public QueueNode
{
public:
    portTickType m_timeout;
    bool m_obsolete;

private:
    ControlBase* m_control;

public:
    TimeoutQueueNode() :
            m_control(NULL),m_obsolete(false),m_timeout(0)
    {
    }

    void setControl(ControlBase* control)
    {
        m_control = control;
    }

    E_NodeType getNodeType()
    {
        return E_NodeType_TimeoutNode;
    }

    virtual void execute()
    {
        // reset the timeout value, since it can't expire again.
        // this would ensure that it wouldn't get re-inserted into the scheduler.
        m_timeout = 0;
        if (!m_obsolete)
            m_control->timeoutExpired(getSubType());
    }

    virtual uint16_t getPssId()
    {
        return m_control->getPssId();
    }

    virtual portTickType getUpdateInterval()
    {
        return m_timeout;
    }
    virtual void print()
    {
        printf("Wakeup: %d, PSS ID %d\n", m_nextWakeupTime, getPssId());
    }
};

class ProtectionDebounceQueueNode: public QueueNode
{
public:
    portTickType m_timeout;
    bool m_obsolete;

private:
    ProtectionCheckerBase* m_protectionCheckerBase;

public:
    ProtectionDebounceQueueNode() :
        m_protectionCheckerBase(NULL),m_obsolete(false),m_timeout(0)
    {
    }

    void setProtectionChecker(ProtectionCheckerBase* protectionCheckerBase)
    {
        m_protectionCheckerBase = protectionCheckerBase;
    }

    E_NodeType getNodeType()
    {
        return E_NodeType_ProtectionChecker;
    }

    virtual void execute()
    {
        // reset the timeout value, since it can't expire again.
        // this would ensure that it wouldn't get re-inserted into the scheduler.
        m_timeout = 0;
        if (!m_obsolete)
            m_protectionCheckerBase->timeoutExpired(getSubType());
    }

    virtual uint16_t getPssId()
    {
        return m_protectionCheckerBase->getPssId();
    }

    virtual portTickType getUpdateInterval()
    {
        return m_timeout;
    }

    virtual void print()
    {
        printf("Wakeup: %d, PSS ID %d\n", m_nextWakeupTime, getPssId());
    }
};

class KeepAliveTimeoutQueueNode: public TimeoutQueueNode
{
public:
    KeepAliveTimeoutQueueNode() : TimeoutQueueNode()
    {}

    virtual uint16_t getPssId()
    {
        return M_KEEP_ALIVE_TIMEOUT_PSSID;
    }

    virtual void execute();
};

struct T_UpdateEvent
{
    IObserver* observer;
    ElementBase* element;

    void notifyObserver()
    {
        observer->updateNotification(element);
    }
};
typedef std::list<T_UpdateEvent> T_UpdateEventList;

class UpdateSchedulerTaskBase: public AManagedTask
{
private:
    QueueNode *m_pollingQueueHead;
    QueueNode *m_currentWaitingNode;
    portTickType m_currentTickCount;
    portTickType m_lastKeepaliveTickCount;
    T_UpdateEventList m_updateEventList;

    CBinarySemaphore m_observeSemaphore;
    CBinarySemaphore m_runloopSyncSemaphore;

    bool m_boardInReady;

protected:
    UpdateSchedulerTaskBase();

public:
    virtual ~UpdateSchedulerTaskBase();

//    static UpdateSchedulerTaskBase& getInstance();

    /**
     * Task control loop.
     */
    virtual void run();

    void addDevice(PeripheralBase* newDevice);
    void addTimeout(ControlBase* control, portTickType timeout, uint16_t timeoutType = 0);
    void addProtectionDebounceTimeout(ProtectionCheckerBase* protectionChecker, portTickType timeout);
    void cancelTimeout(ControlBase* control, uint16_t timeoutType);

    void addUpdateEvent(IObserver* observer, ElementBase* element);

    void cleanScheduler();

    /**
     * The framework calls this function before the task is created to give a chance to a managed task to perform its
     * own initialization code and stop the task creation process if needed.
     *
     * @param pcName specifies the task name.
     * @param usStackDepth specifies the task stack depth.
     * @param uxPriority specifies the task priority.
     *
     * @return pdTRUE if success, pdFALSE otherwise. If the method return pdFALSE the task creation process
     * is stopped and no FreeRTOS resource are allocated.
     */
    virtual portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
            unsigned portBASE_TYPE uxPriority);

    bool isBoardInReady() const
    {
        return m_boardInReady;
    }

    void setBoardInReady(bool boardInReady);

    void suspendScheduler();
    void resumeScheduler();

//    void keepaliveReceived();

private:
    QueueNode* popHead();
    QueueNode* getHead();
    QueueNode* findNode(E_NodeType nodeType, uint16_t pssId, uint16_t subType);
    bool eraseNode(E_NodeType nodeType, uint16_t pssId, uint16_t subType);
    void insertNode(QueueNode *node);
    void notifyObservers();
    void printfQueue(char* message);
};

class UpdateSchedulerTask : public UpdateSchedulerTaskBase
{
private:
    static UpdateSchedulerTask* p_instance;
public:
    static UpdateSchedulerTask* getInstance();
};

class ModbusSchedulerTask : public UpdateSchedulerTaskBase
{
private:
    static ModbusSchedulerTask* p_instance;
public:
    static ModbusSchedulerTask* getInstance();
};

#endif /* UPDATESCHEDULERTASK_H_ */
