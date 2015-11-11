/*
 * PeriodicMonitorTask.cpp
 *
 *  Created on: 9 Dec 2012
 *      Author: maora
 */

#include "PeriodicMonitorTask.h"
#include "TmcMasterServer.h"
#include "os_cnfg.h"
#include "TemperatureZone.h"
#include "logger.h"
#include "CfgDB.h"
#include "cfg_db_if_cnfg.h"
#include "monitor.h"

uint32_t periodicmonitor_stack_wm;
PeriodicMonitorTask PeriodicMonitorTask::s_instance;

PeriodicMonitorTask::PeriodicMonitorTask() {
	m_pZoneManager = NULL;
	m_boardAddress = 0xff;

//    // prepare the unchanging fields in the reply message.
//    m_replyMessage.header.id.split.src = m_boardAddress;
//    m_replyMessage.header.id.split.dst = 0x1;
//    m_replyMessage.header.id.split.id = MSG_MonitorDataNotification;
	setNotificationPeriod(1000);
	m_notificationCycleCounter = 0;
	m_tmcServerReady = false;
}

PeriodicMonitorTask::~PeriodicMonitorTask() {
}

PeriodicMonitorTask& PeriodicMonitorTask::getInstance() {
	if (!s_instance.isValid()) {
		// Create the task.
		s_instance.create("PeriodMonitor", MONITOR_TASK_STACK_SZ,
				MONITOR_TASK_PRIORITY);
	}

	return s_instance;
}

