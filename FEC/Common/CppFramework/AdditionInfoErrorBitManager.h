/*
 * AdditionInfoErrorBitManager.h
 *
 *  Created on: 9 Aug 2016
 *      Author: maora
 */

#ifndef ADDITIONINFOERRORBITMANAGER_H_
#define ADDITIONINFOERRORBITMANAGER_H_

#include <PscServer/PscMessageStructs.h>
#include <map>

typedef std::map<uint32_t, uint32_t> T_AdditionalInfoErrorMap;
typedef std::map<uint32_t, uint32_t>::iterator T_AdditionalInfoErrorMapIterator;

struct T_AdditionalInfoBitManager
{
    T_AdditionalInfoErrorMap m_errorBitsMap;
    T_AdditionalInfoErrorMap m_warningBitsMap;
    T_AdditionalInfoBitManager(){}
    bool hasErrors();
    bool hasWarnings();
};

typedef std::map<uint16_t, T_AdditionalInfoBitManager> T_AdditionalInfoBitManagerMap;
typedef std::map<uint16_t, T_AdditionalInfoBitManager>::iterator T_AdditionalInfoBitManagerMapIterator;

class AdditionInfoErrorBitManager
{
protected:
    T_AdditionalInfoBitManagerMap m_additionalInfoMap;
    uint16_t m_pssId;

public:
    AdditionInfoErrorBitManager();
    virtual ~AdditionInfoErrorBitManager();

    virtual uint32_t raiseError(uint16_t secondaryPssId, E_PSSErrors mainError, bool errorState);
    virtual uint32_t raiseError(uint16_t secondaryPssId, E_PSSErrors mainError, uint32_t additionalError, E_ValueType valueType, void* value, bool errorState);
//    virtual uint32_t setError(uint16_t secondaryPssId, E_PSSErrors mainError, uint32_t additionalError, E_ValueType valueType, void* value);
    virtual uint32_t raiseWarning(uint16_t secondaryPssId, E_PSSWarnings mainWarning, bool warningState);
    virtual uint32_t raiseWarning(uint16_t secondaryPssId, E_PSSWarnings mainWarning, uint32_t additionalWarning, E_ValueType valueType, void* value, bool warningState);
//    virtual uint32_t setWarning(uint16_t secondaryPssId, E_PSSWarnings mainWarning, uint32_t additionalWarning, E_ValueType valueType, void* value);
//    virtual bool isErrorRaised(E_PSSErrors error);
//    virtual bool isWarningRaised(E_PSSWarnings warning);
    //virtual uint32_t getErrors(uint16_t secondaryPssId) {return getBitContainer(secondaryPssId)->m_errorBits;}
    //virtual uint32_t getWarnings(uint16_t secondaryPssId) {return getBitContainer(secondaryPssId)->m_warningBits;}
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
};

#endif /* ADDITIONINFOERRORBITMANAGER_H_ */
