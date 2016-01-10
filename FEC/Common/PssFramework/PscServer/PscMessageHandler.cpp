/*
 * PscMessageHandler.cpp
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#include "PscMessageHandler.h"
#include <PscSubsystem.h>
#include <version.h>
#include <logger.h>
#include "PscMasterServer.h"
#include <string.h>
#include "SubsystemInstantiator.h"
#include <Controls/ControlRepository.h>
#include <Elements/ElementRepository.h>
#include <Tasks/UpdateSchedulerTask.h>
#include <Peripherals/PeripheralRepository.h>
#ifndef WIN32
#include <sntp.h>
#endif
#include <board_setup.h>
#ifdef FEC2_BOARD
#include <Peripherals/PSoC/PsocMessageStruct.h>
#endif
#include <Controls/ProtectionConstantDeltaChecker.h>
#include <Controls/ProtectionProportionalChecker.h>
#include <controls/ProtectionCurrentLimitsChecker.h>

#undef M_BEGIN_MESSAGE_MAP
#undef M_END_MESSAGE_MAP
#undef M_MESSAGE_MAP_ENTRY

#define M_BEGIN_MESSAGE_MAP BEGIN_MESSAGE_MAP
#define M_END_MESSAGE_MAP END_MESSAGE_MAP
#define M_MESSAGE_MAP_ENTRY MESSAGE_MAP_ENTRY

#include "GeneratedMessages.msg.h"

#undef M_BEGIN_MESSAGE_MAP
#undef M_END_MESSAGE_MAP
#undef M_MESSAGE_MAP_ENTRY

// message id strings
#define M_BEGIN_MESSAGE_MAP(a,b) static const char messageIdStrings[][60] = {
#define M_END_MESSAGE_MAP(a) };
#define M_MESSAGE_MAP_ENTRY(a,b,c) #c ,

#include "GeneratedMessages.msg.h"

#undef M_BEGIN_MESSAGE_MAP
#undef M_END_MESSAGE_MAP
#undef M_MESSAGE_MAP_ENTRY

#define M_BEGIN_MESSAGE_MAP(a,b) static const uint32_t messageIdDict[] = {
#define M_END_MESSAGE_MAP(a) };
#define M_MESSAGE_MAP_ENTRY(a,b,c) b ,

#include "GeneratedMessages.msg.h"

#undef M_BEGIN_MESSAGE_MAP
#undef M_END_MESSAGE_MAP
#undef M_MESSAGE_MAP_ENTRY

#define M_CHECK_BOARD_ID(bid, msgId, sn, pssId) if (bid != m_cableId) { sendAck(msgId, sn, bid, pssId, E_AckStatus_InvalidDevice); return; }
#define M_CHECK_FOR_MASTER_ONLY(bid, slaveId, msgId, sn, pssId) if (slaveId != 0) { sendAck(msgId, sn, bid, pssId, E_AckStatus_InvalidDevice); return; }
#define M_CHECK_BOARD_STATE(expectedState, msgId, sn, pssId) if (expectedState != m_boardState.getState()) { sendAck(msgId, sn, m_cableId, pssId, E_AckStatus_InvalidState); return; }
#define M_CHECK_BOARD_STATE2(expectedState, expectedState2, msgId, sn, pssId) if (expectedState != m_boardState.getState() && expectedState2 != m_boardState.getState()) { sendAck(msgId, sn, m_cableId, pssId, E_AckStatus_InvalidState); return; }

PscMessageHandler* PscMessageHandler::p_instance = NULL;

const char* getMessageString(uint32_t messageId)
{
    int i = 0;
    do
    {
        if (messageIdDict[i] == messageId)
            return messageIdStrings[i];
    } while (messageIdDict[++i] != 0);

    return NULL;
}

// TODO: Need to define more error messages to the OPC.
PscMessageHandler::PscMessageHandler()
{
    m_boardAddress = 5;
    m_cableId = 1;
    m_masterControllerId = 1;
    m_deviceType = E_PSSDeviceType_Dryer;
    m_currentMessage = 0;

    m_pAppLoader = NULL;

    m_boardMode = E_BoardMode_Normal;
    m_deleteControlsOnDisconnection = true;
}

PscMessageHandler::~PscMessageHandler()
{
}

bool PscMessageHandler::hardwareInit()
{
    return true;
}

void PscMessageHandler::instantiateZoneManager()
{

    switch (Psc_CableId)
    {
    case E_PscSubsystem_Dryer:
        m_deviceType = E_PSSDeviceType_Dryer;
        break;
    case E_PscSubsystem_Ibt:
        m_deviceType = E_PSSDeviceType_IBD;
        break;
    case E_PscSubsystem_Ith:
//        m_subsystemInitiator.ith();
        m_deviceType = E_PSSDeviceType_ITH;
        break;
    case E_PscSubsystem_Btc:
        m_deviceType = E_PSSDeviceType_Cleaning;
        break;
    default:
        break;
    }

    m_cableId = Psc_ControllerId;
    m_masterControllerId = m_cableId;
}

void PscMessageHandler::performBoardShutdown()
{
    m_boardState.eventStartShuttingDown();

#ifdef FEC2_BOARD
    m_psocManager.startShutdown();
#endif

    if (m_pAppLoader != NULL)
    {
        delete m_pAppLoader;
        m_pAppLoader = NULL;
    }

    reset();

#ifdef FEC2_BOARD
    m_psocManager.reset();
#endif

    m_boardState.eventShuttingDownComplete();

    m_deleteControlsOnDisconnection = true;
    m_boardMode = E_BoardMode_Normal;
    m_psocManager.setBoardMode(m_boardMode);
}

void PscMessageHandler::MessageDisconnect(unsigned long param)
{
    // we need to perform the ordered shutdown operation regardless of the delete
    // controls mode, so that controls that should not stay operational after disconnection
    // will stop.
    if (UpdateSchedulerTask::getInstance()->isBoardInReady())
        ControlRepository::getInstance().executeShutdownOperation(true);

    if (m_deleteControlsOnDisconnection == true)
    {
        performBoardShutdown();
    }
}

void PscMessageHandler::MessageConnect(unsigned long param)
{
    // TODO: Add timeout on waiting for the board to shutdown.
    // TODO: Timeout should take into consideration the ordered shutdown state.
//    while (m_boardState.getState() != E_BoardState_Off)
//    {
//        delay(100);
//    }
    if (m_deleteControlsOnDisconnection == false)
    {
        performBoardShutdown();
    }
    m_boardMode = E_BoardMode_Normal;
    m_deleteControlsOnDisconnection = true;

    m_boardState.eventStartUp();
    // todo: move this to a more suitable place.
    m_cableId = Psc_ControllerId;
    m_masterControllerId = m_cableId;

    UpdateSchedulerTask::getInstance();
    ModbusSchedulerTask::getInstance();

//    PeripheralRepository::getInstance().initInternalPeripherals();

//    ControlRepository::getInstance().initProtectionControl();

    m_boardState.eventStartUpFinished();
}

void PscMessageHandler::MessageNotHandled(unsigned long param)
{
    PscMessageStruct& message = m_messages[param];

    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Message ID 0x%x not handled", message.header.id.split.id);

    sendAck(message.header.id.full, message.header.sn, M_BOARD_ID_ALL, M_PSS_ID_ALL, E_AckStatus_NotImplemented);
}

void PscMessageHandler::MessageGetDefinedPeriphealsHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];

    PSSGetDefinedPeripheralsReplyMsg *payload = &(message->payload.pSSGetDefinedPeripheralsReplyMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSGetDefinedPeripheralsReplyMsg: cableId=%d totalNumberOfPeriphs=%d periphIndex=%d periphType=%d deviceNum=%d",
            payload->cableId, payload->totalNumberOfPeripherals, payload->peripheralIndex, payload->peripheralType,
            payload->devicesNum);

    message->header.id.split.id = MSG_GetDefinedPeriphealsReply;
    message->header.id.split.src = M_PSC_ICD_ID;
    message->header.id.split.dst = M_OPC_ICD_ID;
    message->header.length = sizeof(*payload) + sizeof(message->header);
    payload->cableId = m_cableId;
    payload->totalNumberOfPeripherals = PeripheralRepository::getInstance().getPeripheralCount();
}

void PscMessageHandler::sendAck(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId,
        E_AckStatus status)
{
    PscMasterServer::getInstance().sendAck(messageId, sn, boardId, pssId, status);
}

void PscMessageHandler::sendSeqEnded(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId,
        E_SeqEndedStatus status)
{
    PscMasterServer::getInstance().sendSeqEnded(messageId, sn, boardId, pssId, status);
}

void PscMessageHandler::MessageKeepAliveHandler(unsigned long param)
{
    PscMessageStruct reply;
    reply.header.id.split.src = m_boardAddress;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_KeepAliveReply;
    reply.header.length = sizeof(reply.header);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Keep Alive Reply");
    PscMasterServer::getInstance().sendMessage(reply);

    //UpdateSchedulerTask::getInstance().keepaliveReceived();
}

void PscMessageHandler::MessageGetBoardTypeHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSGetBoardTypeReplyMsg *payload = &message->payload.pSSGetBoardTypeReplyMsg;

//    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSGetBoardTypeReplyMsg: cableId=%d slaveIndex=%d boardType=%d numOfSlaves=%d",
//            payload->cableId, payload->slaveIndex, payload->boardType, payload->numberOfSlaves);

    if (payload->cableId == m_cableId)
    {
        message->header.id.split.src = M_PSC_ICD_ID;
        message->header.id.split.dst = M_OPC_ICD_ID;
        message->header.id.split.id = MSG_GetBoardTypReply;
        message->header.length = sizeof(PscMessageHeader) + sizeof(PSSGetBoardTypeReplyMsg);

        payload->cableId = m_cableId;
        payload->boardType = Psc_GetBoardType();
        payload->numberOfSlaves = Psc_GetNumberOfSlaves();

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                "PSSGetBoardTypeReplyMsg: cableId=%d slaveIndex=%d boardType=%d numOfSlaves=%d", payload->cableId,
                payload->slaveIndex, payload->boardType, payload->numberOfSlaves);
        //TODO: Add log message
        PscMasterServer::getInstance().sendMessage(*message);
    }
    else
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_InvalidDevice);
    }

}

void PscMessageHandler::MessageGetCableIdHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSGetCableIdMsg *payload = &message->payload.pSSGetCableIdMsg;

//    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSGetCableIdMsg: cableId=%d slaveIndex=%d", payload->cableId,
            payload->slaveIndex);

    if (payload->slaveIndex == 0)
    {
        PSSGetCableIdReplyMsg *replyPayload = &message->payload.pSSGetCableIdReplyMsg;

        message->header.id.split.src = M_PSC_ICD_ID;
        message->header.id.split.dst = M_OPC_ICD_ID;
        message->header.id.split.id = MSG_GetCableIdReplay;
        message->header.length = sizeof(PscMessageHeader) + sizeof(PSSGetCableIdReplyMsg);

        replyPayload->masterCableId = m_cableId;
        replyPayload->errorBits = 0;
        replyPayload->cableId1 = m_cableId;
        replyPayload->cableId2 = m_cableId;
        replyPayload->cableId3 = m_cableId;
        replyPayload->slaveIndex = 0;

        //TODO: Add log message
        PscMasterServer::getInstance().sendMessage(*message);
    }
    else
    {
#ifdef FEC2_BOARD

//        T_PsocVersion psocVersion;
        E_PsocSpiError spiError;
        PSSGetCableIdReplyMsg *replyPayload = &message->payload.pSSGetCableIdReplyMsg;

        uint8_t cable1, cable2, cable3, errors;
//        spiError = m_psocManager.getPsocHandlerByIndex(payload->slaveIndex - 1)->getCableID(cable1, cable2,
//                cable3, errors);
        spiError = m_psocManager.getPsocCableID(payload->slaveIndex - 1, cable1, cable2, cable3, errors);

        message->header.id.split.src = M_PSC_ICD_ID;
        message->header.id.split.dst = M_OPC_ICD_ID;
        message->header.id.split.id = MSG_GetCableIdReplay;
        message->header.length = sizeof(PscMessageHeader) + sizeof(PSSGetCableIdReplyMsg);

        replyPayload->cableId1 = cable1;
        replyPayload->cableId2 = cable2;
        replyPayload->cableId3 = cable3;
        replyPayload->errorBits = spiError;

        //TODO: Add log message
        PscMasterServer::getInstance().sendMessage(*message);

#else
        sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_InvalidDevice);
#endif
    }

}

void PscMessageHandler::MessageSetPsocSlaveConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];

    PSSSetPsocSlaveConfigMsg *payload = &message->payload.pSSSetPsocSlaveConfigMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSSetPsocSlaveConfigMsg: cableId=%d slaveCableId=%d primaryFunction=%d",
            payload->cableId, payload->slaveCableId, payload->psocPrimaryFunction);

    E_AckStatus status;
    // if this message is intended to the master board:
//    if (payload->slaveIndex == 0)
//    {
//        status = E_AckStatus_InvalidDevice;
//    }
//    else
//    {
#ifdef FEC2_BOARD
//        bool result = m_psocManager.setPsocAllowedCableId(payload->slaveIndex - 1, payload->cableIdMask);

    // ignore the master:
    if (payload->slaveCableId == 0)
    {
        status = E_AckStatus_Success;
        switch ((E_PsocPrimaryFunction) payload->psocPrimaryFunction)
        {
//        case E_PsocPrimaryFunction_Undefined:
//            m_boardMode = E_BoardMode_Normal;
//            break;
        case E_PsocPrimaryFunction_HwValidation:
            m_boardMode = E_BoardMode_HwValidation;
            break;
//        default:
//            status = E_AckStatus_InvalidState;
//            break;
        }
        m_psocManager.setBoardMode(m_boardMode);
    }
    else
    {
        bool result = m_psocManager.setPsocChannelFunction(payload->slaveCableId,
                (E_PsocPrimaryFunction) payload->psocPrimaryFunction);

        if (result)
            status = E_AckStatus_Success;
        else
            status = E_AckStatus_InvalidDevice;

        //TODO: Send the PSoC primary function message to the PSoC.
        switch ((E_PsocPrimaryFunction) payload->psocPrimaryFunction)
        {
        case E_PsocPrimaryFunction_LiftPbOnError:
        {
            // if the primary function is LiftPbOnError, we should initialize the control on the master board as well:
            LiftPbOnError* liftControl = new LiftPbOnError();
            if (liftControl->initInputElements(payload->slaveCableId))
                status = E_AckStatus_Success;
            else
                status = E_AckStatus_InvalidDevice;
            break;
        }
        case E_PsocPrimaryFunction_LiftPbOnErrorGen2:
        {
            // if the primary function is LiftPbOnError, we should initialize the control on the master board as well:
            LiftPbOnErrorCcsGen2* liftControl = new LiftPbOnErrorCcsGen2();
            if (liftControl->initInputElements(payload->slaveCableId))
                status = E_AckStatus_Success;
            else
                status = E_AckStatus_InvalidDevice;
            break;
        }
#else
            status = E_AckStatus_InvalidDevice;
#endif
        }
    }
    sendAck(message->header.id.full, message->header.sn, m_cableId, M_PSS_ID_ALL, status);
}

void PscMessageHandler::MessageSetPsocAllowedCableMaskHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];

    PSSSetPsocAllowedCableMaskMsg *payload = &message->payload.pSSSetPsocAllowedCableMaskMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSSetPsocAllowedCableMaskMsg: cableId=%d slaveIndex=%d allowedCableIdMask=%x",
            payload->cableId, payload->slaveIndex, payload->allowedCableIdMask);

    E_AckStatus status;
    // if this message is intended to the master board:
    if (payload->slaveIndex == 0)
    {
        status = E_AckStatus_InvalidDevice;
    }
    else
    {
#ifdef FEC2_BOARD
        bool result = m_psocManager.setPsocAllowedCableId(payload->slaveIndex - 1, payload->allowedCableIdMask);
        if (result)
            status = E_AckStatus_Success;
        else
            status = E_AckStatus_InvalidDevice;

#else
        status = E_AckStatus_InvalidDevice;
#endif
    }
    sendAck(message->header.id.full, message->header.sn, m_cableId, M_PSS_ID_ALL, status);
}

void PscMessageHandler::MessageGetVersionHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSGetVersionReplyMsg *payload = &message->payload.pSSGetVersionReplyMsg;

//    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    if (payload->slaveIndex == 0)
    {
        message->header.id.split.src = M_PSC_ICD_ID;
        message->header.id.split.dst = M_OPC_ICD_ID;
        message->header.id.split.id = MSG_GetVersionReply;
        message->header.length = sizeof(PscMessageHeader) + sizeof(PSSGetVersionReplyMsg);

        payload->firmwareVersion = ((unsigned __int64) M_FEC_FIRMWARE_VERSION_MAJOR << 48)
                | ((unsigned __int64) M_FEC_FIRMWARE_VERSION_MINOR << 32)
                | ((unsigned __int64) M_FEC_FIRMWARE_VERSION_BUILD << 16)
                | ((unsigned __int64) M_FEC_FIRMWARE_VERSION_REVISION);
        payload->protocolVersion = ((unsigned __int64) M_OPC_ICD_VERSION_MAJOR << 48)
                | ((unsigned __int64) M_OPC_ICD_VERSION_MINOR << 32)
                | ((unsigned __int64) M_OPC_ICD_VERSION_BUILD << 16) | ((unsigned __int64) M_OPC_ICD_VERSION_REVISION);
        payload->cableId = m_cableId;
        payload->cableId1 = m_cableId;
        payload->cableId2 = m_cableId;
        payload->cableId3 = m_cableId;

        //TODO: Add log message
        PscMasterServer::getInstance().sendMessage(*message);
    }
    else
    {
#ifdef FEC2_BOARD
        T_PsocVersion psocVersion;
//        E_PsocSpiError spiError;
        uint8_t cable1, cable2, cable3, errorBits;
//        spiError = m_psocManager.getPsocHandlerByIndex(payload->slaveIndex - 1)->getVersion(psocVersion);
        /*spiError = */m_psocManager.getPsocVersion(payload->slaveIndex - 1, psocVersion);
        /*spiError = */m_psocManager.getPsocCableID(payload->slaveIndex - 1, cable1, cable2, cable3, errorBits);

        message->header.id.split.src = M_PSC_ICD_ID;
        message->header.id.split.dst = M_OPC_ICD_ID;
        message->header.id.split.id = MSG_GetVersionReply;
        message->header.length = sizeof(PscMessageHeader) + sizeof(PSSGetVersionReplyMsg);

        // need to reverse the word order:
        payload->firmwareVersion = ((unsigned __int64) psocVersion.firmwareVersion.split.major << 48)
                | ((unsigned __int64) psocVersion.firmwareVersion.split.minor << 32)
                | ((unsigned __int64) psocVersion.firmwareVersion.split.build << 16)
                | ((unsigned __int64) psocVersion.firmwareVersion.split.revision);
        payload->protocolVersion = ((unsigned __int64) psocVersion.protocolVersion.split.major << 48)
                | ((unsigned __int64) psocVersion.protocolVersion.split.minor << 32)
                | ((unsigned __int64) psocVersion.protocolVersion.split.build << 16)
                | ((unsigned __int64) psocVersion.protocolVersion.split.revision);
