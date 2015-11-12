/*
 * TmcMessageHandler.cpp
 *
 *  Created on: 18 бреб 2012
 *      Author: maora
 */

#include <CMessageTask.h>
#include <string.h>

#include "TmcMasterServer.h"
#include "TmcMessageHandler.h"
#include "logger.h"
#include "version.h"
#include <BaseZone.h>
#include <TemperatureZone.h>
#include <AirBlowerZone.h>
#include <AirBleedingZone.h>
#include <HeaterZone.h>
#include <HeaterBase.h>
#ifdef FEC_BOARD
#include <dg_in_ctrl.h>
#include <fec.h>
#include <FecZones.h>
#include <DryerZoneManager.h>
#include <IbdZoneManager.h>
#include <IthZoneManager.h>
#include <AirInletValve.h>
#include <BtcZoneManager.h>
#include <CBtc.h>
#include <CBtcComponents.h>
#endif

#include "GnrlCfgDB.h"

#define CMessageTestTask TmcMessageHandler

#undef M_BEGIN_MESSAGE_MAP
#undef M_END_MESSAGE_MAP
#undef M_MESSAGE_MAP_ENTRY

#define M_BEGIN_MESSAGE_MAP BEGIN_MESSAGE_MAP
#define M_END_MESSAGE_MAP END_MESSAGE_MAP
#define M_MESSAGE_MAP_ENTRY MESSAGE_MAP_ENTRY

#include "GeneratedMessages.msg.h"

// TODO: Need to define more error messages to the OPC.
TmcMessageHandler::TmcMessageHandler()
{
    m_boardAddress = 5;
    m_boardId = 1;
    m_masterControllerId = 1;
    m_deviceType = E_TMSDeviceType_Dryer;
    m_pZoneManager = NULL;
    m_currentMessage = 0;

    //reset();
}

TmcMessageHandler::~TmcMessageHandler()
{
}

bool TmcMessageHandler::hardwareInit()
{
    return true;
}

void TmcMessageHandler::instantiateZoneManager()
{
#ifdef PHANTOM_BOARD
#ifdef _NEW_IBD_CONTROL_
    m_pZoneManager = new PhantomIbdZones();
#else
    m_pZoneManager = new PhantomZones();
#endif
#endif

#ifdef FEC_BOARD

    GlobalSubSysId = (SUBSYS_ID) Get_SSID_dig();

    switch (GlobalSubSysId)
    {
    case SUBSYS_DRYER:
//        m_pZoneManager = new FecDryerZones();
        m_pZoneManager = new DryerZoneManager();
        m_deviceType = E_TMSDeviceType_Dryer;
        break;
    case SUBSYS_IBD:
//        m_pZoneManager = new FecIbdZones();
        m_pZoneManager = new IbdZoneManager();
        m_deviceType = E_TMSDeviceType_IBD;
        break;
    case SUBSYS_ITH:
        m_pZoneManager = new IthZoneManager();
        m_deviceType = E_TMSDeviceType_ITH;
        break;
    case SUBSYS_BTC:
        m_pZoneManager = new BtcZoneManager();
        m_deviceType = E_TMSDeviceType_Cleaning;
        CLogger::getInstance().setAllTaskMask(M_LOGGER_LEVEL_VERBOSE);
        break;
    default:
        m_pZoneManager = new FecZones();
        break;
    }

    char nodeId = nod_id_get();
    m_boardId = (GlobalSubSysId & 0xf) << 4 | nodeId;
    m_masterControllerId = m_boardId;

#endif
}

void TmcMessageHandler::MessageDisconnect(unsigned long param)
{
    reset();
}

void TmcMessageHandler::MessageNotHandled(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];

    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Message ID 0x%x not handled", message.header.id.split.id);

    sendAck(message.header.id.full, message.header.sn, M_BOARD_ID_ALL, M_DEVICE_ID_ALL, E_AckStatus_NotImplemented);
}

