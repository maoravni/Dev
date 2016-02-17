/*
 * UpdateSchedulerTask.cpp
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#include <stdio.h>
#include <stdlib.h>
#include "UpdateSchedulerTask.h"
#include <logger.h>
#include <Peripherals/PeripheralRepository.h>
#include <PscServer/PscMasterServer.h>
#ifndef WIN32
#include <arm_itm.h>
#else
#define ITM_EVENT8
#endif
#include <Controls/ControlRepository.h>

#define M_KEEP_ALIVE_TIMEOUT_MULTIPLIER 5

UpdateSchedulerTask *UpdateSchedulerTask::p_instance;
ModbusSchedulerTask *ModbusSchedulerTask::p_instance;
#define M_UPDATE_SCHEDULER_STACK_SIZE 1100

UpdateSchedulerTaskBase::UpdateSchedulerTaskBase()
{
    m_pollingQueueHead = NULL;
    m_currentWaitingNode = NULL;
    m_currentTickCount = 0;
    m_lastKeepaliveTickCount = 0;
    m_boardInReady = false;
}

UpdateSchedulerTaskBase::~UpdateSchedulerTaskBase()
{
}

void UpdateSchedulerTaskBase::cleanScheduler()
{
    QueueNode* head;

    // first take the run loop semaphore, so that we can be sure that the
    // run loop has finished an iteration:
    if (isBoardInReady())
        m_runloopSyncSemaphore.take(portMAX_DELAY);

//    notifyObservers();

    // clean the scheduler list:
    head = popHead();
    while (head != NULL)
    {
        // if this node is a peripheral node, execute it one last time, as it might have some pending writes.
//        if (head->getNodeType() == E_NodeType_PeripheralNode)
//        {
//            head->execute();
//            notifyObservers();
//        }
        delete head;
        head = popHead();
    }

//    notifyObservers();

    m_currentWaitingNode = NULL;

    // clean the observer list:
    m_updateEventList.clear();

    m_lastKeepaliveTickCount = 0;

    setBoardInReady(false);
}

QueueNode* UpdateSchedulerTaskBase::findNode(E_NodeType nodeType, uint16_t pssId, uint16_t subType)
{
    QueueNode *node = getHead();
    // look for the control in the queue. If it exists, remove it from the queue.
    // Important note: I assume that a PSSID in the queue can appear once for update and once for timeouts.
    while (node != NULL)
    {
        if (node->getNodeType() == nodeType && node->getPssId() == pssId && node->getSubType() == subType)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Found Queue Node {[PSSID:%d]}", pssId);
            return node;
        }
        node = node->m_next;
    }

    return node;
}

bool UpdateSchedulerTaskBase::eraseNode(E_NodeType nodeType, uint16_t pssId, uint16_t subType)
{
    QueueNode *node = getHead();
    // look for the control in the queue. If it exists, remove it from the queue.
    // Important note: I assume that a PSSID in the queue can't appear twice, once for controls
    // and once for
    while (node->m_next != NULL)
    {
        if (node->m_next->getNodeType() == nodeType && node->m_next->getPssId() == pssId
                && node->getSubType() == subType)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "deleting node {[PSSID:%d]}", pssId);

            // set the next of the current to the next of the next
            enterCritical();
            node->m_next = node->m_next->m_next;
            exitCritical();

            delete node;
            return true;
        }
        node = node->m_next;
    }
    return false;
}

void UpdateSchedulerTaskBase::printfQueue(char* message)
{
    puts(message);
    QueueNode* head;
    head = m_pollingQueueHead;
    while (head != NULL)
    {
        head->print();
        head = head->m_next;
    }
}

void UpdateSchedulerTaskBase::run()
{
    portTickType wakeupTime;

    for (;;)
    {
#ifdef USE_IWDG
        // reset the watchdog:
        IWDG_ReloadCounter();
#endif

        // when the board moves to ready, the semaphore is given.
        // we use semaphores instead of delay in case a notification is required.
        // In this case the semaphore is given, which wakes the loop
        m_runloopSyncSemaphore.take(portMAX_DELAY);

        // get the device at the head of the queue:
        popHead();

        if (m_currentWaitingNode == NULL)
        {
            m_observeSemaphore.take(portMAX_DELAY);
            notifyObservers();
            continue;
        }

        // get the delay increment
        wakeupTime = m_currentWaitingNode->getNextWakeupTime();
#ifdef M_PROFILE_SCHEDULER_JITTER
        portTickType afterWait;
#endif

        // if the current tick count is smaller than the wake up time, wait
        while (wakeupTime > m_currentTickCount)
        {
            // if a disconnect occured while we are waiting, we should break out of the while and
            // restart the entire loop:
            if (m_currentWaitingNode == NULL)
                break;

//            M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Device ID %d waiting until %d", m_currentWaitingNode->getPssId(),
//                    wakeupTime);

            m_currentTickCount = getTickCount();

            // wait for the update:
//            delayUntil(&m_currentTickCount, wakeupTime - m_currentTickCount);
            // try to take the semaphore - this will insert a delay equal to the time to wait for the next wake up.
            if (wakeupTime > m_currentTickCount)
            {
                m_observeSemaphore.take(wakeupTime - m_currentTickCount);
#ifdef M_PROFILE_SCHEDULER_JITTER
                if (wakeupTime != getTickCount())
                printf("Wakeup time vs. Current Tick Count: %d <-> %d\n", wakeupTime, getTickCount());
                afterWait = getTickCount();
#endif
                ITM_EVENT8(1, 1);
                notifyObservers();
            }
        }

#ifdef M_PROFILE_SCHEDULER_JITTER
        portTickType beforeExecute = getTickCount();
        if (beforeExecute != afterWait)
        printf("After Wait vs. Before Execute: %d\n", beforeExecute - afterWait);
#endif
        // if a disconnect occured while we are waiting, we should break out of the while and
        // restart the entire loop:
        if (m_currentWaitingNode == NULL)
            continue;

        // execute the device
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Device ID %d executing", m_currentWaitingNode->getPssId());
        m_currentWaitingNode->execute();

        // put the device back in the list:
        insertNode(m_currentWaitingNode);

        // update all observers.
        notifyObservers();

#ifdef M_PROFILE_SCHEDULER_JITTER
        portTickType afterExecute = getTickCount();
        portTickType executeTime = afterExecute - beforeExecute;
        if (executeTime >= 3)
        printf("Execute Time: %d\n", executeTime);
#endif
        ITM_EVENT8(1, 0);

        // give the run loop semaphore back, so we can know when its finished:
        m_runloopSyncSemaphore.give();
        // yield so that the message handler, which is in a lower priority, can try to halt the
        // scheduler when cleaning up.
        yield();
    }
}

void UpdateSchedulerTaskBase::addDevice(PeripheralBase* newDevice)
{
    if (newDevice->getUpdateInterval() == 0)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Device ID {[PSSID:%d]} update interval 0 not added to UpdateScheduler",
                newDevice->getPssId());
        return;
    }

    if (newDevice->getSchedulerTask() != this)
    {
        newDevice->getSchedulerTask()->addDevice(newDevice);
        return;
    }

    QueueNode *node = findNode(E_NodeType_PeripheralNode, newDevice->getPssId(), 0);
    if (node != NULL)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Device ID {[PSSID:%d]} already exists in the UpdateScheduler",
                newDevice->getPssId());
        return;
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Device ID {[PSSID:%d]} added to UpdateScheduler", newDevice->getPssId());

    PeripheralQueueNode *periphNode = new PeripheralQueueNode();
    periphNode->setDevice(newDevice);
    insertNode(periphNode);
}

void UpdateSchedulerTaskBase::addTimeout(ControlBase* control, portTickType timeout, uint16_t timeoutType)
{
//    cancelTimeout(control);
    QueueNode *node = findNode(E_NodeType_TimeoutNode, control->getPssId(), timeoutType);

    if (node != NULL)
    {
        TimeoutQueueNode* oldNode = static_cast<TimeoutQueueNode*>(node);
        oldNode->m_obsolete = true;
    }

    // if there's an existing node like this in the scheduler, we want to make the existing node obsolete.
    // if the current timeout is 0, we also don't want to add the new node to the scheduler.
    if (timeout == 0)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Control ID {[PSSID:%d]} timeout 0 not added to UpdateScheduler",
                control->getPssId());
        return;
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Control ID {[PSSID:%d]} added to UpdateScheduler", control->getPssId());

    TimeoutQueueNode *toNode = new TimeoutQueueNode();
    toNode->setControl(control);
    toNode->m_timeout = timeout;
    toNode->setSubType(timeoutType);
    insertNode(toNode);
}

void UpdateSchedulerTaskBase::addProtectionDebounceTimeout(ProtectionCheckerBase* protectionChecker,
portTickType timeout)
{
    if (timeout == 0)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Control ID {[PSSID:%d]} timeout 0 not added to UpdateScheduler",
                protectionChecker->getPssId());
        return;
    }

//    cancelTimeout(control);
    QueueNode *node = findNode(E_NodeType_ProtectionChecker, protectionChecker->getPssId(), 0);

    if (node != NULL)
    {
        ProtectionDebounceQueueNode* oldNode = static_cast<ProtectionDebounceQueueNode*>(node);
        oldNode->m_obsolete = true;
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Protection ID {[PSSID:%d]} added to UpdateScheduler",
            protectionChecker->getPssId());

    ProtectionDebounceQueueNode *toNode = new ProtectionDebounceQueueNode();
    toNode->setProtectionChecker(protectionChecker);
    toNode->m_timeout = timeout;
    toNode->setSubType(0);
    insertNode(toNode);
}

void UpdateSchedulerTaskBase::addForcedUpdateTimeout()
{
    //    cancelTimeout(control);
    QueueNode *node = findNode(E_NodeType_ForcedUpdate, M_FORCED_UPDATE_TIMEOUT_PSSID, 0);

    if (node != NULL)
    {
        ForcedUpdateTimeoutQueueNode* oldNode = static_cast<ForcedUpdateTimeoutQueueNode*>(node);
        oldNode->m_obsolete = true;
    }

    ForcedUpdateTimeoutQueueNode *newNode = new ForcedUpdateTimeoutQueueNode();
    newNode->m_timeout = M_FORCED_UPDATE_TIMEOUT_PERIOD;
    insertNode(newNode);
}

void UpdateSchedulerTaskBase::cancelTimeout(ControlBase* control, uint16_t timeoutType)
{
    QueueNode *node = getHead();
// look for the control in the queue. If it exists, remove it from the queue.
// Important note: I assume that a PSSID in the queue can't appear twice, once for controls
// and once for
    while (node->m_next != NULL)
    {
        if (node->m_next->getNodeType() == E_NodeType_TimeoutNode && node->m_next->getPssId() == control->getPssId()
                && node->m_next->getSubType() == timeoutType)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "deleting control ID timeout {[PSSID:%d]}", control->getPssId());

            // set the next of the current to the next of the next
            enterCritical();
            node->m_next = node->m_next->m_next;
            exitCritical();

            delete node;
            return;
        }
        node = node->m_next;
    }
}
/*
 {
 QueueNode *node = findNode(E_NodeType_TimeoutNode, control->getPssId());
 if (node != NULL)
 {
 TimeoutQueueNode* oldNode = static_cast<TimeoutQueueNode*>(node);
 oldNode->m_obsolete = true;
 }
 }
 */

