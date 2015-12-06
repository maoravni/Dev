/*
 * ValidationElement.h
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#ifndef VALIDATIONELEMENT_H_
#define VALIDATIONELEMENT_H_

#include "Element.h"
#include "RangeChecker.h"
#include "ValidationElementBase.h"
#include <PscServer/PscMessageStructs.h>
#include <PscSubsystem.h>
#include <math.h>
//#include <PscServer/PscMasterServer.h>

//enum E_ElementState
//{
//    E_ElementState_Valid, E_ElementState_Invalid
//};

struct T_ValidationElementStatusSplit
{
    // TODO: change name to "Valid" and also meaning of the bit.
    bool notValid :1;
    bool hasErrors :1;
    bool hasWarnings :1;
    //    E_ElementState state :3;
};

union T_ValidationElementStatus
{
    char full;
    T_ValidationElementStatusSplit split;
};

template<class _type>
class ValidationElement: public ValidationElementBase
{
protected:
    _type m_value; //!< Value of the element.
    _type m_notifiedValue;

    // Min/Max values for this element. Values outside these will be clamped.
    RangeChecker<_type> m_allowedRange;

    // Min/Max values for the working range. Values outside will issue a warning.
    RangeChecker<_type> m_workingRange;

    // Min/Max values for the warning range. Values outside will issue an error.
    RangeChecker<_type> m_warningRange;

#ifdef OLD_PROTECTION
    // Soft/Hard limit values for protection.
    RangeChecker<_type> m_softProtectionRange;
    RangeChecker<_type> m_hardProtectionRange;
#endif

    uint32_t m_errorBits;
    uint32_t m_warningBits;

    T_ValidationElementStatus m_status;

public:
    ValidationElement();

    virtual ~ValidationElement();

    virtual void updateAllowedRange(float min, float max, bool checkMin, bool checkMax);
    virtual void updateWorkingRange(float min, float max, bool checkMin, bool checkMax);
    virtual void updateWarningRange(float min, float max, bool checkMin, bool checkMax);
#ifdef OLD_PROTECTION
    virtual void updateSoftProtectionRange(float low, float high, bool useLow, bool useHigh);
    virtual void updateHardProtectionRange(float low, float high, bool useLow, bool useHigh);
#endif
    virtual bool isInAllowedRange()
    {
        return (!m_status.split.notValid && m_allowedRange.inRange(m_value));
    }
    virtual bool isInWorkingRange()
    {
        return (!m_status.split.notValid && m_workingRange.inRange(m_value));
    }
    virtual bool isInWarningRange()
    {
        return (!m_status.split.notValid && m_warningRange.inRange(m_value));
    }
//    virtual bool isAboveWarningRange() {return (!m_status.split.notValid && m_warningRange.aboveMaxRange(m_value));}
//    virtual bool isAboveWorkingRange() {return (!m_status.split.notValid && m_workingRange.aboveMaxRange(m_value));}
//    virtual bool isBelowWarningRange() {return (!m_status.split.notValid && m_warningRange.belowRange(m_value));}
//    virtual bool isBelowWorkingRange() {return (!m_status.split.notValid && m_workingRange.belowRange(m_value));}
#ifdef OLD_PROTECTION
    virtual bool isInsideLimit()
    {   return (!m_status.split.notValid && m_softProtectionRange.inRange(m_value));}
    virtual bool isOutsideSoftLimit()
    {   return (!m_status.split.notValid && !m_softProtectionRange.inRange(m_value)/* && m_hardProtectionRange.inRange(m_value)*/);}
    virtual bool isOutsideHardLimit()
    {   return (!m_status.split.notValid && !m_hardProtectionRange.inRange(m_value));}
#endif

    bool isInAllowedRange(_type value)
    {
        return m_allowedRange.inRange(value);
    }
    bool isInWorkingRange(_type value)
    {
        return m_workingRange.inRange(value);
    }
    bool isInWarningRange(_type value)
    {
        return m_warningRange.inRange(value);
    }

    _type getMinAllowed()
    {
        return m_allowedRange.getMin();
    }
    _type getMaxAllowed()
    {
        return m_allowedRange.getMax();
    }
    _type getMinWorking()
    {
        return m_workingRange.getMin();
    }
    _type getMaxWorking()
    {
        return m_workingRange.getMax();
    }
    _type getMinWarning()
    {
        return m_warningRange.getMin();
    }
    _type getMaxWarning()
    {
        return m_warningRange.getMax();
    }