void TmcMessageHandler::MessageReadSystemDataHandler(unsigned long param)
{
    //reset();

    int i;
//    int zoneCounter[E_ZoneType_Size];
//    for (i = 0; i < E_ZoneType_Size; ++i)
//        zoneCounter[i] = 0;
//
//    // count the zones:
//    for (int i = 0; i < m_pZoneManager->getTotalNumberOfZones(); ++i)
//    {
//        ++zoneCounter[(*m_pZoneManager)[i]->getZoneType()];
//    }

// start filling the reply message:
    TmcMessageStruct message;
    PSSReadSystemDataReplyMsg& payload = message.payload.pSSReadSystemDataReplyMsg;

    message.header.id.split.dst = 0x1;
    message.header.id.split.src = m_boardAddress;
    message.header.id.split.id = m_tmcMessages[param].header.id.split.id;
    payload.listLength = 1;
    payload.systemData.boardId = m_boardId;
    payload.systemData.masterControllerId = m_masterControllerId;
    payload.systemData.firmwareVersion = ((unsigned __int64) M_FEC_FIRMWARE_VERSION_MAJOR << 48)
            | ((unsigned __int64) M_FEC_FIRMWARE_VERSION_MINOR << 32)
            | ((unsigned __int64) M_FEC_FIRMWARE_VERSION_BUILD << 16)
            | ((unsigned __int64) M_FEC_FIRMWARE_VERSION_REVISION);
    payload.systemData.protocolVersion = ((unsigned __int64) M_ICD_VERSION_MAJOR << 48)
            | ((unsigned __int64) M_ICD_VERSION_MINOR << 32) | ((unsigned __int64) M_ICD_VERSION_BUILD << 16)
            | ((unsigned __int64) M_ICD_VERSION_REVISION);
//    payload.systemData.availableAirBleedingZones = zoneCounter[E_ZoneType_AirBleeding];
//    payload.systemData.availableBlowerZones = zoneCounter[E_ZoneType_Blower] + zoneCounter[E_ZoneType_AirInletValve];
//    payload.systemData.availableFanZones = zoneCounter[E_ZoneType_Fan];
//    payload.systemData.availableTemperatureSensorZones = zoneCounter[E_ZoneType_TemperatureSensor]
//            + zoneCounter[E_ZoneType_AnalogInput];
//    payload.systemData.availableTemperatureZones = zoneCounter[E_ZoneType_Heater];

    message.header.length = sizeof(message.header) + sizeof(payload.listLength)
            + sizeof(payload.systemData) * payload.listLength;

    TmcMasterServer::getInstance().sendMessage(message);

}

void TmcMessageHandler::MessageSetControllerConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetControllerConfigMsg &payload = message.payload.pSSSetControllerConfigMsg;

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, M_DEVICE_ID_ALL, E_AckStatus_Success);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Message ID 0x%x: Set Controller Configuration, controller=%d",
            message.header.id.split.id, payload.boardId);
    // TODO: MessageSetControllerConfigurationHandler

}