QueueNode* UpdateSchedulerTaskBase::popHead()
{
    if (m_pollingQueueHead == NULL)
        return NULL;

    enterCritical();
    m_currentWaitingNode = m_pollingQueueHead;
    m_pollingQueueHead = m_pollingQueueHead->m_next;
    exitCritical();
    return m_currentWaitingNode;
}

QueueNode * UpdateSchedulerTaskBase::getHead()
{
    return m_pollingQueueHead;
}

void UpdateSchedulerTaskBase::addUpdateEvent(IObserver* observer, ElementBase* element)
{
//    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Element ID %d notified its observer", element->getPssId());

    enterCritical();
    T_UpdateEvent updateEvent =
    { observer, element };
    m_updateEventList.push_back(updateEvent);
    exitCritical();
    m_observeSemaphore.give();
}

UpdateSchedulerTask* UpdateSchedulerTask::getInstance()
{
    if (p_instance == NULL || !p_instance->isValid())
    {
        p_instance = new UpdateSchedulerTask();
        // Create the task.
        p_instance->create("UpdateScheduler", M_UPDATE_SCHEDULER_STACK_SIZE, M_TASK_PRIORITY_MAIN_UPDATE_SCHEDULER);
    }

    return p_instance;
}

void UpdateSchedulerTaskBase::notifyObservers()
{
#ifdef M_PROFILE_SCHEDULER_JITTER
    portTickType beforeNotify = getTickCount();
#endif
// all objects that notified their observers added notification events to the update list.
// we now pull all update events from the list and update the observers.
    while (m_updateEventList.size() > 0)
    {
        // pop the update event at the top of the list:
        enterCritical();
        T_UpdateEvent updateEvent = m_updateEventList.front();
        m_updateEventList.pop_front();
        exitCritical();

        updateEvent.notifyObserver();
    }
#ifdef M_PROFILE_SCHEDULER_JITTER
    portTickType afterNotify = getTickCount();
    portTickType notificationTime = (afterNotify - beforeNotify);
    if (notificationTime >= 3)
    printf("Notification Time: %d\n", notificationTime);
#endif
}