//        payload->slaveIndex = payload->slaveIndex;
//        payload->cableId = cable1;
        payload->cableId = m_cableId;
        payload->cableId1 = cable1;
        payload->cableId2 = cable2;
        payload->cableId3 = cable3;

        // TODO: Psoc: check compatibility of protocol versions.

        //TODO: Add log message
        PscMasterServer::getInstance().sendMessage(*message);

#else
        sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_InvalidDevice);
#endif
    }

}

void PscMessageHandler::MessageGetBoardStatusHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSBoardStatusNotificationMsg *payload = &message->payload.pSSBoardStatusNotificationMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    message->header.id.split.src = M_PSC_ICD_ID;
    message->header.id.split.dst = M_OPC_ICD_ID;
    message->header.id.split.id = MSG_BoardStatusNotification;

    payload->cableId = m_cableId;
    payload->exceptions = 0;
    payload->boardState = m_boardState.getState();

    //TODO: Add log message
    PscMasterServer::getInstance().sendMessage(*message);
}

void PscMessageHandler::MessageDefineLoggingHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetLoggingLevelMsg *payload = &message->payload.pSSSetLoggingLevelMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSSetLoggingLevelMsg: cableId=%d logLevel=%d taskId=%d enableLogging=%d",
            payload->cableId, payload->logLevel, payload->taskId, payload->enableLogging);

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

    // TODO: Add log message.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_Success);
}

void PscMessageHandler::MessageSetBoardConfigurationHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetBoardConfigMsg *payload = &message->payload.pSSSetBoardConfigMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_CHECK_BOARD_STATE(E_BoardState_On, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSSetBoardConfigMsg: cableId=%d ntpServerAddress=%x baudRate=%d",
            payload->cableId, payload->ntpServerAddress, payload->baudRate);

#ifndef WIN32
    if (sntp_server_address == 0 && payload->ntpServerAddress != 0)
        sntp_init(payload->ntpServerAddress);
#endif

#ifndef WIN32
    ChangeUsartBaudrate(USART3, payload->baudRate);
#endif

    // TODO: Add log message.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_Success);
}

void PscMessageHandler::MessageResetBoardHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSResetBoardMsg *payload = &message->payload.pSSResetBoardMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    // TODO: Add log message.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_Success);

    delay(1000);
    resetBoard(payload->startupApp);
}

void PscMessageHandler::MessageBoardRecoveryHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSBoardRecoveryMsg *payload = &message->payload.pSSBoardRecoveryMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSBoardRecoveryMsg: cableId=%d", payload->cableId);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_Success);

    if (m_boardState.getState() == E_BoardState_EMR)
        recoverFromEmrError();

    m_boardState.eventStartRecovery();

    // take the scheduler semaphores, so that no updates will occur:
    UpdateSchedulerTask::getInstance()->suspendScheduler();
    ModbusSchedulerTask::getInstance()->suspendScheduler();

    m_psocManager.startRecovery();
    PeripheralRepository::getInstance().startRecovery();
    ControlRepository::getInstance().startRecovery();

    // restart the update process:
    UpdateSchedulerTask::getInstance()->resumeScheduler();
    ModbusSchedulerTask::getInstance()->resumeScheduler();

    m_boardState.eventEndRecovery();

    if (m_boardState.getState() != E_BoardState_Ready)
        sendSeqEnded(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL,
                E_SeqEndedStatus_Error);
    else
        sendSeqEnded(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL,
                E_SeqEndedStatus_Success);
}

void PscMessageHandler::MessageStartBoardConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSStartBoardConfigMsg *payload = &message->payload.pSSStartBoardConfigMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    m_boardState.eventBoardConfigStart();

    // here we check that board successfully moved to the initializing state. We don't need to check the start start, as
    // if the start state is incorrect, the board wouldn't move to the requested state.
    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