void TmcMessageHandler::MessageEnableHandler(unsigned long param)
{
    // TODO: check if this message handler should be moved to an asynchronous handler.
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSEnableMsg &payload = message.payload.pSSEnableMsg;

    E_AckStatus status = E_AckStatus_Success;

    // check if we need to stop all or a specific zone:
    if (payload.zoneId == M_DEVICE_ID_ALL)
    {
        // send an ack that the message was received:
        sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, E_AckStatus_Success);

        int numOfZones = m_pZoneManager->getNumberOfZones();
        for (int i = 0; i < numOfZones; ++i)
        {
            if ((*m_pZoneManager)[i]->getZoneId() != M_BASE_ZONE_UNASSIGNED_ID)
            {
                (*m_pZoneManager)[i]->m_lastSn = message.header.sn;
                (*m_pZoneManager)[i]->enableZone(true);
            }
        }
    }
    else
    {
        BaseZone* zone = m_pZoneManager->getZoneById(payload.zoneId);
        if (zone != NULL)
        {
            status = E_AckStatus_Success;
            zone->m_lastSn = message.header.sn;
            zone->enableZone(true);
        }
        else
        {
            status = E_AckStatus_InvalidDevice;
        }
        // send an ack that the message was received:
        sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
    }

    sendSeqEnded(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageStopHandler(unsigned long param)
{
    // TODO: check if this message handler should be moved to an asynchronous handler.
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSStopMsg &payload = message.payload.pSSStopMsg;

    E_AckStatus status = E_AckStatus_Success;

    if (payload.boardId == M_BOARD_ID_ALL || payload.boardId == m_boardId)
    {
        // check if we need to stop all or a specific zone:
        if (payload.zoneId == M_DEVICE_ID_ALL)
        {
            // send an ack that the message was received:
            sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, E_AckStatus_Success);

            int numOfZones = m_pZoneManager->getNumberOfZones();
            for (int i = 0; i < numOfZones; ++i)
            {
                // enable all zones that were assigned an ID:
                if ((*m_pZoneManager)[i]->getZoneId() != M_BASE_ZONE_UNASSIGNED_ID)
                {
                    (*m_pZoneManager)[i]->m_lastSn = message.header.sn;
                    (*m_pZoneManager)[i]->enableZone(false);

                }
            }
        }
        else
        {
            BaseZone* zone = m_pZoneManager->getZoneById(payload.zoneId);
            if (zone != NULL)
            {
                status = E_AckStatus_Success;
                zone->m_lastSn = message.header.sn;
                zone->enableZone(false);
            }
            else
            {
                status = E_AckStatus_InvalidDevice;
            }
            // send an ack that the message was received:
            sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
        }
    }
    else
    {
        // send an ack that the message was received:
        sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, E_AckStatus_Success);
    }

    sendSeqEnded(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageActivateMonitoringHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSActivateMonitoringMsg &payload = message.payload.pSSActivateMonitoringMsg;
    BaseZone* zone;
    //ZoneNode* zoneNode;
    E_AckStatus status;
    int numOfZones = m_pZoneManager->getNumberOfZones();

    // check which zones we should set:
    if (payload.zoneId == M_DEVICE_ID_ALL)
    {
        for (int i = 0; i < numOfZones; ++i)
        {
            if ((*m_pZoneManager)[i]->getZoneId() != M_BASE_ZONE_UNASSIGNED_ID)
            {
                (*m_pZoneManager)[i]->enableMonitoring(payload.enable);
            }
        }
        status = E_AckStatus_Success;
    }
    else
    {
        zone = m_pZoneManager->getZoneById(payload.zoneId);
        if (zone != NULL)
        {
            zone->enableMonitoring(payload.enable);
            status = E_AckStatus_Success;
        }
        else
        {
            // TODO: assign a correct status to the ACK.
            status = E_AckStatus_InvalidDevice;
        }
    }

    for (int i = 0; i < numOfZones; ++i)
    {
        if ((*m_pZoneManager)[i]->isMonitoringEnabled())
            break;
    }

    // if zone is NULL, it means no zones have monitoring enabled. If so, we should suspend the monitor task.
    if (zone == NULL)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Monitoring suspended");
        //m_monitorTask.suspend();
    }
    else
    {
        PeriodicMonitorTask::getInstance().setNotificationPeriod(payload.interval);



        //m_monitorTask.resume();

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Monitoring activated, interval=%d", payload.interval);
    }

}

void TmcMessageHandler::MessageSetTemperatureZoneConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetTemperatureZoneConfigMsg &payload = message.payload.pSSSetTemperatureZoneConfigMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index, E_ZoneType_Heater))
            != NULL)
    {
        U32 setStatus = static_cast<HeaterBase*>(zone)->setPowerLimit(payload.maxPower);
        setStatus = static_cast<HeaterBase*>(zone)->setMaxTemperature(payload.maxTemperature);
        //status = (setStatus == OK) ? E_AckStatus_Success : E_AckStatus_Size;
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set temperature zone ID 0x%x not found", payload.zoneId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
}

void TmcMessageHandler::MessageSetBlowerZoneConfigHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetBlowerZoneConfigMsg &payload = message.payload.pSSSetBlowerZoneConfigMsg;

    E_AckStatus status;
    BaseZone* zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index, E_ZoneType_Blower))
            != NULL)
    {
//        AirBlowerZone* blower = static_cast<AirBlowerZone*>(zone);
        status = E_AckStatus_Success;
    }
    else
    {
        if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index,
                E_ZoneType_AirInletValve)) != NULL)
        {
            status = E_AckStatus_Success;
        }
        else
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set blower zone ID 0x%x not found", payload.zoneId);
            status = E_AckStatus_InvalidDevice;
        }
    }

//send an ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
}

void TmcMessageHandler::MessageSetFanZoneConfigHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetFanZoneConfigMsg &payload = message.payload.pSSSetFanZoneConfigMsg;
    E_AckStatus status;
    BaseZone* zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index, E_ZoneType_Fan))
            != NULL)
    {
        //        AirBlowerZone* blower = static_cast<AirBlowerZone*>(zone);
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set fan zone ID 0x%x not found", payload.zoneId);
        status = E_AckStatus_InvalidDevice;
    }

    //send an ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
}

