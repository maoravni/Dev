/*
 * ControlBase.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef CONTROLBASE_H_
#define CONTROLBASE_H_

#include <Elements/ElementBase.h>
#include <Elements/IObserver.h>
#include <PscServer/PscMessageStructs.h>
#include <stdint.h>
#include <Elements/ValidationElementBase.h>
#include "DeviceProtectionChecker.h"
#include <list>
#include <ErrorBitManager.h>
#include "DeviceThresholdChecker.h"

enum E_ControlState
{
    E_ControlState_Off = 0,
    E_ControlState_On = 1,
    E_ControlState_Initializing = 2,
    E_ControlState_Standby = 3,
    E_ControlState_Move2Ready = 4,
    E_ControlState_Ready = 6,
    E_ControlState_Stopping = 11,
    E_ControlState_Error = 13,
    E_ControlState_Recovery = 16,
    E_ControlState_Emergency = 17,
    E_ControlState_Size
};

enum E_ControlType
{
    E_ControlType_PidControl,
    E_ControlType_AirBleeding,
    E_ControlType_Protection,
    E_ControlType_AnalogOutInverter,
    E_ControlType_ModbusInverter,
    E_ControlType_ActivationWithFeedback,
    E_ControlType_HysteresisControl,
    E_ControlType_3SensorLiquidLevel,
    E_ControlType_LiquidLevelPumpControl,
    E_ControlType_EmergencyInputControl,
    E_ControlType_LiftPbOnError,
    E_ControlType_LeakageDetectionControl,
    E_ControlType_OrderedShutdown,
    E_ControlType_LiftPbOnErrorCcsGen2,
    E_ControlType_ObserveAndNotifyControl,
    E_ControlType_SubtractTwoDevices,
    E_ControlType_CalculateOnTwoDevices,
    E_ControlType_LiftPbOnErrorCcsGen3,
};

enum E_ExceptionState
{
    E_ExceptionState_Set,
    E_ExceptionState_Reset,
    E_ExceptionState_Unchanged
};

enum E_ActivationState
{
    E_ActivationState_Unknown = 0, E_ActivationState_Active = 1, E_ActivationState_Inactive = 2,
};

enum E_CalculationFunction
{
    E_CalculationFunction_Subtract,
    E_CalculationFunction_Add,
    E_CalculationFunction_Multiply,
    E_CalculationFunction_Divide,
    E_CalculationFunction_Maximum,
    E_CalculationFunction_Minimum,
};

struct T_ControlStatusSplit
{
    char hasErrors :1;
    char hasWarnings :1;
};

union T_ControlStatus
{
    char full;
    T_ControlStatusSplit split;
};

class ControlBase: public IObserver/*, public ErrorBitManager*/
{
private:
    uint16_t m_controlIndex; //!< ID of the element in the element repository.
    uint16_t m_pssId; //!< ID of the element assigned by the SUM.
    T_ControlStatus m_controlExceptions;
    T_ControlStatus m_previousControlExceptions;

    bool m_stopOnEmrBehavior;
    bool m_stopOnDisconnection;
//    uint32_t m_errorBits;
//    uint32_t m_warningBits;

protected:
    bool m_monitoringEnabled;
    bool m_isEnabled;
    bool m_isProtectionActive;
    bool m_isProtectionWarningActive;
    E_ControlState m_controlState;
//    StateMachine m_controlStateMachine;
    ErrorBitManager m_errorBitManager;

    uint32_t m_lastSn; //!< Last serial number of the message requiring a sequence end.
    uint32_t m_lastStateChangeMessageId;


#ifdef OLD_PROTECTION
    T_ElementArray m_protectionElements;
#endif
    T_ProtectionCheckerList m_protectionCheckers;
//    T_DeviceProtectionCheckerArray m_protectionCheckers;
    // TODO: Consider changing the vector to a pointer to a vector, to save memory when the
    // dependency check is not required.
    T_DeviceCheckerList m_dependentCheckers;

public:
    ControlBase();
    virtual ~ControlBase();