void UpdateSchedulerTaskBase::insertNode(QueueNode* node)
{
    portTickType nextUpdateInterval = node->getUpdateInterval();
    if (nextUpdateInterval == 0)
    {
        // need to delete the node as we're not inserting it to the update list:
        delete node;
        return;
    }

// get the next update interval from the device:
    node->setNextWakeupTime(getTickCount() + node->getUpdateInterval());

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Device ID {[PSSID:%d]} next update interval at %d", node->getPssId(),
            node->getNextWakeupTime());

// if the list is empty, put the node at the head of the list.
    if (m_pollingQueueHead == NULL)
    {
        enterCritical();
        m_pollingQueueHead = node;
        node->m_next = NULL;
        exitCritical();
        m_observeSemaphore.give();
        return;
    }

// iterate through the list until we find the place for the new node, where the
// update interval is bigger than the current:
    QueueNode* current = m_pollingQueueHead;

    if (node->getNextWakeupTime() < m_pollingQueueHead->getNextWakeupTime())
    {
        enterCritical();
        node->m_next = m_pollingQueueHead;
        m_pollingQueueHead = node;
        exitCritical();
    }
    else
    {
        for (; current->m_next != NULL && *node >= *current->m_next; current = current->m_next)
            if (node->getNextWakeupTime() < current->getNextWakeupTime())
                break;

        // place the new node instead of the next element of the current:
        enterCritical();
        node->m_next = current->m_next;
        current->m_next = node;
        exitCritical();
    }
}