//#ifdef FEC2_BOARD
//    m_psocManager.
//#endif
    // TODO: Add log message.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_Success);
}

void PscMessageHandler::MessageEndBoardConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSEndBoardConfigMsg *payload = &message->payload.pSSEndBoardConfigMsg;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    m_boardState.eventBoardConfigEnd();

    // here we check that board successfully moved to the initializing state. We don't need to check the start start, as
    // if the start state is incorrect, the board wouldn't move to the requested state.
    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSEndBoardConfig: cableId=%d deleteOnDisconnection=%d", payload->cableId,
            payload->deleteOnDisconnection);

    // TODO: Add log message.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, M_PSS_ID_ALL, E_AckStatus_Success);

    m_deleteControlsOnDisconnection = payload->deleteOnDisconnection;

    //m_psocManager.startRecovery();
    //PeripheralRepository::getInstance().startRecovery();
    //ControlRepository::getInstance().startRecovery();

    // Just to be on the safe side - if the SUM didn't initialize the protection control,
    // we need to do it anyway so the dry contact will open.
    ControlRepository::getInstance().initProtectionControl();
#ifdef FEC2_BOARD
//#ifndef DEBUG
    if (m_boardMode != E_BoardMode_HwValidation)
        ControlRepository::getInstance().initEmergencyInputControl();
//#endif
#endif

    // TODO: move to state machine?
    UpdateSchedulerTask::getInstance()->setBoardInReady(true);
    ModbusSchedulerTask::getInstance()->setBoardInReady(true);

#ifdef FEC2_BOARD
    m_psocManager.downloadConfigurationToPsocs();
#endif

    ControlRepository::getInstance().sendUpdateNotificationForAllControls();

    PeripheralRepository::getInstance().getDryContactOutput()->setDryContactState(true);
}

void PscMessageHandler::MessageDefineOnboardPT100PeriphHandler(unsigned long param)
{
    E_AckStatus status;
    PscMessageStruct* message = &m_messages[param];
    PSSDefineOnboardPT100PeriphMsg* payload = &(message->payload.pSSDefineOnboardPT100PeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineOnboardPT100PeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} sampleInterval=%d numOfDevices=%d lowPassWindow=%d",
            payload->cableId, payload->slaveCableId, payload->periphPSSId, payload->sampleInterval,
            payload->numberOfDevices, payload->lowPassWindow);

    if (payload->slaveCableId == 0)
    {

        M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn,
                payload->periphPSSId);

        if (PeripheralRepository::getInstance().initInternalTemperatureSensors(payload->periphPSSId,
                payload->sampleInterval, payload->lowPassWindow, payload->numberOfDevices) == false)
            status = E_AckStatus_InvalidDevice;
        else
            status = E_AckStatus_Success;

    }
    else
    {
#ifdef FEC2_BOARD
        bool result = m_psocManager.initTemperaturePeripheralByCableId(payload->slaveCableId, payload->periphPSSId,
                payload->sampleInterval, payload->lowPassWindow, payload->numberOfDevices);
        if (result)
            status = E_AckStatus_Success;
        else
            status = E_AckStatus_InvalidDevice;
#else
        status = E_AckStatus_InvalidDevice;
#endif
    }
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, status);
}

void PscMessageHandler::MessageDefineOnboardI2cMi3IrPeriphHandler(unsigned long param)
{
#ifdef FEC2_BOARD
    E_AckStatus status;
    PscMessageStruct* message = &m_messages[param];
    PSSDefineOnboardI2cMi3IrPeriph* payload = &(message->payload.pSSDefineOnboardI2cMi3IrPeriph);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineOnboardI2cMi3IrPeriph: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} sampleInterval=%d numOfDevices=%d lowPassWindow=%d",
            payload->cableId, payload->slaveCableId, payload->periphPSSId, payload->sampleInterval,
            payload->numberOfDevices, payload->lowPassWindow);

    if (payload->slaveCableId == 0)
    {

        M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn,
                payload->periphPSSId);

        Mi3I2CIrPeripheral* mi3Periph = new Mi3I2CIrPeripheral();
        if (mi3Periph == NULL)
            status = E_AckStatus_InvalidDevice;
        else
        {
            mi3Periph->setPssId(payload->periphPSSId);
            mi3Periph->setUpdateInterval(payload->sampleInterval);

            PeripheralRepository::getInstance().addPeripheral(mi3Periph);
            status = E_AckStatus_Success;
        }
    }
    else
    {
        status = E_AckStatus_InvalidDevice;
    }

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, status);
#else
    MessageNotHandled(param);
#endif

}

void PscMessageHandler::MessageDefineOnboardPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineOnboardPeriphMsg* payload = &(message->payload.pSSDefineOnboardPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineOnboardPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} numOfDevices=%d periphType=%d",
            payload->cableId, payload->slaveCableId, payload->periphPSSId, payload->numberOfDevices,
            payload->periphType);

    E_AckStatus status = E_AckStatus_Success;
    bool result;

    E_PeripheralType pType = (E_PeripheralType) payload->periphType;

    if (payload->slaveCableId == 0)
    {
        switch (pType)
        {
        case E_PeripheralType_DO:
            result = PeripheralRepository::getInstance().initDigitalOutputs(payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_SwPwm:
            result = PeripheralRepository::getInstance().initSwPwmOutput(payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_DryContact:
            result = PeripheralRepository::getInstance().initDryContactOutput(payload->periphPSSId);
            break;
        case E_PeripheralType_DI:
            result = PeripheralRepository::getInstance().initDigitalInputs(payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_AO:
            result = PeripheralRepository::getInstance().initAnalogCurrentOutputs(payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_AI:
            result = PeripheralRepository::getInstance().initAnalogInputs(payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_SmcHrsChiller:
        {
            ModbusSmcHrsChiller* periph = new ModbusSmcHrsChiller(0);
            if (periph == NULL)
            {
                status = E_AckStatus_InvalidDevice;
            }

            break;
        }
        default:
            status = E_AckStatus_InvalidDevice;
            break;
        }
        if (result == false)
            status = E_AckStatus_InvalidDevice;
    }
    else
    {
#ifdef FEC2_BOARD
        bool result;
        switch (pType)
        {
        case E_PeripheralType_HwPWM:
            result = m_psocManager.initPwmPeripheralByCableId(payload->slaveCableId, payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_DO:
            result = m_psocManager.initDigitalOutputPeripheralByCableId(payload->slaveCableId, payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_DI:
            result = m_psocManager.initDigitalInputPeripheralByCableId(payload->slaveCableId, payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        case E_PeripheralType_AI:
            result = m_psocManager.initAnalogInputPeripheralByCableId(payload->slaveCableId, payload->periphPSSId,
                    payload->sampleInterval, payload->lowPassFilter, payload->numberOfDevices);
            break;
//        case E_PeripheralType_DryContact:
//            result = PeripheralRepository::getInstance().initDryContactOutput(payload->periphPSSId);
//            break;
        case E_PeripheralType_AO:
            result = m_psocManager.initAnalogOutputPeripheralByCableId(payload->slaveCableId, payload->periphPSSId,
                    payload->numberOfDevices);
            break;
        default:
            result = false;
            status = E_AckStatus_InvalidDevice;
            break;
        }
        if (result)
            status = E_AckStatus_Success;
        else
            status = E_AckStatus_InvalidDevice;
#else
        status = E_AckStatus_InvalidDevice;
#endif
    }
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, status);
}

void PscMessageHandler::MessageDefine6RTDPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefine6RTDPeriphMsg* payload = &(message->payload.pSSDefine6RTDPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefine6RTDPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} modbusId=%d", payload->cableId,
            payload->slaveCableId, payload->periphPSSId, payload->modbusId);

    Modbus6RTDPeripheral* periph = new Modbus6RTDPeripheral((uint8_t) payload->modbusId);
    periph->setPssId(payload->periphPSSId);
    PeripheralRepository::getInstance().addPeripheral(periph);
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefine8TCPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefine8TCPeriphMsg* payload = &(message->payload.pSSDefine8TCPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefine8TCPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} modbusId=%d", payload->cableId,
            payload->slaveCableId, payload->periphPSSId, payload->modbusId);

    Modbus8TCPeripheral* periph = new Modbus8TCPeripheral((uint8_t) payload->modbusId);
    periph->setPssId(payload->periphPSSId);
    PeripheralRepository::getInstance().addPeripheral(periph);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineDataCardPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineDataCardPeriphMsg* payload = &(message->payload.pSSDefineDataCardPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineDataCardPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} modbusId=%d",
            payload->cableId, payload->slaveCableId, payload->periphPSSId, payload->modbusId);

    ModbusDataCardPeripheral* periph = new ModbusDataCardPeripheral((uint8_t) payload->modbusId);
    periph->setPssId(payload->periphPSSId);
    PeripheralRepository::getInstance().addPeripheral(periph);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineInverterPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineInverterPeriphMsg* payload = &(message->payload.pSSDefineInverterPeriphMsg);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineInverterPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} modbusId=%d invType=%d config=%f/%f/%f/%f/%f/%f/%f",
            payload->cableId, payload->slaveCableId, payload->periphPSSId, payload->modbusId, payload->inverterType,
            payload->motorFrequency, payload->motorCurrent, payload->nominalRpm, payload->minSpeed, payload->maxSpeed,
            payload->accelRate, payload->decelRate);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    ModbusInverterPeripheralBase* periph;

    switch (payload->inverterType)
    {
    case E_ModbusInverterType_CommanderSK:
        periph = new ModbusInverterCommanderSK((uint8_t) payload->modbusId);
        //periph->setInverterType((E_ModbusInverterType) payload->inverterType);
        break;
    case E_ModbusInverterType_UnidriveM200:
        periph = new ModbusInverterUnidriveM200((uint8_t) payload->modbusId);
//        periph->setInverterType((E_ModbusInverterType) payload->inverterType);
        break;
    case E_ModbusInverterType_SchneiderAltivar32:
        periph = new ModbusInverterSchneiderAtv32((uint8_t) payload->modbusId);
        break;
    }

    periph->setupInverterMaxSpeed(payload->maxSpeed);
    periph->setupInverterMinSpeed(payload->minSpeed);
    periph->setupInverterAccelRate(payload->accelRate);
    periph->setupInverterDecelRate(payload->decelRate);
    periph->setupInverterMotorCurrent(payload->motorCurrent);
    periph->setupInverterMotorFrequency(payload->motorFrequency);
    periph->setupInverterMotorNominalRPM(payload->nominalRpm);

    ((InputPeripheralBase*) periph)->setPssId(payload->periphPSSId);
    PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*) periph);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, E_AckStatus_Success);
}

void PscMessageHandler::MessageWriteModbusRegisterHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSWriteModbusRegister* payload = &(message->payload.pSSWriteModbusRegister);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSWriteModbusRegister: cableId=%d periphPssId={[PSSID:%d]} registerAddress=%d value=%d", payload->cableId,
            payload->periphPSSId, payload->registerAddress, payload->registerValue);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    PeripheralBase* periph;

    periph = PeripheralRepository::getInstance().getPeripheralByPssId(payload->periphPSSId);

    if (periph == NULL)
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId,
                E_AckStatus_InvalidDevice);
    else
    {
        dynamic_cast<ModbusInputOutputPeripheralBase*>(periph)->addGeneralSetupRegister(payload->registerAddress,
                payload->registerValue);
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId,
                E_AckStatus_Success);
    }
}

