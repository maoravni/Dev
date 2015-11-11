/*
 * TmcStateMachine.h
 *
 *  Created on: 2 Dec 2012
 *      Author: maora
 */

#ifndef TMCSTATEMACHINE_H_
#define TMCSTATEMACHINE_H_

#include "StateMachine.h"

enum E_TmcStateStates
{
    E_TmcState_Reset, //!< First state, after reset
    E_TmcState_On, //!< Second state, after configuration is done and all is well.
    E_TmcState_Standby, //!< Ready to receive commands from the OPC
    E_TmcState_Warming, //!< Transitional state - temperature was set, waiting to reach.;
    E_TmcState_Ready, //!< Temperature was reached.
    E_TmcState_Error, //!< An error occured in the system.
    E_TmcState_Emergency, //!< An emergecy occured in the system.
    E_TmcState_Size //!< Dummy enum, to know the number of enums.
};

/**
 * Implements the state machine of the TmcServer
 */
class TmcStateMachine: public StateMachine
{
public:
    virtual ~TmcStateMachine();
    TmcStateMachine() : StateMachine(E_TmcState_Size) {}
};

#endif /* TMCSTATEMACHINE_H_ */
