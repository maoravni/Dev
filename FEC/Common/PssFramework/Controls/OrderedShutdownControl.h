/*
 * OrderedShutdownControl.h
 *
 *  Created on: 22 Dec 2014
 *      Author: maora
 */

#ifndef ORDEREDSHUTDOWNCONTROL_H_
#define ORDEREDSHUTDOWNCONTROL_H_

#include "ControlBase.h"
#include <list>

enum E_ShutdownOperation
{
    E_ShutdownOperation_ResetToOn,
    E_ShutdownOperation_Init,
    E_ShutdownOperation_Stop,
    E_ShutdownOperation_Activate
};

struct T_OperationNode
{
    portTickType delay;
    ControlBase* control;
    E_ShutdownOperation operation;
    float setpoint;
};

typedef std::list<T_OperationNode> T_ShutdownOperationList;
typedef std::list<T_OperationNode>::iterator T_ShutdownOperationListIterator;

class OrderedShutdownControl: public ControlBase
{
private:
    T_ShutdownOperationList m_shutdownOperationList;
    T_ShutdownOperationListIterator m_currentOperation;

    CBinarySemaphore m_operationCompleteSemaphore;

    bool m_exceptStopOnDisconnection;

public:
    OrderedShutdownControl();
    virtual ~OrderedShutdownControl();

    virtual E_ControlType getControlType() {return E_ControlType_OrderedShutdown;}

    virtual ElementBase* getMainControlElement() {return NULL;}

    virtual void execute();
    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool sendNotification();
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate);
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType);

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element);
    virtual void timeoutExpired(uint16_t timeoutType);

    bool addOperation(int delay, uint16_t control, E_ShutdownOperation operation, float setpoint);
    void performDisconnectionSequence(bool exceptStopOnDisconnection);

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

private:
    void executeCurrentOperation();
};

#endif /* ORDEREDSHUTDOWNCONTROL_H_ */
