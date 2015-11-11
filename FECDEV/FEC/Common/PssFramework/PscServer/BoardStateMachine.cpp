/*
 * BoardStateMachine.cpp
 *
 *  Created on: 22 αιεμ 2013
 *      Author: maora
 */

#include "BoardStateMachine.h"
#include <PscServer/PscMasterServer.h>
#include <PscServer/PscMessageStructs.h>
#include <Controls/ControlRepository.h>
#include <PscSubsystem.h>
#include <logger.h>

BoardStateMachine::BoardStateMachine() :
        StateMachine(E_BoardStatePrivate_Size)
{
    currentState = E_BoardState_Off;
}

BoardStateMachine::~BoardStateMachine()
{
}

void BoardStateMachine::ST_Off()
{
}

void BoardStateMachine::ST_Startup()
{
}

void BoardStateMachine::ST_On()
{
}

void BoardStateMachine::ST_Initializing()
{
}

void BoardStateMachine::ST_Ready()
{
    // check the emergency input status. If it's still in EMR, we need to move back to the emergency state.
    if (ControlRepository::getInstance().isInEmergency())
        eventEmergency();
}

void BoardStateMachine::ST_Error()
{
}

void BoardStateMachine::ST_Emergency()
{
}

void BoardStateMachine::ST_ShuttingDown()
{
}

void BoardStateMachine::ST_ApplicationUpload()
{
}

void BoardStateMachine::ST_Recovery()
{
}

void BoardStateMachine::sendStatusUpdate()
{
    if (currentState == E_BoardState_ShuttingDown || currentState == E_BoardStatePrivate_Off)
        return;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "sendStateMessage");
    sendStateMessage();
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "sendErrorsMessage");
    sendErrorsMessage();
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "sendWarningsMessage");
    sendWarningsMessage();
}

void BoardStateMachine::sendStateMessage()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_BoardStatusNotification;
    replyMessage.payload.pSSBoardStatusNotificationMsg.cableId = Psc_ControllerId;

    // TODO: Implement board state exceptions when in error/warnings.
    replyMessage.payload.pSSBoardStatusNotificationMsg.exceptions = 0;
    replyMessage.payload.pSSBoardStatusNotificationMsg.boardState = getState();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSBoardStatusNotificationMsg);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "StateMessage: CableId=%d State=%d", Psc_ControllerId, getState());

    // TODO: Add log message
    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void BoardStateMachine::sendErrorsMessage()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ErrorNotification;
    replyMessage.payload.pSSErrorNotificationMsg.cableId = Psc_ControllerId;

    // TODO: Implement board state exceptions when in error/warnings.
    replyMessage.payload.pSSErrorNotificationMsg.pssId = 0;
    replyMessage.payload.pSSErrorNotificationMsg.errors = 0;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(replyMessage.payload.pSSErrorNotificationMsg);

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "ErrorsMessage: CableId=%d State=%d", Psc_ControllerId, getState());
    // TODO: Add log message
    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void BoardStateMachine::sendWarningsMessage()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_WarningNotification;
    replyMessage.payload.pSSWarningNotificationMsg.cableId = Psc_ControllerId;

    // TODO: Implement board state exceptions when in error/warnings.
    replyMessage.payload.pSSWarningNotificationMsg.pssId = 0;
    replyMessage.payload.pSSWarningNotificationMsg.warnings = 0;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(replyMessage.payload.pSSWarningNotificationMsg);

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "WarningsMessage: CableId=%d State=%d", Psc_ControllerId, getState());
    // TODO: Add log message
    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void BoardStateMachine::ST_Stopping()
{
}

void BoardStateMachine::eventStartShuttingDown()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(EVENT_IGNORED) //ST_Off
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_Startup
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_On
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_Initializing
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_Ready
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_Error
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_Stopping
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_Emergency
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ShuttingDown)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
}

void BoardStateMachine::eventStartUp()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Startup) //ST_Off
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Startup
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_On
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Startup)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
//    sendStatusUpdate();
}

void BoardStateMachine::eventStartUpFinished()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_On)//ST_Startup
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_On
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_On)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventBoardConfigStart()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Initializing)//ST_On
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventBoardConfigEnd()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_On
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Ready)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventError()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Error)//ST_Startup
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Error)//ST_On
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Error)//ST_Initializing
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Error)//ST_Ready
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Error
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Error)//ST_Stopping
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Error)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventStop()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_On
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_On)//ST_Initializing
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_On)//ST_Ready
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Error
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Stopping
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventStartRecovery()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_On
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Initializing
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Recovery)//ST_Ready
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Recovery)//ST_Error
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Stopping
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Recovery)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Recovery)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventEndRecovery()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_On
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Ready)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventEmergency()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_On
    TRANSITION_MAP_ENTRY(EVENT_IGNORED)//ST_Initializing
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_EMR)//ST_Ready
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_EMR)//ST_Error
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_EMR)//ST_Stopping
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_EMR)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_EMR)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventShuttingDownComplete()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_On
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Emergency
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_Off)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

void BoardStateMachine::eventApplicationUpload()
{
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) //ST_Off
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Startup
    TRANSITION_MAP_ENTRY(E_BoardStatePrivate_ApplicationUpload)//ST_On
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Initializing
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Ready
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Error
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Stopping
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Emergency
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ShuttingDown
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_ApplicationUpload
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)//ST_Recovery
    END_TRANSITION_MAP(NULL)
    sendStatusUpdate();
}

E_BoardState BoardStateMachine::getState()
{
    static const E_BoardState stateLookupTable[] =
    { E_BoardState_Off, E_BoardState_Startup, E_BoardState_On, E_BoardState_Initializing, E_BoardState_Ready,
            E_BoardState_Error, E_BoardState_Stopping, E_BoardState_EMR, E_BoardState_ShuttingDown,
            E_BoardState_ApplicationUpload, E_BoardState_Size, E_BoardState_Recovery };

    return stateLookupTable[currentState];
}

