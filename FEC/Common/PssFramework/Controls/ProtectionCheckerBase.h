/*
 * ProtectionCheckerBase.h
 *
 *  Created on: 3 Sep 2014
 *      Author: maora
 */

#ifndef PROTECTIONCHECKERBASE_H_
#define PROTECTIONCHECKERBASE_H_

#include <Elements/ElementRepository.h>
#include <ErrorBitManager.h>

enum E_DeviceProtectionState
{
    E_DeviceProtectionState_InRange = 1,
    E_DeviceProtectionState_SoftLimitExceededWarning = 2,
    E_DeviceProtectionState_SoftLimitExceeded = 3,
    E_DeviceProtectionState_HardLimitExceeded = 4,
};

enum E_ProtectionCheckerType
{
    E_ProtectionCheckerType_AbsoluteValue,
    E_ProtectionCheckerType_ConstantDelta,
    E_ProtectionCheckerType_Proportional,
    E_ProtectionCheckerType_CurrentLimits
};

class ProtectionCheckerBase : public IObserver/*, public ErrorBitManager*/
{
private:
    uint16_t m_protectionIndex;

protected:
    ElementBase* m_observedElement;
    ElementU8* m_protectionStatus;
    uint16_t m_debounceTimeout;
    bool m_debounceTimeoutStarted;

public:
    ProtectionCheckerBase();
    ProtectionCheckerBase(F_FILE* f);
    virtual ~ProtectionCheckerBase();

    virtual void addObserver(IObserver* observer);
    virtual void setElement(ElementBase* element);
    virtual ElementBase* getProtectionElement() {return m_observedElement;}
    virtual E_DeviceProtectionState getProtectionState()
    {
        return (E_DeviceProtectionState) (m_protectionStatus->getValueI32());
    }
    virtual E_ProtectionCheckerType getProtectionType() = 0;
    virtual E_PSSErrors getErrorType() = 0;
    virtual E_PSSWarnings getWarningType() = 0;
    virtual uint16_t getPssId();
    void setProtectionIndex(uint16_t index);
    uint16_t getProtectionIndex() {return m_protectionIndex;}

    uint16_t getDebounceTimeout() const
    {
        return m_debounceTimeout;
    }

    void setDebounceTimeout(uint16_t debounceTimeout)
    {
        m_debounceTimeout = debounceTimeout;
    }

    virtual E_DeviceProtectionState calcProtectionState(ElementBase* element) = 0;
    virtual void setNewProtectionState(E_DeviceProtectionState state);
    virtual void timeoutExpired(uint16_t timeoutType);

    virtual void serialize(F_FILE* f) = 0;

    template <class T> friend class Serializer;
};

#endif /* PROTECTIONCHECKERBASE_H_ */
