/*
 * DeviceThresholdChecker.h
 *
 *  Created on: 10 May 2015
 *      Author: maora
 */

#ifndef DEVICETHRESHOLDCHECKER_H_
#define DEVICETHRESHOLDCHECKER_H_

#include <vector>
#include <Elements/ElementBase.h>

enum E_DependencyCheckType
{
    E_DependencyCheckType_Ignore,
    E_DependencyCheckType_Activate,
    E_DependencyCheckType_Deactivate,
    E_DependencyCheckType_DeactivateAndActivate
};

enum E_PreviousCheckResult
{
    E_DependencyCheckResult_False, E_DependencyCheckResult_True, E_DependencyCheckResult_Unchecked,
};

class DeviceThresholdChecker/*: public ErrorBitManager*/
{
private:
    ElementBase* m_element;
    E_PreviousCheckResult m_previousCheckResult;
    uint16_t m_pssId;

public:
    float m_thresholdValue;
    bool m_greaterThan;
    E_DependencyCheckType m_dependencyCheckType;

    /// This variable defines whether the observing device should
    /// be deactivated if feedback device fails while active.
    bool m_deactivateOnChange;

    /// This variable should affect the observed device - if the observing device
    /// is the observed device allowed to change state?
    bool m_lockChange;

    bool checkDevice(/*bool isControlActive*/)
    {
//        if (isControlActive && !m_deactivateOnChange)
//            return true;
        if (m_greaterThan)
            return (m_element->getValueF() >= m_thresholdValue);
        else
            return (m_element->getValueF() < m_thresholdValue);
    }

    bool checkDevice(bool isControlActive)
    {
        switch (m_dependencyCheckType)
        {
        case E_DependencyCheckType_Ignore:
            return true;
        case E_DependencyCheckType_Activate:
            if (!isControlActive)
                return true;
            break;
        case E_DependencyCheckType_Deactivate:
            if (isControlActive)
                return true;
            break;
        default:
            break;
        }

        return checkDevice();
    }

    DeviceThresholdChecker() :
            m_element(NULL), m_thresholdValue(0), m_greaterThan(false), m_dependencyCheckType(
                    E_DependencyCheckType_DeactivateAndActivate), m_previousCheckResult(
                    E_DependencyCheckResult_Unchecked), m_deactivateOnChange(false), m_lockChange(false)
    {

    }
    DeviceThresholdChecker(F_FILE* f);
    virtual ~DeviceThresholdChecker() {}

    ElementBase* getElement()
    {
        return m_element;
    }

    void setElement(uint16_t primaryPssId, ElementBase* element)
    {
        setPssId(element->getPssId());
        m_element = element;
//        setSecondaryPssId(m_element->getPssId());
    }

    // return true if the result is the same as the previous result.
    bool compareToPreviousCheckResult(bool result)
    {
        if (m_previousCheckResult == E_DependencyCheckResult_Unchecked)
        {
            m_previousCheckResult = (result == true) ? E_DependencyCheckResult_True : E_DependencyCheckResult_False;
            return false;
        }
        bool previousResult = (m_previousCheckResult == E_DependencyCheckResult_True);
        m_previousCheckResult = (result) ? E_DependencyCheckResult_True : E_DependencyCheckResult_False;
        return (previousResult == result);
    }
    void resetPreviousCheckResult();

    uint16_t getPssId() const
    {
        return m_pssId;
    }

    void setPssId(uint16_t pssId)
    {
        m_pssId = pssId;
    }

    virtual void serialize(F_FILE* f);

    template <class T> friend class Serializer;
};

typedef std::vector<DeviceThresholdChecker> T_DeviceCheckerList;
typedef std::vector<DeviceThresholdChecker>::iterator T_DeviceCheckerListIterator;


#endif /* DEVICETHRESHOLDCHECKER_H_ */
