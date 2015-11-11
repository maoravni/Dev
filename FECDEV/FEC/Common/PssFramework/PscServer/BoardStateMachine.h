/*
 * BoardStateMachine.h
 *
 *  Created on: 22 αιεμ 2013
 *      Author: maora
 */

#ifndef BOARDSTATEMACHINE_H_
#define BOARDSTATEMACHINE_H_

#include <StateMachine.h>
#include "PscMessageStructs.h"

class BoardStateMachine: public StateMachine
{
private:
    enum E_BoardStateMachine_Private
    {
        E_BoardStatePrivate_Off,
        E_BoardStatePrivate_Startup,
        E_BoardStatePrivate_On,
        E_BoardStatePrivate_Initializing,
        E_BoardStatePrivate_Ready,
        E_BoardStatePrivate_Error,
        E_BoardStatePrivate_Stopping,
//        E_BoardStatePrivate_StoppingOnEMR,
        E_BoardStatePrivate_EMR,
        E_BoardStatePrivate_ShuttingDown,
        E_BoardStatePrivate_ApplicationUpload,
        E_BoardStatePrivate_Recovery,
        E_BoardStatePrivate_Size,
    };

public:
    BoardStateMachine();
    virtual ~BoardStateMachine();

    E_BoardState getState();

    void eventStartUp();
    void eventStartUpFinished();
    void eventBoardConfigStart();
    void eventBoardConfigEnd();
    void eventError();
    void eventStop();
    void eventStartShuttingDown();
    void eventEmergency();
    void eventShuttingDownComplete();
    void eventApplicationUpload();
    void eventStartRecovery();
    void eventEndRecovery();

    void sendStatusUpdate();
    void sendStateMessage();
    void sendErrorsMessage();
    void sendWarningsMessage();
private:
    void ST_Off();
    void ST_Startup();
    void ST_On();
    void ST_Initializing();
    void ST_Ready();
    void ST_Error();
    void ST_Stopping();
    void ST_Emergency();
    void ST_ShuttingDown();
    void ST_ApplicationUpload();
    void ST_Recovery();

    BEGIN_STATE_MAP
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Off)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Startup)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_On)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Initializing)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Ready)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Error)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Stopping)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Emergency)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_ShuttingDown)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_ApplicationUpload)
    STATE_MAP_ENTRY(&BoardStateMachine::ST_Recovery)
    END_STATE_MAP

//public:
//    const StateStruct* GetStateMap()
//    {
//        StateStruct s;
//        s.pStateFunc = reinterpret_cast<StateFunc>(&BoardStateMachine::ST_Off);
////        static const StateStruct StateMap[] =
////        {
////        { ST_Off } };
//
//    }
    };

#endif /* BOARDSTATEMACHINE_H_ */