    virtual void execute() = 0;
    virtual bool executeProtectionCheck(ElementBase* element/*, E_PSSErrors error*/);
    virtual bool executeProtectionCheck(/*E_PSSErrors error*/);
    virtual void clearProtectionCheckErrors();

    virtual E_ControlType getControlType() = 0;

    virtual ElementBase* getMainControlElement() = 0;

    uint16_t getControlIndex() const
    {
        return m_controlIndex;
    }

    void setControlIndex(uint16_t controlIndex)
    {
        m_controlIndex = controlIndex;
    }

    uint16_t getPssId() const
    {
        return m_pssId;
    }

    virtual void setPssId(uint16_t deviceId)
    {
        m_pssId = deviceId;
        m_errorBitManager.setPssId(deviceId);
//        m_secondaryPssId = 0xfffe;
    }

    bool initControl(uint32_t msgId, uint32_t sn);
    bool move2Standby(uint32_t msgId, uint32_t delay, uint32_t sn);
    bool reset2On(uint32_t msgId, uint32_t sn);
    bool move2Error(uint32_t msgId, uint32_t sn);
    bool stopOnEmr();
    bool recoverFromEmr();

    virtual bool setSetpoint(float setpoint, uint32_t sn) = 0;

    virtual bool onInitControl() = 0;
    virtual bool onMove2Standby(uint32_t delay) = 0;
    virtual bool onReset2On() = 0;
    virtual bool onMove2Error() = 0;
    virtual bool onStopOnEmr() = 0;
    virtual bool onRecoverFromEmr() = 0;

    virtual void startRecovery() {}

    bool isMonitoringEnabled() const
    {
        return m_monitoringEnabled;
    }

    void setMonitoringEnabled(bool monitoringEnabled);
    virtual void setMinInterval(int16_t minInterval) {}
    virtual void setMaxInterval(int16_t maxInterval) {}

    virtual bool sendNotification() = 0;
#ifdef OLD_PROTECTION
    virtual void addProtectionElement(ElementBase* element);
#endif

    virtual E_ActivationState getActivationState() = 0;
    virtual void addProtectionChecker(ProtectionCheckerBase* protectionChecker);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) = 0;
    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType);

    virtual void logDependencyCheckFailures(E_ActivationState state, E_ControlState controlState, E_PSSErrors error);
    virtual void clearDependencyCheckFailures(E_PSSErrors error);

    virtual bool raiseError(uint16_t secondaryPssId, E_PSSErrors error, bool errorState);
    virtual bool raiseWarning(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState);
    virtual void checkAllDependencies(E_ActivationState state, E_ControlState controlState,
            bool &resultConsiderDeactivateOnChange, bool &resultIgnoreDeactiveOnChange);
    virtual bool checkAllDependenciesForOppositeState();
//    bool isErrorRaised(E_PSSErrors error);

    virtual void sendCurrentErrors();
    virtual void sendCurrentWarnings();

    char getControlExceptions() {return m_controlExceptions.full;}

    virtual void setBoardInReady(bool state);

    bool isStopOnEmrBehavior() const
    {
        return m_stopOnEmrBehavior;
    }

    void setStopOnEmrBehavior(bool stopOnEmrBehavior)
    {
        m_stopOnEmrBehavior = stopOnEmrBehavior;
    }

    bool isStopOnDisconnection() const
    {
        return m_stopOnDisconnection;
    }

    void setStopOnDisconnection(bool stopOnDisconnection)
    {
        m_stopOnDisconnection = stopOnDisconnection;
    }

private:
    void setControlExceptions(E_ExceptionState errors, E_ExceptionState warnings);

protected:
    void endInitControl();
    void endMove2Standby();
    void endReset2On();
    void endMove2Error();
    void endStopOnEmr();
    void endRecoverFromEmr();

};


#endif /* CONTROLBASE_H_ */
