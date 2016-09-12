/*
 * ErrorBitManager.h
 *
 *  Created on: 15 Feb 2015
 *      Author: maora
 */

#ifndef ERRORBITMANAGER_H_
#define ERRORBITMANAGER_H_

#include <PscServer/PscMessageStructs.h>
#include <map>

struct T_BitContainer
{
    uint32_t m_errorBits;
    uint32_t m_warningBits;
    T_BitContainer():m_errorBits(0),m_warningBits(0){}
};

typedef std::map<uint16_t, T_BitContainer> T_BitContainerMap;
typedef std::map<uint16_t, T_BitContainer>::iterator T_BitContainerMapIterator;

class ErrorBitManager
{
protected:
    T_BitContainerMap m_bitContainerMap;
    uint16_t m_pssId;

public:
    ErrorBitManager();
    virtual ~ErrorBitManager();

    virtual uint32_t raiseErrorSimple(uint16_t secondaryPssId, E_PSSErrors error, bool errorState);
    virtual uint32_t raiseWarningSimple(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState);
    virtual uint32_t raiseErrorWithInfo(uint16_t secondaryPssId, E_PSSErrors error, bool errorState, uint8_t dataType, const void *dataValue, uint32_t additionalError);
    virtual uint32_t raiseWarningWithInfo(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState, uint8_t dataType, const void *dataValue, uint32_t additionalError);
//    virtual bool isErrorRaised(E_PSSErrors error);
//    virtual bool isWarningRaised(E_PSSWarnings warning);
    virtual uint32_t getErrors(uint16_t secondaryPssId) {return getBitContainer(secondaryPssId)->m_errorBits;}
    virtual uint32_t getWarnings(uint16_t secondaryPssId) {return getBitContainer(secondaryPssId)->m_warningBits;}
    virtual void sendCurrentErrors();
    virtual void sendCurrentWarnings();

    uint16_t getPssId() const
    {
        return m_pssId;
    }

    void setPssId(uint16_t pssId)
    {
        m_pssId = pssId;
    }

private:
    T_BitContainer* getBitContainer(uint16_t secondary);
};

#endif /* ERRORBITMANAGER_H_ */