void TmcMessageHandler::MessageSetTemperatureSensorZoneConfigHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetTemperatureSensorZoneConfigMsg &payload = message.payload.pSSSetTemperatureSensorZoneConfigMsg;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Set temperature zoneId=%d", payload.zoneId);

    E_AckStatus status;
    BaseZone* zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index,
            E_ZoneType_TemperatureSensor)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
//        if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index,
//                E_ZoneType_AnalogInput)) != NULL)
//        {
//            status = E_AckStatus_Success;
//        }
//        else
//        {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set temperature sensor zone ID 0x%x not found", payload.zoneId);
        status = E_AckStatus_InvalidDevice;
//        }
    }

    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
}

void TmcMessageHandler::MessageSetBlowerSpeedHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetBlowerSpeedMsg &payload = message.payload.pSSSetBlowerSpeedMsg;
    E_AckStatus status;
    BaseZone* zone = m_pZoneManager->getZoneById(payload.zoneId);

    if (zone == NULL)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        switch (zone->getZoneType())
        {
        case E_ZoneType_Blower:
            static_cast<AirBlowerZone*>(zone)->configValues(payload.speed);
            status = E_AckStatus_Success;
            break;
        case E_ZoneType_AirInletValve:
            static_cast<AirInletValve*>(zone)->configValues(payload.speed);
            status = E_AckStatus_Success;
            break;
        default:
            status = E_AckStatus_InvalidDevice;
            break;
        }
    }

    // send ACK:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageSetAirBleedingZoneConfigHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetAirBleedingZoneConfigMsg &payload = message.payload.pSSSetAirBleedingZoneConfigMsg;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Set Air Bleeding zoneId=%d", payload.zoneId);
    E_AckStatus status;
    BaseZone* zone;

    // TODO: When the actual valve zone is implemented, replace with the relevant zone type.
    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index,
            E_ZoneType_AirBleeding)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set air bleeding zone ID 0x%x not found", payload.zoneId);
        status = E_AckStatus_InvalidDevice;
        //TODO: Send an error ACK.
    }

    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
}

void TmcMessageHandler::MessageEnableRegistersNotificationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSEnableRegistersNotificationMsg &payload = message.payload.pSSEnableRegistersNotificationMsg;

    E_AckStatus status;

    sendAck(message.header.id.full, message.header.sn, M_BOARD_ID_ALL, M_DEVICE_ID_ALL, E_AckStatus_Success);

    m_registerMonitor.m_enabled = (payload.enable != 0);

    //m_monitorTask.resume();
}

void TmcMessageHandler::MessageSetRegisterHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetRegisterMsg &payload = message.payload.pSSSetRegisterMsg;

    m_registerMonitor.setRegister(payload.registerID, payload.value);

    sendAck(message.header.id.full, message.header.sn, M_BOARD_ID_ALL, M_DEVICE_ID_ALL, E_AckStatus_Success);
}

void TmcMessageHandler::MessageSetTemperatureHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSSetTemperatureMsg &payload = message.payload.pSSSetTemperatureMsg;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload.zoneId);
    E_AckStatus status;

    if (zone == NULL || zone->getZoneType() != E_ZoneType_Heater)
    {
        // TODO: Error status
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        // set the setpoint:
        if (static_cast<HeaterBase*>(zone)->configValues(payload.setPointTemperature, payload.lowWarningLimit,
                payload.lowRangeLimit, payload.highRangeLimit, payload.highWarningLimit, message.header.sn))
        {
            PeriodicMonitorTask::getInstance().setPointWasActivated(message);
            status = E_AckStatus_Success;
        }
        else
        {
            status = E_AckStatus_InvalidState;
        }
    }

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageSetPowerHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSSetPowerMsg &payload = message.payload.pSSSetPowerMsg;

    E_AckStatus status;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload.zoneId);

    if (zone == NULL || zone->getZoneType() != E_ZoneType_Heater)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        // set the setpoint:
        if (static_cast<HeaterBase*>(zone)->setPowerLimit(payload.power))
        {
            status = E_AckStatus_Success;
        }
        else
        {
            status = E_AckStatus_InvalidState;
        }
    }

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageSetValveDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetValveDeviceConfigurationMsg &payload = message.payload.pSSSetValveDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex, E_ZoneType_Valve))
            != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set air inlet valve ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageActivateValveHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSActivateValveMsg &payload = message.payload.pSSActivateValveMsg;

    E_AckStatus status;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload.zoneId);

    if (zone == NULL || zone->getZoneType() != E_ZoneType_Valve)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        // set the setpoint:
        if (static_cast<CAirKnifePressureValveComponent*>(zone)->configValues(payload.valveState))
        {
            status = E_AckStatus_Success;
        }
        else
        {
            status = E_AckStatus_InvalidState;
        }
    }
    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageActivateZoneHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSActivateZoneMsg &payload = message.payload.pSSActivateZoneMsg;

    E_AckStatus status;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload.zoneId);

    if (zone == NULL)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        zone->m_lastSn = message.header.sn;
        zone->enableZone(payload.activate);
        status = E_AckStatus_Success;
        //zone->enableMonitoring(payload.activate);
    }

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);

}

