/*
 * DeviceProtectionChecker.h
 *
 *  Created on: 1 αιεπ 2014
 *      Author: maora
 */

#ifndef DEVICEPROTECTIONCHECKER_H_
#define DEVICEPROTECTIONCHECKER_H_

#include <Elements/ElementRepository.h>
#include <Elements/RangeChecker.h>
#include <Controls/ProtectionCheckerBase.h>
#include <list>

//enum E_DeviceProtectionState
//{
//    E_DeviceProtectionState_InRange = 1,
//    E_DeviceProtectionState_SoftLimitExceeded = 2,
//    E_DeviceProtectionState_HardLimitExceeded = 3,
//};

class DeviceProtectionChecker: public ProtectionCheckerBase
{
private:
//    ElementBase* m_observedElement;
//    ElementU8* m_protectionStatus;
    RangeChecker<float> m_softProtectionRange;
    RangeChecker<float> m_hardProtectionRange;

public:
    DeviceProtectionChecker();
    virtual ~DeviceProtectionChecker();

    virtual void updateNotification(ElementBase* element);

    /**
     * This method asks the observer if a value change is permitted.
     * It's main use is when an output is required to change it's value
     * and this means that some other controls should be disabled,
     * or the output change is not permitted.
     * @param element
     */
    virtual bool requestValueChange(ElementBase* element);

//    void setElement(ElementBase* element);
    void updateSoftProtectionRange(float low, float high, bool useLow, bool useHigh);
    void updateHardProtectionRange(float low, float high, bool useLow, bool useHigh);
//    void addObserver(IObserver* observer);

    bool isInsideLimit();
    bool isOutsideSoftLimit();
    bool isOutsideHardLimit();
    bool modifyProtectionResultAccordingToMissingPriority(bool result);

//    ElementBase* getProtectionElement() {return m_observedElement;}

    float getUpperHardLimit()
    {
        return m_hardProtectionRange.getMax();
    }
    float getLowerHardLimit()
    {
        return m_hardProtectionRange.getMin();
    }

    virtual E_ProtectionCheckerType getProtectionType()
    {
        return E_ProtectionCheckerType_AbsoluteValue;
    }
    virtual E_PSSErrors getErrorType()
    {
        return E_PSSErrors_DeviceExceedsSoftLimits;
    }
    virtual E_PSSWarnings getWarningType()
    {
        return E_PSSWarnings_ControlExceedsLimits;
    }
    virtual E_DeviceProtectionState calcProtectionState(ElementBase* element);

};

inline bool DeviceProtectionChecker::modifyProtectionResultAccordingToMissingPriority(bool result)
{
    switch (m_observedElement->getMissingDevicePriority())
    {
    case E_MissingDevicePriority_High:
        break;
    case E_MissingDevicePriority_Med:
    case E_MissingDevicePriority_Low:
        // if the observed element is invalid, in med and low priority we
        // don't want to issue a protection error.
        if (!m_observedElement->isValid())
            result = false;
        break;
    }
}

inline bool DeviceProtectionChecker::isInsideLimit()
{
    if (!m_observedElement->isValid())
    {
        if (m_observedElement->getMissingDevicePriority() == E_MissingDevicePriority_High)
            return false;
        else
            return true;
    }
    return (m_softProtectionRange.inRange(m_observedElement->getValueF()));
}

inline bool DeviceProtectionChecker::isOutsideSoftLimit()
{
    if (!m_observedElement->isValid())
    {
        if (m_observedElement->getMissingDevicePriority() == E_MissingDevicePriority_High)
            return false;
        else
            return true;
    }
    return (!m_softProtectionRange.inRange(m_observedElement->getValueF()));
}

inline bool DeviceProtectionChecker::isOutsideHardLimit()
{
    if (!m_observedElement->isValid())
    {
        if (m_observedElement->getMissingDevicePriority() == E_MissingDevicePriority_High)
            return false;
        else
            return true;
    }
    return (!m_hardProtectionRange.inRange(m_observedElement->getValueF()));
}

//typedef DynamicArray<DeviceProtectionChecker*> T_DeviceProtectionCheckerArray;
typedef std::vector<ProtectionCheckerBase*> T_ProtectionCheckerList;
typedef std::vector<ProtectionCheckerBase*>::iterator T_ProtectionCheckerListIterator;

#endif /* DEVICEPROTECTIONCHECKER_H_ */