portBASE_TYPE UpdateSchedulerTaskBase::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    bool result = m_observeSemaphore.create();
    m_observeSemaphore.take(0);
    if (result == false)
        return pdFALSE;

    result = m_runloopSyncSemaphore.create();
    m_runloopSyncSemaphore.take(0);
    if (result == false)
        return pdFALSE;

    return pdTRUE;
}

void UpdateSchedulerTaskBase::setBoardInReady(bool boardInReady)
{
    PeripheralRepository::getInstance().setBoardInReady(boardInReady);
    ControlRepository::getInstance().setBoardInReady(boardInReady);
    if (boardInReady)
    {
        m_runloopSyncSemaphore.give();
    }
    else
    {
        if (m_boardInReady && getHead() != NULL)
        {
            printf("Current Tick Count: %d\n", getTickCount());
            printfQueue("Before Semaphore Take");
            if ((m_runloopSyncSemaphore.take(5000) != pdPASS))
            {
                printf("Current Tick Count: %d\n", getTickCount());
                printfQueue("Semaphore Take Failed");
                int i = 0;
            }
        }
    }
    m_boardInReady = boardInReady;
//            resume();
}

void UpdateSchedulerTaskBase::suspendScheduler()
{
    if (m_boardInReady && getHead() != NULL)
        m_runloopSyncSemaphore.take(portMAX_DELAY);
}

void UpdateSchedulerTaskBase::resumeScheduler()
{
    m_runloopSyncSemaphore.give();
}

void ForcedUpdateTimeoutQueueNode::execute()
{
    if (m_obsolete)
        m_timeout = 0;
// reset the timeout value, since it can't expire again.
// this would ensure that it wouldn't get re-inserted into the scheduler.
//    ControlRepository::getInstance().sendUpdateNotificationForAllControls();
//    ElementRepository::getInstance().sendUpdateNotificationForAllElements();
}

ModbusSchedulerTask* ModbusSchedulerTask::getInstance()
{
    if (p_instance == NULL || !p_instance->isValid())
    {
        p_instance = new ModbusSchedulerTask();
        // Create the task.
        p_instance->create("ModbusScheduler", M_UPDATE_SCHEDULER_STACK_SIZE, M_TASK_PRIORITY_MODBUS_UPDATE_SCHEDULER);
    }

    return p_instance;
}

//void UpdateSchedulerTask::setBoardInReady(bool boardInReady)
//{
//    UpdateSchedulerTaskBase::setBoardInReady(boardInReady);
////    if (boardInReady)
////        addForcedUpdateTimeout();
//}
/*
 int lastElementPssID = 0;
 IObserver* lastObserver;

 void T_UpdateEvent::notifyObserver()
 {
 lastObserver = observer;
 lastElementPssID = element->getPssId();
 if (observer == NULL)
 int i = 0;
 observer->updateNotification(element);
 }

 */