void TmcMessageHandler::MessageSetFanSpeedHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSSetFanSpeedMsg &payload = message.payload.pSSSetFanSpeedMsg;
    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, E_AckStatus_Success);
}

void TmcMessageHandler::MessageAutoTuneHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSAutoTuneMsg *payload = &message.payload.pSSAutoTuneMsg;

    E_AckStatus status;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload->zoneId);

    if (zone == NULL)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        switch (zone->getZoneType())
        {
        case E_ZoneType_Heater:
            status = static_cast<HeaterBase*>(zone)->startAutoTune(payload->setPoint, payload->overShoot,
                    payload->powerAtStart, payload->powerStep, payload->calculation, false, message.header.sn);
            break;
        case E_ZoneType_IBD:
            status = static_cast<HeatExchangeZone*>(zone)->startAutoTune(payload->setPoint, payload->overShoot,
                    payload->powerAtStart, payload->powerStep, payload->calculation, false, message.header.sn);
            break;
        default:
            status = E_AckStatus_InvalidDevice;
            break;
        }
    }

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload->boardId, payload->zoneId, status);

}

void TmcMessageHandler::MessageGetPressureSensorStatusHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSGetPressureSensorStatusMsg &payload = message.payload.pSSGetPressureSensorStatusMsg;

    sendZoneStatusHandler(message, payload.boardId, payload.deviceId, E_ZoneType_AnalogInput);
}

void TmcMessageHandler::MessageSetPressureSensorDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSSetPressureSensorDeviceConfigurationMsg &payload = message.payload.pSSSetPressureSensorDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_PressureSensor)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set pressure sensor ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }

    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageGetValveStatusHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSGetValveStatusMsg &payload = message.payload.pSSGetValveStatusMsg;

    sendZoneStatusHandler(message, payload.boardId, payload.deviceId, E_ZoneType_Valve);
}

void TmcMessageHandler::MessageSetEngageSystemDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetEngageSystemConfigurationMsg &payload = message.payload.pSSSetEngageSystemConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_BtcEngageSystem)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Engage System ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageGetEngageSystemStatusHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSGetEngageSystemStatusMsg &payload = message.payload.pSSGetEngageSystemStatusMsg;

    sendZoneStatusHandler(message, payload.boardId, payload.deviceId, E_ZoneType_BtcEngageSystem);
}

void TmcMessageHandler::MessageActivateEngageSystemHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSActivateEngageSystemMsg &payload = message.payload.pSSActivateEngageSystemMsg;

    E_AckStatus status;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload.deviceId);

    if (zone == NULL || zone->getZoneType() != E_ZoneType_BtcEngageSystem)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        if (payload.position)
            static_cast<CEngageComponent*>(zone)->setCommand(BTC_COMMANDS_SERVICE_ENABLE_ENGAGE);
        else
            static_cast<CEngageComponent*>(zone)->setCommand(BTC_COMMANDS_SERVICE_DISABLE_ENGAGE);
    }

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageSetFlowMeterDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetFlowMeterDeviceConfigurationMsg &payload = message.payload.pSSSetFlowMeterDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_FlowMeter)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Flow Meter ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageSetLimitSwitchDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetLimitSwitchDeviceConfigurationMsg &payload = message.payload.pSSSetLimitSwitchDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_DigitalInput)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Limit Switch ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageSetPressureSwitchDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetPressureSensorDeviceConfigurationMsg &payload = message.payload.pSSSetPressureSensorDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_DigitalInput)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Limit Switch ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageSetTankLevelDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetTankLevelDeviceConfigurationMsg &payload = message.payload.pSSSetTankLevelDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_TankLevelSens)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Tank Level ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageGetFlowMeterStatusHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSGetFlowMeterStatusMsg &payload = message.payload.pSSGetFlowMeterStatusMsg;

    sendZoneStatusHandler(message, payload.boardId, payload.deviceId, E_ZoneType_FlowMeter);
}