void PscMessageHandler::MessageDefinePumaPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefinePumaPeriphMsg* payload = &(message->payload.pSSDefinePumaPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPSSId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPSSId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->periphPSSId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefinePumaPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} modbusId=%d", payload->cableId,
            payload->slaveCableId, payload->periphPSSId, payload->modbusId);

    ModbusPumaPeripheral* periph = new ModbusPumaPeripheral((uint8_t) payload->modbusId);
    periph->setPssId(payload->periphPSSId);
    PeripheralRepository::getInstance().addPeripheral(periph);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPSSId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineModbusPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineModbusPeriphMsg* payload = &(message->payload.pSSDefineModbusPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPssId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->periphPssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineModbusPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} modbusId=%d periphType=%d",
            payload->cableId, payload->slaveCableId, payload->periphPssId, payload->modbusId, payload->peripheralType);

    E_PeripheralType pType = (E_PeripheralType) payload->peripheralType;

    if (payload->slaveCableId == 0)
    {
        PeripheralBase* periph;
        switch (pType)
        {
        case E_PeripheralType_SmcHrsChiller:
            periph = (InputPeripheralBase*) (new ModbusSmcHrsChiller(payload->modbusId));
            break;
        case E_PeripheralType_Puma:
            periph = (InputPeripheralBase*) (new ModbusPumaPeripheral(payload->modbusId));
            break;
        case E_PeripheralType_6RTD:
            periph = (InputPeripheralBase*) (new Modbus6RTDPeripheral(payload->modbusId));
            break;
        case E_PeripheralType_8TC:
            periph = (InputPeripheralBase*) (new Modbus8TCPeripheral(payload->modbusId));
            break;
        case E_PeripheralType_DataCard:
            periph = (InputPeripheralBase*) (new ModbusDataCardPeripheral(payload->modbusId));
            break;
        default:
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        periph->setPssId(payload->periphPssId);
        PeripheralRepository::getInstance().addPeripheral(periph);

        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPssId,
                E_AckStatus_Success);

    }
    else
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPssId,
                E_AckStatus_InvalidDevice);

}

void PscMessageHandler::MessageDefineVirtualPeriphHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineVirtualPeriphMsg* payload = &(message->payload.pSSDefineVirtualPeriphMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->periphPssId);
    M_CHECK_FOR_MASTER_ONLY(payload->cableId, payload->slaveCableId, message->header.id.full, message->header.sn,
            payload->periphPssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->periphPssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineVirtualPeriphMsg: cableId=%d slaveCableId=%d periphPssId={[PSSID:%d]} periphType=%d",
            payload->cableId, payload->slaveCableId, payload->periphPssId, payload->peripheralType);

    E_PeripheralType pType = (E_PeripheralType) payload->peripheralType;

    if (payload->slaveCableId == 0)
    {
        PeripheralBase* periph;
        switch (pType)
        {
        case E_PeripheralType_VirtualFloatPeripheral:
        case E_PeripheralType_VirtualIntegerPeripheral:
            periph = new VirtualPeripheral(pType);
            break;
        default:
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        periph->setPssId(payload->periphPssId);
        PeripheralRepository::getInstance().addPeripheral(periph);

        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPssId,
                E_AckStatus_Success);
    }
    else
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->periphPssId,
                E_AckStatus_InvalidDevice);

}

void PscMessageHandler::MessageSetDeviceConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetDeviceConfigMsg* payload = &(message->payload.pSSSetDeviceConfigMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetDeviceConfigMsg: cableId=%d pssId={[PSSID:%d]} periphPssId={[PSSID:%d]} deviceIndex=%d",
            payload->cableId, payload->pssId, payload->periphPSSId, payload->deviceIndex);

