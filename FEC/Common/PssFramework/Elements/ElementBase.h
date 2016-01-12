/*
 * ElementBase.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef ELEMENTBASE_H_
#define ELEMENTBASE_H_

#include <stdint.h>
#include <Elements/IObserver.h>
#include <FreeRTOS.h>
#include <PscServer/PscMessageStructs.h>
#include <list>
#include <vector>

#define M_UNASSIGNED_PSS_ID 0

enum E_MissingDevicePriority
{
    E_MissingDevicePriority_High,
    E_MissingDevicePriority_Med,
    E_MissingDevicePriority_Low
};

class ElementBase
{
protected:
    uint16_t m_elementIndex; //!< ID of the element in the element repository.
    uint16_t m_pssId; //!< ID of the element assigned by the SUM.
    portTickType m_lastNotificationTick; //!< the tick count of the last update
    int8_t m_minInterval; //!< Minimum interval for sending updates.
    int8_t m_maxInterval; //!< Maximum interval that can pass without sending updates.

//    uint32_t m_errorBits;
//    uint32_t m_warningBits;

//    T_ObserverVector m_observerVector;
    T_ObserverList m_observerList;

protected:
    bool m_monitoringEnabled;

public:
    ElementBase();
    virtual ~ElementBase();

    virtual void updateObservers();
    void addObserver(IObserver* observer);
    void removeObserver(IObserver* observer);

    uint16_t getPssId() const
    {
        return m_pssId;
    }

    void setPssId(uint16_t pssId)
    {
        m_pssId = pssId;
    }

    uint16_t getElementIndex() const
    {
        return m_elementIndex;
    }

    void setElementIndex(uint16_t elementIndex)
    {
        m_elementIndex = elementIndex;
    }

    // getters and setters
//    virtual void setValue(const uint8_t value) = 0;
//    virtual void setValue(const uint16_t value) = 0;
    virtual void setValue(const uint32_t value) = 0;
//    virtual void setValue(const int8_t value) = 0;
//    virtual void setValue(const int16_t value) = 0;
    virtual void setValue(const int32_t value) = 0;
    virtual void setValue(const float value) = 0;

//    virtual uint8_t operator=(const uint8_t value) = 0;
//    virtual uint16_t operator=(const uint16_t value) = 0;
    virtual uint32_t operator=(const uint32_t value) = 0;
//    virtual int8_t operator=(const int8_t value) = 0;
//    virtual int16_t operator=(const int16_t value) = 0;
    virtual int32_t operator=(const int32_t value) = 0;
    virtual float operator=(const float value) = 0;

//    virtual uint8_t getValueU8() const = 0;
//    virtual uint16_t getValueU16() const = 0;
    virtual uint32_t getValueU32() const = 0;
//    virtual int8_t getValueI8() const = 0;
//    virtual int16_t getValueI16() const = 0;
    virtual int32_t getValueI32() const = 0;
    virtual float getValueF() const = 0;

    bool isMonitoringEnabled() const
    {
        return m_monitoringEnabled;
    }

    void setMonitoringEnabled(bool monitoringEnabled)
    {
        m_monitoringEnabled = monitoringEnabled;
        if (m_monitoringEnabled)
            sendDeviceStatus();
    }

    virtual void updateAllowedRange(float min, float max, bool checkMin, bool checkMax) = 0;
    virtual void updateWorkingRange(float min, float max, bool checkMin, bool checkMax) = 0;
    virtual void updateWarningRange(float min, float max, bool checkMin, bool checkMax) = 0;
#ifdef OLD_PROTECTION
    virtual void updateSoftProtectionRange(float low, float high, bool useLow, bool useHigh) = 0;
    virtual void updateHardProtectionRange(float low, float high, bool useLow, bool useHigh) = 0;
#endif

    virtual bool isInAllowedRange() = 0;

    virtual bool hasError() = 0;
    virtual bool hasWarning() = 0;
    virtual char hasExceptions() = 0;

    virtual bool isInWorkingRange() = 0;
//    virtual bool isAboveWorkingRange() = 0;
//    virtual bool isBelowWorkingRange() = 0;

    virtual bool isInWarningRange() = 0;
//    virtual bool isAboveWarningRange() = 0;
//    virtual bool isBelowWarningRange() = 0;

#ifdef OLD_PROTECTION
    virtual bool isInsideLimit() = 0;
    virtual bool isOutsideSoftLimit() = 0;
    virtual bool isOutsideHardLimit() = 0;
#endif

    virtual void sendDeviceStatus() = 0;
    virtual void sendCurrentErrors() = 0;
    virtual void sendCurrentWarnings() = 0;

    virtual void setValueValid(bool valid) = 0;
    virtual bool isValid() = 0;
    virtual char getStatus() = 0;

    virtual void setMissingDevicePriority(E_MissingDevicePriority priority) = 0;
    virtual E_MissingDevicePriority getMissingDevicePriority() = 0;

    uint16_t getMaxInterval() const
    {
        return m_maxInterval * 100;
    }

    void setMaxInterval(int16_t maxInterval)
    {
        m_maxInterval = (int8_t)(maxInterval * 0.01);
    }

    uint16_t getMinInterval() const
    {
        return m_minInterval * 100;
    }

    void setMinInterval(int16_t minInterval)
    {
        if (minInterval < 0)
            m_minInterval = -1;
        else
            m_minInterval = (int8_t)(minInterval * 0.01);
    }

    virtual void updateErrorBits(E_PSSErrors error, bool state) = 0;
    virtual void updateWarningBits(E_PSSErrors warning, bool state) = 0;
    virtual uint32_t getErrors() = 0;
    virtual uint32_t getWarnings() = 0;

protected:
    virtual void sendDeviceStatus(uint8_t value);
    virtual void sendDeviceStatus(uint16_t value);
    virtual void sendDeviceStatus(uint32_t value);
    virtual void sendDeviceStatus(int8_t value);
    virtual void sendDeviceStatus(int16_t value);
    virtual void sendDeviceStatus(int32_t value);
    virtual void sendDeviceStatus(float value);

    virtual bool checkIfCanSendUpdate();

    void sendError(uint16_t boardId, uint16_t pssId, uint32_t errors);
    void sendWarning(uint16_t boardId, uint16_t pssId, uint32_t warnings);

};

typedef std::vector<ElementBase*> T_ElementList;
typedef std::vector<ElementBase*>::iterator T_ElementListIterator;

#endif /* ELEMENTBASE_H_ */