void TmcMessageHandler::MessageSetPumpDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetPumpDeviceConfigurationMsg &payload = message.payload.pSSSetPumpDeviceConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex, E_ZoneType_Pump))
            != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Pump ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageSetWaterContaminationDeviceConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetWaterContaminationConfigurationMsg &payload = message.payload.pSSSetWaterContaminationConfigurationMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.deviceId, payload.ioIndex,
            E_ZoneType_AnalogInput)) != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Water Contamination ID 0x%x not found", payload.deviceId);
        status = E_AckStatus_InvalidDevice;
    }
    // send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageGetPumpStatusHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSGetFlowMeterStatusMsg &payload = message.payload.pSSGetFlowMeterStatusMsg;

    sendZoneStatusHandler(message, payload.boardId, payload.deviceId, E_ZoneType_Pump);
}

void TmcMessageHandler::MessageActivatePumpHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSActivatePumpMsg &payload = message.payload.pSSActivatePumpMsg;

    E_AckStatus status;

    // get the zone ID:
    BaseZone *zone = m_pZoneManager->getZoneById(payload.deviceId);

    if (zone == NULL || zone->getZoneType() != E_ZoneType_Pump)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
        static_cast<CBasePumpComponent*>(zone)->setCommand((payload.powerLimit != 0), payload.powerLimit);
        status = E_AckStatus_Success;
    }

    // send an ack that the message was received:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.deviceId, status);
}

void TmcMessageHandler::MessageGetDigitalSensorStatusHandler(unsigned long param)
{
    TmcMessageStruct &message = m_tmcMessages[param];
    PSSGetDigitalSensorStatusMsg &payload = message.payload.pSSGetDigitalSensorStatusMsg;

    sendZoneStatusHandler(message, payload.boardId, payload.deviceId, E_ZoneType_DigitalInput);
}

void TmcMessageHandler::MessageBootLoaderHandler(unsigned long param)
{
    STATUS db_res;
    int Data = 1;

    TmcMessageStruct &message = m_tmcMessages[param];
    PSSBootLoaderMsg &payload = message.payload.pSSBootLoaderMsg;

    if (payload.boardId == m_boardId)
    {
        sendAck(message.header.id.full, message.header.sn, payload.boardId, M_DEVICE_ID_ALL, E_AckStatus_Success);
        vTaskDelay(3000);
        db_res = CfgPutParam(NULL, ID_GNRL_CMND_INT_FW_UPGRD,  (void*) (&Data), (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);      
    }
}

void TmcMessageHandler::MessageResetBoardHandler(unsigned long param)
{
    STATUS db_res;
    int Data = 1;

    TmcMessageStruct &message = m_tmcMessages[param];
    PSSBootLoaderMsg &payload = message.payload.pSSBootLoaderMsg;

    if (payload.boardId == m_boardId)
    {
        sendAck(message.header.id.full, message.header.sn, payload.boardId, M_DEVICE_ID_ALL, E_AckStatus_Success);
        vTaskDelay(3000);
        db_res = CfgPutParam(NULL, ID_GNRL_CMND_INT_FEC_RESET, (void*) (&Data), (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
        //Igor: Insert code here.
    }
}

void TmcMessageHandler::MessageSetPIDHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetPIDMsg *payload = &message.payload.pSSSetPIDMsg;

    if (payload->boardId != m_boardId || payload->zoneId == M_DEVICE_ID_ALL)
    {
        sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL,
                E_AckStatus_InvalidDevice);
        return;
    }

    BaseZone* zone;
    zone = m_pZoneManager->getZoneById(payload->zoneId);
    if (zone == NULL)
    {
        sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL,
                E_AckStatus_InvalidDevice);
        return;
    }

    static_cast<HeaterBase*>(zone)->configPidValues(payload->p, payload->i, payload->d, payload->setPointSmoothing, payload->iThreshold, payload->outputSmoothing);

    sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL, E_AckStatus_Success);
}

void TmcMessageHandler::MessageGetPIDHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSGetPIDMsg *payload = &message.payload.pSSGetPIDMsg;

    if (payload->boardId != m_boardId || payload->zoneId == M_DEVICE_ID_ALL)
    {
        sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL,
                E_AckStatus_InvalidDevice);
        return;
    }

    BaseZone* zone;
    zone = m_pZoneManager->getZoneById(payload->zoneId);
    if (zone == NULL)
    {
        sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL,
                E_AckStatus_InvalidDevice);
        return;
    }

    static_cast<HeaterBase*>(zone)->sendPidValues();
}

