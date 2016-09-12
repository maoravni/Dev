/*
 * Element.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <stdint.h>
#include <Elements/ElementBase.h>
#include <Elements/ValidationElement.h>
#include <math.h>
#include <Persistency/ElementSerializers.h>

extern "C" uint16_t Psc_ControllerId;

template <class _type>
class Element : public ElementBase
{
protected:
    _type m_value; //!< Value of the element.
    _type m_notifiedValue;

public:
    Element();
    virtual ~Element();

    _type getValue() const
    {
        return m_value;
    }

    virtual const void* getValueP() const
    {
        return &m_value;
    }

//    virtual _type& operator=(const _type& rhs);
    virtual _type& operator()();

#ifdef __GNUC__
    virtual void setValue(const int value);
#endif
//    virtual void setValue(const uint8_t value);
    virtual void setValue(const uint32_t value);
    virtual void setValue(const int32_t value);
    virtual void setValue(const float value);

    virtual uint32_t operator=(const uint32_t value);
    virtual int32_t operator=(const int32_t value);
    virtual float operator=(const float value);

    virtual uint32_t getValueU32() const;
    virtual int32_t getValueI32() const;
    virtual float getValueF() const;

    virtual void updateAllowedRange(float min, float max, bool checkMin, bool checkMax) {}
    virtual void updateWorkingRange(float min, float max, bool checkMin, bool checkMax) {}
    virtual void updateWarningRange(float min, float max, bool checkMin, bool checkMax) {}
    virtual void updateSoftProtectionRange(float soft, float hard, bool useLow, bool useHigh) {}
    virtual void updateHardProtectionRange(float soft, float hard, bool useLow, bool useHigh) {}

    virtual bool isInAllowedRange() {return true;}

    virtual bool isInWorkingRange() {return true;}
    virtual bool isAboveWorkingRange() {return false;}
    virtual bool isBelowWorkingRange() {return false;}

    virtual bool isInWarningRange() {return false;}
    virtual bool isAboveWarningRange() {return false;}
    virtual bool isBelowWarningRange() {return true;}

    virtual bool isInsideLimit() {return true;}
    virtual bool isOutsideSoftLimit() {return false;}
    virtual bool isOutsideHardLimit() {return false;}

    virtual void sendDeviceStatus();
    virtual void sendCurrentErrors();
    virtual void sendCurrentWarnings();
    virtual E_ValueType getValueType();

    virtual bool hasError() {return false;}
    virtual bool hasWarning() {return false;}
    virtual char hasExceptions() {return false;}

    virtual void setValueValid(bool valid) {};
    virtual bool isValid() {return true;}
    virtual char getStatus() {return 0;}

    virtual void setMissingDevicePriority(E_MissingDevicePriority priority){}
    virtual E_MissingDevicePriority getMissingDevicePriority(){return E_MissingDevicePriority_High;}

    virtual void updateErrorBits(E_PSSErrors error, bool state) {}
    virtual void updateWarningBits(E_PSSErrors warning, bool state) {}
    virtual uint32_t getErrors() {return 0;}
    virtual uint32_t getWarnings() {return 0;}

    virtual bool checkIfCanSendUpdate();

    virtual void serialize(F_FILE* f);// {return _serialize(f);}
    virtual void deserialize(F_FILE* f);// {return _serialize(f);}

private:
    virtual void _setValue(_type value)
    {
        m_value = value;
        if (isMonitoringEnabled() && checkIfCanSendUpdate())
            sendDeviceStatus();
        updateObservers();
    }
    //int _serialize(F_FILE* f);
};

template<class _type>
inline Element<_type>::Element():ElementBase()
{
    m_value = 0;
}

template<class _type>
inline Element<_type>::~Element()
{
}

//template<class _type>
//inline _type& Element<_type>::operator =(const _type& rhs)
//{
//    m_value = rhs;
//    updateObservers();
//}
//

template<class _type>
inline _type& Element<_type>::operator()()
{
    return m_value;
}

typedef Element<float> ElementFloat;
typedef Element<std::uint8_t> ElementU8;
typedef Element<std::uint16_t> ElementU16;
typedef Element<std::uint32_t> ElementU32;
typedef Element<std::int8_t> ElementI8;
typedef Element<std::int16_t> ElementI16;
typedef Element<std::int32_t> ElementI32;

#ifdef __GNUC__
template<class _type>
inline void Element<_type>::setValue(const int value)
{
    _setValue((_type)value);
}
#endif

//template<class _type>
//inline void Element<_type>::setValue(const uint8_t value)
//{
//    _setValue((_type)value);
//}

template<class _type>
inline void Element<_type>::setValue(const uint32_t value)
{
    _setValue((_type)value);
}

template<class _type>
inline void Element<_type>::setValue(const int32_t value)
{
    _setValue((_type)value);
}

template<class _type>
inline void Element<_type>::setValue(const float value)
{
    _setValue((_type)value);
}

template<class _type>
inline uint32_t Element<_type>::operator =(const uint32_t value)
{
    _setValue((_type)value);
    return (uint32_t)(value);
}

template<class _type>
inline int32_t Element<_type>::operator =(const int32_t value)
{
    _setValue((_type)value);
    return (int32_t)(value);
}

template<class _type>
inline float Element<_type>::operator =(const float value)
{
    _setValue((_type)value);
    return (float)(value);
}

template<class _type>
inline uint32_t Element<_type>::getValueU32() const
{
    return (uint32_t)m_value;
}

template<class _type>
inline int32_t Element<_type>::getValueI32() const
{
    return (int32_t)m_value;
}

template<class _type>
inline float Element<_type>::getValueF() const
{
    return m_value;
}

template<class _type>
inline void Element<_type>::sendDeviceStatus()
{
    m_notifiedValue = m_value;
    ElementBase::sendDeviceStatus(m_value);
}

template<class _type>
inline void Element<_type>::sendCurrentErrors()
{
    ElementBase::sendError(Psc_ControllerId, getPssId(), 0);
}

template<class _type>
inline void Element<_type>::sendCurrentWarnings()
{
    ElementBase::sendWarning(Psc_ControllerId, getPssId(), 0);
}

template<class _type>
inline bool Element<_type>::checkIfCanSendUpdate()
{
    if (m_minInterval < 0 && (abs((float)m_notifiedValue-(float)m_value) >= 0.1))
        return true;

    return ElementBase::checkIfCanSendUpdate();
}

template<class _type>
inline void Element<_type>::serialize(F_FILE* f)
{
    Serializer<Element<_type> > s;
    s.serialize(f, *this);
}

template<class _type>
inline void Element<_type>::deserialize(F_FILE* f)
{
    Serializer<Element<_type> > s;
    s.deserialize(f, *this);
}

template<class _type>
inline E_ValueType Element<_type>::getValueType()
{
    static_assert(sizeof(_type) == 0, "Please implement template specialization for the specific type");
    throw "Not Implemented";
}

template<>
inline E_ValueType Element<std::int8_t>::getValueType()
{
    return E_ValueType_S8;
}

template<>
inline E_ValueType Element<std::int16_t>::getValueType()
{
    return E_ValueType_S16;
}

template<>
inline E_ValueType Element<std::int32_t>::getValueType()
{
    return E_ValueType_S32;
}

template<>
inline E_ValueType Element<std::uint8_t>::getValueType()
{
    return E_ValueType_U8;
}

template<>
inline E_ValueType Element<std::uint16_t>::getValueType()
{
    return E_ValueType_U16;
}

template<>
inline E_ValueType Element<std::uint32_t>::getValueType()
{
    return E_ValueType_U32;
}

template<>
inline E_ValueType Element<float>::getValueType()
{
    return E_ValueType_Float;
}

#endif /* ELEMENT_H_ */
