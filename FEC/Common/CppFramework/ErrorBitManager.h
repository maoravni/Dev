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

struct T_PrimarySecondaryKey
{
    uint16_t m_primaryPssId;
    uint16_t m_secondaryPssId;

    bool operator<(const T_PrimarySecondaryKey& rhs)
    {
        if (m_primaryPssId < rhs.m_primaryPssId)
            return true;
        else if (m_primaryPssId == rhs.m_secondaryPssId)
            return (m_secondaryPssId < rhs.m_secondaryPssId);
        return false;
    }

};
typedef std::map<uint16_t, T_BitContainer> T_BitContainerMap;
typedef std::map<uint16_t, T_BitContainer>::iterator T_BitContainerMapIterator;

class ErrorBitManager
{
protected:
    //uint32_t m_errorBits;
    //uint32_t m_warningBits;
//    T_BitContainer m_bitContainer;
    T_BitContainerMap m_bitContainerMap;
    uint16_t m_pssId;
//    uint16_t m_secondaryPssId;

public:
    ErrorBitManager();
    virtual ~ErrorBitManager();

    virtual uint32_t raiseError(uint16_t secondaryPssId, E_PSSErrors error, bool errorState);
    virtual uint32_t raiseWarning(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState);
    virtual bool isErrorRaised(E_PSSErrors error);
    virtual bool isWarningRaised(E_PSSWarnings warning);
    virtual uint32_t getErrors(uint16_t secondaryPssId) {return getBitContainer(secondaryPssId)->m_errorBits;}
    virtual uint32_t getWarnings(uint16_t secondaryPssId) {return getBitContainer(secondaryPssId)->m_warningBits;}
    virtual void sendCurrentErrors();
    virtual void sendCurrentWarnings();

//    uint32_t getErrorBits() const
//    {
//        return m_errorBits;
//    }
//
//    void setErrorBits(uint32_t errorBits)
//    {
//        m_errorBits = errorBits;
//    }

//    uint16_t getSecondaryPssId() const
//    {
//        return m_secondaryPssId;
//    }

//    void setSecondaryPssId(uint16_t secondaryPssId);

//    uint32_t getWarningBits() const
//    {
//        return m_warningBits;
//    }
//
//    void setWarningBits(uint32_t warningBits)
//    {
//        m_warningBits = warningBits;
//    }

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
//    virtual uint32_t raiseError(uint16_t secondaryPssId, E_PSSErrors error, bool errorState, int dummy);
//    virtual uint32_t raiseWarning(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState, int dummy);
};

#endif /* ERRORBITMANAGER_H_ */