//    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByIndex(
//            (E_PeripheralType) payload->periphType, payload->periphIndex);
    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByPssId(payload->periphPSSId);

    if (periph == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ElementBase* element = periph->getElementByIndex(payload->deviceIndex);

    if (element == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    if (element->getPssId() != M_UNASSIGNED_PSS_ID)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    element->setPssId(payload->pssId);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageSetAnalogDeviceConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetAnalogDeviceConfigMsg* payload = &(message->payload.pSSSetAnalogDeviceConfigMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetAnalogDeviceConfigMsg: cableId=%d pssId={[PSSID:%d]} periphPssId={[PSSID:%d]} deviceIndex=%d calA=%f calB=%f scaleA=%f scaleB=%f",
            payload->cableId, payload->pssId, payload->periphPssid, payload->deviceIndex, payload->aCoff,
            payload->bCoff, payload->scalingA, payload->scalingB);

//    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByIndex(
//            (E_PeripheralType) payload->periphType, payload->periphIndex);
    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByPssId(payload->periphPssid);

    if (periph == NULL
            || (periph->getPeripheralType() != E_PeripheralType_AI && periph->getPeripheralType() != E_PeripheralType_AO))
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    if (periph->getPeripheralType() == E_PeripheralType_AI)
    {
        if (payload->scalingA != 0)
            static_cast<AnalogInputPeripheralBase*>(periph)->setScalingCoeff(payload->deviceIndex, payload->scalingA,
                    payload->scalingB);

        if (payload->aCoff != 0)
            static_cast<AnalogInputPeripheralBase*>(periph)->setCalibrationCoeff(payload->deviceIndex, payload->aCoff,
                    payload->bCoff);
    }

    ElementBase* element = periph->getElementByIndex(payload->deviceIndex);

    if (element == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    element->setPssId(payload->pssId);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageSetTemperatureDeviceConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetTemperatureDeviceConfigMsg* payload = &(message->payload.pSSSetTemperatureDeviceConfigMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetTemperatureDeviceConfigMsg: cableId=%d pssId={[PSSID:%d]} periphPssId={[PSSID:%d]} deviceIndex=%d calA=%f calB=%f sensType=%d",
            payload->cableId, payload->pssId, payload->periphPssid, payload->deviceIndex, payload->aCoff,
            payload->bCoff, payload->sensorType);

//    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByIndex(
//            (E_PeripheralType) payload->periphType, payload->periphIndex);
    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByPssId(payload->periphPssid);

    if (periph == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    if (periph->getPeripheralType() == E_PeripheralType_InternalTemperature
            || periph->getPeripheralType() == E_PeripheralType_8TC
            || periph->getPeripheralType() == E_PeripheralType_DataCard
            /*|| periph->getPeripheralType() == E_PeripheralType_6RTD
             || periph->getPeripheralType() == E_PeripheralType_Puma*/)
    {
        if (payload->aCoff != 0)
            static_cast<AnalogInputPeripheralBase*>(periph)->setCalibrationCoeff(payload->deviceIndex, payload->aCoff,
                    payload->bCoff);
        static_cast<AnalogInputPeripheralBase*>(periph)->setSensorType(payload->deviceIndex, payload->sensorType);
    }
    else if (periph->getPeripheralType() == E_PeripheralType_6RTD
            || periph->getPeripheralType() == E_PeripheralType_Puma)
    {
    }
    else
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ElementBase* element = periph->getElementByIndex(payload->deviceIndex);

    if (element == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    element->setPssId(payload->pssId);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageSetMi3IrDeviceConfigHandler(unsigned long param)
{
#ifdef FEC2_BOARD
    PscMessageStruct* message = &m_messages[param];
    PSSSetMi3IrDeviceConfigMsg* payload = &(message->payload.pSSSetMi3IrDeviceConfigMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetMi3IrDeviceConfigMsg: cableId=%d pssId={[PSSID:%d]} periphPssId={[PSSID:%d]} deviceIndex=%d i2cChannel=%d calA=%f calB=%f",
            payload->cableId, payload->pssId, payload->periphPssid, payload->deviceIndex, payload->i2cChannel,
            payload->aCoff, payload->bCoff);

    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByPssId(payload->periphPssid);

    if (periph == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    if (periph->getPeripheralType() != E_PeripheralType_MI3IRSensorsPeripheral)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    Mi3I2CIrPeripheral* mi3Periph = static_cast<Mi3I2CIrPeripheral*>(periph);

    if (payload->deviceIndex == 0)
    {
        mi3Periph->setResetCountPssId(payload->pssId);
    }
    else
    {
        Mi3Sensor* mi3Sensor = mi3Periph->getSensorByAddress(payload->deviceIndex);

        if (mi3Sensor == NULL)
        {
            //TODO: add log message
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        mi3Sensor->setTargTempElementPssId(payload->pssId);
        if (mi3Sensor->setI2CChannel(payload->i2cChannel) == false)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        if (payload->aCoff == 0)
        {
            payload->aCoff = 1;
            payload->bCoff = 0;
        }
        mi3Sensor->writeGain(payload->aCoff);
        mi3Sensor->writeOffset(payload->bCoff);
        mi3Sensor->writeAmbientBackground(payload->ambientBackground);
        mi3Sensor->writeAmbientBackgroundCompensation(payload->ambientBackgroundCompensation);
        mi3Sensor->writeBottomTemp(payload->bottomTemp);
        mi3Sensor->writeTopTemp(payload->topTemp);
        mi3Sensor->writeEmissivity(payload->emissivity);
        mi3Sensor->writeTransmissivity(payload->transmissivity);
    }
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
#else
    MessageNotHandled(param);
#endif
}

void PscMessageHandler::MessageSetSwPWMDeviceConfigHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetSWPWMDeviceConfigMsg* payload = &(message->payload.pSSSetSWPWMDeviceConfigMsg);
    E_AckStatus status = E_AckStatus_Success;
    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetSWPWMDeviceConfigMsg: cableId=%d pssId={[PSSID:%d]} periphPssId={[PSSID:%d]} deviceIndex=%d pwmLength=%d pwmGroup=%d riseStep=%d fallStep=%d riseTime=%d fallTime=%d",
            payload->cableId, payload->pssId, payload->periphPSSId, payload->deviceIndex, payload->pwmCycleLength,
            payload->pwmGroupID, payload->rampRiseStep, payload->rampFallStep, payload->rampRiseTime,
            payload->rampFallTime);

//    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByIndex(
//            (E_PeripheralType) payload->periphType, payload->periphIndex);
    PeripheralBase* periph = PeripheralRepository::getInstance().getPeripheralByPssId(payload->periphPSSId);

    if (periph == NULL)
    {
        //TODO: add log message
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    switch (periph->getPeripheralType())
    {
    case E_PeripheralType_SwPwm:
    {
        ElementBase* element = periph->getElementByIndex(payload->deviceIndex);

        if (element == NULL)
        {
            //TODO: add log message
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        // config the DO to be a SW PWM channel.
        SwPwmOutputPeripheral* swpwm = static_cast<SwPwmOutputPeripheral*>(periph);
        swpwm->configPwmChannel(payload->deviceIndex, payload->pwmCycleLength, payload->pwmGroupID);
        swpwm->enableElementByIndex(payload->deviceIndex, true);

        element->setPssId(payload->pssId);
        break;
    }
    case E_PeripheralType_HwPWM:
    {
#ifdef FEC2_BOARD
        ElementBase* element = periph->getElementByIndex(payload->deviceIndex);

        if (element == NULL)
        {
            //TODO: add log message
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        // config the the pwm channel type:
        PsocPwmOutputPeripheral* pwm = static_cast<PsocPwmOutputPeripheral*>(periph);
        pwm->configPwmChannel(payload->deviceIndex, payload->pwmCycleLength, payload->rampRiseStep,
                payload->rampFallStep, payload->rampRiseTime, payload->rampFallTime);
        element->setPssId(payload->pssId);
#endif
        break;
    }
    default:
        status = E_AckStatus_InvalidDevice;
    }
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, status);
}

void PscMessageHandler::MessageDefineDeviceProtectionHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineDeviceProtectionControlMsg* payload = &(message->payload.pSSDefineDeviceProtectionControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineDeviceProtectionControlMsg: cableId=%d pssId={[PSSID:%d]} dataType=%d upperSoft=%d upperHard=%d lowerSoft=%d lowerHard=%d checkLow=%d checkHard=%d connectedControl={[PSSID:%d]} debounceTimer = %d",
            payload->cableId, payload->pssId, payload->dataType, payload->upperSoftLimit, payload->upperHardLimit,
            payload->lowerSoftLimit, payload->lowerHardLimit, payload->checkLowLimit, payload->checkHighLimit,
            payload->connectedControl, payload->debounceTimer);

    ProtectionControl *protectionControl = ControlRepository::getInstance().getProtectionControl();

    ValidationElementBase* element =
            static_cast<ValidationElementBase*>(ElementRepository::getInstance().getElementByPssId(payload->pssId));

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    float lowerSoftLimit = payload->lowerSoftLimit;
    float upperSoftLimit = payload->upperSoftLimit;
    float lowerHardLimit = payload->lowerHardLimit;
    float upperHardLimit = payload->upperHardLimit;
    unsigned short debounceTimer = payload->debounceTimer;

    DeviceProtectionChecker *deviceChecker = protectionControl->createDeviceProtectionChecker();
    deviceChecker->setElement(element);

// TODO: refactor this method to a static method in the element base.
    switch (payload->dataType)
    {
    case E_ValueType_S8:
    case E_ValueType_Bool:
        deviceChecker->updateSoftProtectionRange(*(int8_t*) &lowerSoftLimit, *(int8_t*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(int8_t*) &lowerHardLimit, *(int8_t*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    case E_ValueType_S16:
        deviceChecker->updateSoftProtectionRange(*(int16_t*) &lowerSoftLimit, *(int16_t*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(int16_t*) &lowerHardLimit, *(int16_t*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    case E_ValueType_S32:
        deviceChecker->updateSoftProtectionRange(*(int32_t*) &lowerSoftLimit, *(int32_t*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(int32_t*) &lowerHardLimit, *(int32_t*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    case E_ValueType_U8:
        deviceChecker->updateSoftProtectionRange(*(uint8_t*) &lowerSoftLimit, *(uint8_t*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(uint8_t*) &lowerHardLimit, *(uint8_t*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    case E_ValueType_U16:
        deviceChecker->updateSoftProtectionRange(*(uint16_t*) &lowerSoftLimit, *(uint16_t*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(uint16_t*) &lowerHardLimit, *(uint16_t*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    case E_ValueType_U32:
        deviceChecker->updateSoftProtectionRange(*(uint32_t*) &lowerSoftLimit, *(uint32_t*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(uint32_t*) &lowerHardLimit, *(uint32_t*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    case E_ValueType_Float:
        deviceChecker->updateSoftProtectionRange(*(float*) &lowerSoftLimit, *(float*) &upperSoftLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        deviceChecker->updateHardProtectionRange(*(float*) &lowerHardLimit, *(float*) &upperHardLimit,
                payload->checkLowLimit, payload->checkHighLimit);
        break;
    }

    deviceChecker->setDebounceTimeout(debounceTimer);

#ifdef OLD_PROTECTION
// add the element to the protection for hard limit checking:
    protectionControl->addProtectionElement(element);
#endif

// if the connected control pss ID is not 0, we need to connect the element to a control for soft limit checking.
    if (payload->connectedControl != 0)
    {
        ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->connectedControl);
        if (control == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->connectedControl,
                    E_AckStatus_InvalidDevice);
            return;
        }

#ifdef OLD_PROTECTION
        // add the the element to the control for soft limit checking.
        control->addProtectionElement(element);
#else
        control->addProtectionChecker(deviceChecker);
#endif
    }

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineConstantDeltaProtectionHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineConstantDeltaProtectionMsg* payload = &(message->payload.pSSDefineConstantDeltaProtectionMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineConstantDeltaProtectionMsg: cableId=%d pssId={[PSSID:%d]} connctedToControlPSSID={[PSSID:%d]} referenceInputPSSID={[PSSID:%d]} allowedUpperDelta=%f allowedLowerDelta=%f debounceTimer=%d",
            payload->cableId, payload->pssId, payload->connectedToControlPSSID, payload->referenceInputPSSID,
            payload->allowedUpperDelta, payload->allowedLowerDelta, payload->debounceTimer);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ElementBase* referenceElement = ElementRepository::getInstance().getElementByPssId(payload->referenceInputPSSID);

    if (referenceElement == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->referenceInputPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->connectedToControlPSSID);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->connectedToControlPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    float lowerDelta = payload->allowedLowerDelta;
    float upperDelta = payload->allowedUpperDelta;
    unsigned short debounceTimer = payload->debounceTimer;

    ProtectionConstantDeltaChecker* protection = new ProtectionConstantDeltaChecker();

    protection->setElement(element);
    protection->setReferenceElement(referenceElement);
    protection->updateAllowedDelta(lowerDelta, upperDelta);
    control->addProtectionChecker(protection);

    ProtectionControl *protectionRepository = ControlRepository::getInstance().getProtectionControl();
    protectionRepository->addProtectionCheckerButDontSubsribe(protection);

    protection->setDebounceTimeout(debounceTimer);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineCurrentLimitsProtectionHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineCurrentLimitsProtectionMsg* payload = &(message->payload.pSSDefineCurrentLimitsProtectionMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineCurrentLimitsProtectionMsg: cableId=%d pssId={[PSSID:%d]} connctedToControlPSSID={[PSSID:%d]} upperLimits=%f lowerWarningLimits=%f lowerErrorLimts=%f, debounceTimer=%d",
            payload->cableId, payload->pssId, payload->connectedToControlPSSID, payload->upperLimits,
            payload->lowerWarningLimits, payload->lowerErrorLimits, payload->debounceTimer);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->connectedToControlPSSID);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->connectedToControlPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    float upperLimits = payload->upperLimits;
    float lowerWarningLimits = payload->lowerWarningLimits;
    float lowerErrorLimits = payload->lowerErrorLimits;
    unsigned short debounceTimer = payload->debounceTimer;

    ProtectionCurrentLimitsChecker* protection = new ProtectionCurrentLimitsChecker();

    protection->setElement(element);
    protection->updateLimits(lowerWarningLimits, lowerErrorLimits, upperLimits);
    control->addProtectionChecker(protection);

    ProtectionControl *protectionRepository = ControlRepository::getInstance().getProtectionControl();
    protectionRepository->addProtectionCheckerButDontSubsribe(protection);

    protection->setDebounceTimeout(debounceTimer);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineProportionalProtectionHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineProportionalProtectionMsg* payload = &(message->payload.pSSDefineProportionalProtectionMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineProportionalProtectionMsg: cableId=%d pssId={[PSSID:%d]} connctedToControlPSSID={[PSSID:%d]} referenceInputPSSID={[PSSID:%d]} allowedUpperDelta=%f allowedLowerDelta=%f gain=%f offset=%f debounceTimer=%d",
            payload->cableId, payload->pssId, payload->connectedToControlPSSID, payload->referenceInputPSSID,
            payload->allowedUpperDelta, payload->allowedLowerDelta, payload->inputGain, payload->inputOffset,
            payload->debounceTimer);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ElementBase* referenceElement = ElementRepository::getInstance().getElementByPssId(payload->referenceInputPSSID);

    if (referenceElement == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->referenceInputPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->connectedToControlPSSID);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->connectedToControlPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    float lowerDelta = payload->allowedLowerDelta;
    float upperDelta = payload->allowedUpperDelta;
    float gain = payload->inputGain;
    float offset = payload->inputOffset;
    unsigned short debounceTimer = payload->debounceTimer;

    ProtectionProportionalChecker* protection = new ProtectionProportionalChecker();

    protection->setElement(element);
    protection->setReferenceElement(referenceElement);
    protection->updateParameters(gain, offset, lowerDelta, upperDelta);

    control->addProtectionChecker(protection);

    ProtectionControl *protectionRepository = ControlRepository::getInstance().getProtectionControl();
    protectionRepository->addProtectionCheckerButDontSubsribe(protection);

    protection->setDebounceTimeout(debounceTimer);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineProtectionControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineProtectionControlMsg* payload = &(message->payload.pSSDefineProtectionControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    ProtectionControl *protectionControl = ControlRepository::getInstance().getProtectionControl();

    protectionControl->setPssId(payload->pssId);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefinePIDControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefinePIDControlMsg* payload = &(message->payload.pSSDefinePIDControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefinePIDControlMsg: cableId=%d pssId={[PSSID:%d]} input={[PSSID:%d]} output={[PSSID:%d]} cascade=%d",
            payload->cableId, payload->pssId, payload->input, payload->output, payload->cascade);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    PidControl* control = new PidControl();
    control->setPssId(payload->pssId);

// TODO: Have PID Control accept more element types.
    ValidationElementFloat* element =
            static_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByPssId(payload->input));

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->input,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementInput(element);

// TODO: Create the set point element inside the PID control.
    element = ElementRepository::getInstance().addValidationElementFloat();

    control->setElementSetpoint(element);

// cascase==0 means that this is a normal pid temperature control loop.
    if (payload->cascade == 0)
    {
        element = static_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByPssId(
                payload->output));
        if (element == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        control->setElementOutput(element);
    }
    else
    {
        // TODO: Make cascaded controls more generic ?

        // if we're here it means this is a cascaded loop control,
        // which means that the output element of this control is the setpoint of another control.
        // first we get that control:
        ControlBase* cascade = ControlRepository::getInstance().getControlByPssId(payload->output);

        // we need to allow every control-type to be controlled, not only PidControls.
        if (cascade == NULL /*|| cascade->getControlType() != E_ControlType_PidControl*/)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
            return;
        }

//        PidControl* cascadedPid = static_cast<PidControl*>(cascade);
//
        // set the output element of this control loop to the setpoint element of the cascaded control loop:
        control->setElementOutput(cascade->getMainControlElement());
    }

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageDefineObserveAndNotifyControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineObserveAndNotifyControlMsg* payload = &(message->payload.pSSDefineObserveAndNotifyControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineObserveAndNotifyControlMsg: cableId=%d pssId={[PSSID:%d]} input={[PSSID:%d]}", payload->cableId,
            payload->pssId, payload->input);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    ObserveAndNotifyControl * control = new ObserveAndNotifyControl();
    control->setPssId(payload->pssId);

    ValidationElementFloat* element =
            static_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByPssId(payload->input));

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->input,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementInput(element);

// TODO: Create the set point element inside the PID control.
    element = ElementRepository::getInstance().addValidationElementFloat();

    control->setElementSetpoint(element);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageDefineActivationWithFeedbackControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineActivationWithFeedbackControlMsg* payload = &(message->payload.pSSDefineActivationWithFeedbackControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssID);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssID);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineActivationWithFeedbackControlMsg: cableId=%d pssId={[PSSID:%d]} activate={[PSSID:%d]} deactivate={[PSSID:%d]} activateTimeout=%d deactivateTimeout=%d BehaveOnInit=%d ignoreProtDly=%d",
            payload->cableId, payload->pssID, payload->activatePSSId, payload->deactivatePSSId,
            payload->activationTimeout, payload->deactivationTimeout, payload->activationWithFeedbackBehaviorOnInit,
            payload->ignoreProtectionsDelay);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    ActivationWithFeedbackControl* control = new ActivationWithFeedbackControl();
    control->setPssId(payload->pssID);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->activatePSSId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->activatePSSId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputEnableDevice(element);

    if (payload->deactivatePSSId != 0)
    {
        ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->deactivatePSSId);

        if (element == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->deactivatePSSId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        control->setOutputDisableDevice(element);
    }

//    control->setActivationTimeout(payload->activationTimeout);
//    control->setDeactivationTimeout(payload->deactivationTimeout);
    control->setIgnoreProtectionsDelay(payload->ignoreProtectionsDelay);
    control->setBehaviorOnInit(payload->activationWithFeedbackBehaviorOnInit);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssID, E_AckStatus_Success);
}

void PscMessageHandler::MessageAddDependentDeviceToControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSAddDependentDeviceToControl* payload = &(message->payload.pSSAddDependentDeviceToControl);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->controlPssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->controlPssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSAddDependentDeviceToControl: cableId=%d controlPssId={[PSSID:%d]} dependentPssId={[PSSID:%d]} thresholdValueType=%d thresholdValue=%d greaterThan=%d deactivateControl=%d lockOnActivate=%d checkType=%d",
            payload->cableId, payload->controlPssId, payload->dependentDevicePssId, payload->thresholdValueType,
            payload->thresholdValue, payload->greaterThan, payload->deactivateControlOnChange, payload->lockOnActivate,
            payload->dependencyCheckType);

    ControlBase* tempControl = ControlRepository::getInstance().getControlByPssId(payload->controlPssId);

// TODO: Support dependency checks to the tempControl base, so all controls will support it.
    if (tempControl == NULL || tempControl->getControlType() != E_ControlType_ActivationWithFeedback)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->controlPssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ActivationWithFeedbackControl* control = static_cast<ActivationWithFeedbackControl*>(tempControl);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->dependentDevicePssId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->dependentDevicePssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    T_ValueTypeUnion tempVal;
    tempVal.u32 = payload->thresholdValue;
    float thresholdVal;
// TODO: refactor this method to a static method in the element base.
    switch (payload->thresholdValueType)
    {
    case E_ValueType_S8:
    case E_ValueType_Bool:
        thresholdVal = tempVal.s8;
        break;
    case E_ValueType_S16:
        thresholdVal = tempVal.s16;
        break;
    case E_ValueType_S32:
        thresholdVal = tempVal.s32;
        break;
    case E_ValueType_U8:
        thresholdVal = tempVal.u8;
        break;
    case E_ValueType_U16:
        thresholdVal = tempVal.u16;
        break;
    case E_ValueType_U32:
        thresholdVal = tempVal.u32;
        break;
    case E_ValueType_Float:
        thresholdVal = tempVal.f;
        break;
    }

    control->addDependentElement(element, thresholdVal, payload->greaterThan, payload->deactivateControlOnChange,
            payload->lockOnActivate, (E_DependencyCheckType) payload->dependencyCheckType);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->controlPssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageAddFeedbackDeviceToControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSAddFeedbackDeviceToControl* payload = &(message->payload.pSSAddFeedbackDeviceToControl);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->controlPssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->controlPssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSAddFeedbackDeviceToControl: cableId=%d controlPssId={[PSSID:%d]} devicePssId={[PSSID:%d]} thresholdValueType=%d thresholdValue=%d greaterThan=%d deactivateControl=%d",
            payload->cableId, payload->controlPssId, payload->devicePssId, payload->thresholdValueType,
            payload->thresholdValue, payload->greaterThan, payload->deactivateControlOnChange);

    ControlBase* tempControl = ControlRepository::getInstance().getControlByPssId(payload->controlPssId);

// TODO: Support dependency checks to the tempControl base, so all controls will support it.
    if (tempControl == NULL || tempControl->getControlType() != E_ControlType_ActivationWithFeedback)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->controlPssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    ActivationWithFeedbackControl* control = static_cast<ActivationWithFeedbackControl*>(tempControl);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->devicePssId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->devicePssId,
                E_AckStatus_InvalidDevice);
        return;
    }

#pragma diag_suppress=Pa039

    float thresholdVal;
// TODO: refactor this method to a static method in the element base.
    switch (payload->thresholdValueType)
    {
    case E_ValueType_S8:
    case E_ValueType_Bool:
        thresholdVal = (*(int8_t*) &payload->thresholdValue);
        break;
    case E_ValueType_S16:
        thresholdVal = (*(int16_t*) &payload->thresholdValue);
        break;
    case E_ValueType_S32:
        thresholdVal = (*(int32_t*) &payload->thresholdValue);
        break;
    case E_ValueType_U8:
        thresholdVal = (*(uint8_t*) &payload->thresholdValue);
        break;
    case E_ValueType_U16:
        thresholdVal = (*(uint16_t*) &payload->thresholdValue);
        break;
    case E_ValueType_U32:
        thresholdVal = (*(uint32_t*) &payload->thresholdValue);
        break;
    case E_ValueType_Float:
        thresholdVal = (*(float*) &payload->thresholdValue);
        break;
    }

    control->addFeedbackElement(element, thresholdVal, payload->greaterThan, payload->deactivateControlOnChange);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->controlPssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageDefineAnalogOutInverterControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineAnalogOutInverterControlMsg* payload = &(message->payload.pSSDefineAnalogOutInverterControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    AnalogOutInverterControl* control = new AnalogOutInverterControl();
    control->setPssId(payload->pssId);

    ValidationElementFloat* element =
            static_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByPssId(payload->speedId));

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->speedId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setSetpointElement(element);

    if (payload->enableId != 0)
    {
        ElementU8* enableElement = static_cast<ElementU8*>(ElementRepository::getInstance().getElementByPssId(
                payload->enableId));

        if (enableElement == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->enableId,
                    E_AckStatus_InvalidDevice);
            return;
        }

        control->setOutputEnableElement(enableElement);
    }
    else
    {
        control->setOutputEnableElement(NULL);
    }

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageDefineModbusInverterControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineModbusInverterControlMsg* payload = &(message->payload.pSSDefineModbusInverterControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineModbusInverterControlMsg: cableId=%d pssId={[PSSID:%d]} enableId={[PSSID:%d]} outputFre={[PSSID:%d]} outputCurr={[PSSID:%d]} outputSP={[PSSID:%d]}",
            payload->cableId, payload->pssId, payload->enableId, payload->outputFrequencyPSSId,
            payload->outputCurrentPSSId, payload->outputSetPointPSSId);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    ModbusInverterControl* control = new ModbusInverterControl();
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->enableId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->enableId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputEnableElement(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->outputCurrentPSSId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->outputCurrentPSSId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputCurrentElement(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->outputFrequencyPSSId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->outputFrequencyPSSId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputFrequencyElement(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->outputSetPointPSSId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->outputSetPointPSSId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputSetpointElement(element);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageInitControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSInitControlMsg *payload = &(message->payload.pSSInitControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
//    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

// move the control to STANDBY.
// The control should now send SEQ_ENDED when finished.
    control->initControl(message->header.id.full, message->header.sn);
}

void PscMessageHandler::MessageStopControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSStopControlMsg *payload = &(message->payload.pSSStopControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
//    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

// move the control to STANDBY.
// The control should now send SEQ_ENDED when finished.
    control->move2Standby(message->header.id.full, message->header.sn);
}

void PscMessageHandler::MessageResetToOnControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSResetToOnControlMsg *payload = &(message->payload.pSSResetToOnControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSResetToOnControl: cableId=%d controlPssId={[PSSID:%d]}", payload->cableId,
            payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
//    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

// move the control to STANDBY.
// The control should now send SEQ_ENDED when finished.
    control->reset2On(message->header.id.full, message->header.sn);
}

void PscMessageHandler::MessageSetPIDHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetPIDMsg* payload = &(message->payload.pSSSetPIDMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSSetPIDMsg: cableId=%d pssId={[PSSID:%d]} p=%f i=%f d=%f iterm=%f",
            payload->cableId, payload->pssId, payload->p, payload->i, payload->d, payload->itermRange);

// TODO: Enable multiple states check
//    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_PidControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    PidControl *pidControl = static_cast<PidControl*>(control);

//    pidControl->setTuningValues(payload->p, payload->i, payload->d, 0);
    pidControl->setTuningValues(payload->p, payload->i, payload->d, payload->itermRange);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageConfigControlStopConditionsHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSConfigControlStopConditions* payload = &(message->payload.pSSConfigControlStopConditions);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSConfigControlStopConditions: cableId=%d pssId={[PSSID:%d]} stopOnEmr=%d stopOnDisconnection=%d",
            payload->cableId, payload->pssId, payload->stopOnEmr, payload->stopOnDisconnection);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setStopOnEmrBehavior(payload->stopOnEmr);
    control->setStopOnDisconnection(payload->stopOnDisconnection);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageGetPIDHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSGetPIDMsg* payload = &(message->payload.pSSGetPIDMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_PidControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    PidControl *pidControl = static_cast<PidControl*>(control);

    pidControl->sendPidValues();

}

void PscMessageHandler::MessageActivateMonitoringHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateMonitoringMsg* payload = &(message->payload.pSSActivateMonitoringMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

//    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivateMonitoringMsg: cableId=%d pssId={[PSSID:%d]} active=%d minInt=%d maxInt=%d", payload->cableId,
            payload->pssId, payload->active, payload->minUpdateInterval, payload->maxUpdateInterval);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control != NULL)
    {
        control->setMonitoringEnabled((payload->active != 0));
    }
    else
    {
        ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);

        if (element != NULL)
        {
            element->setMonitoringEnabled((payload->active != 0));
            element->setMinInterval(payload->minUpdateInterval);
            element->setMaxInterval(payload->maxUpdateInterval);
        }
        else
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
        }
    }

// TODO: Implement update intervals in the controls.

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageSetDeviceOutputValueHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetDeviceOutputValueMsg* payload = &(message->payload.pSSSetDeviceOutputValueMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetDeviceOutputValueMsg: cableId=%d pssId={[PSSID:%d]} valueType=%d value=%d", payload->cableId,
            payload->pssId, payload->valueType, payload->value);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    T_ValueTypeUnion val;
    val.s32 = payload->value;
    E_AckStatus status = E_AckStatus_Success;

    switch (payload->valueType)
    {
    case E_ValueType_S8:
    case E_ValueType_Bool:
        element->setValue(val.s8);
        break;
    case E_ValueType_S16:
        element->setValue(val.s16);
        break;
    case E_ValueType_S32:
        element->setValue(val.s32);
        break;
    case E_ValueType_U8:
        element->setValue(val.u8);
        break;
    case E_ValueType_U16:
        element->setValue(val.u16);
        break;
    case E_ValueType_U32:
        element->setValue(val.u32);
        break;
    case E_ValueType_Float:
        element->setValue(val.f);
        break;
    default:
        status = E_AckStatus_InvalidMessage;
    }

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, status);
}

void PscMessageHandler::MessageActivatePIDControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivatePIDControlMsg *payload = &(message->payload.pSSActivatePIDControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivatePIDControlMsg: cableId=%d pssId={[PSSID:%d]} sp=%f work=%f-%f warn=%f-%f", payload->cableId,
            payload->pssId, payload->setPoint, payload->minWorkingRange, payload->maxWorkingRange,
            payload->minWarningRange, payload->maxWarningRange);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_PidControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    PidControl *pidControl = static_cast<PidControl*>(control);
    pidControl->setSetpoint(payload->setPoint, payload->minWorkingRange, payload->maxWorkingRange,
            payload->minWarningRange, payload->maxWarningRange, message->header.sn);
}

void PscMessageHandler::MessageActivateObserveAndNotifyControlMsgHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateObserveAndNotifyControlMsg *payload = &(message->payload.pSSActivateObserveAndNotifyControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivateObserveAndNotifyControlMsg: cableId=%d pssId={[PSSID:%d]} sp=%f work=%f-%f warn=%f-%f",
            payload->cableId, payload->pssId, payload->setPoint, payload->minWorkingRange, payload->maxWorkingRange,
            payload->minWarningRange, payload->maxWarningRange);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_ObserveAndNotifyControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    ObserveAndNotifyControl * pObserveAndNotifyControl = static_cast<ObserveAndNotifyControl*>(control);
    pObserveAndNotifyControl->setSetpoint(payload->setPoint, payload->minWorkingRange, payload->maxWorkingRange,
            payload->minWarningRange, payload->maxWarningRange, message->header.sn);
}

void PscMessageHandler::MessageActivateInverterControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateInverterControlMsg*payload = &(message->payload.pSSActivateInverterControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSActivateInverterControlMsg: cableId=%d pssId={[PSSID:%d]} setpoint=%f",
            payload->cableId, payload->pssId, payload->setPoint);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    if (control->getControlType() == E_ControlType_AnalogOutInverter)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Activate analog inverter pssId {[PSSID:%d]} setpoint %4.2f", payload->pssId,
                payload->setPoint);
        // Send ACK that the command was accepted.
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

        AnalogOutInverterControl *inverterControl = static_cast<AnalogOutInverterControl*>(control);
        inverterControl->setSetpoint(payload->setPoint, message->header.sn);
    }
    else if (control->getControlType() == E_ControlType_ModbusInverter)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Activate modbus inverter pssId {[PSSID:%d]} setpoint %4.2f", payload->pssId,
                payload->setPoint);
        // Send ACK that the command was accepted.
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

        // TODO: and pure virtual method "setSetpoint" to all controls.
        ModbusInverterControl *inverterControl = static_cast<ModbusInverterControl*>(control);
        inverterControl->setSetpoint(payload->setPoint, message->header.sn);
    }
    else
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
    }

}

void PscMessageHandler::MessageSetPIDControlParametersHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSSetTemperatureControlParametersMsg *payload = &(message->payload.pSSSetTemperatureControlParametersMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

// TODO: Enable checking if we're in multiple states
//M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSSetTemperatureControlParametersMsg: cableId=%d pssId={[PSSID:%d]} maxOutput=%f outFilt=%d pidSetpRange=%f",
            payload->cableId, payload->pssId, payload->maxOutput, payload->outputFilter, payload->pidSetpointRange);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_PidControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    PidControl *pidControl = static_cast<PidControl*>(control);
    pidControl->setPowerLimit(payload->maxOutput);
    pidControl->setOutputSmoothing(payload->outputFilter);
    pidControl->setSetpointRange(payload->pidSetpointRange);
}