#ifdef OLD_PROTECTION
    _type getLowSoftLimit()
    {   return m_softProtectionRange.getMin();}
    _type getHighSoftLimit()
    {   return m_softProtectionRange.getMax();}
    _type getLowHardLimit()
    {   return m_hardProtectionRange.getMin();}
    _type getHighHardLimit()
    {   return m_hardProtectionRange.getMax();}
#endif

    _type getValue() const
    {
        return m_value;
    }

    virtual _type& operator()();

    virtual bool hasError()
    {
        return m_status.split.hasErrors;
    }

    virtual bool hasWarning()
    {
        return m_status.split.hasWarnings;
    }

    virtual char hasExceptions()
    {
        char stat = getStatus();
        return (stat >> 1);
    }

    virtual void setValueValid(bool valid);
    virtual bool isValid()
    {
        return !m_status.split.notValid;
    }
    virtual char getStatus()
    {
        return m_status.full;
    }

    virtual void setValue(const uint32_t value);
    virtual void setValue(const int32_t value);
    virtual void setValue(const float value);

    virtual uint32_t operator=(const uint32_t value);
    virtual int32_t operator=(const int32_t value);
    virtual float operator=(const float value);

    virtual uint32_t getValueU32() const;
    virtual int32_t getValueI32() const;
    virtual float getValueF() const;

    virtual void sendDeviceStatus();
    virtual void sendCurrentErrors();
    virtual void sendCurrentWarnings();

    virtual void updateErrorBits(E_PSSErrors error, bool state);
    virtual void updateWarningBits(E_PSSErrors warning, bool state);
    virtual uint32_t getErrors();
    virtual uint32_t getWarnings();

private:
    void _setValue(_type value);
    void updateErrorBits();

protected:
    virtual bool checkIfCanSendUpdate();

};

template<class _type>
inline ValidationElement<_type>::ValidationElement()
{
    m_status.full = 0;
    m_errorBits = 0;
    m_warningBits = 0;
    m_value = 0;
}

template<class _type>
inline ValidationElement<_type>::~ValidationElement()
{
}

// TODO: Cast to _type
template<class _type>
inline void ValidationElement<_type>::updateAllowedRange(float min, float max, bool checkMin, bool checkMax)
{
    m_allowedRange.updateRange((_type)min, (_type)max, checkMin, checkMax);
}

template<class _type>
inline void ValidationElement<_type>::updateWorkingRange(float min, float max, bool checkMin, bool checkMax)
{
    m_workingRange.updateRange((_type)min, (_type)max, checkMin, checkMax);
}

template<class _type>
inline void ValidationElement<_type>::updateWarningRange(float min, float max, bool checkMin, bool checkMax)
{
    m_warningRange.updateRange((_type)min, (_type)max, checkMin, checkMax);
}

#ifdef OLD_PROTECTION
template<class _type>
inline void ValidationElement<_type>::updateSoftProtectionRange(float low, float high, bool useLow, bool useHigh)
{
    m_softProtectionRange.updateRange(low, high, useLow, useHigh);
}

template<class _type>
inline void ValidationElement<_type>::updateHardProtectionRange(float low, float high, bool useLow, bool useHigh)
{
    m_hardProtectionRange.updateRange(low, high, useLow, useHigh);
}
#endif

template<class _type>
inline void ValidationElement<_type>::sendDeviceStatus()
{
    ValidationElementBase::sendDeviceStatus(m_value);
}

template<class _type>
inline void ValidationElement<_type>::sendCurrentErrors()
{
    sendError(Psc_ControllerId, getPssId(), m_errorBits);
}

template<class _type>
inline void ValidationElement<_type>::sendCurrentWarnings()
{
    sendWarning(Psc_ControllerId, getPssId(), m_warningBits);
}

template<class _type>
inline void ValidationElement<_type>::_setValue(_type value)
{
    m_value = m_allowedRange.clampToRange(value);
    updateErrorBits();
    if (isMonitoringEnabled() && checkIfCanSendUpdate())
        sendDeviceStatus();
    updateObservers();
//    return m_value;
}

template<class _type>
inline uint32_t ValidationElement<_type>::getErrors()
{
    return m_errorBits;
}