void TmcMessageHandler::MessageSetLoggingLevelHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetLoggingLevelMsg *payload = &message.payload.pSSSetLoggingLevelMsg;

    if (payload->boardId != m_boardId && payload->boardId != M_BOARD_ID_ALL)
    {
        sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL,
                E_AckStatus_InvalidDevice);
        return;
    }

    if (payload->enableLogging)
    {
        CLogger::getInstance().enableOutputUdp(true);
        CLogger::getInstance().setAllTaskMask(payload->logLevel);
    }
    else
    {
        CLogger::getInstance().enableOutputUdp(false);
        CLogger::getInstance().setAllTaskMask(M_LOGGER_LEVEL_OFF);
    }

    sendAck(message.header.id.full, message.header.sn, payload->boardId, M_DEVICE_ID_ALL, E_AckStatus_Success);
}

void TmcMessageHandler::MessageActivateBTCControlHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageSetBtcControlConfigurationHandler(unsigned long param)
{
    TmcMessageStruct& message = m_tmcMessages[param];
    PSSSetBTCControlConfigMsg &payload = message.payload.pSSSetBTCControlConfigMsg;

    E_AckStatus status;
    BaseZone *zone;

    if ((zone = m_pZoneManager->assignIdToZoneType(payload.boardId, payload.zoneId, payload.index, E_ZoneType_Cooling))
            != NULL)
    {
        status = E_AckStatus_Success;
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Set Cooling ID 0x%x not found", payload.zoneId);
        status = E_AckStatus_InvalidDevice;
    }
// send an Ack:
    sendAck(message.header.id.full, message.header.sn, payload.boardId, payload.zoneId, status);
}

void TmcMessageHandler::sendAck(unsigned long messageId, unsigned long sn, char boardId, char zoneId, int status)
{
    TmcMessageStruct reply;
    reply.header.id.split.src = m_boardAddress;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_AckMessage;
    reply.header.length = sizeof(reply.header) + sizeof(reply.payload.pSSAckMsg);
    reply.payload.pSSAckMsg.originalRequestMsgId = messageId;
    reply.payload.pSSAckMsg.originalRequestSN = sn;
    reply.payload.pSSAckMsg.boardId = boardId;
    reply.payload.pSSAckMsg.zoneId = zoneId;
    reply.payload.pSSAckMsg.status = status;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Ack Message ID 0x%x, controller=%d, zone=%d, status=%d sn=%d", messageId,
            boardId, zoneId, status, sn);

    TmcMasterServer::getInstance().sendMessage(reply);
}

void TmcMessageHandler::sendSeqEnded(unsigned long messageId, unsigned long sn, char boardId, char zoneId, int status)
{
    TmcMessageStruct reply;
    reply.header.id.split.src = m_boardAddress;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_SeqEnded;
    reply.header.length = sizeof(reply.header) + sizeof(reply.payload.pSSSeqEndedMsg);
    reply.payload.pSSSeqEndedMsg.originalRequestMsgId = messageId;
    reply.payload.pSSSeqEndedMsg.originalRequestSN = sn;
    reply.payload.pSSSeqEndedMsg.boardId = boardId;
    reply.payload.pSSSeqEndedMsg.zoneId = zoneId;
    reply.payload.pSSSeqEndedMsg.status = status;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Seq Ended sent for message 0x%x", messageId);
    TmcMasterServer::getInstance().sendMessage(reply);
}

void TmcMessageHandler::sendZoneStatusHandler(TmcMessageStruct& message, char boardId, char deviceId,
        E_ZoneType zoneType)
{
    BaseZone* zone;

    if (boardId == m_boardId || boardId == M_BOARD_ID_ALL)
    {
        if (deviceId == M_DEVICE_ID_ALL)
        {
            for (int i = 0; i < m_pZoneManager->getNumberOfZones(); ++i)
            {
                zone = (*m_pZoneManager)[i];
                if (zone->getZoneType() == zoneType)
                    zone->sendZoneStatus();
            }
            sendAck(message.header.id.full, message.header.sn, boardId, deviceId, E_AckStatus_Success);
        }
        else
        {
            zone = m_pZoneManager->getZoneById(deviceId);
            if (zone == NULL || zone->getZoneType() != zoneType)
            {
                sendAck(message.header.id.full, message.header.sn, boardId, deviceId, E_AckStatus_InvalidDevice);
            }
            else
            {
                zone->sendZoneStatus();
                sendAck(message.header.id.full, message.header.sn, boardId, deviceId, E_AckStatus_Success);
            }
        }
    }
    else
    {
        sendAck(message.header.id.full, message.header.sn, boardId, deviceId, E_AckStatus_Success);
    }
}