void PscMessageHandler::MessageAutoTuneHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSAutoTuneMsg *payload = &(message->payload.pSSAutoTuneMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssID);

// TODO: Enable checking if we're in multiple states
//M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSAutoTuneMsg: cableId=%d pssId={[PSSID:%d]} setpoint=%f os=%f powStart=%f powStep=%f calcWind=%d",
            payload->cableId, payload->pssID, payload->setPoint, payload->overShoot, payload->powerAtStart,
            payload->powerStep, payload->calculation);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssID);
    E_AckStatus status;

    if (control == NULL || control->getControlType() != E_ControlType_PidControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssID,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
//    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssID, E_AckStatus_Success);

    PidControl *pidControl = static_cast<PidControl*>(control);
    status = pidControl->startAutoTune(payload->setPoint, payload->overShoot, payload->powerAtStart, payload->powerStep,
            payload->calculation, false, message->header.sn);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssID, status);
}

void PscMessageHandler::MessageDefineHysteresisTemperatureControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineHysteresisTemperatureControlMsg* payload = &(message->payload.pSSDefineHysteresisTemperatureControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineHysteresisTemperatureControlMsg: cableId=%d pssId={[PSSID:%d]} inputPssId={[PSSID:%d]} outputPssId={[PSSID:%d]} outputValue=%f",
            payload->cableId, payload->pssId, payload->inputPssId, payload->outputPssId, payload->outputValue);

    if (payload->outputValue == 0)
        payload->outputValue = 1;