template<class _type>
inline uint32_t ValidationElement<_type>::getWarnings()
{
    return m_warningBits;
}

template<class _type>
inline void ValidationElement<_type>::updateErrorBits()
{
    m_status.split.hasErrors = false;
    m_status.split.hasWarnings = false;

    if (m_status.split.notValid)
    {
        m_status.split.hasErrors = true;
        return;
    }
    if (!m_warningRange.inRange(m_value))
    {
        m_status.split.hasErrors = true;
        m_status.split.hasWarnings = false;
        return;
    }
    if (!m_workingRange.inRange(m_value))
    {
        m_status.split.hasErrors = false;
        m_status.split.hasWarnings = true;
        return;
    }
}

//template<class _type>
//inline _type& ValidationElement<_type>::operator =(const _type& rhs)
//{
//    _setValue(rhs);
//    return m_value;
//}

template<class _type>
inline _type& ValidationElement<_type>::operator()()
{
    return m_value;
}

template<class _type>
inline void ValidationElement<_type>::setValueValid(bool valid)
{
//    if (!valid && !m_status.split.notValid)
    valid = !valid;
    if (valid != m_status.split.notValid)
    {
        m_status.split.notValid = valid;
        updateErrorBits(E_PSSErrors_SensorMalfunction, valid);
        updateErrorBits();
        updateObservers();
    }
}

typedef ValidationElement<float> ValidationElementFloat;
typedef ValidationElement<uint8_t> ValidationElementU8;
typedef ValidationElement<uint16_t> ValidationElementU16;
typedef ValidationElement<uint32_t> ValidationElementU32;
typedef ValidationElement<int8_t> ValidationElementI8;
typedef ValidationElement<int16_t> ValidationElementI16;
typedef ValidationElement<int32_t> ValidationElementI32;

template<class _type>
inline void ValidationElement<_type>::setValue(const uint32_t value)
{
    _setValue((_type) value);
}

template<class _type>
inline void ValidationElement<_type>::setValue(const int32_t value)
{
    _setValue((_type) value);
}

template<class _type>
inline void ValidationElement<_type>::setValue(const float value)
{
    _setValue((_type) value);
}

template<class _type>
inline uint32_t ValidationElement<_type>::operator =(const uint32_t value)
{
    _setValue((_type) value);
    return (uint32_t)(m_value);
}

template<class _type>
inline int32_t ValidationElement<_type>::operator =(const int32_t value)
{
    _setValue((_type) value);
    return (int32_t)(m_value);
}

template<class _type>
inline float ValidationElement<_type>::operator =(const float value)
{
    _setValue((_type) value);
    return (float) (m_value);
}

template<class _type>
inline uint32_t ValidationElement<_type>::getValueU32() const
{
    return (uint32_t) m_value;
}

template<class _type>
inline int32_t ValidationElement<_type>::getValueI32() const
{
    return (int32_t) m_value;
}

template<class _type>
inline float ValidationElement<_type>::getValueF() const
{
    return (float) m_value;
}

template<class _type>
inline void ValidationElement<_type>::updateErrorBits(E_PSSErrors error, bool state)
{
    uint32_t previousErrorBits = m_errorBits;

    if (state)
        m_errorBits |= error;
    else
        m_errorBits &= ~error;

    m_status.split.hasErrors = (m_errorBits != 0);

    if (m_errorBits != previousErrorBits && getPssId() != 0)
        sendError(Psc_ControllerId, getPssId(), m_errorBits);
}

template<class _type>
inline void ValidationElement<_type>::updateWarningBits(E_PSSErrors warning, bool state)
{
    uint32_t previousWarningBits = m_errorBits;

    if (state)
        m_warningBits |= warning;
    else
        m_warningBits &= ~warning;

    m_status.split.hasWarnings = (m_warningBits != 0);

    if (m_warningBits != previousWarningBits && getPssId() != 0)
        sendWarning(Psc_ControllerId, getPssId(), m_warningBits);
}

template<class _type>
inline bool ValidationElement<_type>::checkIfCanSendUpdate()
{
    if (m_minInterval == 0xff && (abs((float)m_notifiedValue-(float)m_value) >= 0.1))
        return true;

    return ElementBase::checkIfCanSendUpdate();
}

#endif /* VALIDATIONELEMENT_H_ */