portBASE_TYPE PeriodicMonitorTask::onCreate(const portCHAR * const pcName,
		unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {
//    if (m_pZoneManager == NULL)
//        return pdFALSE;
//
//    if (m_pRegisterMonitor == NULL)
//        return pdFALSE;
//
//    if (m_pZoneManager->getTotalNumberOfZones() == 0)
//        return pdFALSE;
//
//    if (m_boardAddress == 0xff)
//        return pdFALSE;

	return AManagedTask::onCreate(pcName, usStackDepth, uxPriority);
}

void PeriodicMonitorTask::setZoneManager(ZoneManager* zoneManager) {
	m_pZoneManager = zoneManager;
	checkForTmcServerPointers();
}

void PeriodicMonitorTask::setRegisterMonitor(
		MiddlewareRegisterMonitor* registerMonitor) {
	m_pRegisterMonitor = registerMonitor;
	checkForTmcServerPointers();
}

void PeriodicMonitorTask::setPointWasActivated(TmcMessageStruct &message) {
	m_setPointWasActivated = true;
	m_setPointMessage = message;
}

void PeriodicMonitorTask::run() {
	portTickType previousTickCount;
	int i;
	U32 value;
	U32 error;
	bool allInReady;

	//CLogger::getInstance().setCurrentTaskMask(M_LOGGER_LEVEL_DEBUG);

	// first thing we do when we start the task is we suspend. The task will then be resumed by the message handler
	// when the OPC requests periodic monitoring.

	//CTask::suspend();

	// get the current tick count.
	U32 valve = 0;

	// get the current tick count.
	previousTickCount = getTickCount();

	while (1) {
        // reset the watchdog:
        IWDG_ReloadCounter();

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
		periodicmonitor_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif
		// delay for the notification period.
		delayUntil(&previousTickCount, MONITOR_PERIODIC_MSEC);

		// get the current tick count.
		previousTickCount = getTickCount();

		// call the original MiddleWare monitor function:
		monitor_function();

		++m_notificationCycleCounter;
		if (m_notificationCycleCounter >= m_notificationPeriodInCycles
				&& m_tmcServerReady) {
			m_notificationCycleCounter = 0;

			allInReady = m_pZoneManager->executeZones();

//            int numOfZones = m_pZoneManager->getNumberOfZones();
//
//            for (int i = 0; i < numOfZones; ++i)
//            {
//                BaseZone *zone = (*m_pZoneManager)[i];
//
////                if (zone->getZoneId() != M_BASE_ZONE_UNASSIGNED_ID)
////                {
//                // if monitoring is enabled, send a monitor reply.
//                error = zone->executeZone();
//
//                // if a set point was activated, wait until all zones are in ready, and then
//                // send a sequence ended message to the OPC;
//                if (m_setPointWasActivated && zone->getState() != E_ZoneState_Ready)
//                    setPointReached = false;
////                }
//            }

			if (m_setPointWasActivated && allInReady) {
				m_setPointWasActivated = false;
				sendSeqEnded(m_setPointMessage.header.id.full,
						m_setPointMessage.header.sn, 0xff, 0xff,
						E_AckStatus_Success);
			}

			if (m_pRegisterMonitor->m_enabled)
				m_pRegisterMonitor->sendAllRegisters();
		}

	}
}

//void PeriodicMonitorTask::sendReply(BaseZone* zone, U32 value)
//{
//    // static instance for the master server.
//    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
//
//    m_replyMessage.payload.pSSMonitoringDataNotificationMsg.controllerId = zone->m_controllerId;
//    m_replyMessage.payload.pSSMonitoringDataNotificationMsg.zoneId = zone->m_zoneId;
//    m_replyMessage.payload.pSSMonitoringDataNotificationMsg.tmsDeviceTypeId = zone->m_deviceTypeId;
//    m_replyMessage.payload.pSSMonitoringDataNotificationMsg.dataSpecification = zone->m_dataType;
//    m_replyMessage.payload.pSSMonitoringDataNotificationMsg.dataLength = zone->m_dataLength;
//    *((U32*) m_replyMessage.payload.pSSMonitoringDataNotificationMsg.data) = value;
//    m_replyMessage.header.length = sizeof(m_replyMessage.header)
//            + sizeof(m_replyMessage.payload.pSSMonitoringDataNotificationMsg)
//            - sizeof(m_replyMessage.payload.pSSMonitoringDataNotificationMsg.data) + zone->m_dataLength;
//
//    masterServerInstance.sendMessage(m_replyMessage);
//}

void PeriodicMonitorTask::setBoardAddress(char boardAddress) {
	m_boardAddress = boardAddress;
	checkForTmcServerPointers();
}

void PeriodicMonitorTask::sendSeqEnded(unsigned long messageId,
		unsigned long sn, char boardId, char zoneId, int status) {
	TmcMessageStruct reply;
	reply.header.id.split.src = m_boardAddress;
	reply.header.id.split.dst = 0x1;
	reply.header.id.split.id = MSG_SeqEnded;
	reply.header.length = sizeof(reply.header)
			+ sizeof(reply.payload.pSSSeqEndedMsg);
	reply.payload.pSSSeqEndedMsg.originalRequestMsgId = messageId;
	reply.payload.pSSSeqEndedMsg.originalRequestSN = sn;
	reply.payload.pSSSeqEndedMsg.cableId = boardId;
	reply.payload.pSSSeqEndedMsg.zoneId = zoneId;
	reply.payload.pSSSeqEndedMsg.status = status;

	M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
			"Seq Ended sent for message 0x%x", messageId);
	TmcMasterServer::getInstance().sendMessage(reply);
}

void PeriodicMonitorTask::reset() {
	m_setPointWasActivated = false;
}

void PeriodicMonitorTask::setNotificationPeriod(int period) {
	m_notificationPeriod = period;
	m_notificationPeriodInCycles = period / MONITOR_PERIODIC_MSEC;

	if (m_pZoneManager != NULL)
	    m_pZoneManager->configSampleTime(period);
}

bool PeriodicMonitorTask::checkForTmcServerPointers() {
	if (m_pZoneManager == NULL)
		return false;

	if (m_pRegisterMonitor == NULL)
		return false;

	if (m_pZoneManager->getTotalNumberOfZones() == 0)
		return false;

	if (m_boardAddress == 0xff)
		return false;

	m_tmcServerReady = true;

	return true;
}