void TmcMessageHandler::MessageKeepAliveHandler(unsigned long param)
{
    TmcMessageStruct reply;
    reply.header.id.split.src = m_boardAddress;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_KeepAliveReply;
    reply.header.length = sizeof(reply.header);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Keep Alive Reply");
    TmcMasterServer::getInstance().sendMessage(reply);
}

void TmcMessageHandler::handleMessage(TmcMessageStruct& message)
{
    CMessage m;

//    while (m_postMessageSemaphore.take(10) != pdPASS)
//        ;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Message ID Received: 0x%x sn=%d", message.header.id.split.id,
            message.header.sn);
// set the message ID:
    m.m_nId = message.header.id.split.id + APP_MSG;

// set the index of the message in the array:
    m.wParam = m_currentMessage;
// copy the message to the messages array.
    memcpy(m_tmcMessages + m_currentMessage, &message, sizeof(TmcMessageStruct));

//exitCritical();

// post the message to the queue:

    while (postMessage(&m, 1000) != pdPASS)
        ;
// advance the current index:
    m_currentMessage = (m_currentMessage + 1) & (MESSAGE_QUEUE_SIZE - 1);

//m_postMessageSemaphore.give();
}

void TmcMessageHandler::run()
{
//vTaskSetTaskNumber(getCurrentTaskHandle(), M_LOGGER_TASK_NUMBER_TMC_MESSAGE_HANDLER);
//CLogger::getInstance().setCurrentTaskMask(1);

    CMessageTask::run();
}

void TmcMessageHandler::reset()
{
    m_currentMessage = 0;

// suspend the monitoring task, as this is the first message received when the OPC connects to the board:
//    if (m_monitorTask.isValid())
//        m_monitorTask.suspend();
    PeriodicMonitorTask::getInstance().reset();
    m_pZoneManager->reset();
    m_registerMonitor.reset();
}

portBASE_TYPE TmcMessageHandler::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    portBASE_TYPE res;

    instantiateZoneManager();

    if (m_pZoneManager)
    {
        // initialize the zone manager
        m_pZoneManager->configZoneManager(m_boardId, m_deviceType);

        PeriodicMonitorTask::getInstance().setZoneManager(m_pZoneManager);
    }

    PeriodicMonitorTask::getInstance().setRegisterMonitor(&m_registerMonitor);
// create the periodic monitor task:
    PeriodicMonitorTask::getInstance().setBoardAddress(m_boardAddress);

    if ((res = m_postMessageSemaphore.create()) != pdPASS)
        return res;

    return CMessageTask::onCreate(pcName, usStackDepth, uxPriority);
}

void TmcMessageHandler::MessageGetAirBleedingZoneConfigHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetBlowerZoneConfigHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetControllerConfigurationHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetErrorHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetFanZoneConfigHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetTemperatureSensorZoneConfigHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetWarningsHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetTemperatureZoneConfigurationHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageReadControllerDataHandler(unsigned long param)
{
    MessageNotHandled(param);
}

//void TmcMessageHandler::MessageReadControllerDebugDataHandler(unsigned long param)
//{
//    MessageNotHandled(param);
//}

void TmcMessageHandler::MessageReadControllerSpecificDataHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageSetErrorMaskHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageSetWarningMaskHandler(unsigned long param)
{
    MessageNotHandled(param);
}

//void TmcMessageHandler::MessageGetPressureSwitchStatusHandler(unsigned long param)
//{
//    MessageNotHandled(param);
//}

void TmcMessageHandler::MessageGetTankLevelStatusHandler(unsigned long param)
{
    MessageNotHandled(param);
}

//void TmcMessageHandler::MessageSetloggerLevelHandler(unsigned long param)
//{
//    MessageNotHandled(param);
//}

void TmcMessageHandler::MessageGetWaterContaminationStatusHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetBtcControlStatusHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void TmcMessageHandler::MessageGetVersionHandler(unsigned long param)
{
    MessageNotHandled(param);
}