// TODO: Add a check, so when an allocation fails we move the board to error state.
    HysteresisControl* control = new HysteresisControl();
    control->setPssId(payload->pssId);

// TODO: Have PID Control accept more element types.
    ValidationElementFloat* element =
            static_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByPssId(payload->inputPssId));

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->inputPssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementInput(element);

    element = static_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByPssId(
            payload->outputPssId));
    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

    // TODO: Make cascaded controls more generic ?
    control->setElementOutput(element);

    control->setOutputValue(payload->outputValue);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

}

void PscMessageHandler::MessageActivateHysteresisTemperatureControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateHysteresisTemperatureControlMsg *payload = &(message->payload.pSSActivateHysteresisTemperatureControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivateHysteresisTemperatureControlMsg: cableId=%d pssId={[PSSID:%d]} sp=%f deactiveSp=%f work=%f-%f warn=%f-%f",
            payload->cableId, payload->pssId, payload->setPoint, payload->deactivateSetPoint, payload->minWorkingRange,
            payload->maxWorkingRange, payload->minWarningRange, payload->maxWarningRange);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_HysteresisControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    HysteresisControl *hystControl = static_cast<HysteresisControl*>(control);
    hystControl->setSetpoint(payload->setPoint, payload->deactivateSetPoint, payload->minWorkingRange,
            payload->maxWorkingRange, payload->minWarningRange, payload->maxWarningRange, message->header.sn);
}

void PscMessageHandler::MessageActivateWaterTankLevelControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateWaterTankLevelControlMsg *payload = &(message->payload.pSSActivateWaterTankLevelControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivateWaterTankLevelControlMsg: cableId=%d pssId={[PSSID:%d]} levels=%f-%f-%f", payload->cableId,
            payload->pssId, payload->lowLevelSetPoint, payload->midLevelSetPoint, payload->highLevelSetPoint);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_LiquidLevelPumpControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    LiquidLevelPumpControl *liquidControl = static_cast<LiquidLevelPumpControl*>(control);

    liquidControl->setSetpoint(payload->lowLevelSetPoint, payload->midLevelSetPoint, payload->highLevelSetPoint,
            message->header.sn);

}

void PscMessageHandler::MessageDefineSubtractTwoDevicesControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineSubtractTwoDevicesControlMsg* payload = &(message->payload.pSSDefineSubtractTwoDevicesControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineSubtractTwoDevicesControlMsg: cableId=%d pssId={[PSSID:%d]} input1={[PSSID:%d]} input2={[PSSID:%d]} output={[PSSID:%d]}",
            payload->cableId, payload->pssId, payload->input1, payload->input2, payload->output);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    SubtractTwoDevicesControl* control = new SubtractTwoDevicesControl();
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->input1);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->input1,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setInput1Element(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->input2);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->input2,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setInput2Element(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->output);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->output,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputElement(element);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineCalculateOnTwoDevicesControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineCalculateOnTwoDevicesControlMsg* payload = &(message->payload.pSSDefineCalculateOnTwoDevicesControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "MessageDefineCalculateOnTwoDevicesControlHandler: cableId=%d pssId={[PSSID:%d]} input1={[PSSID:%d]} input2={[PSSID:%d]} output={[PSSID:%d]} calcFunc=%d",
            payload->cableId, payload->pssId, payload->input1, payload->input2, payload->output,
            payload->calculationFunction);

    CalculateOnTwoDevicesControl *control;

    switch (payload->calculationFunction)
    {
    case E_CalculationFunction_Subtract:
        control = new SubtractTwoDevicesControl();
        break;
    case E_CalculationFunction_Add:
        control = new AddTwoDevicesControl();
        break;
    case E_CalculationFunction_Multiply:
        control = new MultiplyTwoDevicesControl();
        break;
    case E_CalculationFunction_Divide:
        control = new DivideTwoDevicesControl();
        break;
    case E_CalculationFunction_Maximum:
        control = new MaxTwoDevicesControl();
        break;
    case E_CalculationFunction_Minimum:
        control = new MinTwoDevicesControl();
        break;
    }

// TODO: Add a check, so when an allocation fails we move the board to error state.
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->input1);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->input1,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setInput1Element(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->input2);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->input2,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setInput2Element(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->output);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->output,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setOutputElement(element);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefine3SensorWaterTankHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefine3SensorWaterTankMsg* payload = &(message->payload.pSSDefine3SensorWaterTankMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefine3SensorWaterTankMsg: cableId=%d pssId={[PSSID:%d]} low={[PSSID:%d]} mid={[PSSID:%d]} high={[PSSID:%d]} fill={[PSSID:%d]}",
            payload->cableId, payload->pssId, payload->lowSensorPssid, payload->midSensorPssid,
            payload->highSensorPssid, payload->fillPssid);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    LiquidLevel3Sensors* control = new LiquidLevel3Sensors();
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->lowSensorPssid);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->lowSensorPssid,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementLowSensor(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->midSensorPssid);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->midSensorPssid,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementMidSensor(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->highSensorPssid);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->highSensorPssid,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementHighSensor(element);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineLeakageDetectionControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineLeakageDetectionControlMsg* payload = &(message->payload.pSSDefineLeakageDetectionControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineLeakageDetectionControlMsg: cableId=%d pssId={[PSSID:%d]} tank={[PSSID:%d]} tub={[PSSID:%d]} upper=%f lower=%f",
            payload->cableId, payload->pssId, payload->tankLevelPSSID, payload->tubLevelPSSID,
            payload->upperBoundForLeak, payload->lowerBoundForLeak);

    // TODO: Add a check, so when an allocation fails we move the board to error state.
    LeakDetectionControl* control = new LeakDetectionControl();
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->tankLevelPSSID);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->tankLevelPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementTankLevel(element);

    element = ElementRepository::getInstance().getElementByPssId(payload->tubLevelPSSID);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->tubLevelPSSID,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementTubLevel(element);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefineAnalogSensorWaterTankHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefineAnalogSensorWaterTankMsg* payload = &(message->payload.pSSDefineAnalogSensorWaterTankMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefineAnalogSensorWaterTankMsg: cableId=%d pssId={[PSSID:%d]} sensorPssId={[PSSID:%d]} fillPssId={[PSSID:%d]} levels=%f-%f-%f",
            payload->cableId, payload->pssId, payload->sensorPssid, payload->fillPssid, payload->lowLevelThreshold,
            payload->midLevelThreshold, payload->highLevelThreshold);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    AnalogLiquidLevelControl* control = new AnalogLiquidLevelControl();
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->sensorPssid);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->sensorPssid,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementLevelSensor(element);

    control->setLevelThresholds(payload->lowLevelThreshold, payload->midLevelThreshold, payload->highLevelThreshold);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageDefine3SensorWaterTankLevelHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSDefine3SensorWaterTankLevelMsg* payload = &(message->payload.pSSDefine3SensorWaterTankLevelMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE(E_BoardState_Initializing, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSDefine3SensorWaterTankLevelMsg: cableId=%d pssId={[PSSID:%d]} tankPssId={[PSSID:%d]} drainPssId={[PSSID:%d]} fillPssId={[PSSID:%d]}",
            payload->cableId, payload->pssId, payload->tankPssid, payload->drainPssid, payload->fillPssid);

// TODO: Add a check, so when an allocation fails we move the board to error state.
    LiquidLevelPumpControl* control = new LiquidLevelPumpControl();
    control->setPssId(payload->pssId);

    ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->tankPssid);

    if (element == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->tankPssid,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementTankLevelInput(element);

    ControlBase* drain = ControlRepository::getInstance().getControlByPssId(payload->drainPssid);

    if (drain == NULL)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->drainPssid,
                E_AckStatus_InvalidDevice);
        return;
    }

    control->setElementDrainPump(drain->getMainControlElement());

    if (payload->fillPssid != 0)
    {
        ControlBase* fill = ControlRepository::getInstance().getControlByPssId(payload->fillPssid);
        if (fill == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->fillPssid,
                    E_AckStatus_InvalidDevice);
            return;
        }
        control->setElementFillPump(fill->getMainControlElement());
    }

    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
}

void PscMessageHandler::MessageActivateActivationWithFeedbackControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateActivationWithFeedbackControlMsg *payload =
            &(message->payload.pSSActivateActivationWithFeedbackControlMsg);

    bool result;

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivateActivationWithFeedbackControlMsg: cableId=%d pssId={[PSSID:%d]} outVal=%d timeout=%d",
            payload->cableId, payload->pssId, payload->outputValue, payload->timeout);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_ActivationWithFeedback)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    ActivationWithFeedbackControl *activationControl = static_cast<ActivationWithFeedbackControl*>(control);

    result = activationControl->activateControl(payload->outputValue, payload->timeout, message->header.sn);

}

void PscMessageHandler::MessageActivateLeakageDetectionControlHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSActivateLeakageDetectionControlMsg *payload = &(message->payload.pSSActivateLeakageDetectionControlMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

    M_CHECK_BOARD_STATE2(E_BoardState_Ready, E_BoardState_EMR, message->header.id.full, message->header.sn,
            payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSActivateLeakageDetectionControlMsg: cableId=%d pssId={[PSSID:%d]} stage=%d timeout=%d window=%d deviation=%f",
            payload->cableId, payload->pssId, payload->calibrationStage, payload->stabilizationTimeout,
            payload->calculationWindow, payload->allowedDeviation);;

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL || control->getControlType() != E_ControlType_LeakageDetectionControl)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                E_AckStatus_InvalidDevice);
        return;
    }

// Send ACK that the command was accepted.
    sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);

    LeakDetectionControl *leakDetectionControl = static_cast<LeakDetectionControl*>(control);

    leakDetectionControl->activateControl(payload->calibrationStage, payload->stabilizationTimeout,
            payload->calculationWindow, payload->allowedDeviation, message->header.sn);

}

void PscMessageHandler::MessageGetErrorsHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSGetErrorsMsg *payload = &(message->payload.pSSGetErrorsMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

//    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSGetErrorsMsg: cableId=%d pssId={[PSSID:%d]}", payload->cableId,
            payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);
        if (element == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
        }
        else
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
            element->sendCurrentErrors();
        }
    }
    else
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
        control->sendCurrentErrors();
    }

}

void PscMessageHandler::MessageGetWarningsHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSGetWarningsMsg *payload = &(message->payload.pSSGetWarningsMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->pssId);

//    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSGetWarningsMsg: cableId=%d pssId={[PSSID:%d]}", payload->cableId,
            payload->pssId);

    ControlBase* control = ControlRepository::getInstance().getControlByPssId(payload->pssId);

    if (control == NULL)
    {
        ElementBase* element = ElementRepository::getInstance().getElementByPssId(payload->pssId);
        if (element == NULL)
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId,
                    E_AckStatus_InvalidDevice);
        }
        else
        {
            sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
            element->sendCurrentWarnings();
        }
    }
    else
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->pssId, E_AckStatus_Success);
        control->sendCurrentWarnings();
    }

}

void PscMessageHandler::MessageAddShutdownOperationHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSAddShutdownOperation *payload = &(message->payload.pSSAddShutdownOperation);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, payload->controlPssId);

//    M_CHECK_BOARD_STATE(E_BoardState_Ready, message->header.id.full, message->header.sn, payload->pssId);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "PSSAddShutdownOperation: cableId=%d pssId={[PSSID:%d]} delay=%d op=%d setpoint=%f", payload->cableId,
            payload->controlPssId, payload->delay, payload->operation, payload->setpoint);

    bool result = ControlRepository::getInstance().addShutdownOperation(payload->delay, payload->controlPssId,
            (E_ShutdownOperation) payload->operation, payload->setpoint);

    if (result)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->controlPssId,
                E_AckStatus_Success);
    }
    else
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, payload->controlPssId,
                E_AckStatus_InvalidDevice);
    }
}

void PscMessageHandler::MessageStartApplicationUploadHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSStartApplicationUpload *payload = &(message->payload.pSSStartApplicationUpload);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSStartApplicationUpload: cableId=%d uploadTarget=%x numOfPackets=%d",
            payload->cableId, payload->uploadTarget, payload->numberOfPackets);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, 0);

    m_boardState.eventApplicationUpload();

    M_CHECK_BOARD_STATE(E_BoardState_ApplicationUpload, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    if (payload->uploadTarget == 0)
    {
        m_pAppLoader = new StAppLoader();
    }
    else
    {
        m_pAppLoader = new PsocAppLoader(&m_psocManager);
    }
    m_pAppLoader->startAppLoad(payload->numberOfPackets, payload->uploadTarget, message->header.id.full,
            message->header.sn);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, 0, E_AckStatus_Success);
}

void PscMessageHandler::MessageEndApplicationUploadHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSEndBoardConfigMsg *payload = &(message->payload.pSSEndBoardConfigMsg);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);
    M_CHECK_BOARD_STATE(E_BoardState_ApplicationUpload, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    sendAck(message->header.id.full, message->header.sn, payload->cableId, 0, E_AckStatus_Success);

    bool result = m_pAppLoader->endAppLoad();

    if (result)
    {
        sendSeqEnded(message->header.id.full, message->header.sn, Psc_ControllerId, M_PSS_ID_ALL,
                E_SeqEndedStatus_Success);
        delay(1000);
        resetBoard(2);
    }
    else
    {
        if (m_pAppLoader != NULL)
        {
            delete m_pAppLoader;
            m_pAppLoader = NULL;
        }

        m_psocManager.reset();

        sendSeqEnded(m_pAppLoader->getMsgId(), m_pAppLoader->getSerialNumber(), Psc_ControllerId, M_PSS_ID_ALL,
                E_SeqEndedStatus_Error);
        m_boardState.eventStartUpFinished();
    }
}

void PscMessageHandler::MessageNextAppPacketHandler(unsigned long param)
{
    PscMessageStruct* message = &m_messages[param];
    PSSNextAppPacket *payload = &(message->payload.pSSNextAppPacket);

    M_CHECK_BOARD_ID(payload->cableId, message->header.id.full, message->header.sn, M_PSS_ID_ALL);
    M_CHECK_BOARD_STATE(E_BoardState_ApplicationUpload, message->header.id.full, message->header.sn, M_PSS_ID_ALL);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSNextAppPacket: cableId=%d packetNumber=%d packetLength=%d",
            payload->cableId, payload->packetNumber, payload->packetLength);

    E_PacketResult result;

    result = m_pAppLoader->nextPacket(payload->packetNumber, (uint8_t*) payload->payload, payload->packetLength,
            payload->packetCrc);

    if (result == E_PacketResult_Ok)
    {
        sendAck(message->header.id.full, message->header.sn, payload->cableId, 0, E_AckStatus_Success);
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Flash Write Failed %d", result);
        sendAck(message->header.id.full, message->header.sn, payload->cableId, 0, E_AckStatus_InvalidCRC);
        m_boardState.eventStartUpFinished();
        if (m_pAppLoader != NULL)
        {
            delete m_pAppLoader;
            m_pAppLoader = NULL;
        }
    }

}

void PscMessageHandler::handleMessage(PscMessageStruct& message)
{
    CMessage m;

// set the message ID:
    m.m_nId = message.header.id.split.id + APP_MSG;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Message Received: sn=%d id=0x%x %s ", message.header.sn,
            message.header.id.split.id, getMessageString(m.m_nId));

//    CLogger::getInstance().logTaskList();

// set the index of the message in the array:
    m.wParam = m_currentMessage;

// copy the message to the messages array.
    memcpy(m_messages + m_currentMessage, &message, sizeof(PscMessageStruct));

// post the message to the queue:
    while (postMessage(&m, 50) != pdPASS)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PscMessageHandler queue is full");
        //puts("PscMessageHandler queue is full!");
    }
// advance the current index:
    m_currentMessage = (m_currentMessage + 1) & (MESSAGE_QUEUE_SIZE - 1);
}

void PscMessageHandler::run()
{
#ifdef FEC2_BOARD
    m_psocManager.initAllPsocs();
#endif

    CMessageTask::run();
}

void PscMessageHandler::reset()
{
    m_currentMessage = 0;

////    ControlRepository::getInstance().resetAllControlsToOn();
//    if (UpdateSchedulerTask::getInstance()->isBoardInReady())
//        ControlRepository::getInstance().executeShutdownOperation(false);

    // take the scheduler semaphores, so that no updates will occur:
    UpdateSchedulerTask::getInstance()->setBoardInReady(false);
    ModbusSchedulerTask::getInstance()->setBoardInReady(false);

    // clean the schedulers:
    UpdateSchedulerTask::getInstance()->cleanScheduler();
    ModbusSchedulerTask::getInstance()->cleanScheduler();

    ControlRepository::getInstance().destroyAllControls();
    PeripheralRepository::getInstance().destroyAllPeripherals();
    ElementRepository::getInstance().destroyAllElements();

}

void PscMessageHandler::stopOnEmr()
{
    ControlRepository::getInstance().stopOnEmr();
    m_boardState.eventEmergency();
}

void PscMessageHandler::recoverFromEmrError()
{
    m_boardState.eventStartRecovery();
    ControlRepository::getInstance().recoverFromEmergency();
}

portBASE_TYPE PscMessageHandler::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    portBASE_TYPE res;

    if ((res = m_postMessageSemaphore.create()) != pdPASS)
        return res;

#ifdef FEC2_BOARD
    // note that the message handler should be of a higher priority, in case the
    // requests arrive from it.
    m_psocManager.create("psocManager", 600, uxPriority - 1);
    m_psocManager.setBoardState(&m_boardState);
#endif

    return CMessageTask::onCreate(pcName, usStackDepth, uxPriority);
}

void PscMessageHandler::MessageDefineGenericModbusPeriphHandler(unsigned long param)
{
    MessageNotHandled(param);
}
void PscMessageHandler::MessageSetErrorsMaskHandler(unsigned long param)
{
    MessageNotHandled(param);
}
void PscMessageHandler::MessageSetWarningsMaskHandler(unsigned long param)
{
    MessageNotHandled(param);
}

void PscMessageHandler::MessageActivateWaterTankControlHandler(unsigned long param)
{
    MessageNotHandled(param);
}
void PscMessageHandler::MessageGetStatusHandler(unsigned long param)
{
    MessageNotHandled(param);
}
void PscMessageHandler::MessageSetLeakageDetectionParametersHandler(unsigned long param)
{
    MessageNotHandled(param);
}
